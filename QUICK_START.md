# NURBS-CurveLab 快速入门指南

## 🚀 5分钟快速开始

### 前置要求

1. **OpenCASCADE 7.6.0+** - 几何建模内核
2. **Visual Studio 2019+** (Windows) 或 **GCC 9+** (Linux/macOS)
3. **CMake 3.16+** - 构建系统

### Windows 快速构建

```batch
# 1. 设置OpenCASCADE环境变量
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

## 📝 基础使用示例

### 创建NURBS曲线

```cpp
#include "src/core/NurbsCurve.h"

// 定义控制点
std::vector<gp_Pnt> points = {
    gp_Pnt(0, 0, 0),
    gp_Pnt(1, 2, 0),
    gp_Pnt(3, 3, 0),
    gp_Pnt(5, 1, 0)
};

// 创建3次NURBS曲线
auto curve = std::make_shared<NurbsCurve>();
curve->CreateFromControlPoints(points, 3);

// 计算曲线上的点
gp_Pnt point = curve->EvaluatePoint(0.5);
gp_Vec tangent = curve->EvaluateTangent(0.5);
double curvature = curve->EvaluateCurvature(0.5);
```

### 创建NURBS曲面

```cpp
#include "src/core/NurbsSurface.h"

// 创建4x4控制点网格
std::vector<std::vector<gp_Pnt>> controlPoints(4, std::vector<gp_Pnt>(4));
for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        controlPoints[i][j] = gp_Pnt(i, j, sin(i*0.5)*cos(j*0.5));
    }
}

// 创建双三次NURBS曲面
auto surface = std::make_shared<NurbsSurface>();
surface->CreateFromControlPoints(controlPoints, 3, 3);

// 计算曲面上的点和法向量
gp_Pnt point = surface->EvaluatePoint(0.5, 0.5);
gp_Vec normal = surface->EvaluateNormal(0.5, 0.5);
```

### 导出到STEP文件

```cpp
#include "src/io/StepExporter.h"

StepExporter exporter;
exporter.SetAuthor("Your Name");
exporter.AddCurve(curve, "MyCurve");
exporter.AddSurface(surface, "MySurface");
exporter.ExportToFile("model.step");
```

## 🎯 运行演示程序

程序提供交互式菜单：

```
========================================
    NURBS-CurveLab v1.0
    NURBS曲线和曲面建模工具
    基于OpenCASCADE 7.6.0
========================================

请选择演示功能:
1. NURBS曲线创建
2. NURBS曲面创建
3. 几何操作
4. 文件输入输出
5. 运行所有演示
0. 退出
```

## 🔧 常见问题

### Q: 编译时找不到OpenCASCADE头文件
**A:** 确保设置了正确的OPENCASCADE_ROOT环境变量，指向OpenCASCADE安装目录。

### Q: 链接时出现库文件错误
**A:** 检查OpenCASCADE库文件路径，确保所有必需的.lib文件都存在。

### Q: 运行时出现DLL错误
**A:** 将OpenCASCADE的bin目录添加到系统PATH环境变量中。

### Q: CMake配置失败
**A:** 确保CMake版本≥3.16，并正确设置CMAKE_PREFIX_PATH。

## 📚 学习资源

### 项目文件结构
```
NURBS-CurveLab/
├── src/core/           # 核心几何算法
├── src/io/             # 文件输入输出
├── examples/           # 使用示例
├── config/             # 项目配置
└── docs/               # 文档（待添加）
```

### 关键类说明
- **NurbsCurve** - NURBS曲线的完整实现
- **NurbsSurface** - NURBS曲面的完整实现
- **GeometryMath** - 几何数学工具集
- **StepExporter** - STEP文件导出器
- **ModelLoader** - 模型文件加载器

### 下一步学习
1. 阅读 `examples/` 目录下的示例代码
2. 查看 `src/core/` 中的核心实现
3. 尝试修改参数观察效果
4. 扩展新的几何操作功能

## 🎉 开始探索

现在你已经准备好开始探索NURBS几何建模的世界了！

- 运行演示程序了解基本功能
- 查看示例代码学习API使用
- 阅读源码理解实现原理
- 尝试创建自己的几何模型

祝你使用愉快！ 🚀