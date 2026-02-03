#pragma once

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <vector>
#include <memory>

class NurbsCurve;

/**
 * 几何数学工具类（简化版）
 * 提供NURBS几何的基础数学操作和分析功能
 */
class GeometryMath {
public:
    // 曲线采样
    static std::vector<gp_Pnt> SampleCurve(
        const std::shared_ptr<NurbsCurve>& curve,
        int numSamples);
    
    // 弧长计算
    static double CalculateArcLength(
        const std::shared_ptr<NurbsCurve>& curve,
        double startParam = -1.0,
        double endParam = -1.0);
    
    // 最近点查找
    static gp_Pnt FindClosestPoint(
        const std::shared_ptr<NurbsCurve>& curve,
        const gp_Pnt& targetPoint,
        double& parameter);
    
    // 基础工具函数
    static double CalculateDistance(const gp_Pnt& p1, const gp_Pnt& p2);
    static double CalculateAngle(const gp_Vec& v1, const gp_Vec& v2);
    
private:
    // 私有辅助函数
    static double ApproximateArcLength(const std::shared_ptr<NurbsCurve>& curve,
                                      double startParam, double endParam, 
                                      int numSegments = 100);
};