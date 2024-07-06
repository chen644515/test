#include <iostream>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <gst/gst.h>

int image_width = 640;  // 图像宽度
int image_height = 400;  // 图像高度

bool should_exit = false;
void signalHandler(int signum)
{
    // ROS_INFO("Received exit signal. Exiting the while loop.");
    should_exit = true;
}

int main(int argc, char **argv)
{
    std::cout << "image_receive\n";

    std::string image_src_raw = "udpsrc port=12345 ! application/x-rtp,media=video,encoding-name=H264 ! rtph264depay ! queue ! h264parse ! avdec_h264 ! videoconvert ! appsink name=appsink";
    cv::VideoCapture cap_raw(image_src_raw, cv::CAP_GSTREAMER);

    if(!cap_raw.isOpened()) {
      std::cout << "open failed" << '\n';
      return 0;
    }

    cv::Mat frame_raw;
    int count = 0;
    while (!should_exit) {
        cap_raw >> frame_raw;

        int type = frame_raw.type();
        int depth = type & CV_MAT_DEPTH_MASK;
        int channels = 1 + (type >> CV_CN_SHIFT);

        int img_raw_wid = frame_raw.cols;  // 图像宽度
        int img_raw_hei = frame_raw.rows;  // 图像高度
        std::cout << img_raw_wid << ' ' << img_raw_hei << '\n';

        cv::Mat res_raw(image_height * 2, image_width, CV_8UC3);
        // cv::Mat res_dep(image_height, image_width, CV_8UC3);

        for (int y = 0; y < image_height * 2; y++) {
            for (int x = 0; x < image_width; x++) {
                // 访问像素 (x, y
                // std::cout << y << ' ' << x << '\n';
                res_raw.at<cv::Vec3b>(y, x)[0] = frame_raw.at<cv::Vec3b>(y, x)[0];
                res_raw.at<cv::Vec3b>(y, x)[1] = frame_raw.at<cv::Vec3b>(y, x + image_width)[0];
                res_raw.at<cv::Vec3b>(y, x)[2] = frame_raw.at<cv::Vec3b>(y, x + 2 * image_width)[0];
            }
        }

        // 打印结果
        std::cout << "Type: " << type << std::endl;
        std::cout << "Depth: " << depth << std::endl;
        std::cout << "Channels: " << channels << std::endl;
        if(frame_raw.empty())
            break;
        imshow("video", res_raw);
        cv::waitKey(20);
        count++;
        std::cout << count << '\n';
    }
    cap_raw.release();//释放资源
    return 0;

}
