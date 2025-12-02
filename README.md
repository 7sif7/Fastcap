# Fastcap

**Fastcap** is a lightweight, high-performance **C++ video capture utility** built on top of the Linux **V4L2 (Video4Linux2)** subsystem.

It provides **direct, low-level access** to video capture devices while prioritizing:

- ⚡ **Speed**
- 🪶 **Simplicity**
- 🎯 **Minimal overhead**

Fastcap is ideal for **small Linux-based embedded systems** and **computers with minimal or no GUI support**, where traditional multimedia frameworks (e.g., GStreamer, FFmpeg) are too heavy.

---

## ✨ Features

- ⚡ High-performance video capture  
- 🔧 Direct V4L2 access without unnecessary abstraction  
- 📦 Minimal external dependencies  
- 🧩 Designed for embedded Linux devices (ARM boards, SBCs, industrial modules)  
- 🖥️ Console-based — **no GUI libraries required**  
- 🪶 Compact, easy-to-integrate codebase  

---

## 🛠️ Use Cases

- Embedded camera modules  
- Machine vision and industrial automation  
- Lightweight robotics systems  
- Custom multimedia pipelines  
- Resource-constrained devices (low CPU/RAM)  

---

## 📦 Requirements

- Linux system with **V4L2** support  
- A C++ compiler (e.g., `g++`)  
- A V4L2-compatible video device (e.g., `/dev/video0`)
- OpenCV4

---
 🏗️ Build instructions
 - Just run the build.sh script
