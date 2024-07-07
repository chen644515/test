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

#define SERVER_IP "127.0.0.1"
#define PORT 12346
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
    cv::resize(image, image, cv::Size(image.cols* 2, image.rows * 2));
    vector<uchar> decode;
    imencode(".jpg", image, decode);
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
    std::cout << "hello world!!!!!!!!!!!\n";

    ImageTCPSend imgSend;
    std::cout << "start init socket\n";
    if (imgSend.init() != 0) {
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
        // while (!should_exit) {
            for (int i = 0; i < left_file_names.size(); i++) {
                std::string left_path = left_directory_path + "/" + left_file_names[i];
                std::string right_path = right_directory_path + "/" + right_file_names[i];
                cv::Mat left_image = cv::imread(left_path);
                cv::Mat right_image = cv::imread(right_path);
                if (left_image.empty() || right_image.empty()) {
                    std::cout << "Failded\n";
                } else {
                    // cv::Mat image;
                    // std::vector<cv::Mat> a = {left_image, right_image};
                    // vconcat(a, image);
                    // cv::resize(image, image, cv::Size(image.cols / 2, image.rows / 2));
                    // std::cout << 1 << '\n';
                    // vector<uchar> buffer;
                    // imencode(".jpg", image, buffer);
                    // std::cout << 2 << '\n';
                    // int imgSize = buffer.size();
                    // if (send(sock, &imgSize, sizeof(imgSize), 0) == -1) {
                    //     std::cerr << "Error: Failed to send image size." << endl;
                    //     return 1;
                    // }
                    // std::cout << 3 << '\n';
                    // // Send the image data
                    // if (send(sock, buffer.data(), imgSize, 0) == -1) {
                    //     std::cerr << "Error: Failed to send image data." << endl;
                    //     return 1;
                    // }
                    // usleep(200000);
                    // cout << "send successfully\n";
                    // std::cout << "count" << count << "***************************\n";
                    // count++;
                    if (imgSend.imageSend(left_image, right_image) != 0) {
                        std::cout << "send image failed\n";
                    }
                    else {
                        std::cout << "iamge send successfully\n";
                    }
                    usleep(400000);
                    
                }
            }
        // }
    } else {
        std::cout << "Failed to open directory\n";
        return EXIT_FAILURE;
    }
    std::cout << count << '\n';

    return 0;
}
