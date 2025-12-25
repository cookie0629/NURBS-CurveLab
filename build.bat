@echo off
REM NURBS-CurveLab Windows构建脚本
REM 自动化编译和构建过程

echo ========================================
echo    NURBS-CurveLab 构建脚本
echo    Windows Visual Studio 构建
echo ========================================
echo.

REM 检查是否存在OpenCASCADE环境变量
if not defined OPENCASCADE_ROOT (
    echo 错误: 未找到OPENCASCADE_ROOT环境变量
    echo 请设置OpenCASCADE安装路径,例如:
    echo set OPENCASCADE_ROOT=D:\OpenCASCADE-7.6.0-vc14-64\opencascade-7.6.0
    pause
    exit /b 1
)

echo 使用OpenCASCADE路径: %OPENCASCADE_ROOT%
echo.

REM 创建构建目录
if not exist build mkdir build
cd build

REM 配置CMake
echo 正在配置CMake...
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_PREFIX_PATH="%OPENCASCADE_ROOT%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX="%CD%\install"

if %ERRORLEVEL% neq 0 (
    echo CMake配置失败!
    pause
    exit /b 1
)

echo.
echo CMake配置成功!
echo.

REM 编译项目
echo 正在编译项目...
cmake --build . --config Release --parallel

if %ERRORLEVEL% neq 0 (
    echo 编译失败!
    pause
    exit /b 1
)

echo.
echo 编译成功!
echo.

REM 安装项目
echo 正在安装项目...
cmake --install . --config Release

if %ERRORLEVEL% neq 0 (
    echo 安装失败!
    pause
    exit /b 1
)

echo.
echo ========================================
echo    构建完成!
echo ========================================
echo.
echo 可执行文件位置:
echo   主程序: %CD%\bin\Release\NURBS-CurveLab.exe
echo   曲线示例: %CD%\bin\Release\BasicCurveExample.exe
echo   曲面示例: %CD%\bin\Release\SurfaceModelingExample.exe
echo.
echo 安装文件位置: %CD%\install
echo.

REM 询问是否运行主程序
set /p choice="是否运行主程序? (y/n): "
if /i "%choice%"=="y" (
    echo.
    echo 启动NURBS-CurveLab...
    echo.
    bin\Release\NURBS-CurveLab.exe
)

cd ..
pause