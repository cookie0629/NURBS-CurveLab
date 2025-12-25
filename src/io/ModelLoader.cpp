#include "ModelLoader.h"
#include "../core/NurbsCurve.h"
#include "../core/NurbsSurface.h"

#include <STEPControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomConvert.hxx>
#include <IFSelect_ReturnStatus.hxx>

ModelLoader::ModelLoader() 
    : m_tolerance(1e-6)
    , m_loadCurves(true)
    , m_loadSurfaces(true) {
}

ModelLoader::~ModelLoader() {
    Clear();
}

bool ModelLoader::LoadFromStep(const std::string& filename) {
    Clear();
    
    try {
        STEPControl_Reader reader;
        
        IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());
        if (status != IFSelect_RetDone) {
            SetError("Failed to read STEP file: " + filename);
            return false;
        }
        
        // 转换所有实体
        int nbRoots = reader.NbRootsForTransfer();
        if (nbRoots == 0) {
            SetError("No transferable entities found in STEP file");
            return false;
        }
        
        reader.PrintCheckTransfer(Standard_False, IFSelect_ItemsByEntity);
        
        for (int i = 1; i <= nbRoots; i++) {
            Standard_Boolean ok = reader.TransferRoot(i);
            if (!ok) {
                SetError("Failed to transfer entity " + std::to_string(i));
                return false;
            }
        }
        
        int nbShapes = reader.NbShapes();
        if (nbShapes == 0) {
            SetError("No shapes found in STEP file");
            return false;
        }
        
        // 处理所有形状
        for (int i = 1; i <= nbShapes; i++) {
            TopoDS_Shape shape = reader.Shape(i);
            if (m_loadCurves) {
                ExtractCurvesFromShape(shape);
            }
            if (m_loadSurfaces) {
                ExtractSurfacesFromShape(shape);
            }
        }
        
        return true;
    }
    catch (const Standard_Failure& e) {
        SetError("STEP loading failed: " + std::string(e.GetMessageString()));
        return false;
    }
    catch (const std::exception& e) {
        SetError("STEP loading failed: " + std::string(e.what()));
        return false;
    }
}

bool ModelLoader::LoadFromIges(const std::string& filename) {
    Clear();
    
    try {
        IGESControl_Reader reader;
        
        IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());
        if (status != IFSelect_RetDone) {
            SetError("Failed to read IGES file: " + filename);
            return false;
        }
        
        reader.PrintCheckTransfer(Standard_False, IFSelect_ItemsByEntity);
        
        int nbShapes = reader.TransferRoots();
        if (nbShapes == 0) {
            SetError("No shapes transferred from IGES file");
            return false;
        }
        
        // 处理所有形状
        for (int i = 1; i <= nbShapes; i++) {
            TopoDS_Shape shape = reader.Shape(i);
            if (m_loadCurves) {
                ExtractCurvesFromShape(shape);
            }
            if (m_loadSurfaces) {
                ExtractSurfacesFromShape(shape);
            }
        }
        
        return true;
    }
    catch (const Standard_Failure& e) {
        SetError("IGES loading failed: " + std::string(e.GetMessageString()));
        return false;
    }
    catch (const std::exception& e) {
        SetError("IGES loading failed: " + std::string(e.what()));
        return false;
    }
}

void ModelLoader::ExtractCurvesFromShape(const TopoDS_Shape& shape) {
    TopExp_Explorer explorer(shape, TopAbs_EDGE);
    
    while (explorer.More()) {
        TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
        
        double first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        
        if (!curve.IsNull()) {
            // 尝试转换为B样条曲线
            Handle(Geom_BSplineCurve) bsplineCurve;
            
            if (curve->IsKind(STANDARD_TYPE(Geom_BSplineCurve))) {
                bsplineCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
            } else {
                // 转换其他类型的曲线为B样条
                try {
                    bsplineCurve = GeomConvert::CurveToBSplineCurve(curve);
                } catch (const Standard_Failure&) {
                    // 转换失败，跳过这条曲线
                    explorer.Next();
                    continue;
                }
            }
            
            if (!bsplineCurve.IsNull()) {
                // 创建NurbsCurve对象
                auto nurbsCurve = std::make_shared<NurbsCurve>();
                // 这里需要添加从Handle创建NurbsCurve的方法
                // 暂时跳过，等待后续实现
                // m_curves.push_back(nurbsCurve);
            }
        }
        
        explorer.Next();
    }
}

void ModelLoader::ExtractSurfacesFromShape(const TopoDS_Shape& shape) {
    TopExp_Explorer explorer(shape, TopAbs_FACE);
    
    while (explorer.More()) {
        TopoDS_Face face = TopoDS::Face(explorer.Current());
        
        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        
        if (!surface.IsNull()) {
            // 尝试转换为B样条曲面
            Handle(Geom_BSplineSurface) bsplineSurface;
            
            if (surface->IsKind(STANDARD_TYPE(Geom_BSplineSurface))) {
                bsplineSurface = Handle(Geom_BSplineSurface)::DownCast(surface);
            } else {
                // 转换其他类型的曲面为B样条
                try {
                    bsplineSurface = GeomConvert::SurfaceToBSplineSurface(surface);
                } catch (const Standard_Failure&) {
                    // 转换失败，跳过这个曲面
                    explorer.Next();
                    continue;
                }
            }
            
            if (!bsplineSurface.IsNull()) {
                // 创建NurbsSurface对象
                auto nurbsSurface = std::make_shared<NurbsSurface>();
                // 这里需要添加从Handle创建NurbsSurface的方法
                // 暂时跳过，等待后续实现
                // m_surfaces.push_back(nurbsSurface);
            }
        }
        
        explorer.Next();
    }
}

void ModelLoader::Clear() {
    m_curves.clear();
    m_surfaces.clear();
    m_lastError.clear();
}