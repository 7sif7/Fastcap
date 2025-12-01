#include "V4l2Capture.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <opencv2/opencv.hpp>


V4L2Capture::V4L2Capture(const std::string &device) : device_name(device), fd(-1) {
    setupDevice();
}

V4L2Capture::~V4L2Capture() {
    stopCapture();
}

void V4L2Capture::setupDevice() {
    fd = open(device_name.c_str(), O_RDWR);
    if (fd == -1) {
        perror("Opening video device");
        exit(1);
    }

    // Get device capabilities
    struct v4l2_capability cap;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        perror("Querying Capabilities");
        exit(1);
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        std::cerr << "Device does not support video capture." << std::endl;
        exit(1);
    }

    // Set the video format
    struct v4l2_format fmt;
    std::memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 640;  // Set desired width
    fmt.fmt.pix.height = 480; // Set desired height
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // Pixel format
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("Setting Pixel Format");
        exit(1);
    }

    // Request buffers
    struct v4l2_requestbuffers req;
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        perror("Requesting Buffers");
        exit(1);
    }

    // Map buffers
    struct v4l2_buffer v4l2_buf;
    std::memset(&v4l2_buf, 0, sizeof(v4l2_buf));
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
    v4l2_buf.index = 0;

    if (ioctl(fd, VIDIOC_QUERYBUF, &v4l2_buf) == -1) {
        perror("Querying Buffer");
        exit(1);
    }

    buf.length = v4l2_buf.length;
    buf.start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, v4l2_buf.m.offset);
    if (buf.start == MAP_FAILED) {
        perror("Mapping Buffer");
        exit(1);
    }

    // Queue the buffer
    if (ioctl(fd, VIDIOC_QBUF, &v4l2_buf) == -1) {
        perror("Queueing Buffer");
        exit(1);
    }
}

void V4L2Capture::startCapture() {
    struct v4l2_requestbuffers req;
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_STREAMON, &req.type) == -1) {
        perror("Starting Capture");
        exit(1);
    }
}

void V4L2Capture::stopCapture() {
    munmap(buf.start, buf.length);
    close(fd);
}

void V4L2Capture::captureAndDisplay() {
    startCapture();

    // Dequeue the buffer to capture the image
    struct v4l2_buffer v4l2_buf;
    std::memset(&v4l2_buf, 0, sizeof(v4l2_buf));
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
    v4l2_buf.index = 0;

    if (ioctl(fd, VIDIOC_DQBUF, &v4l2_buf) == -1) {
        perror("Dequeueing Buffer");
        exit(1);
    }

    // Display the captured frame using OpenCV
    cv::Mat img(480, 640, CV_8UC2, buf.start); // YUYV format
    cv::Mat rgb_img;
    cv::cvtColor(img, rgb_img, cv::COLOR_YUV2BGR_YUYV); // Convert YUYV to BGR for display
    cv::imshow("Captured Frame", rgb_img);
    cv::waitKey(0);

    stopCapture();
}
