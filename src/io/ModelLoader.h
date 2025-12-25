#pragma once

#include <string>
#include <vector>
#include <memory>

class NurbsCurve;
class NurbsSurface;

/**
 * 模型加载器
 * 支持从STEP/IGES文件加载NURBS几何
 */
class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();

    // 加载文件
    bool LoadFromStep(const std::string& filename);
    bool LoadFromIges(const std::string& filename);
    
    // 获取加载的几何对象
    std::vector<std::shared_ptr<NurbsCurve>> GetCurves() const { return m_curves; }
    std::vector<std::shared_ptr<NurbsSurface>> GetSurfaces() const { return m_surfaces; }
    
    // 统计信息
    int GetCurveCount() const { return static_cast<int>(m_curves.size()); }
    int GetSurfaceCount() const { return static_cast<int>(m_surfaces.size()); }
    
    // 错误处理
    std::string GetLastError() const { return m_lastError; }
    bool HasError() const { return !m_lastError.empty(); }
    
    // 清除数据
    void Clear();
    
    // 设置加载选项
    void SetTolerance(double tolerance) { m_tolerance = tolerance; }
    void SetLoadCurves(bool load) { m_loadCurves = load; }
    void SetLoadSurfaces(bool load) { m_loadSurfaces = load; }

private:
    std::vector<std::shared_ptr<NurbsCurve>> m_curves;
    std::vector<std::shared_ptr<NurbsSurface>> m_surfaces;
    
    // 加载选项
    double m_tolerance;
    bool m_loadCurves;
    bool m_loadSurfaces;
    
    // 错误信息
    mutable std::string m_lastError;
    
    // 辅助函数
    void SetError(const std::string& error) const { m_lastError = error; }
    bool ProcessLoadedShapes();
    void ExtractCurvesFromShape(const class TopoDS_Shape& shape);
    void ExtractSurfacesFromShape(const class TopoDS_Shape& shape);
};