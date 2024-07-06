#include <iostream>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <gst/gst.h>

GstElement* pipeline;
GstElement* source;
GstElement* udpsink;

int image_width = 640;  // 图像宽度
int image_height = 400;  // 图像高度

cv::Mat res(image_height * 2, image_width * 3, CV_8UC1);

void tran_image(cv::Mat &image)
{
    for (int y = 0; y < image_height * 2; y++) {
        for (int x = 0; x < image_width; x++) {
            cv::Vec3b pixel_value = image.at<cv::Vec3b>(y, x);
            res.at<uint8_t>(y, x) = pixel_value[0];
            res.at<uint8_t>(y, x + image_width) = pixel_value[1];
            res.at<uint8_t>(y, x + 2 * image_width) = pixel_value[2];
        }
    }
}

bool compareNames(const std::string& a, const std::string& b) {
    return a < b;
}

int main(int argc, char** argv) {

    // 创建 GStreamer 管道
    gst_init(&argc, &argv);
  
    pipeline = gst_parse_launch("appsrc name=source ! videoconvert ! x264enc tune=zerolatency bitrate=1000 speed-preset=superfast intra-refresh=true key-int-max=5 ! rtph264pay ! udpsink host=127.0.0.1 port=12345 , name = tcp", NULL);
    // pipeline = gst_parse_launch("appsrc name=source ! videoconvert ! x264enc tune=zerolatency bitrate=1000 speed-preset=superfast ! video/x-h264, stream-format=byte-stream ! rtph264pay ! udpsink host=127.0.0.1 port=12345, name = udp", NULL);

    source = gst_bin_get_by_name(GST_BIN(pipeline), "source");
    udpsink = gst_bin_get_by_name(GST_BIN(pipeline), "udp");
    if (!source) {
        g_printerr ("source could be created.\n");
    }
    if (!pipeline) {
        g_printerr ("pipeline could be created.\n");
    }
    if (!udpsink) {
        g_printerr ("udpsink could be created.\n");
    }

    // 设置图像属性
    GstCaps* caps = gst_caps_new_simple("video/x-raw",
                                        "format", G_TYPE_STRING, "GRAY8",
                                        "width", G_TYPE_INT, image_width * 3,
                                        "height", G_TYPE_INT, image_height * 2,
                                        "framerate", GST_TYPE_FRACTION, 10, 1,
                                        NULL);
    std::cout << 11 << '\n';                   
    g_object_set(G_OBJECT(source), "caps", caps, NULL);
    std::cout << 22 << '\n';      
    g_object_set(G_OBJECT(udpsink), "buffer-size", 10, NULL);
    std::cout << 33 << '\n';      

    g_object_set (G_OBJECT(source), "is-live",      TRUE,   NULL);
    g_object_set (G_OBJECT(source), "stream-type",  0, NULL);
    g_object_set (G_OBJECT(source), "format",       GST_FORMAT_TIME, NULL);
    // 给实时视频流打上时间戳
    g_object_set (G_OBJECT(source), "do-timestamp", TRUE,   NULL);
    g_object_set (G_OBJECT(source), "min-percent",  0, NULL); // then the min-latency must be set to 0 because it timestamps based on the running-time when the buffer entered appsrc

    g_object_set (G_OBJECT(udpsink), "sync", FALSE, NULL);
    //是否在preroll期间渲染视频帧。
    g_object_set (G_OBJECT(udpsink), "show-preroll-frame", FALSE, NULL);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

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
                 int type = image.type();
                int depth = type & CV_MAT_DEPTH_MASK;
                int channels = image.channels();

                // 打印结果
                std::cout << "Type: " << type << std::endl;
                std::cout << "Depth: " << depth << std::endl;
                std::cout << "Channels: " << channels << std::endl;
                cv::imshow("Image", image);
                cv::waitKey(0);  // 按任意键继续
                tran_image(image);

                const uint8_t* image_data = res.data;
                int image_size = res.cols * res.rows;
                std::cout << res.cols << ' ' << res.rows << '\n';
                std::cout << "image_size: " << image_size << '\n';
                GstBuffer* buffer = gst_buffer_new_allocate(NULL, image_size, NULL);
                GstMapInfo map;
                gst_buffer_map(buffer, &map, GST_MAP_WRITE);
                memcpy(map.data, image_data, image_size);
                gst_buffer_unmap(buffer, &map);
                if (source == NULL || buffer == NULL) std::cout << "error\n";
                g_signal_emit_by_name(source, "push-buffer", buffer, NULL);
                count++;
            }
        }
    } else {
        std::cerr << "Failed to open directory\n";
        return EXIT_FAILURE;
    }
    std::cout << count << '\n';
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
