/**
 * NURBS-CurveLab - NURBS曲线建模工具（简化版）
 * 基于OpenCASCADE的NURBS曲线建模演示程序
 */

#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

// NURBS类
#include "../src/core/NurbsCurve.h"
#include "../src/core/GeometryMath.h"

// OpenCASCADE头文件
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

void PrintHeader() {
    std::cout << "    NURBS曲线建模工具\n";
    std::cout << "========================================\n\n";
}

void DemonstrateCurveCreation() {
    std::cout << "=== NURBS曲线创建演示 ===\n";
    
    // 创建控制点
    std::vector<gp_Pnt> controlPoints = {
        gp_Pnt(0, 0, 0),
        gp_Pnt(1, 2, 0),
        gp_Pnt(3, 3, 0),
        gp_Pnt(5, 1, 0),
        gp_Pnt(6, 0, 0)
    };
    
    // 创建NURBS曲线
    auto curve = std::make_shared<NurbsCurve>();
    if (curve->CreateFromControlPoints(controlPoints, 3)) {
        std::cout << "✓ 成功创建3次NURBS曲线\n";
        std::cout << "  控制点数量: " << curve->GetControlPointCount() << "\n";
        std::cout << "  曲线次数: " << curve->GetDegree() << "\n";
        std::cout << "  参数范围: [" << curve->GetFirstParameter() 
                  << ", " << curve->GetLastParameter() << "]\n";
        
        // 计算曲线上的点
        double param = (curve->GetFirstParameter() + curve->GetLastParameter()) / 2.0;
        gp_Pnt midPoint = curve->EvaluatePoint(param);
        gp_Vec tangent = curve->EvaluateTangent(param);
        double curvature = curve->EvaluateCurvature(param);
        
        std::cout << "  中点坐标: (" << std::fixed << std::setprecision(3)
                  << midPoint.X() << ", " << midPoint.Y() << ", " << midPoint.Z() << ")\n";
        std::cout << "  中点切线: (" << tangent.X() << ", " << tangent.Y() << ", " << tangent.Z() << ")\n";
        std::cout << "  中点曲率: " << curvature << "\n";
    } else {
        std::cout << "✗ 曲线创建失败\n";
    }
    
    std::cout << "\n";
}

void DemonstrateBasicOperations() {
    std::cout << "=== 基础几何操作演示 ===\n";
    
    // 创建测试曲线
    std::vector<gp_Pnt> controlPoints = {
        gp_Pnt(0, 0, 0),
        gp_Pnt(1, 3, 0),
        gp_Pnt(4, 4, 0),
        gp_Pnt(6, 2, 0),
        gp_Pnt(8, 0, 0)
    };
    
    auto curve = std::make_shared<NurbsCurve>();
    if (curve->CreateFromControlPoints(controlPoints, 3)) {
        std::cout << "✓ 创建测试曲线成功\n";
        
        // 计算弧长
        double arcLength = GeometryMath::CalculateArcLength(curve);
        std::cout << "  曲线弧长: " << std::fixed << std::setprecision(3) << arcLength << "\n";
        
        // 采样曲线点
        std::vector<gp_Pnt> samples = GeometryMath::SampleCurve(curve, 5);
        std::cout << "  采样点数量: " << samples.size() << "\n";
        std::cout << "  采样点坐标:\n";
        for (size_t i = 0; i < samples.size(); ++i) {
            const auto& pt = samples[i];
            std::cout << "    点" << i << ": (" << std::fixed << std::setprecision(2)
                      << pt.X() << ", " << pt.Y() << ", " << pt.Z() << ")\n";
        }
        
        // 查找最近点
        gp_Pnt targetPoint(3, 1, 0);
        double parameter;
        gp_Pnt closestPoint = GeometryMath::FindClosestPoint(curve, targetPoint, parameter);
        std::cout << "  目标点: (3.0, 1.0, 0.0)\n";
        std::cout << "  最近点: (" << std::fixed << std::setprecision(2) 
                  << closestPoint.X() << ", " << closestPoint.Y() << ", " << closestPoint.Z() << ")\n";
        std::cout << "  参数值: " << std::setprecision(3) << parameter << "\n";
        std::cout << "  距离: " << GeometryMath::CalculateDistance(targetPoint, closestPoint) << "\n";
    } else {
        std::cout << "✗ 测试曲线创建失败\n";
    }
    
    std::cout << "\n";
}

void DemonstrateCurveAnalysis() {
    std::cout << "=== 曲线分析演示 ===\n";
    
    // 创建简单的二次曲线
    std::vector<gp_Pnt> controlPoints = {
        gp_Pnt(0, 0, 0),
        gp_Pnt(2, 4, 0),
        gp_Pnt(4, 0, 0)
    };
    
    auto curve = std::make_shared<NurbsCurve>();
    if (curve->CreateFromControlPoints(controlPoints, 2)) {
        std::cout << "✓ 创建二次曲线成功\n";
        
        // 分析不同参数点的几何信息
        std::vector<double> testParams = {0.0, 0.25, 0.5, 0.75, 1.0};
        
        for (double t : testParams) {
            double param = curve->GetFirstParameter() + 
                          t * (curve->GetLastParameter() - curve->GetFirstParameter());
            
            gp_Pnt point = curve->EvaluatePoint(param);
            gp_Vec tangent = curve->EvaluateTangent(param);
            double curvature = curve->EvaluateCurvature(param);
            
            std::cout << "  参数 t=" << std::fixed << std::setprecision(2) << t << ":\n";
            std::cout << "    点坐标: (" << std::setprecision(2)
                      << point.X() << ", " << point.Y() << ", " << point.Z() << ")\n";
            std::cout << "    切线长度: " << std::setprecision(3) << tangent.Magnitude() << "\n";
            std::cout << "    曲率: " << curvature << "\n";
        }
    } else {
        std::cout << "✗ 二次曲线创建失败\n";
    }
    
    std::cout << "\n";
}

void ShowMenu() {
    std::cout << "请选择演示功能:\n";
    std::cout << "1. NURBS曲线创建\n";
    std::cout << "2. 基础几何操作\n";
    std::cout << "3. 曲线分析\n";
    std::cout << "4. 运行所有演示\n";
    std::cout << "0. 退出\n";
    std::cout << "请输入选择 (0-4): ";
}

int main() {
    try {
        PrintHeader();
        
        int choice;
        do {
            ShowMenu();
            std::cin >> choice;
            std::cout << "\n";
            
            switch (choice) {
            case 1:
                DemonstrateCurveCreation();
                break;
            case 2:
                DemonstrateBasicOperations();
                break;
            case 3:
                DemonstrateCurveAnalysis();
                break;
            case 4:
                DemonstrateCurveCreation();
                DemonstrateBasicOperations();
                DemonstrateCurveAnalysis();
                break;
            case 0:
                std::cout << "感谢使用 NURBS-CurveLab!\n";
                break;
            default:
                std::cout << "无效选择，请重新输入。\n\n";
                break;
            }
            
            if (choice != 0) {
                std::cout << "按回车键继续...\n";
                std::cin.ignore();
                std::cin.get();
                std::cout << "\n";
            }
            
        } while (choice != 0);
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "未知错误" << std::endl;
        return 1;
    }
    
    return 0;
}