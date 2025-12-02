# ESIM (Fork) — Modern Build Support (ROS Noetic + Pixi + WSL2)

This repository is a **modernized fork** of the original  
**ESIM: an Open Event Camera Simulator**, updated so the entire ESIM/imp/ze ecosystem builds and runs cleanly on current systems.

This fork focuses **only on build/compatibility updates**.  
All simulation algorithms remain identical to the original ESIM.

---

## ✔ What’s New in This Fork

This repository adds:

- **ROS Noetic** compatibility (original ESIM targeted Kinetic)
- **Pixi-based reproducible development environment**
- Verified support for **WSL2 (Ubuntu 24.04)**
- Updates for modern toolchains (GCC 12–14, CMake ≥ 3.22)
- Small fixes for deprecated APIs and missing includes required by newer compilers


## 📦 Environment Setup (Pixi)

Install pixi:
```bash
curl -fsSL https://pixi.sh/install.sh | bash
```

Activate environment:
```bash
pixi shell
```

## Build
Set catkin config
```bash
catkin config --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=14
```

Build package
```bash
catkin build esim_ros
```
