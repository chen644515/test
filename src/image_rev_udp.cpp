#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 12345  // UDP端口号
#define MAX_BUFFER_SIZE 65507  // UDP数据包的最大大小

void error(const char *msg) {
    perror(msg);
    exit(1);
}

bool should_exit = false;
void signalHandler(int signum)
{
    // ROS_INFO("Received exit signal. Exiting the while loop.");
    should_exit = true;
}

int main() {
    int sockfd;
    struct sockaddr_in my_addr, cli_addr;
    socklen_t slen = sizeof(cli_addr);
    char buf[MAX_BUFFER_SIZE];

    // 创建UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("socket");
    }

    // 初始化本地地址
    memset((char *) &my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定本地地址
    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
        error("bind");
    }

    std::cout << "UDP server is running and waiting for data on port " << PORT << std::endl;

    // 接收图像数据
    while (!should_exit) {
        int recv_len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &cli_addr, &slen);
        if (recv_len == -1) {
            error("recvfrom");
        }

        // 将接收到的数据存储为图像文件
        std::vector<uchar> image_data(buf, buf + recv_len);

        cv::Mat image = cv::imdecode(image_data, cv::IMREAD_COLOR);
        cv::resize(image, image, cv::Size(image.cols * 2, image.rows * 2));

        if (cv::waitKey(1) == 27) {
            break;
        }


        imshow("Received Image", image);
        // cv::waitKey(0);


        // 可选：打印接收到的数据信息
        std::cout << "Received " << recv_len << " bytes from " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;
    }

    close(sockfd);

    return 0;
}
