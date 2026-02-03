#include "GeometryMath.h"
#include "NurbsCurve.h"
#include <Precision.hxx>
#include <algorithm>
#include <cmath>
#include <limits>

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