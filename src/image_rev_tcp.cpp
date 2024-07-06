#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace cv;
using namespace std;

#define PORT 12348
int a;

bool should_exit = false;
void signalHandler(int signum)
{
    // ROS_INFO("Received exit signal. Exiting the while loop.");
    should_exit = true;
}

int main() {
    int serverSock, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // 创建 TCP 套接字
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
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

    // 接收图像大小
    int count = 0;
    while (!should_exit) {
        uint16_t imgSize;
        if (recv(newSocket, &imgSize, sizeof(imgSize), 0) == -1) {
            cerr << "Error: Failed to receive image size." << endl;
            close(newSocket);
            close(serverSock);
            return 1;
        }
        std::cout << "imgsize:" << imgSize << '\n';
        // 接收图像数据
        vector<uchar> buffer(imgSize);
        if (recv(newSocket, buffer.data(), imgSize, 0) == -1) {
            cerr << "Error: Failed to receive image data." << endl;
            close(newSocket);
            close(serverSock);
            return 1;
        }
        

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
        cv::resize(image, image, cv::Size(image.cols * 2, image.rows * 2));
        cv::imshow("image", image);
 
        // 按ESC键退出循环
        if (cv::waitKey(1) == 27) {
            break;
        }

        std::cout << "rev successfully\n";
        std::cout << "count" << count << "***************************\n";
        count++;

        // 显示接收到的图像
        // imshow("Received Image", image);
        // waitKey(0);
    }

    // 关闭套接字
    close(newSocket);
    close(serverSock);

    return 0;
}