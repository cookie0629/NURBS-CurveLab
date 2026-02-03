#pragma once

#include <Geom_BSplineCurve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <vector>
#include <memory>

/**
 * NURBS曲线类（简化版）
 * 封装OpenCASCADE的Geom_BSplineCurve，提供基础的NURBS曲线功能
 */
class NurbsCurve {
public:
    // 构造函数
    NurbsCurve();

    // 基础操作 - 通过控制点创建曲线
    bool CreateFromControlPoints(const std::vector<gp_Pnt>& controlPoints, int degree = 3);
    
    // 控制点查询
    int GetControlPointCount() const;
    gp_Pnt GetControlPoint(int index) const;
    
    // 曲线属性
    int GetDegree() const;
    bool IsValid() const;
    double GetFirstParameter() const;
    double GetLastParameter() const;
    
    // 曲线分析
    gp_Pnt EvaluatePoint(double parameter) const;
    gp_Vec EvaluateTangent(double parameter) const;
    double EvaluateCurvature(double parameter) const;
    
    // 获取OpenCASCADE句柄
    Handle(Geom_BSplineCurve) GetOCCCurve() const { return m_curve; }

private:
    Handle(Geom_BSplineCurve) m_curve;
};