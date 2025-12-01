#include <iostream>
#include "V4l2Capture.h"

int main() {
    
    V4L2Capture capture("/dev/video0");
    capture.captureAndDisplay();
    return 0;
}
