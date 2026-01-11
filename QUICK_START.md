# NURBS-CurveLab 快速入门指南

## 5分钟快速开始

### 前置要求

1. **OpenCASCADE 7.6.0+** - 几何建模内核
2. **Visual Studio 2019+** (Windows) 或 **GCC 9+** (Linux/macOS)
3. **CMake 3.16+** - 构建系统

### Windows 快速构建

```batch
# 1. 设置OpenCASCADE环境变量(自己电脑上的位置)
set OPENCASCADE_ROOT=D:\OpenCASCADE-7.6.0-vc14-64\opencascade-7.6.0

# 2. 运行构建脚本
build.bat

# 3. 运行程序
build\bin\Release\NURBS-CurveLab.exe
```

### Linux/macOS 快速构建

```bash
# 1. 设置OpenCASCADE环境变量
export OPENCASCADE_ROOT=/usr/local

# 2. 运行构建脚本
chmod +x build.sh
./build.sh

# 3. 运行程序
build/bin/NURBS-CurveLab
```
