#pragma once

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <vector>
#include <memory>

class NurbsCurve;
class NurbsSurface;

/**
 * 几何数学工具类
 * 提供NURBS几何的高级数学操作和分析功能
 */
class GeometryMath {
public:
    // 曲线操作
    static std::shared_ptr<NurbsCurve> BlendCurves(
        const std::shared_ptr<NurbsCurve>& curve1,
        const std::shared_ptr<NurbsCurve>& curve2,
        double blendFactor = 0.5);
    
    static std::shared_ptr<NurbsCurve> OffsetCurve(
        const std::shared_ptr<NurbsCurve>& curve,
        double offsetDistance,
        const gp_Vec& offsetDirection);
    
    static std::vector<gp_Pnt> SampleCurve(
        const std::shared_ptr<NurbsCurve>& curve,
        int numSamples);
    
    // 曲面操作
    static std::shared_ptr<NurbsSurface> LoftSurface(
        const std::vector<std::shared_ptr<NurbsCurve>>& curves);
    
    static std::shared_ptr<NurbsSurface> SweepSurface(
        const std::shared_ptr<NurbsCurve>& profile,
        const std::shared_ptr<NurbsCurve>& path);
    
    static std::shared_ptr<NurbsSurface> RevolvedSurface(
        const std::shared_ptr<NurbsCurve>& profile,
        const gp_Pnt& axisPoint,
        const gp_Vec& axisDirection,
        double angle);
    
    // 分析工具
    static double CalculateArcLength(
        const std::shared_ptr<NurbsCurve>& curve,
        double startParam = -1.0,
        double endParam = -1.0);
    
    static double CalculateSurfaceArea(
        const std::shared_ptr<NurbsSurface>& surface);
    
    static gp_Pnt FindClosestPoint(
        const std::shared_ptr<NurbsCurve>& curve,
        const gp_Pnt& targetPoint,
        double& parameter);
    
    static gp_Pnt FindClosestPointOnSurface(
        const std::shared_ptr<NurbsSurface>& surface,
        const gp_Pnt& targetPoint,
        double& u, double& v);
    
    // 交点计算
    static std::vector<gp_Pnt> FindCurveIntersections(
        const std::shared_ptr<NurbsCurve>& curve1,
        const std::shared_ptr<NurbsCurve>& curve2,
        double tolerance = 1e-6);
    
    static std::vector<std::shared_ptr<NurbsCurve>> FindSurfaceIntersections(
        const std::shared_ptr<NurbsSurface>& surface1,
        const std::shared_ptr<NurbsSurface>& surface2,
        double tolerance = 1e-6);
    
    // 拟合工具
    static std::shared_ptr<NurbsCurve> FitCurveToPoints(
        const std::vector<gp_Pnt>& points,
        int degree = 3,
        double tolerance = 1e-3);
    
    static std::shared_ptr<NurbsSurface> FitSurfaceToPoints(
        const std::vector<std::vector<gp_Pnt>>& points,
        int uDegree = 3,
        int vDegree = 3,
        double tolerance = 1e-3);
    
    // 连续性检查
    enum class ContinuityType {
        C0, // 位置连续
        C1, // 切线连续
        C2  // 曲率连续
    };
    
    static bool CheckCurveContinuity(
        const std::shared_ptr<NurbsCurve>& curve1,
        const std::shared_ptr<NurbsCurve>& curve2,
        ContinuityType continuity,
        double tolerance = 1e-6);
    
    static bool CheckSurfaceContinuity(
        const std::shared_ptr<NurbsSurface>& surface1,
        const std::shared_ptr<NurbsSurface>& surface2,
        ContinuityType continuity,
        bool alongU = true,
        double tolerance = 1e-6);
    
    // 工具函数
    static std::vector<double> GenerateUniformKnotVector(
        int numControlPoints,
        int degree,
        bool clamped = true);
    
    static std::vector<double> GenerateChordLengthParameters(
        const std::vector<gp_Pnt>& points);
    
    static double CalculateDistance(const gp_Pnt& p1, const gp_Pnt& p2);
    static double CalculateAngle(const gp_Vec& v1, const gp_Vec& v2);
    
private:
    // 私有辅助函数
    static std::vector<double> ComputeChordLengths(const std::vector<gp_Pnt>& points);
    static void NormalizeKnotVector(std::vector<double>& knots);
    static double ApproximateArcLength(const std::shared_ptr<NurbsCurve>& curve,
                                      double startParam, double endParam, 
                                      int numSegments = 100);
};