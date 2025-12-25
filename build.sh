#!/bin/bash

# NURBS-CurveLab Linux/macOS构建脚本
# 自动化编译和构建过程

echo "========================================"
echo "    NURBS-CurveLab 构建脚本"
echo "    Linux/macOS 构建"
echo "========================================"
echo

# 检查必要的工具
check_command() {
    if ! command -v $1 &> /dev/null; then
        echo "错误: 未找到 $1 命令"
        echo "请安装 $1 后重试"
        exit 1
    fi
}

echo "检查构建工具..."
check_command cmake
check_command make
check_command g++

# 检查OpenCASCADE
if [ -z "$OPENCASCADE_ROOT" ]; then
    echo "警告: 未设置OPENCASCADE_ROOT环境变量"
    echo "尝试使用系统默认路径..."
    
    # 常见的OpenCASCADE安装路径
    POSSIBLE_PATHS=(
        "/usr/local"
        "/opt/opencascade"
        "/usr"
        "$HOME/opencascade"
    )
    
    for path in "${POSSIBLE_PATHS[@]}"; do
        if [ -d "$path/include/opencascade" ] || [ -d "$path/inc" ]; then
            export OPENCASCADE_ROOT="$path"
            echo "找到OpenCASCADE: $OPENCASCADE_ROOT"
            break
        fi
    done
    
    if [ -z "$OPENCASCADE_ROOT" ]; then
        echo "错误: 未找到OpenCASCADE安装"
        echo "请设置OPENCASCADE_ROOT环境变量或安装OpenCASCADE"
        exit 1
    fi
else
    echo "使用OpenCASCADE路径: $OPENCASCADE_ROOT"
fi

echo

# 创建构建目录
echo "创建构建目录..."
mkdir -p build
cd build

# 配置CMake
echo "正在配置CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$OPENCASCADE_ROOT" \
    -DCMAKE_INSTALL_PREFIX="$(pwd)/install" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [ $? -ne 0 ]; then
    echo "CMake配置失败!"
    exit 1
fi

echo
echo "CMake配置成功!"
echo

# 编译项目
echo "正在编译项目..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    echo "编译失败!"
    exit 1
fi

echo
echo "编译成功!"
echo

# 安装项目
echo "正在安装项目..."
make install

if [ $? -ne 0 ]; then
    echo "安装失败!"
    exit 1
fi

echo
echo "========================================"
echo "    构建完成!"
echo "========================================"
echo
echo "可执行文件位置:"
echo "  主程序: $(pwd)/bin/NURBS-CurveLab"
echo "  曲线示例: $(pwd)/bin/BasicCurveExample"
echo "  曲面示例: $(pwd)/bin/SurfaceModelingExample"
echo
echo "安装文件位置: $(pwd)/install"
echo

# 询问是否运行主程序
read -p "是否运行主程序? (y/n): " choice
case "$choice" in 
    y|Y|yes|YES ) 
        echo
        echo "启动NURBS-CurveLab..."
        echo
        ./bin/NURBS-CurveLab
        ;;
    * ) 
        echo "构建完成。"
        ;;
esac

cd ..