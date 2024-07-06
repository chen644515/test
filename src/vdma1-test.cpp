#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <poll.h>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>	   /*包含库的核心功能*/
#include <opencv2/imgproc.hpp> /*包括主要的图像处理函数*/
#include <opencv2/highgui.hpp> /*提供了读写图像和视频的函数以及一些用户交互函数*/
/*argc:应用程序参数个数
*argv[]：具体参数内容，字符串形式
<./led-drv-test>  <filename>
*/

using namespace std;
using namespace cv;

#define VDMA_IOCTL_MAGIC 'M'

#define CMD_GETWIN _IO(VDMA_IOCTL_MAGIC, 0)
#define CMD_READ_DMA0 _IO(VDMA_IOCTL_MAGIC, 1)
#define CMD_READ_DMA1 _IO(VDMA_IOCTL_MAGIC, 2)

#define SERVER_IP "192.168.3.22"
#define UDP_PORT 12345


class UDP_image_send {
private:
    int udp_socket;
    struct sockaddr_in udp_server_addr;

public:
    int init();

    int image_send(cv::Mat &left, cv::Mat &right);

    UDP_image_send(){
        close(this->udp_socket);
    };

};

int UDP_image_send::init()
{
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        // ROS_ERROR("Failed to create UDP socket");
        return -1;
    }
    memset((char *)&udp_server_addr, 0, sizeof(udp_server_addr));
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_PORT);
    udp_server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    return 0;
}

int UDP_image_send::image_send(cv::Mat &left_image, cv::Mat &right_image)
{
    if (left_image.empty() || right_image.empty()) {
        return -1;
    }
    cv::Mat image;
    std::vector<cv::Mat> a = {left_image, right_image};
    vconcat(a, image);
    cv::resize(image, image, cv::Size(image.cols / 2, image.rows / 2));
    std::vector<uchar> buf;
    cv::imencode(".jpg", image, buf);
    sendto(udp_socket, buf.data(), buf.size(), 0, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr));
    return 0;
}

int main(int argc, char *argv[])
{
	UDP_image_send UDP_send;
	UDP_send.init();
	// unsigned char timestamp[30];
	// Mat buffer1(400,640, CV_8UC1);
	int ret = 0;
	int i, j;
	int fd, fd1, fd0;
	int buf_size, image_size;
	unsigned char *Lbuf, *Lbuf1, *Rbuf, *Rbuf1;
	struct pollfd poll_fd, poll_fd1, poll_vdma1;
	struct timeval tv;
	clock_t start, finish1;
	double totaltime;
	int64_t *time_mmap;
	uint32_t Ltime1, Ltime2, Rtime1, Rtime2; // 左目时间戳数据转换，Ltime1为低32位，Ltime2为高32位，右目同理
	float Ltime, Rtime;
	unsigned char Ltimestamp[6], Rtimestamp[6];
	int count = 3000;
	char *buf;

	// open
	// int fd = open("/dev/vdma1", O_RDWR);
	// if (fd < 0)
	// {
	// 	printf("fd open fail!\r\n");
	// 	return -1;
	// }

	fd0 = open("/dev/vdma1", O_RDWR);
	if (fd0 < 0)
	{
		printf("fd_vdma1 open fail!\r\n");
		return -1;
	}

	fd = open("/dev/vdma5", O_RDWR);
	if (fd < 0)
	{
		printf("fd open fail!\r\n");
		return -1;
	}

	fd1 = open("/dev/vdma6", O_RDWR);
	if (fd1 < 0)
	{
		printf("fd open fail!\r\n");
		return -1;
	}

	buf_size = 400 * 640 * 4;
	image_size = 640;
	Lbuf = (unsigned char *)malloc(buf_size);
	// Lbuf1 = (unsigned char *)malloc(image_size); // 存放左目时间戳
	memset(Lbuf, 0, buf_size);
	// memset(Lbuf1, 0, image_size);

	Rbuf = (unsigned char *)malloc(buf_size);
	// Rbuf1 = (unsigned char *)malloc(image_size); // 存放右目时间戳
	memset(Rbuf, 0, buf_size);
	// memset(Rbuf1, 0, image_size);

	poll_vdma1.events = POLLIN;
	poll_vdma1.fd = fd0;
	poll_fd.events = POLLIN;
	poll_fd.fd = fd;
	poll_fd1.events = POLLIN;
	poll_fd1.fd = fd1;

	// count = 0;

	ofstream outFile, outFile1, outFile2;
	outFile.open("/run/media/nvme0n1p1/20231110/mav0/cam0/data.csv", ios::out);
	outFile1.open("/run/media/nvme0n1p1/20231110/mav0/cam1/data.csv", ios::out);
	outFile2.open("/run/media/nvme0n1p1/20231110/mav0/timestamp.txt", ios::out);

	outFile << "#timestamp [ns],filename" << endl;
	outFile1 << "#timestamp [ns],filename" << endl;

	// 循环读取视频中各帧

	// while (count--)
	for(int i=0; i < 100; i++)
	{
		

		// ret = poll(&poll_vdma1, 1, -1);
		// if (ret < 0)
		// {
		// 	printf("error\n");
		// 	// break;
		// }
		
		// start = clock();

		cout<<"debug1"<<endl;

		usleep(5000);

		// // 开启一次传输
		write(fd, &buf, 1);
		write(fd1, &buf, 1);



		// start = clock();
		ret = poll(&poll_fd, 1, -1);
		if (ret < 0)
		{
			printf("error\n");
			break;
		}

		// ret = poll(&poll_fd1, 1, -1);
		// if (ret < 0)
		// {
		// 	printf("error\n");
		// 	break;
		// }

		cout<<"debug2"<<endl;

		// ret = ioctl(fd, CMD_READ_DMA0, Lbuf);
		// if (ret < 0)
		// {
		// 	printf("get data err\n");
		// }

		// ret = ioctl(fd1, CMD_READ_DMA0, Rbuf);
		// if (ret < 0)
		// {
		// 	printf("get data err\n");
		// }
		read(fd, Lbuf, buf_size);
		read(fd1, Rbuf, buf_size);

		cout<<"debug3"<<endl;

		

		

		// cout<<"done!"<<endl;

		// // memcpy(Lbuf1, Lbuf, buf_size);
		// // memcpy(Rbuf1, Rbuf, buf_size);
		// printf("\n");
		// cout<<sizeof(Lbuf)<<endl;

		// gettimeofday(&tv, NULL);

		// // long long Rtime_ns = Rtime * 1e9;
		Mat buffer2(400, 640, CV_8UC4, Lbuf);
		Mat buffer3(400, 640, CV_8UC4, Rbuf);
		// string img_name_l = "/home/longmen/media/slam/left/" + to_string(i) + ".bmp";
		// string img_name_r = "/home/longmen/media/slam/right/" + to_string(i) + ".bmp";
		// imwrite(img_name_l, buffer2);
		// imwrite(img_name_r, buffer3);
		UDP_send.image_send(buffer2, buffer3);

		cout<<"count= "<<i<<endl;

		// count++;
		// usleep(50000);

		// // count++;

		// outFile.precision(13);
		// outFile << Ltime_ns << "," << Ltime_ns << ".png" << endl;
		// outFile1.precision(13);
		// outFile1 << Ltime_ns << "," << Ltime_ns << ".png" << endl;
		// outFile1.precision(13);
		// outFile2 << Ltime_ns<< endl;

		// if (waitKey(1) == 27)
		// {
		// 	break;
		// }
	}

	outFile.close();
	outFile1.close();
	int num = close(fd);
	if (num < 0)
	{
		printf("fd open fail!\r\n");
	}


	int num1 = close(fd1);
	if (num1 < 0)
	{
		printf("fd open fail!\r\n");
	}

	return 0;
}