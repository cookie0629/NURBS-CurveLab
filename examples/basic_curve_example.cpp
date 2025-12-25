/**
 * 基础NURBS曲线示例
 * 演示如何创建、编辑和分析NURBS曲线
 */

#include "../src/core/NurbsCurve.h"
#include "../src/core/GeometryMath.h"
#include <iostream>
#include <vector>

void BasicCurveExample() {
    std::cout << "=== 基础NURBS曲线示例 ===\n\n";
    
    // 1. 创建控制点
    std::cout << "1. 创建控制点...\n";
    std::vector<gp_Pnt> controlPoints = {
        gp_Pnt(0, 0, 0),    // 起点
        gp_Pnt(1, 3, 0),    // 控制点1
        gp_Pnt(4, 4, 0),    // 控制点2
        gp_Pnt(6, 2, 0),    // 控制点3
        gp_Pnt(8, 0, 0)     // 终点
    };
    
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        const auto& pt = controlPoints[i];
        std::cout << "  控制点" << i << ": (" << pt.X() << ", " << pt.Y() << ", " << pt.Z() << ")\n";
    }
    
    // 2. 创建NURBS曲线
    std::cout << "\n2. 创建3次NURBS曲线...\n";
    auto curve = std::make_shared<NurbsCurve>();
    
    if (curve->CreateFromControlPoints(controlPoints, 3)) {
        std::cout << "  曲线创建成功\n";
        std::cout << "  - 控制点数量: " << curve->GetControlPointCount() << "\n";
        std::cout << "  - 曲线次数: " << curve->GetDegree() << "\n";
        std::cout << "  - 参数范围: [" << curve->GetFirstParameter() 
                  << ", " << curve->GetLastParameter() << "]\n";
    } else {
        std::cout << "  曲线创建失败\n";
        return;
    }
    
    // 3. 曲线分析
    std::cout << "\n3. 曲线分析...\n";
    
    // 计算关键点的几何信息
    std::vector<double> testParams = {0.0, 0.25, 0.5, 0.75, 1.0};
    
    for (double t : testParams) {
        double param = curve->GetFirstParameter() + 
                      t * (curve->GetLastParameter() - curve->GetFirstParameter());
        
        gp_Pnt point = curve->EvaluatePoint(param);
        gp_Vec tangent = curve->EvaluateTangent(param);
        double curvature = curve->EvaluateCurvature(param);
        
        std::cout << "  参数 t=" << t << ":\n";
        std::cout << "    点坐标: (" << std::fixed << std::setprecision(3)
                  << point.X() << ", " << point.Y() << ", " << point.Z() << ")\n";
        std::cout << "    切线: (" << tangent.X() << ", " << tangent.Y() << ", " << tangent.Z() << ")\n";
        std::cout << "    曲率: " << curvature << "\n";
    }
    
    // 4. 几何操作
    std::cout << "\n4. 几何操作...\n";
    
    // 计算弧长
    double arcLength = GeometryMath::CalculateArcLength(curve);
    std::cout << "  曲线总弧长: " << std::fixed << std::setprecision(3) << arcLength << "\n";
    
    // 查找最近点
    gp_Pnt targetPoint(3, 1, 0);
    double closestParam;
    gp_Pnt closestPoint = GeometryMath::FindClosestPoint(curve, targetPoint, closestParam);
    
    std::cout << "  目标点: (" << targetPoint.X() << ", " << targetPoint.Y() << ", " << targetPoint.Z() << ")\n";
    std::cout << "  最近点: (" << closestPoint.X() << ", " << closestPoint.Y() << ", " << closestPoint.Z() << ")\n";
    std::cout << "  最近点参数: " << closestParam << "\n";
    std::cout << "  距离: " << GeometryMath::CalculateDistance(targetPoint, closestPoint) << "\n";
    
    // 5. 曲线采样
    std::cout << "\n5. 曲线采样...\n";
    std::vector<gp_Pnt> samples = GeometryMath::SampleCurve(curve, 10);
    std::cout << "  采样点数量: " << samples.size() << "\n";
    std::cout << "  采样点坐标:\n";
    
    for (size_t i = 0; i < samples.size(); ++i) {
        const auto& pt = samples[i];
        std::cout << "    点" << i << ": (" << std::fixed << std::setprecision(3)
                  << pt.X() << ", " << pt.Y() << ", " << pt.Z() << ")\n";
    }
    
    std::cout << "\n=== 示例完成 ===\n";
}

int main() {
    try {
        BasicCurveExample();
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}