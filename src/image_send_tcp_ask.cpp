#include <iostream>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int m = 0;

int image_width = 640;  // 图像宽度
int image_height = 400;  // 图像高度

#define SERVER_IP "192.168.3.123"
#define PORT 12349
#define BUFFER_SIZE 1024

using namespace std;

class ImageTCPSend {
public:
    int init();
    int imageSend(cv::Mat &left, cv::Mat &right);
    ~ImageTCPSend() {
        close(this->sock);
    }
private:

    int sock;
    struct sockaddr_in serverAddr;
};

int ImageTCPSend::init() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cout << "sock failded\n";
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cout << "Error: inet_pton failed.\n";
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cout << "Error: Connection failed.\n";
        close(sock);
        return 1;
    }
    return 0;
}

int ImageTCPSend::imageSend(cv::Mat &left, cv::Mat &right) 
{
    cv::Mat image;
    std::vector<cv::Mat> a = {left, right};
    vconcat(a, image);
    cv::Mat img1;
    cv::hconcat(image, image, img1); 
    cv::Mat img2;
    cv::hconcat(img1, img1, img2); 
        cv::resize(img2, img2, cv::Size(img2.cols / 2, img2.rows / 2));
    vector<uchar> decode;
    imencode(".jpg", img2, decode);
    uint32_t len = decode.size();
    std::cout << "size:" << len << '\n';
    vector<uchar> buffer(len + 8);
    buffer[0] = 'B';
    buffer[1] = 'F';
    buffer[2] = 'B';
    buffer[3] = 'F';
    buffer[4] = static_cast<unsigned char>((len) & 0xFF);
    buffer[5] = static_cast<unsigned char>((len >> 8) & 0xFF);
    buffer[6] = static_cast<unsigned char>((len >> 16) & 0xFF);
    buffer[7] = static_cast<unsigned char>((len >> 24) & 0xFF);
    std::cout << "head:" << buffer[0] << buffer[1] << buffer[2] << buffer[3] << (uint8_t)buffer[4] << (uint8_t)buffer[5] <<'\n';

    std::cout << "buffer.size:" << buffer.size() << "\n";
    for (int i = 0; i < len; i++) {
        // std::cout << i << ' ';
        buffer[i + 8] = decode[i];
    }
    if (send(sock, buffer.data(), buffer.size(), 0) == -1) {
        std::cout << "Error: Failed to send image data.\n";
        return -1;
    }
    if (m == 0) {
        std::cout << "buffer.data:\n";
        for (int i = len - 1; i >= 0; i--) std::cout << buffer[i] << '\n';
        m++;
        std::cout << "\n\n\n";
    }
    return 0;
}

#define PORT1 12348
class AskRev {
public:
    int init();
    int run();
    ~AskRev() {
        // 关闭套接字
        close(newSocket);
        close(serverSock);
    }
private:
    int serverSock, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
};

int AskRev::init()
{
    // 创建 TCP 套接字
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT1);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字
    bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    // 监听传入的连接
    if (listen(serverSock, 10) == 0) {
        cout << "Listening for incoming connections..." << endl;
    } else {
        cout << "Failed to listen" << endl;
        return -1;
    }
    // 等待客户端连接
    addr_size = sizeof(serverStorage);
    newSocket = accept(serverSock, (struct sockaddr *) &serverStorage, &addr_size);
    cout << "Client connected" << endl;
    return 0;
}

int AskRev::run()
{
    int ret;
    vector<uchar> rev(2);
    for (int i = 0; i < 2; i++) {
        uchar s;
        ret = recv(newSocket, &s, 1, 0);
        rev[i] = s;
    }
    if (rev[0] == 'O' && rev[1] == 'K') return 0;
    return 0;
}

bool should_exit = false;
void signalHandler(int signum)
{
    // ROS_INFO("Received exit signal. Exiting the while loop.");
    should_exit = true;
}


bool compareNames(const std::string& a, const std::string& b) {
    return a < b;
}

int main(int argc, char** argv) {

    ImageTCPSend imgSend;
    AskRev askrev;
    std::cout << "start init socket\n";
    if (imgSend.init() != 0) {
        std::cout << "init failed\n";
        return 1;
    } else {
        std::cout << "init successfully\n";
    }

    if (askrev.init() != 0) {
        std::cout << "init failed\n";
        return 1;
    } else {
        std::cout << "init successfully\n";
    }
    
    std::string left_directory_path = "./left";
    std::vector<std::string> left_file_names;
    std::string right_directory_path = "./right";
    std::vector<std::string> right_file_names;

    DIR *left_dir;
    DIR *right_dir;
    struct dirent *left_ent;
    struct dirent *right_ent;

    int count = 0;
    std::cout << "out\n";
    if ((left_dir = opendir(left_directory_path.c_str())) != NULL && (right_dir = opendir(right_directory_path.c_str())) != NULL) {
        std::cout << "in\n";
        while ((left_ent = readdir(left_dir)) != NULL) {
            std::string file_name = left_ent->d_name;
            if (file_name.length() > 2 && file_name.substr(file_name.length() - 4) == ".bmp") { // 只处理 jpg 格式的文件
                left_file_names.push_back(file_name);
                std::cout << "read successfully\n";
            } else {
                std::cout << "read failed\n";
            }
        }
        closedir(left_dir);

        while ((right_ent = readdir(right_dir)) != NULL) {
            std::string file_name = right_ent->d_name;
            if (file_name.length() > 2 && file_name.substr(file_name.length() - 4) == ".bmp") { // 只处理 jpg 格式的文件
                right_file_names.push_back(file_name);
                std::cout << "read successfully\n";
            } else {
                std::cout << "read failed\n";
            }
        }
        closedir(right_dir);

        // 按文件名排序
        std::sort(left_file_names.begin(), left_file_names.end(), compareNames);
        std::sort(right_file_names.begin(), right_file_names.end(), compareNames);
        int count = 0;
        std::cout << "start\n";
        while (!should_exit) {
            for (int i = 0; i < left_file_names.size(); i++) {
                std::string left_path = left_directory_path + "/" + left_file_names[i];
                std::string right_path = right_directory_path + "/" + right_file_names[i];
                cv::Mat left_image = cv::imread(left_path);
                cv::Mat right_image = cv::imread(right_path);
                if (left_image.empty() || right_image.empty()) {
                    std::cout << "Failded\n";
                } else {
                   
                    if (imgSend.imageSend(left_image, right_image) != 0) {
                        std::cout << "send image failed\n";
                    }
                    else {
                        std::cout << "iamge send successfully\n";
                    }

                    if (askrev.run() != 0) {
                        std::cout << "rev ask failded\n";
                    }
                    else {
                        std::cout << "rev ask successfully\n";
                    }
                    count++;
                    cout << "count:" << count << "***************\n";
                    // usleep(100000);
                    
                }
            }
        }
    } else {
        std::cout << "Failed to open directory\n";
        return EXIT_FAILURE;
    }
    std::cout << count << '\n';

    return 0;
}
