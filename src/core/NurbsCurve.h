#pragma once

#include <Geom_BSplineCurve.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <gp_Pnt.hxx>
#include <vector>
#include <memory>

/**
 * NURBS曲线类 - 封装OpenCASCADE的Geom_BSplineCurve
 * 提供创建、编辑和分析NURBS曲线的功能
 */
class NurbsCurve {
public:
    // 构造函数
    NurbsCurve();
    NurbsCurve(const std::vector<gp_Pnt>& controlPoints, 
               const std::vector<double>& weights,
               const std::vector<double>& knots,
               const std::vector<int>& multiplicities,
               int degree);

    // 基础操作
    bool CreateFromControlPoints(const std::vector<gp_Pnt>& controlPoints, int degree = 3);
    bool CreateWithWeights(const std::vector<gp_Pnt>& controlPoints,
                          const std::vector<double>& weights,
                          int degree = 3);
    
    // 控制点操作
    void SetControlPoint(int index, const gp_Pnt& point);
    gp_Pnt GetControlPoint(int index) const;
    int GetControlPointCount() const;
    std::vector<gp_Pnt> GetAllControlPoints() const;
    
    // 权重操作
    void SetWeight(int index, double weight);
    double GetWeight(int index) const;
    std::vector<double> GetAllWeights() const;
    
    // 节点向量操作
    std::vector<double> GetKnotVector() const;
    std::vector<int> GetMultiplicities() const;
    int GetDegree() const;
    
    // 曲线分析
    gp_Pnt EvaluatePoint(double parameter) const;
    gp_Vec EvaluateTangent(double parameter) const;
    gp_Vec EvaluateNormal(double parameter) const;
    double EvaluateCurvature(double parameter) const;
    
    // 几何操作
    void Reverse();
    std::shared_ptr<NurbsCurve> ExtendToPoint(const gp_Pnt& targetPoint, bool atStart = false);
    std::shared_ptr<NurbsCurve> Trim(double startParam, double endParam);
    
    // 插入节点
    void InsertKnot(double parameter, int multiplicity = 1);
    void RemoveKnot(int index, int multiplicity = 1);
    
    // 升阶/降阶
    void IncreaseDegree(int targetDegree);
    bool DecreaseDegree();
    
    // 获取OpenCASCADE句柄
    Handle(Geom_BSplineCurve) GetOCCCurve() const { return m_curve; }
    
    // 验证曲线有效性
    bool IsValid() const;
    
    // 参数范围
    double GetFirstParameter() const;
    double GetLastParameter() const;

private:
    Handle(Geom_BSplineCurve) m_curve;
    
    // 辅助函数
    void UpdateCurve();
    bool ValidateInputs(const std::vector<gp_Pnt>& controlPoints,
                       const std::vector<double>& weights,
                       int degree) const;
};