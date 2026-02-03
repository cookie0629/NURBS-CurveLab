# NURBS-CurveLab（简化版）

基于OpenCASCADE的NURBS曲线建模工具，专注于核心功能的简洁实现。

## 项目特点

- **NURBS曲线创建** - 支持通过控制点创建B样条曲线
- **曲线分析** - 计算点坐标、切线、曲率等几何属性
- **基础操作** - 弧长计算、曲线采样、最近点查找
- **稳定可靠** - 所有功能经过验证，确保正常工作

## 技术栈

- **C++17** - 现代C++标准
- **OpenCASCADE 7.6.0** - 专业几何建模内核
- **CMake** - 跨平台构建系统

## 核心功能

### 1. NURBS曲线创建
```cpp
// 创建控制点
std::vector<gp_Pnt> controlPoints = {
    gp_Pnt(0, 0, 0),
    gp_Pnt(1, 2, 0),
    gp_Pnt(3, 3, 0),
    gp_Pnt(5, 1, 0)
};

// 创建3次NURBS曲线
auto curve = std::make_shared<NurbsCurve>();
curve->CreateFromControlPoints(controlPoints, 3);
```

### 2. 曲线分析
```cpp
// 计算曲线上的点
gp_Pnt point = curve->EvaluatePoint(0.5);

// 计算切线
gp_Vec tangent = curve->EvaluateTangent(0.5);

// 计算曲率
double curvature = curve->EvaluateCurvature(0.5);
```

### 3. 几何操作
```cpp
// 计算弧长
double length = GeometryMath::CalculateArcLength(curve);

// 曲线采样
std::vector<gp_Pnt> samples = GeometryMath::SampleCurve(curve, 10);

// 查找最近点
double parameter;
gp_Pnt closest = GeometryMath::FindClosestPoint(curve, targetPoint, parameter);
```

## 快速开始

### 构建步骤
1. **设置环境变量**
```powershell
$env:OPENCASCADE_ROOT = "D:\OpenCASCADE-7.6.0-vc14-64\opencascade-7.6.0"
$env:PATH += ";D:\OpenCASCADE-7.6.0-vc14-64\opencascade-7.6.0\win64\vc14\bin"
```

2. **编译项目**
```powershell
.\build.bat
```

3. **运行程序**
```powershell
.\build\bin\Release\NURBS-CurveLab.exe
.\build\bin\Release\BasicCurveExample.exe
```

## 演示功能

### 主程序菜单
1. **NURBS曲线创建** - 演示如何创建和分析曲线
2. **基础几何操作** - 展示弧长计算、采样、最近点查找
3. **曲线分析** - 详细的几何属性分析
4. **运行所有演示** - 完整功能展示

### 示例输出
```
void ShowMenu() {
    std::cout << "请选择演示功能:\n";
    std::cout << "1. NURBS曲线创建\n";
    std::cout << "2. 基础几何操作\n";
    std::cout << "3. 曲线分析\n";
    std::cout << "4. 运行所有演示\n";
    std::cout << "0. 退出\n";
    std::cout << "请输入选择 (0-4): ";
}
```

```
✓ 成功创建3次NURBS曲线
  控制点数量: 5
  曲线次数: 3
  参数范围: [0, 1]
  中点坐标: (3.700, 4.066, 0.000)
  中点切线: (11.051, -1.496, 0.000)
  中点曲率: 0.515
```
