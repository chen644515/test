#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace cv;
using namespace std;

#define PORT1 12349
int a;

class ImageRev {
public:
    int init();
    int run();
    ~ImageRev() {
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

int ImageRev::init() 
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

int ImageRev::run()
{
    uint32_t imgSize;
    int ret = 0;
    vector<uchar> head(8);
    for (int i = 0; i < 8; i++) {
        uchar s;
        ret = recv(newSocket, &s, 1, 0);
        // if (ret != 1) {
        //     std::cout << "error\n";
        // }
        head[i] = s;
    }
   
    std::cout << "rev head successfully\n";
    imgSize = (static_cast<uint16_t>(head[7]) << 24) | (static_cast<uint16_t>(head[6]) << 16) | (static_cast<uint16_t>(head[5]) << 8) | static_cast<uint16_t>(head[4]);
    vector<uchar> buffer(imgSize);
    // if (recv(newSocket, buffer.data(), imgSize, 0) <= 0) {
    //     std::cout << "rev image failed\n";
    //     continue;
    // }
    for (int i = 0; i < imgSize; i++) {
        uchar s;
        if (recv(newSocket, &s, 1, 0) <= 0) {
            // std::cout << "rev data failed\n";
        }
        buffer[i] = s;
    }
    // std::cout << "buffer.data:\n";
    // for (int i = imgSize - 1; i >= 0; i--) cout << buffer[i] << '\n';
    // std::cout << "\n\n\n";
    // 解码图像数据
    Mat image;
    try {
        // std::cout << buffer.size() << '\n';
        image = imdecode(buffer, IMREAD_ANYCOLOR);
        if (image.empty()) {
            cerr << "Error: Failed to decode image." << endl;
            close(newSocket);
            close(serverSock);
            return 1;
        }
    } catch (cv::Exception& e) {
        cerr << "OpenCV exception: " << e.what() << endl;
        close(newSocket);
        close(serverSock);
        return 1;
    }
    cv::resize(image, image, cv::Size(image.cols * 3, image.rows * 3));
    cv::imshow("image", image);
    return 0;
}

#define SERVER_IP "192.168.3.10"
#define PORT 12348
#define BUFFER_SIZE 1024
class AskSend{
public:
    int init();
    int askSend(string &ask);
    ~AskSend() {
        close(this->sock);
    }
private:
    int sock;
    struct sockaddr_in serverAddr;
};

int AskSend::init()
{
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
    std::cout << "Connect successfully\n";
    return 0;
}

int AskSend::askSend(string &ask)
{
    vector<uchar> buffer(ask.size());
    for (int i = 0; i < ask.size(); i++) {
        buffer[i] = ask[i];
    }
    if (send(sock, buffer.data(), buffer.size(), 0) == -1) {
        std::cout << "Error: Failed to send image data.\n";
        return -1;
    }
    return 0;
}


bool should_exit = false;
void signalHandler(int signum)
{
    // ROS_INFO("Received exit signal. Exiting the while loop.");
    should_exit = true;
}

int main() {
    
    ImageRev imgrev;
    imgrev.init();
    std::cout << "d1\n";
    usleep(2000000);
    AskSend asksend;
    asksend.init();
    std::cout << "d2\n";
    string s = "OK";

    // 接收图像大小
    int count = 0;
    while (!should_exit) {
        if (imgrev.run() != 0) {
            std::cout << "error exit\n";
            break;
        }
        if (asksend.askSend(s) != 0) {
            std::cout << "error1 exit\n";
            break;
        }
        // 按ESC键退出循环
        if (cv::waitKey(1) == 27) {
            break;
        }
        std::cout << "count" << count << "***************************\n";
        count++;
    }

    return 0;
}
