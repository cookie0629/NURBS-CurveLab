#pragma once

#include <string>
#include <vector>
#include <memory>

class NurbsCurve;
class NurbsSurface;

/**
 * STEP文件导出器
 * 支持将NURBS几何导出为STEP格式
 */
class StepExporter {
public:
    StepExporter();
    ~StepExporter();

    // 设置导出选项
    void SetPrecision(double precision) { m_precision = precision; }
    void SetUnits(const std::string& units) { m_units = units; }
    void SetAuthor(const std::string& author) { m_author = author; }
    void SetOrganization(const std::string& org) { m_organization = org; }

    // 添加几何对象
    bool AddCurve(const std::shared_ptr<NurbsCurve>& curve, const std::string& name = "");
    bool AddSurface(const std::shared_ptr<NurbsSurface>& surface, const std::string& name = "");
    
    // 导出到文件
    bool ExportToFile(const std::string& filename);
    
    // 清除所有几何对象
    void Clear();
    
    // 获取状态信息
    int GetCurveCount() const { return static_cast<int>(m_curves.size()); }
    int GetSurfaceCount() const { return static_cast<int>(m_surfaces.size()); }
    std::string GetLastError() const { return m_lastError; }

private:
    struct CurveData {
        std::shared_ptr<NurbsCurve> curve;
        std::string name;
    };
    
    struct SurfaceData {
        std::shared_ptr<NurbsSurface> surface;
        std::string name;
    };
    
    std::vector<CurveData> m_curves;
    std::vector<SurfaceData> m_surfaces;
    
    // 导出设置
    double m_precision;
    std::string m_units;
    std::string m_author;
    std::string m_organization;
    
    // 错误信息
    mutable std::string m_lastError;
    
    // 辅助函数
    void SetError(const std::string& error) const { m_lastError = error; }
    bool InitializeDocument();
    bool WriteHeader();
    bool WriteCurves();
    bool WriteSurfaces();
};