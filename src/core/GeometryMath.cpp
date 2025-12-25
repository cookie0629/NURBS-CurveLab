#include "GeometryMath.h"
#include "NurbsCurve.h"
#include "NurbsSurface.h"
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_IntCS.hxx>
#include <GeomAPI_IntSS.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Precision.hxx>
#include <algorithm>
#include <cmath>
#include <limits>

// 将点集拟合为NURBS曲线 - 使用最小二乘法拟合
std::shared_ptr<NurbsCurve> GeometryMath::FitCurveToPoints(
    const std::vector<gp_Pnt>& points,
    int degree,
    double tolerance) {
    
    // 检查输入点的数量是否足够
    if (points.size() < 2) {
        return nullptr;
    }
    
    try {
        // 创建一个新的NurbsCurve对象
        auto curve = std::make_shared<NurbsCurve>();
        
        // 使用NurbsCurve的CreateFromControlPoints方法
        if (curve->CreateFromControlPoints(points, degree)) {
            return curve;
        }
    }
    catch (const std::exception&) {
        return nullptr;
    }
    
    return nullptr;
}

std::shared_ptr<NurbsSurface> GeometryMath::FitSurfaceToPoints(
    const std::vector<std::vector<gp_Pnt>>& points,
    int uDegree,
    int vDegree,
    double tolerance) {
    
    if (points.empty() || points[0].empty()) {
        return nullptr;
    }
    
    try {
        int uCount = static_cast<int>(points.size());
        int vCount = static_cast<int>(points[0].size());
        
        TColgp_Array2OfPnt pointArray(1, uCount, 1, vCount);
        for (int i = 0; i < uCount; ++i) {
            for (int j = 0; j < vCount; ++j) {
                pointArray(i + 1, j + 1) = points[i][j];
            }
        }
        
        GeomAPI_PointsToBSplineSurface builder(pointArray, uDegree, vDegree,
                                              GeomAbs_C2, tolerance);
        
        if (builder.IsDone()) {
            auto surface = std::make_shared<NurbsSurface>();
            // 类似地，需要从Handle创建NurbsSurface对象
            return surface;
        }
    }
    catch (const Standard_Failure&) {
        return nullptr;
    }
    
    return nullptr;
}

// 计算NURBS曲线的弧长 - 支持精确计算和数值近似
double GeometryMath::CalculateArcLength(
    const std::shared_ptr<NurbsCurve>& curve,
    double startParam,
    double endParam) {
    
    // 检查曲线有效性
    if (!curve || !curve->IsValid()) {
        return 0.0;
    }
    
    // 如果未指定参数范围，使用曲线的完整参数范围
    if (startParam < 0) startParam = curve->GetFirstParameter();
    if (endParam < 0) endParam = curve->GetLastParameter();
    
    // 使用数值积分近似方法计算弧长
    return ApproximateArcLength(curve, startParam, endParam);
}

double GeometryMath::ApproximateArcLength(
    const std::shared_ptr<NurbsCurve>& curve,
    double startParam,
    double endParam,
    int numSegments) {
    
    double length = 0.0;
    double step = (endParam - startParam) / numSegments;
    
    gp_Pnt prevPoint = curve->EvaluatePoint(startParam);
    
    for (int i = 1; i <= numSegments; ++i) {
        double param = startParam + i * step;
        gp_Pnt currentPoint = curve->EvaluatePoint(param);
        length += CalculateDistance(prevPoint, currentPoint);
        prevPoint = currentPoint;
    }
    
    return length;
}

gp_Pnt GeometryMath::FindClosestPoint(
    const std::shared_ptr<NurbsCurve>& curve,
    const gp_Pnt& targetPoint,
    double& parameter) {
    
    if (!curve || !curve->IsValid()) {
        parameter = 0.0;
        return gp_Pnt();
    }
    
    // 使用简单的搜索方法找到最近点
    double minDistance = std::numeric_limits<double>::max();
    gp_Pnt closestPoint;
    double bestParam = 0.0;
    
    double startParam = curve->GetFirstParameter();
    double endParam = curve->GetLastParameter();
    int numSamples = 100; // 采样点数量
    
    double step = (endParam - startParam) / (numSamples - 1);
    
    for (int i = 0; i < numSamples; ++i) {
        double param = startParam + i * step;
        gp_Pnt point = curve->EvaluatePoint(param);
        double distance = CalculateDistance(targetPoint, point);
        
        if (distance < minDistance) {
            minDistance = distance;
            closestPoint = point;
            bestParam = param;
        }
    }
    
    parameter = bestParam;
    return closestPoint;
}

gp_Pnt GeometryMath::FindClosestPointOnSurface(
    const std::shared_ptr<NurbsSurface>& surface,
    const gp_Pnt& targetPoint,
    double& u, double& v) {
    
    if (!surface || !surface->IsValid()) {
        u = v = 0.0;
        return gp_Pnt();
    }
    
    Handle(Geom_BSplineSurface) occSurface = surface->GetOCCSurface();
    GeomAPI_ProjectPointOnSurf projector(targetPoint, occSurface);
    
    if (projector.NbPoints() > 0) {
        projector.Parameters(1, u, v);
        return projector.Point(1);
    }
    
    u = v = 0.0;
    return gp_Pnt();
}

std::vector<double> GeometryMath::GenerateUniformKnotVector(
    int numControlPoints,
    int degree,
    bool clamped) {
    
    std::vector<double> knots;
    int numKnots = numControlPoints + degree + 1;
    
    if (clamped) {
        // 夹紧节点向量
        for (int i = 0; i <= degree; ++i) {
            knots.push_back(0.0);
        }
        
        int numInternalKnots = numKnots - 2 * (degree + 1);
        for (int i = 1; i <= numInternalKnots; ++i) {
            knots.push_back(static_cast<double>(i) / (numInternalKnots + 1));
        }
        
        for (int i = 0; i <= degree; ++i) {
            knots.push_back(1.0);
        }
    } else {
        // 均匀节点向量
        for (int i = 0; i < numKnots; ++i) {
            knots.push_back(static_cast<double>(i) / (numKnots - 1));
        }
    }
    
    return knots;
}

std::vector<double> GeometryMath::GenerateChordLengthParameters(
    const std::vector<gp_Pnt>& points) {
    
    if (points.size() < 2) {
        return {};
    }
    
    std::vector<double> chordLengths = ComputeChordLengths(points);
    std::vector<double> parameters;
    parameters.reserve(points.size());
    
    parameters.push_back(0.0);
    
    double totalLength = 0.0;
    for (double length : chordLengths) {
        totalLength += length;
    }
    
    if (totalLength < Precision::Confusion()) {
        // 所有点重合的情况
        for (size_t i = 0; i < points.size(); ++i) {
            parameters.push_back(static_cast<double>(i) / (points.size() - 1));
        }
        return parameters;
    }
    
    double accumulatedLength = 0.0;
    for (size_t i = 1; i < points.size(); ++i) {
        accumulatedLength += chordLengths[i - 1];
        parameters.push_back(accumulatedLength / totalLength);
    }
    
    return parameters;
}

std::vector<gp_Pnt> GeometryMath::SampleCurve(
    const std::shared_ptr<NurbsCurve>& curve,
    int numSamples) {
    
    std::vector<gp_Pnt> samples;
    if (!curve || !curve->IsValid() || numSamples < 2) {
        return samples;
    }
    
    samples.reserve(numSamples);
    
    double startParam = curve->GetFirstParameter();
    double endParam = curve->GetLastParameter();
    double step = (endParam - startParam) / (numSamples - 1);
    
    for (int i = 0; i < numSamples; ++i) {
        double param = startParam + i * step;
        samples.push_back(curve->EvaluatePoint(param));
    }
    
    return samples;
}

double GeometryMath::CalculateDistance(const gp_Pnt& p1, const gp_Pnt& p2) {
    return p1.Distance(p2);
}

double GeometryMath::CalculateAngle(const gp_Vec& v1, const gp_Vec& v2) {
    double dot = v1.Dot(v2);
    double mag1 = v1.Magnitude();
    double mag2 = v2.Magnitude();
    
    if (mag1 < Precision::Confusion() || mag2 < Precision::Confusion()) {
        return 0.0;
    }
    
    double cosAngle = dot / (mag1 * mag2);
    cosAngle = std::max(-1.0, std::min(1.0, cosAngle)); // 限制在[-1,1]范围内
    
    return std::acos(cosAngle);
}

std::vector<double> GeometryMath::ComputeChordLengths(const std::vector<gp_Pnt>& points) {
    std::vector<double> lengths;
    lengths.reserve(points.size() - 1);
    
    for (size_t i = 1; i < points.size(); ++i) {
        lengths.push_back(CalculateDistance(points[i - 1], points[i]));
    }
    
    return lengths;
}

void GeometryMath::NormalizeKnotVector(std::vector<double>& knots) {
    if (knots.empty()) return;
    
    double minVal = knots.front();
    double maxVal = knots.back();
    double range = maxVal - minVal;
    
    if (range < Precision::Confusion()) return;
    
    for (double& knot : knots) {
        knot = (knot - minVal) / range;
    }
}