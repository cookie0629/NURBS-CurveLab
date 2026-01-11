/**
 * NURBS-CurveLab - NURBS曲线和曲面建模工具
 * 基于OpenCASCADE的NURBS几何建模演示程序
 */

#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

// NURBS类
#include "../src/core/NurbsCurve.h"
#include "../src/core/NurbsSurface.h"
#include "../src/core/GeometryMath.h"
#include "../src/io/StepExporter.h"
#include "../src/io/ModelLoader.h"

// OpenCASCADE头文件
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

void PrintHeader() {
    std::cout << "========================================\n";
    std::cout << "    NURBS-CurveLab v1.0\n";
    std::cout << "    NURBS曲线和曲面建模工具\n";
    std::cout << "    基于OpenCASCADE 7.6.0\n";
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
        std::cout << "成功创建3次NURBS曲线\n";
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
        std::cout << "曲线创建失败\n";
    }
    
    std::cout << "\n";
}

void DemonstrateSurfaceCreation() {
    std::cout << "=== NURBS曲面创建演示 ===\n";
    
    // 创建4x4控制点网格
    std::vector<std::vector<gp_Pnt>> controlPoints(4, std::vector<gp_Pnt>(4));
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double x = static_cast<double>(i);
            double y = static_cast<double>(j);
            double z = std::sin(x * 0.5) * std::cos(y * 0.5); // 创建波浪形曲面
            controlPoints[i][j] = gp_Pnt(x, y, z);
        }
    }
    
    // 创建NURBS曲面
    auto surface = std::make_shared<NurbsSurface>();
    if (surface->CreateFromControlPoints(controlPoints, 3, 3)) {
        std::cout << "成功创建3x3次NURBS曲面\n";
        std::cout << "  U方向控制点数: " << surface->GetUControlPointCount() << "\n";
        std::cout << "  V方向控制点数: " << surface->GetVControlPointCount() << "\n";
        std::cout << "  U方向次数: " << surface->GetUDegree() << "\n";
        std::cout << "  V方向次数: " << surface->GetVDegree() << "\n";
        
        // 计算曲面上的点和法向量
        double uMin, uMax, vMin, vMax;
        surface->GetUParameterRange(uMin, uMax);
        surface->GetVParameterRange(vMin, vMax);
        
        double u = (uMin + uMax) / 2.0;
        double v = (vMin + vMax) / 2.0;
        
        gp_Pnt centerPoint = surface->EvaluatePoint(u, v);
        gp_Vec normal = surface->EvaluateNormal(u, v);
        double gaussCurvature = surface->EvaluateGaussianCurvature(u, v);
        double meanCurvature = surface->EvaluateMeanCurvature(u, v);
        
        std::cout << "  中心点坐标: (" << std::fixed << std::setprecision(3)
                  << centerPoint.X() << ", " << centerPoint.Y() << ", " << centerPoint.Z() << ")\n";
        std::cout << "  中心点法向量: (" << normal.X() << ", " << normal.Y() << ", " << normal.Z() << ")\n";
        std::cout << "  高斯曲率: " << gaussCurvature << "\n";
        std::cout << "  平均曲率: " << meanCurvature << "\n";
    } else {
        std::cout << "曲面创建失败\n";
    }
    
    std::cout << "\n";
}

void DemonstrateGeometryOperations() {
    std::cout << "=== 几何操作演示 ===\n";
    
    // 创建简单的控制点
    std::vector<gp_Pnt> points = {
        gp_Pnt(0, 0, 0),
        gp_Pnt(1, 1, 0),
        gp_Pnt(2, 0, 0),
        gp_Pnt(3, -1, 0)
    };
    
    auto curve = GeometryMath::FitCurveToPoints(points, 3, 1e-3);
    if (curve && curve->IsValid()) {
        std::cout << "成功拟合曲线到点集\n";
        
        // 计算弧长
        double arcLength = GeometryMath::CalculateArcLength(curve);
        std::cout << "  曲线弧长: " << std::fixed << std::setprecision(3) << arcLength << "\n";
        
        // 采样曲线点
        std::vector<gp_Pnt> samples = GeometryMath::SampleCurve(curve, 10);
        std::cout << "  采样点数量: " << samples.size() << "\n";
        
        // 查找最近点
        gp_Pnt targetPoint(1.5, 0.5, 0);
        double parameter;
        gp_Pnt closestPoint = GeometryMath::FindClosestPoint(curve, targetPoint, parameter);
        std::cout << "  目标点: (1.5, 0.5, 0)\n";
        std::cout << "  最近点: (" << closestPoint.X() << ", " << closestPoint.Y() << ", " << closestPoint.Z() << ")\n";
        std::cout << "  参数值: " << parameter << "\n";
    } else {
        std::cout << "曲线拟合失败\n";
    }
    
    std::cout << "\n";
}

void DemonstrateFileIO() {
    std::cout << "=== 文件输入输出演示 ===\n";
    
    // 创建测试曲线
    std::vector<gp_Pnt> controlPoints = {
        gp_Pnt(0, 0, 0),
        gp_Pnt(1, 2, 1),
        gp_Pnt(3, 3, 0),
        gp_Pnt(5, 1, -1),
        gp_Pnt(6, 0, 0)
    };
    
    auto curve = std::make_shared<NurbsCurve>();
    if (curve->CreateFromControlPoints(controlPoints, 3)) {
        // 导出到STEP文件
        StepExporter exporter;
        exporter.SetAuthor("NURBS-CurveLab Demo");
        exporter.SetOrganization("OpenCASCADE User");
        
        if (exporter.AddCurve(curve, "DemoCurve")) {
            std::string filename = "demo_curve.step";
            if (exporter.ExportToFile(filename)) {
                std::cout << "成功导出曲线到 " << filename << "\n";
                std::cout << "  导出的曲线数量: " << exporter.GetCurveCount() << "\n";
            } else {
                std::cout << "导出失败: " << exporter.GetLastError() << "\n";
            }
        }
    }
    
    std::cout << "\n";
}

void ShowMenu() {
    std::cout << "请选择演示功能:\n";
    std::cout << "1. NURBS曲线创建\n";
    std::cout << "2. NURBS曲面创建\n";
    std::cout << "3. 几何操作\n";
    std::cout << "4. 文件输入输出\n";
    std::cout << "5. 运行所有演示\n";
    std::cout << "0. 退出\n";
    std::cout << "请输入选择 (0-5): ";
}

int main() {
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
            DemonstrateSurfaceCreation();
            break;
        case 3:
            DemonstrateGeometryOperations();
            break;
        case 4:
            DemonstrateFileIO();
            break;
        case 5:
            DemonstrateCurveCreation();
            DemonstrateSurfaceCreation();
            DemonstrateGeometryOperations();
            DemonstrateFileIO();
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
    
    return 0;
}