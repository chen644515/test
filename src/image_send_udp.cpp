#include <iostream>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>



int image_width = 640;  // 图像宽度
int image_height = 400;  // 图像高度

#define UDP_PORT 12345

int udp_socket;
struct sockaddr_in udp_server_addr;


using namespace std;

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


   udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        // ROS_ERROR("Failed to create UDP socket");
        return -1;
    }

    memset((char *)&udp_server_addr, 0, sizeof(udp_server_addr));
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_PORT);
    udp_server_addr.sin_addr.s_addr = inet_addr("192.168.3.12");

    std::string left_directory_path = "../left";
    std::vector<std::string> left_file_names;
    std::string right_directory_path = "../right";
    std::vector<std::string> right_file_names;

    DIR *left_dir;
    DIR *right_dir;
    struct dirent *left_ent;
    struct dirent *right_ent;

    int count = 0;

    if ((left_dir = opendir(left_directory_path.c_str())) != NULL && (right_dir = opendir(right_directory_path.c_str())) != NULL) {
        while ((left_ent = readdir(left_dir)) != NULL) {
            std::string file_name = left_ent->d_name;
            if (file_name.length() > 2 && file_name.substr(file_name.length() - 4) == ".bmp") { // 只处理 jpg 格式的文件
                left_file_names.push_back(file_name);
            }
        }
        closedir(left_dir);

        while ((right_ent = readdir(right_dir)) != NULL) {
            std::string file_name = right_ent->d_name;
            if (file_name.length() > 2 && file_name.substr(file_name.length() - 4) == ".bmp") { // 只处理 jpg 格式的文件
                right_file_names.push_back(file_name);
            }
        }
        closedir(right_dir);

        // 按文件名排序
        std::sort(left_file_names.begin(), left_file_names.end(), compareNames);
        std::sort(right_file_names.begin(), right_file_names.end(), compareNames);
        uchar count = 0;
        while (!should_exit) {
            for (int i = 0; i < left_file_names.size(); i++) {
                std::string left_path = left_directory_path + "/" + left_file_names[i];
                std::string right_path = right_directory_path + "/" + right_file_names[i];
                cv::Mat left_image = cv::imread(left_path);
                cv::Mat right_image = cv::imread(right_path);
                if (left_image.empty() || right_image.empty()) {
                    std::cout << "Failded\n";
                } else {
                    cv::Mat image;
                    std::vector<cv::Mat> a = {left_image, right_image};
                    vconcat(a, image);
                    cv::resize(image, image, cv::Size(image.cols / 2, image.rows / 2));
                    std::cout << 1 << '\n';
                    
                    std::vector<uchar> buf;
                    cv::imencode(".jpg", image, buf);
                    // buf.push_back(count);
                    // Send the image data over UDP
                    sendto(udp_socket, buf.data(), buf.size(), 0, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr));

                    std::cout << buf.size() << '\n';
                    
                    usleep(200000);
                    cout << "send successfully\n";
                    std::cout << "count" << count << "***************************\n";
                    count++;
                    
                }
            }
            break;
        }
    } else {
        std::cerr << "Failed to open directory\n";
        return EXIT_FAILURE;
    }
    std::cout << count << '\n';
    close(udp_socket);

    return 0;
}
