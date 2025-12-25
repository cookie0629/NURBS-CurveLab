#include "NurbsCurve.h"
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <BSplCLib.hxx>
#include <Precision.hxx>
#include <stdexcept>

// 默认构造函数 - 初始化空的NURBS曲线
NurbsCurve::NurbsCurve() : m_curve(nullptr) {
}

// 完整构造函数 - 通过控制点、权重、节点向量和重数创建NURBS曲线
NurbsCurve::NurbsCurve(const std::vector<gp_Pnt>& controlPoints,
                       const std::vector<double>& weights,
                       const std::vector<double>& knots,
                       const std::vector<int>& multiplicities,
                       int degree) {
    
    // 验证输入参数的有效性
    if (!ValidateInputs(controlPoints, weights, degree)) {
        throw std::invalid_argument("Invalid NURBS curve parameters");
    }
    
    try {
        // 将STL容器转换为OpenCASCADE数组格式
        // 控制点数组
        TColgp_Array1OfPnt poles(1, static_cast<int>(controlPoints.size()));
        for (size_t i = 0; i < controlPoints.size(); ++i) {
            poles(static_cast<int>(i + 1)) = controlPoints[i];
        }
        
        // 权重数组
        TColStd_Array1OfReal weightsArray(1, static_cast<int>(weights.size()));
        for (size_t i = 0; i < weights.size(); ++i) {
            weightsArray(static_cast<int>(i + 1)) = weights[i];
        }
        
        // 节点向量数组
        TColStd_Array1OfReal knotsArray(1, static_cast<int>(knots.size()));
        for (size_t i = 0; i < knots.size(); ++i) {
            knotsArray(static_cast<int>(i + 1)) = knots[i];
        }
        
        // 重数数组
        TColStd_Array1OfInteger multsArray(1, static_cast<int>(multiplicities.size()));
        for (size_t i = 0; i < multiplicities.size(); ++i) {
            multsArray(static_cast<int>(i + 1)) = multiplicities[i];
        }
        
        // 创建OpenCASCADE的B样条曲线对象
        m_curve = new Geom_BSplineCurve(poles, weightsArray, knotsArray, multsArray, degree);
    }
    catch (const Standard_Failure& e) {
        throw std::runtime_error("Failed to create NURBS curve: " + std::string(e.GetMessageString()));
    }
}

// 通过控制点创建NURBS曲线 - 使用默认权重和自动生成的节点向量
bool NurbsCurve::CreateFromControlPoints(const std::vector<gp_Pnt>& controlPoints, int degree) {
    // 检查最小控制点数量要求
    if (controlPoints.size() < 2) {
        return false;
    }
    
    try {
        // 将控制点转换为OpenCASCADE数组
        TColgp_Array1OfPnt points(1, static_cast<int>(controlPoints.size()));
        for (size_t i = 0; i < controlPoints.size(); ++i) {
            points(static_cast<int>(i + 1)) = controlPoints[i];
        }
        
        // 使用OpenCASCADE的点到B样条转换器
        // 这会自动生成合适的节点向量和权重
        GeomAPI_PointsToBSpline builder(points, degree, degree);
        if (builder.IsDone()) {
            m_curve = builder.Curve();
            return true;
        }
    }
    catch (const Standard_Failure&) {
        return false;
    }
    
    return false;
}

// 通过控制点和权重创建NURBS曲线 - 支持有理B样条曲线
bool NurbsCurve::CreateWithWeights(const std::vector<gp_Pnt>& controlPoints,
                                  const std::vector<double>& weights,
                                  int degree) {
    // 验证输入参数
    if (!ValidateInputs(controlPoints, weights, degree)) {
        return false;
    }
    
    try {
        // 转换控制点为OpenCASCADE数组
        TColgp_Array1OfPnt poles(1, static_cast<int>(controlPoints.size()));
        for (size_t i = 0; i < controlPoints.size(); ++i) {
            poles(static_cast<int>(i + 1)) = controlPoints[i];
        }
        
        // 转换权重为OpenCASCADE数组
        TColStd_Array1OfReal weightsArray(1, static_cast<int>(weights.size()));
        for (size_t i = 0; i < weights.size(); ++i) {
            weightsArray(static_cast<int>(i + 1)) = weights[i];
        }
        
        // 使用带权重的点到B样条转换器创建有理B样条曲线
        GeomAPI_PointsToBSpline builder(poles, weightsArray, degree, degree);
        if (builder.IsDone()) {
            m_curve = builder.Curve();
            return true;
        }
    }
    catch (const Standard_Failure&) {
        return false;
    }
    
    return false;
}

void NurbsCurve::SetControlPoint(int index, const gp_Pnt& point) {
    if (!m_curve || index < 1 || index > m_curve->NbPoles()) {
        throw std::out_of_range("Invalid control point index");
    }
    
    m_curve->SetPole(index, point);
}

gp_Pnt NurbsCurve::GetControlPoint(int index) const {
    if (!m_curve || index < 1 || index > m_curve->NbPoles()) {
        throw std::out_of_range("Invalid control point index");
    }
    
    return m_curve->Pole(index);
}

int NurbsCurve::GetControlPointCount() const {
    return m_curve ? m_curve->NbPoles() : 0;
}

std::vector<gp_Pnt> NurbsCurve::GetAllControlPoints() const {
    std::vector<gp_Pnt> points;
    if (!m_curve) return points;
    
    points.reserve(m_curve->NbPoles());
    for (int i = 1; i <= m_curve->NbPoles(); ++i) {
        points.push_back(m_curve->Pole(i));
    }
    
    return points;
}

void NurbsCurve::SetWeight(int index, double weight) {
    if (!m_curve || index < 1 || index > m_curve->NbPoles()) {
        throw std::out_of_range("Invalid weight index");
    }
    
    if (weight <= 0.0) {
        throw std::invalid_argument("Weight must be positive");
    }
    
    m_curve->SetWeight(index, weight);
}

double NurbsCurve::GetWeight(int index) const {
    if (!m_curve || index < 1 || index > m_curve->NbPoles()) {
        throw std::out_of_range("Invalid weight index");
    }
    
    return m_curve->Weight(index);
}

std::vector<double> NurbsCurve::GetAllWeights() const {
    std::vector<double> weights;
    if (!m_curve) return weights;
    
    weights.reserve(m_curve->NbPoles());
    for (int i = 1; i <= m_curve->NbPoles(); ++i) {
        weights.push_back(m_curve->Weight(i));
    }
    
    return weights;
}

gp_Pnt NurbsCurve::EvaluatePoint(double parameter) const {
    if (!m_curve) {
        throw std::runtime_error("Curve not initialized");
    }
    
    return m_curve->Value(parameter);
}

gp_Vec NurbsCurve::EvaluateTangent(double parameter) const {
    if (!m_curve) {
        throw std::runtime_error("Curve not initialized");
    }
    
    gp_Pnt point;
    gp_Vec tangent;
    m_curve->D1(parameter, point, tangent);
    return tangent;
}

double NurbsCurve::EvaluateCurvature(double parameter) const {
    if (!m_curve) {
        throw std::runtime_error("Curve not initialized");
    }
    
    gp_Pnt point;
    gp_Vec firstDeriv, secondDeriv;
    m_curve->D2(parameter, point, firstDeriv, secondDeriv);
    
    double firstDerivMag = firstDeriv.Magnitude();
    if (firstDerivMag < Precision::Confusion()) {
        return 0.0;
    }
    
    gp_Vec crossProduct = firstDeriv.Crossed(secondDeriv);
    return crossProduct.Magnitude() / (firstDerivMag * firstDerivMag * firstDerivMag);
}

bool NurbsCurve::IsValid() const {
    return m_curve && !m_curve.IsNull();
}

double NurbsCurve::GetFirstParameter() const {
    return m_curve ? m_curve->FirstParameter() : 0.0;
}

double NurbsCurve::GetLastParameter() const {
    return m_curve ? m_curve->LastParameter() : 0.0;
}

int NurbsCurve::GetDegree() const {
    return m_curve ? m_curve->Degree() : 0;
}

bool NurbsCurve::ValidateInputs(const std::vector<gp_Pnt>& controlPoints,
                               const std::vector<double>& weights,
                               int degree) const {
    if (controlPoints.empty() || degree < 1) {
        return false;
    }
    
    if (!weights.empty() && weights.size() != controlPoints.size()) {
        return false;
    }
    
    // 检查权重是否为正数
    for (double weight : weights) {
        if (weight <= 0.0) {
            return false;
        }
    }
    
    return true;
}