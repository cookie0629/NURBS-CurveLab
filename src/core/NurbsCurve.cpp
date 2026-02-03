#include "NurbsCurve.h"
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Precision.hxx>
#include <stdexcept>

NurbsCurve::NurbsCurve() : m_curve(nullptr) {
}

bool NurbsCurve::CreateFromControlPoints(const std::vector<gp_Pnt>& controlPoints, int degree) {
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

int NurbsCurve::GetControlPointCount() const {
    return m_curve ? m_curve->NbPoles() : 0;
}

gp_Pnt NurbsCurve::GetControlPoint(int index) const {
    if (!m_curve || index < 1 || index > m_curve->NbPoles()) {
        throw std::out_of_range("Invalid control point index");
    }
    return m_curve->Pole(index);
}

int NurbsCurve::GetDegree() const {
    return m_curve ? m_curve->Degree() : 0;
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
    
    // 曲率公式: k = |r' x r''| / |r'|^3
    gp_Vec crossProduct = firstDeriv.Crossed(secondDeriv);
    return crossProduct.Magnitude() / (firstDerivMag * firstDerivMag * firstDerivMag);
}