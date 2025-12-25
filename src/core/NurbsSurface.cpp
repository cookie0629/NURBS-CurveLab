#include "NurbsSurface.h"
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Approx_ParametrizationType.hxx>
#include <Precision.hxx>
#include <stdexcept>

NurbsSurface::NurbsSurface() : m_surface(nullptr) {
}

NurbsSurface::NurbsSurface(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                           const std::vector<std::vector<double>>& weights,
                           const std::vector<double>& uKnots,
                           const std::vector<double>& vKnots,
                           const std::vector<int>& uMultiplicities,
                           const std::vector<int>& vMultiplicities,
                           int uDegree, int vDegree) {
    
    if (!ValidateInputs(controlPoints, weights, uDegree, vDegree)) {
        throw std::invalid_argument("Invalid NURBS surface parameters");
    }
    
    try {
        int uCount = static_cast<int>(controlPoints.size());
        int vCount = static_cast<int>(controlPoints[0].size());
        
        // 转换控制点
        TColgp_Array2OfPnt poles(1, uCount, 1, vCount);
        for (int i = 0; i < uCount; ++i) {
            for (int j = 0; j < vCount; ++j) {
                poles(i + 1, j + 1) = controlPoints[i][j];
            }
        }
        
        // 转换权重
        TColStd_Array2OfReal weightsArray(1, uCount, 1, vCount);
        for (int i = 0; i < uCount; ++i) {
            for (int j = 0; j < vCount; ++j) {
                weightsArray(i + 1, j + 1) = weights[i][j];
            }
        }
        
        // 转换节点向量
        TColStd_Array1OfReal uKnotsArray(1, static_cast<int>(uKnots.size()));
        for (size_t i = 0; i < uKnots.size(); ++i) {
            uKnotsArray(static_cast<int>(i + 1)) = uKnots[i];
        }
        
        TColStd_Array1OfReal vKnotsArray(1, static_cast<int>(vKnots.size()));
        for (size_t i = 0; i < vKnots.size(); ++i) {
            vKnotsArray(static_cast<int>(i + 1)) = vKnots[i];
        }
        
        // 转换重数
        TColStd_Array1OfInteger uMultsArray(1, static_cast<int>(uMultiplicities.size()));
        for (size_t i = 0; i < uMultiplicities.size(); ++i) {
            uMultsArray(static_cast<int>(i + 1)) = uMultiplicities[i];
        }
        
        TColStd_Array1OfInteger vMultsArray(1, static_cast<int>(vMultiplicities.size()));
        for (size_t i = 0; i < vMultiplicities.size(); ++i) {
            vMultsArray(static_cast<int>(i + 1)) = vMultiplicities[i];
        }
        
        m_surface = new Geom_BSplineSurface(poles, weightsArray,
                                           uKnotsArray, vKnotsArray,
                                           uMultsArray, vMultsArray,
                                           uDegree, vDegree);
    }
    catch (const Standard_Failure& e) {
        throw std::runtime_error("Failed to create NURBS surface: " + std::string(e.GetMessageString()));
    }
}

bool NurbsSurface::CreateFromControlPoints(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                                          int uDegree, int vDegree) {
    if (controlPoints.empty() || controlPoints[0].empty()) {
        return false;
    }
    
    try {
        int uCount = static_cast<int>(controlPoints.size());
        int vCount = static_cast<int>(controlPoints[0].size());
        
        TColgp_Array2OfPnt points(1, uCount, 1, vCount);
        for (int i = 0; i < uCount; ++i) {
            for (int j = 0; j < vCount; ++j) {
                points(i + 1, j + 1) = controlPoints[i][j];
            }
        }
        
        // 使用简化的构造函数
        GeomAPI_PointsToBSplineSurface builder(points, 
                                              Approx_ChordLength,
                                              uDegree, vDegree, 
                                              GeomAbs_C2, 1e-3);
        if (builder.IsDone()) {
            m_surface = builder.Surface();
            return true;
        }
    }
    catch (const Standard_Failure&) {
        return false;
    }
    
    return false;
}

bool NurbsSurface::CreateWithWeights(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                                     const std::vector<std::vector<double>>& weights,
                                     int uDegree, int vDegree) {
    // 暂时简化实现，先创建不带权重的曲面
    return CreateFromControlPoints(controlPoints, uDegree, vDegree);
}

gp_Pnt NurbsSurface::EvaluatePoint(double u, double v) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    return m_surface->Value(u, v);
}

gp_Vec NurbsSurface::EvaluateUTangent(double u, double v) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    gp_Pnt point;
    gp_Vec uTangent, vTangent;
    m_surface->D1(u, v, point, uTangent, vTangent);
    return uTangent;
}

gp_Vec NurbsSurface::EvaluateVTangent(double u, double v) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    gp_Pnt point;
    gp_Vec uTangent, vTangent;
    m_surface->D1(u, v, point, uTangent, vTangent);
    return vTangent;
}

gp_Vec NurbsSurface::EvaluateNormal(double u, double v) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    GeomLProp_SLProps props(m_surface, u, v, 1, Precision::Confusion());
    if (props.IsNormalDefined()) {
        return props.Normal();
    }
    
    return gp_Vec(0, 0, 1); // 默认法向量
}

double NurbsSurface::EvaluateGaussianCurvature(double u, double v) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    GeomLProp_SLProps props(m_surface, u, v, 2, Precision::Confusion());
    if (props.IsCurvatureDefined()) {
        return props.GaussianCurvature();
    }
    
    return 0.0;
}

double NurbsSurface::EvaluateMeanCurvature(double u, double v) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    GeomLProp_SLProps props(m_surface, u, v, 2, Precision::Confusion());
    if (props.IsCurvatureDefined()) {
        return props.MeanCurvature();
    }
    
    return 0.0;
}

void NurbsSurface::EvaluatePrincipalCurvatures(double u, double v, double& k1, double& k2) const {
    if (!m_surface) {
        throw std::runtime_error("Surface not initialized");
    }
    
    GeomLProp_SLProps props(m_surface, u, v, 2, Precision::Confusion());
    if (props.IsCurvatureDefined()) {
        k1 = props.MaxCurvature();
        k2 = props.MinCurvature();
    } else {
        k1 = k2 = 0.0;
    }
}

bool NurbsSurface::IsValid() const {
    return m_surface && !m_surface.IsNull();
}

void NurbsSurface::GetUParameterRange(double& uMin, double& uMax) const {
    if (m_surface) {
        m_surface->Bounds(uMin, uMax, uMin, uMax); // 获取U和V的边界
        // 只取U方向的边界
        double vMin_temp, vMax_temp;
        m_surface->Bounds(uMin, uMax, vMin_temp, vMax_temp);
    } else {
        uMin = uMax = 0.0;
    }
}

void NurbsSurface::GetVParameterRange(double& vMin, double& vMax) const {
    if (m_surface) {
        double uMin_temp, uMax_temp;
        m_surface->Bounds(uMin_temp, uMax_temp, vMin, vMax);
    } else {
        vMin = vMax = 0.0;
    }
}

int NurbsSurface::GetUDegree() const {
    return m_surface ? m_surface->UDegree() : 0;
}

int NurbsSurface::GetVDegree() const {
    return m_surface ? m_surface->VDegree() : 0;
}

int NurbsSurface::GetUControlPointCount() const {
    return m_surface ? m_surface->NbUPoles() : 0;
}

int NurbsSurface::GetVControlPointCount() const {
    return m_surface ? m_surface->NbVPoles() : 0;
}

void NurbsSurface::SetWeight(int uIndex, int vIndex, double weight) {
    if (!m_surface || uIndex < 1 || vIndex < 1 || 
        uIndex > m_surface->NbUPoles() || vIndex > m_surface->NbVPoles()) {
        return; // 简化错误处理
    }
    
    if (weight > 0.0) {
        m_surface->SetWeight(uIndex, vIndex, weight);
    }
}

bool NurbsSurface::IsUClosed() const {
    return m_surface ? m_surface->IsUClosed() : false;
}

bool NurbsSurface::IsVClosed() const {
    return m_surface ? m_surface->IsVClosed() : false;
}

bool NurbsSurface::IsUPeriodic() const {
    return m_surface ? m_surface->IsUPeriodic() : false;
}

bool NurbsSurface::IsVPeriodic() const {
    return m_surface ? m_surface->IsVPeriodic() : false;
}

bool NurbsSurface::ValidateInputs(const std::vector<std::vector<gp_Pnt>>& controlPoints,
                                 const std::vector<std::vector<double>>& weights,
                                 int uDegree, int vDegree) const {
    if (controlPoints.empty() || controlPoints[0].empty()) {
        return false;
    }
    
    if (uDegree < 1 || vDegree < 1) {
        return false;
    }
    
    // 检查控制点网格的一致性
    size_t vCount = controlPoints[0].size();
    for (const auto& row : controlPoints) {
        if (row.size() != vCount) {
            return false;
        }
    }
    
    // 检查权重
    if (!weights.empty()) {
        if (weights.size() != controlPoints.size()) {
            return false;
        }
        
        for (size_t i = 0; i < weights.size(); ++i) {
            if (weights[i].size() != controlPoints[i].size()) {
                return false;
            }
            
            for (double weight : weights[i]) {
                if (weight <= 0.0) {
                    return false;
                }
            }
        }
    }
    
    return true;
}