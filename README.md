# NURBS-CurveLab

基于OpenCASCADE的NURBS曲线和曲面建模工具，专为简历展示和学习目的设计。

## 项目概述

NURBS-CurveLab是一个完整的NURBS几何建模工具，展示了现代C++和OpenCASCADE在几何建模领域的应用。项目实现了从基础NURBS数学到高级几何操作的完整功能链。

## 核心功能

### NURBS曲线建模
- **创建与编辑**: 支持通过控制点、权重、节点向量创建NURBS曲线
- **参数化设计**: 完整的参数化曲线编辑功能
- **几何分析**: 曲率、切线、法向量计算
- **高级操作**: 曲线延伸、修剪、升降阶

### NURBS曲面建模
- **双参数曲面**: 支持U、V方向独立控制的曲面创建
- **曲面分析**: 高斯曲率、平均曲率、主曲率计算
- **几何操作**: 曲面延伸、等参线提取
- **连续性检查**: C0、C1、C2连续性验证

### 几何数学工具
- **曲线拟合**: 点集到NURBS曲线的最优拟合
- **曲面拟合**: 点网格到NURBS曲面的拟合
- **几何查询**: 最近点查找、交点计算
- **弧长计算**: 精确和近似弧长计算

### 文件输入输出
- **STEP导出**: 标准STEP格式几何导出
- **IGES支持**: IGES文件格式读写
- **模型加载**: 从CAD文件提取NURBS几何


## 技术特点

### 现代C++设计
- **RAII资源管理**: 智能指针和自动资源管理
- **异常安全**: 完整的异常处理机制
- **STL容器**: 高效的数据结构使用
- **模板编程**: 泛型算法实现

### OpenCASCADE集成
- **深度集成**: 充分利用OpenCASCADE几何内核
- **标准兼容**: 支持工业标准文件格式
- **精度控制**: 可配置的计算精度

### 数学算法
- **B样条基础**: 完整的B样条数学实现
- **节点向量**: 灵活的节点向量生成和操作
- **权重控制**: NURBS权重的精确控制
- **连续性**: 几何连续性的数学验证

## 编译要求

### 环境配置
- **编译器**: Visual Studio 2019+ (MSVC v143)
- **OpenCASCADE**: 7.6.0或更高版本
- **平台**: Windows 10/11 x64

### 依赖库
```
OpenCASCADE模块:
- TKernel, TKMath          # 核心数学库
- TKBRep, TKGeomBase       # 几何表示
- TKGeomAlgo, TKG2d, TKG3d # 几何算法
- TKSTEP, TKIGES           # 文件格式支持
- TKTopAlgo, TKBO          # 拓扑算法
```

## 使用示例

### 创建NURBS曲线
```cpp
// 定义控制点
std::vector<gp_Pnt> controlPoints = {
    gp_Pnt(0, 0, 0),
    gp_Pnt(1, 2, 0),
    gp_Pnt(3, 3, 0),
    gp_Pnt(5, 1, 0)
};

// 创建3次NURBS曲线
auto curve = std::make_shared<NurbsCurve>();
curve->CreateFromControlPoints(controlPoints, 3);

// 分析曲线
double param = 0.5;
gp_Pnt point = curve->EvaluatePoint(param);
gp_Vec tangent = curve->EvaluateTangent(param);
double curvature = curve->EvaluateCurvature(param);
```

### 创建NURBS曲面
```cpp
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

// 曲面分析
gp_Pnt centerPoint = surface->EvaluatePoint(0.5, 0.5);
gp_Vec normal = surface->EvaluateNormal(0.5, 0.5);
double gaussCurvature = surface->EvaluateGaussianCurvature(0.5, 0.5);
```

### 文件导出
```cpp
// 导出到STEP文件
StepExporter exporter;
exporter.SetAuthor("NURBS-CurveLab");
exporter.AddCurve(curve, "MyCurve");
exporter.AddSurface(surface, "MySurface");
exporter.ExportToFile("model.step");
```

## 运行演示

编译并运行程序后，您将看到交互式菜单：

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


## 联系信息

这个项目展示了在几何建模、C++开发和OpenCASCADE应用方面的专业能力。如果您对项目有任何问题或建议，欢迎交流讨论。
