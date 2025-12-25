#pragma once

/**
 * NURBS-CurveLab 项目配置文件
 * 定义项目的全局常量和配置参数
 */

// 项目版本信息
#define NURBS_CURVELAB_VERSION_MAJOR 1
#define NURBS_CURVELAB_VERSION_MINOR 0
#define NURBS_CURVELAB_VERSION_PATCH 0
#define NURBS_CURVELAB_VERSION_STRING "1.0.0"

// 项目信息
#define NURBS_CURVELAB_NAME "NURBS-CurveLab"
#define NURBS_CURVELAB_DESCRIPTION "NURBS曲线和曲面建模工具"
#define NURBS_CURVELAB_AUTHOR "OpenCASCADE用户"
#define NURBS_CURVELAB_COPYRIGHT "Copyright (c) 2024"

// 默认几何参数
namespace NurbsConfig {
    // 默认精度设置
    constexpr double DEFAULT_TOLERANCE = 1e-6;          // 默认几何容差
    constexpr double DEFAULT_ANGULAR_TOLERANCE = 1e-6;  // 默认角度容差
    constexpr double CURVE_SAMPLING_TOLERANCE = 1e-3;   // 曲线采样容差
    constexpr double SURFACE_FITTING_TOLERANCE = 1e-3;  // 曲面拟合容差
    
    // 默认NURBS参数
    constexpr int DEFAULT_CURVE_DEGREE = 3;             // 默认曲线次数
    constexpr int DEFAULT_SURFACE_U_DEGREE = 3;         // 默认曲面U方向次数
    constexpr int DEFAULT_SURFACE_V_DEGREE = 3;         // 默认曲面V方向次数
    constexpr int MIN_CONTROL_POINTS = 2;               // 最小控制点数量
    constexpr int MAX_CURVE_DEGREE = 25;                // 最大曲线次数
    constexpr int MAX_SURFACE_DEGREE = 25;              // 最大曲面次数
    
    // 数值计算参数
    constexpr int DEFAULT_ARC_LENGTH_SEGMENTS = 100;    // 弧长计算默认分段数
    constexpr int DEFAULT_CURVE_SAMPLES = 50;           // 曲线默认采样点数
    constexpr int MAX_ITERATION_COUNT = 1000;           // 迭代算法最大次数
    constexpr double CONVERGENCE_THRESHOLD = 1e-8;      // 收敛阈值
    
    // 文件格式参数
    constexpr double STEP_EXPORT_PRECISION = 1e-6;      // STEP导出精度
    constexpr double IGES_EXPORT_PRECISION = 1e-6;      // IGES导出精度
    constexpr const char* DEFAULT_STEP_UNITS = "MM";    // 默认STEP单位
    constexpr const char* DEFAULT_IGES_UNITS = "MM";    // 默认IGES单位
    
    // 内存管理参数
    constexpr size_t MAX_CONTROL_POINTS_PER_CURVE = 10000;    // 单条曲线最大控制点数
    constexpr size_t MAX_CONTROL_POINTS_PER_SURFACE = 1000000; // 单个曲面最大控制点数
    constexpr size_t DEFAULT_VECTOR_RESERVE_SIZE = 100;        // 向量默认预留大小
    
    // 调试和日志参数
    constexpr bool ENABLE_DEBUG_OUTPUT = false;         // 启用调试输出
    constexpr bool ENABLE_PERFORMANCE_TIMING = false;   // 启用性能计时
    constexpr bool ENABLE_MEMORY_TRACKING = false;      // 启用内存跟踪
}

// 错误代码定义
namespace NurbsErrorCodes {
    constexpr int SUCCESS = 0;                           // 成功
    constexpr int INVALID_INPUT = -1;                    // 无效输入
    constexpr int INSUFFICIENT_POINTS = -2;             // 控制点不足
    constexpr int INVALID_DEGREE = -3;                  // 无效次数
    constexpr int INVALID_WEIGHTS = -4;                 // 无效权重
    constexpr int INVALID_KNOT_VECTOR = -5;             // 无效节点向量
    constexpr int CURVE_CREATION_FAILED = -6;           // 曲线创建失败
    constexpr int SURFACE_CREATION_FAILED = -7;         // 曲面创建失败
    constexpr int FILE_IO_ERROR = -8;                   // 文件IO错误
    constexpr int OPENCASCADE_ERROR = -9;               // OpenCASCADE错误
    constexpr int MEMORY_ALLOCATION_ERROR = -10;        // 内存分配错误
}

// 实用宏定义
#define NURBS_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            throw std::runtime_error("断言失败: " message); \
        } \
    } while(0)

#define NURBS_LOG_DEBUG(message) \
    do { \
        if (NurbsConfig::ENABLE_DEBUG_OUTPUT) { \
            std::cout << "[DEBUG] " << message << std::endl; \
        } \
    } while(0)

#define NURBS_LOG_INFO(message) \
    std::cout << "[INFO] " << message << std::endl

#define NURBS_LOG_WARNING(message) \
    std::cout << "[WARNING] " << message << std::endl

#define NURBS_LOG_ERROR(message) \
    std::cerr << "[ERROR] " << message << std::endl

// 版本检查宏
#define NURBS_VERSION_CHECK(major, minor, patch) \
    ((NURBS_CURVELAB_VERSION_MAJOR > (major)) || \
     (NURBS_CURVELAB_VERSION_MAJOR == (major) && NURBS_CURVELAB_VERSION_MINOR > (minor)) || \
     (NURBS_CURVELAB_VERSION_MAJOR == (major) && NURBS_CURVELAB_VERSION_MINOR == (minor) && NURBS_CURVELAB_VERSION_PATCH >= (patch)))

// 平台特定配置
#ifdef _WIN32
    #define NURBS_PLATFORM_WINDOWS
    #define NURBS_PATH_SEPARATOR "\\"
#elif defined(__linux__)
    #define NURBS_PLATFORM_LINUX
    #define NURBS_PATH_SEPARATOR "/"
#elif defined(__APPLE__)
    #define NURBS_PLATFORM_MACOS
    #define NURBS_PATH_SEPARATOR "/"
#else
    #define NURBS_PLATFORM_UNKNOWN
    #define NURBS_PATH_SEPARATOR "/"
#endif

// 编译器特定配置
#ifdef _MSC_VER
    #define NURBS_COMPILER_MSVC
    #pragma warning(push)
    #pragma warning(disable: 4996) // 禁用不安全函数警告
#elif defined(__GNUC__)
    #define NURBS_COMPILER_GCC
#elif defined(__clang__)
    #define NURBS_COMPILER_CLANG
#endif