#include "StepExporter.h"
#include "../core/NurbsCurve.h"
#include "../core/NurbsSurface.h"

#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <IFSelect_ReturnStatus.hxx>

StepExporter::StepExporter() 
    : m_precision(1e-6)
    , m_units("MM")
    , m_author("NURBS-CurveLab")
    , m_organization("OpenCASCADE User") {
}

StepExporter::~StepExporter() {
    Clear();
}

bool StepExporter::AddCurve(const std::shared_ptr<NurbsCurve>& curve, const std::string& name) {
    if (!curve || !curve->IsValid()) {
        SetError("Invalid curve provided");
        return false;
    }
    
    CurveData data;
    data.curve = curve;
    data.name = name.empty() ? ("Curve_" + std::to_string(m_curves.size() + 1)) : name;
    
    m_curves.push_back(data);
    return true;
}

bool StepExporter::AddSurface(const std::shared_ptr<NurbsSurface>& surface, const std::string& name) {
    if (!surface || !surface->IsValid()) {
        SetError("Invalid surface provided");
        return false;
    }
    
    SurfaceData data;
    data.surface = surface;
    data.name = name.empty() ? ("Surface_" + std::to_string(m_surfaces.size() + 1)) : name;
    
    m_surfaces.push_back(data);
    return true;
}

bool StepExporter::ExportToFile(const std::string& filename) {
    if (m_curves.empty() && m_surfaces.empty()) {
        SetError("No geometry to export");
        return false;
    }
    
    try {
        STEPControl_Writer writer;
        
        // 设置导出参数
        Interface_Static::SetCVal("write.step.unit", m_units.c_str());
        Interface_Static::SetRVal("write.precision.val", m_precision);
        Interface_Static::SetCVal("write.step.author", m_author.c_str());
        Interface_Static::SetCVal("write.step.organization", m_organization.c_str());
        
        // 导出曲线
        for (const auto& curveData : m_curves) {
            Handle(Geom_BSplineCurve) occCurve = curveData.curve->GetOCCCurve();
            if (!occCurve.IsNull()) {
                try {
                    BRepBuilderAPI_MakeEdge edgeBuilder(occCurve);
                    if (edgeBuilder.IsDone()) {
                        TopoDS_Edge edge = edgeBuilder.Edge();
                        IFSelect_ReturnStatus status = writer.Transfer(edge, STEPControl_AsIs);
                        if (status != IFSelect_RetDone) {
                            SetError("Failed to transfer curve: " + curveData.name);
                            return false;
                        }
                    }
                }
                catch (const Standard_Failure& e) {
                    SetError("Error creating edge for curve " + curveData.name + ": " + 
                            std::string(e.GetMessageString()));
                    return false;
                }
            }
        }
        
        // 导出曲面
        for (const auto& surfaceData : m_surfaces) {
            Handle(Geom_BSplineSurface) occSurface = surfaceData.surface->GetOCCSurface();
            if (!occSurface.IsNull()) {
                try {
                    BRepBuilderAPI_MakeFace faceBuilder(occSurface, Precision::Confusion());
                    if (faceBuilder.IsDone()) {
                        TopoDS_Face face = faceBuilder.Face();
                        IFSelect_ReturnStatus status = writer.Transfer(face, STEPControl_AsIs);
                        if (status != IFSelect_RetDone) {
                            SetError("Failed to transfer surface: " + surfaceData.name);
                            return false;
                        }
                    }
                }
                catch (const Standard_Failure& e) {
                    SetError("Error creating face for surface " + surfaceData.name + ": " + 
                            std::string(e.GetMessageString()));
                    return false;
                }
            }
        }
        
        // 写入文件
        IFSelect_ReturnStatus writeStatus = writer.Write(filename.c_str());
        if (writeStatus != IFSelect_RetDone) {
            SetError("Failed to write STEP file: " + filename);
            return false;
        }
        
        return true;
    }
    catch (const Standard_Failure& e) {
        SetError("STEP export failed: " + std::string(e.GetMessageString()));
        return false;
    }
    catch (const std::exception& e) {
        SetError("STEP export failed: " + std::string(e.what()));
        return false;
    }
}

void StepExporter::Clear() {
    m_curves.clear();
    m_surfaces.clear();
    m_lastError.clear();
}