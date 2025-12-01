#ifndef V4L2CAPTURE_H
#define V4L2CAPTURE_H

#include <string>

class V4L2Capture {
public:
    V4L2Capture(const std::string &device);
    ~V4L2Capture();
    void captureAndDisplay();

private:
    std::string device_name;
    int fd;
    struct buffer {
        void *start;
        size_t length;
    };
    buffer buf;
    void setupDevice();
    void startCapture();
    void stopCapture();
};

#endif // V4L2CAPTURE_H
