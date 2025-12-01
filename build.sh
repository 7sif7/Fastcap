#!/bin/bash

g++ -o fast_capture main.cpp V4l2Capture.cpp `pkg-config --cflags --libs opencv4` -lv4l2

echo Done!
