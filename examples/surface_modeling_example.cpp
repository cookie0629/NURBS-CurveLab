/**
 * NURBS曲面建模示例
 * 演示如何创建、编辑和分析NURBS曲面
 */

#include "../src/core/NurbsSurface.h"
#include "../src/core/GeometryMath.h"
#include <iostream>
#include <vector>
#include <cmath>

void CreateWaveSurface() {
    std::cout << "=== 创建波浪形NURBS曲面 ===\n\n";
    
    // 1. 创建控制点网格 (5x5)
    std::cout << "1. 创建5x5控制点网格...\n";
    const int uCount = 5, vCount = 5;
    std::vector<std::vector<gp_Pnt>> controlPoints(uCount, std::vector<gp_Pnt>(vCount));
    
    // 生成波浪形控制点
    for (int i = 0; i < uCount; ++i) {
        for (int j = 0; j < vCount; ++j) {
            double x = static_cast<double>(i) * 2.0;  // X方向范围: 0-8
            double y = static_cast<double>(j) * 2.0;  // Y方向范围: 0-8
            // Z坐标创建波浪效果
            double z = std::sin(x * 0.3) * std::cos(y * 0.3) * 2.0;
            
            controlPoints[i][j] = gp_Pnt(x, y, z);
            std::cout << "  控制点[" << i << "][" << j << "]: (" 
                      << x << ", " << y << ", " << std::fixed << std::setprecision(3) << z << ")\n";
        }
    }
    
    // 2. 创建NURBS曲面
    std::cout << "\n2. 创建双三次NURBS曲面...\n";
    auto surface = std::make_shared<NurbsSurface>();
    
    if (surface->CreateFromControlPoints(controlPoints, 3, 3)) {
        std::cout << "  曲面创建成功\n";
        std::cout << "  - U方向控制点数: " << surface->GetUControlPointCount() << "\n";
        std::cout << "  - V方向控制点数: " << surface->GetVControlPointCount() << "\n";
        std::cout << "  - U方向次数: " << surface->GetUDegree() << "\n";
        std::cout << "  - V方向次数: " << surface->GetVDegree() << "\n";
        
        // 获取参数范围
        double uMin, uMax, vMin, vMax;
        surface->GetUParameterRange(uMin, uMax);
        surface->GetVParameterRange(vMin, vMax);
        std::cout << "  - U参数范围: [" << uMin << ", " << uMax << "]\n";
        std::cout << "  - V参数范围: [" << vMin << ", " << vMax << "]\n";
    } else {
        std::cout << "  曲面创建失败\n";
        return;
    }
    
    // 3. 曲面分析
    std::cout << "\n3. 曲面几何分析...\n";
    
    // 分析网格上的关键点
    std::vector<std::pair<double, double>> testParams = {
        {0.0, 0.0}, {0.5, 0.0}, {1.0, 0.0},  // U方向边界
        {0.0, 0.5}, {0.5, 0.5}, {1.0, 0.5},  // 中间行
        {0.0, 1.0}, {0.5, 1.0}, {1.0, 1.0}   // V方向边界
    };
    
    double uMin, uMax, vMin, vMax;
    surface->GetUParameterRange(uMin, uMax);
    surface->GetVParameterRange(vMin, vMax);
    
    for (const auto& param : testParams) {
        double u = uMin + param.first * (uMax - uMin);
        double v = vMin + param.second * (vMax - vMin);
        
        // 计算曲面上的点和几何信息
        gp_Pnt point = surface->EvaluatePoint(u, v);
        gp_Vec normal = surface->EvaluateNormal(u, v);
        gp_Vec uTangent = surface->EvaluateUTangent(u, v);
        gp_Vec vTangent = surface->EvaluateVTangent(u, v);
        
        // 曲率分析
        double gaussCurvature = surface->EvaluateGaussianCurvature(u, v);
        double meanCurvature = surface->EvaluateMeanCurvature(u, v);
        double k1, k2;
        surface->EvaluatePrincipalCurvatures(u, v, k1, k2);
        
        std::cout << "  参数 (u=" << param.first << ", v=" << param.second << "):\n";
        std::cout << "    点坐标: (" << std::fixed << std::setprecision(3)
                  << point.X() << ", " << point.Y() << ", " << point.Z() << ")\n";
        std::cout << "    法向量: (" << normal.X() << ", " << normal.Y() << ", " << normal.Z() << ")\n";
        std::cout << "    U切线: (" << uTangent.X() << ", " << uTangent.Y() << ", " << uTangent.Z() << ")\n";
        std::cout << "    V切线: (" << vTangent.X() << ", " << vTangent.Y() << ", " << vTangent.Z() << ")\n";
        std::cout << "    高斯曲率: " << gaussCurvature << "\n";
        std::cout << "    平均曲率: " << meanCurvature << "\n";
        std::cout << "    主曲率: k1=" << k1 << ", k2=" << k2 << "\n\n";
    }
    
    // 4. 曲面属性检查
    std::cout << "4. 曲面属性检查...\n";
    std::cout << "  - U方向闭合: " << (surface->IsUClosed() ? "是" : "否") << "\n";
    std::cout << "  - V方向闭合: " << (surface->IsVClosed() ? "是" : "否") << "\n";
    std::cout << "  - U方向周期性: " << (surface->IsUPeriodic() ? "是" : "否") << "\n";
    std::cout << "  - V方向周期性: " << (surface->IsVPeriodic() ? "是" : "否") << "\n";
    
    std::cout << "\n=== 波浪曲面示例完成 ===\n";
}

void CreateControlledSurface() {
    std::cout << "\n=== 创建带权重的NURBS曲面 ===\n\n";
    
    // 1. 创建3x3控制点网格
    std::cout << "1. 创建3x3控制点网格和权重...\n";
    const int uCount = 3, vCount = 3;
    std::vector<std::vector<gp_Pnt>> controlPoints(uCount, std::vector<gp_Pnt>(vCount));
    std::vector<std::vector<double>> weights(uCount, std::vector<double>(vCount));
    
    // 创建简单的控制点网格
    for (int i = 0; i < uCount; ++i) {
        for (int j = 0; j < vCount; ++j) {
            double x = static_cast<double>(i) * 3.0;
            double y = static_cast<double>(j) * 3.0;
            double z = (i == 1 && j == 1) ? 4.0 : 0.0;  // 中心点抬高
            
            controlPoints[i][j] = gp_Pnt(x, y, z);
            
            // 设置权重：中心点权重更大，形成更强的吸引力
            weights[i][j] = (i == 1 && j == 1) ? 3.0 : 1.0;
            
            std::cout << "  控制点[" << i << "][" << j << "]: (" 
                      << x << ", " << y << ", " << z << "), 权重: " << weights[i][j] << "\n";
        }
    }
    
    // 2. 创建带权重的NURBS曲面
    std::cout << "\n2. 创建带权重的双二次NURBS曲面...\n";
    auto surface = std::make_shared<NurbsSurface>();
    
    if (surface->CreateWithWeights(controlPoints, weights, 2, 2)) {
        std::cout << "  带权重曲面创建成功\n";
        
        // 3. 权重效果分析
        std::cout << "\n3. 权重效果分析...\n";
        
        double uMin, uMax, vMin, vMax;
        surface->GetUParameterRange(uMin, uMax);
        surface->GetVParameterRange(vMin, vMax);
        
        // 比较中心区域的高度变化
        std::vector<double> testU = {0.3, 0.5, 0.7};
        std::vector<double> testV = {0.3, 0.5, 0.7};
        
        for (double uParam : testU) {
            for (double vParam : testV) {
                double u = uMin + uParam * (uMax - uMin);
                double v = vMin + vParam * (vMax - vMin);
                
                gp_Pnt point = surface->EvaluatePoint(u, v);
                std::cout << "  参数(" << uParam << ", " << vParam << ") -> 高度: " 
                          << std::fixed << std::setprecision(3) << point.Z() << "\n";
            }
        }
        
        // 4. 修改权重并观察效果
        std::cout << "\n4. 修改中心点权重并观察效果...\n";
        surface->SetWeight(2, 2, 5.0);  // 增加中心点权重
        
        gp_Pnt centerPoint = surface->EvaluatePoint(
            uMin + 0.5 * (uMax - uMin), 
            vMin + 0.5 * (vMax - vMin)
        );
        std::cout << "  权重增加后中心点高度: " << centerPoint.Z() << "\n";
        
    } else {
        std::cout << "  带权重曲面创建失败\n";
    }
    
    std::cout << "\n=== 权重控制曲面示例完成 ===\n";
}

int main() {
    try {
        CreateWaveSurface();
        CreateControlledSurface();
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}