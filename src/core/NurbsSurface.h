#pragma once

#include <Geom_BSplineSurface.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <vector>
#include <memory>

/**
 * NURBS曲面类 - 封装OpenCASCADE的Geom_BSplineSurface
 * 提供创建、编辑和分析NURBS曲面的功能
 */
class NurbsSurface {
public:
    // 构造函数
    NurbsSurface();
    NurbsSurface(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                 const std::vector<std::vector<double>>& weights,
                 const std::vector<double>& uKnots,
                 const std::vector<double>& vKnots,
                 const std::vector<int>& uMultiplicities,
                 const std::vector<int>& vMultiplicities,
                 int uDegree, int vDegree);

    // 基础操作
    bool CreateFromControlPoints(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                                int uDegree = 3, int vDegree = 3);
    bool CreateWithWeights(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                          const std::vector<std::vector<double>>& weights,
                          int uDegree = 3, int vDegree = 3);
    
    // 控制点操作
    void SetControlPoint(int uIndex, int vIndex, const gp_Pnt& point);
    gp_Pnt GetControlPoint(int uIndex, int vIndex) const;
    int GetUControlPointCount() const;
    int GetVControlPointCount() const;
    std::vector<std::vector<gp_Pnt>> GetAllControlPoints() const;
    
    // 权重操作
    void SetWeight(int uIndex, int vIndex, double weight);
    double GetWeight(int uIndex, int vIndex) const;
    std::vector<std::vector<double>> GetAllWeights() const;
    
    // 节点向量操作
    std::vector<double> GetUKnotVector() const;
    std::vector<double> GetVKnotVector() const;
    std::vector<int> GetUMultiplicities() const;
    std::vector<int> GetVMultiplicities() const;
    int GetUDegree() const;
    int GetVDegree() const;
    
    // 曲面分析
    gp_Pnt EvaluatePoint(double u, double v) const;
    gp_Vec EvaluateUTangent(double u, double v) const;
    gp_Vec EvaluateVTangent(double u, double v) const;
    gp_Vec EvaluateNormal(double u, double v) const;
    
    // 曲率分析
    double EvaluateGaussianCurvature(double u, double v) const;
    double EvaluateMeanCurvature(double u, double v) const;
    void EvaluatePrincipalCurvatures(double u, double v, double& k1, double& k2) const;
    
    // 几何操作
    void ReverseU();
    void ReverseV();
    std::shared_ptr<NurbsSurface> ExtractUIsoLine(double v) const;
    std::shared_ptr<NurbsSurface> ExtractVIsoLine(double u) const;
    
    // 插入节点
    void InsertUKnot(double parameter, int multiplicity = 1);
    void InsertVKnot(double parameter, int multiplicity = 1);
    
    // 升阶/降阶
    void IncreaseUDegree(int targetDegree);
    void IncreaseVDegree(int targetDegree);
    bool DecreaseUDegree();
    bool DecreaseVDegree();
    
    // 获取OpenCASCADE句柄
    Handle(Geom_BSplineSurface) GetOCCSurface() const { return m_surface; }
    
    // 验证曲面有效性
    bool IsValid() const;
    
    // 参数范围
    void GetUParameterRange(double& uMin, double& uMax) const;
    void GetVParameterRange(double& vMin, double& vMax) const;
    
    // 曲面属性
    bool IsUClosed() const;
    bool IsVClosed() const;
    bool IsUPeriodic() const;
    bool IsVPeriodic() const;

private:
    Handle(Geom_BSplineSurface) m_surface;
    
    // 辅助函数
    bool ValidateInputs(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                       const std::vector<std::vector<double>>& weights,
                       int uDegree, int vDegree) const;
};