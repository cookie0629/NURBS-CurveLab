# NURBS-CurveLab CMake配置文件（简化版）
# 专注于NURBS曲线建模的核心功能

cmake_minimum_required(VERSION 3.16)

# 项目信息
project(NURBS-CurveLab 
    VERSION 1.0.0
    DESCRIPTION "NURBS曲线建模工具（简化版）"
    LANGUAGES CXX)

# C++标准设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    # Visual Studio编译器选项
    add_compile_options(/W4 /WX- /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang编译器选项
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 手动设置OpenCASCADE路径
set(OpenCASCADE_DIR "$ENV{OPENCASCADE_ROOT}")
set(OpenCASCADE_INCLUDE_DIR "${OpenCASCADE_DIR}/inc")
set(OpenCASCADE_LIBRARY_DIR "${OpenCASCADE_DIR}/win64/vc14/lib")

if(NOT EXISTS ${OpenCASCADE_INCLUDE_DIR})
    message(FATAL_ERROR "未找到OpenCASCADE包含目录: ${OpenCASCADE_INCLUDE_DIR}")
endif()

if(NOT EXISTS ${OpenCASCADE_LIBRARY_DIR})
    message(FATAL_ERROR "未找到OpenCASCADE库目录: ${OpenCASCADE_LIBRARY_DIR}")
endif()

message(STATUS "OpenCASCADE包含目录: ${OpenCASCADE_INCLUDE_DIR}")
message(STATUS "OpenCASCADE库目录: ${OpenCASCADE_LIBRARY_DIR}")

# 包含目录
include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 核心源文件列表（只保留曲线相关）
set(CORE_SOURCES
    src/core/NurbsCurve.cpp
    src/core/GeometryMath.cpp
)

set(HEADER_FILES
    src/core/NurbsCurve.h
    src/core/GeometryMath.h
)

# 创建核心库
add_library(NurbsCore STATIC 
    ${CORE_SOURCES} 
    ${HEADER_FILES}
)

# 设置库目录
link_directories(${OpenCASCADE_LIBRARY_DIR})

# 链接必要的OpenCASCADE库（简化版，只保留曲线相关）
target_link_libraries(NurbsCore 
    TKernel TKMath TKG2d TKG3d TKGeomBase TKGeomAlgo
    TKBRep TKPrim TKTopAlgo
)

# 主程序
add_executable(NURBS-CurveLab 
    NURBS-CurveLab/NURBS-CurveLab.cpp
)

target_link_libraries(NURBS-CurveLab NurbsCore)

# 基础曲线示例程序
add_executable(BasicCurveExample 
    examples/basic_curve_example.cpp
)
target_link_libraries(BasicCurveExample NurbsCore)

# 设置输出目录
set_target_properties(NURBS-CurveLab BasicCurveExample
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 显示配置信息
message(STATUS "")
message(STATUS "NURBS-CurveLab配置完成:")
message(STATUS "  版本: ${PROJECT_VERSION}")
message(STATUS "  编译器: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "  构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "  功能: 专注于NURBS曲线建模")
message(STATUS "")