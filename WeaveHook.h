#ifndef WEAVEHOOK_H
#define WEAVEHOOK_H

#include "PhysicsHook.h"
#include "Weave.h"
#include "GaussNewton.h"
#include "Traces.h"
#include <string>
#include "Surface.h"
#include <igl/unproject_onto_mesh.h>

class CoverMesh;

enum WeaveShading_Enum {
    WS_NONE = 0,
    F1_ENERGY,
    F2_ENERGY,
    F3_ENERGY,
    TOT_ENERGY,
    WS_CONNECTION_ENERGY
};

enum CoverShading_Enum {
    CS_NONE = 0,
    CS_S_VAL,
    FUN_VAL,
    CS_CONNECTION_ENERGY
};

enum GUIMode_Enum {
    WEAVE = 0,
    COVER
};

class WeaveHook : public PhysicsHook
{
public:
    WeaveHook() : PhysicsHook(), weave(NULL), cover(NULL), vectorScale(1.0), normalizeVectors(true)
    {
        gui_mode = GUIMode_Enum::WEAVE;
        weave_shading_state = WeaveShading_Enum::WS_NONE;
        cover_shading_state = CoverShading_Enum::CS_NONE;
        // meshName = "meshes/bunny_coarser.obj";
        meshName = "meshes/tet.obj";
        // vectorFieldName = "bunny_coarser_nosing";
        vectorFieldName = "tet.rlx";
        rodFilename = "example.rod";
        exportPrefix = "export/example";
        params.lambdacompat = 100;
        params.lambdareg = 1e-3;

        traceIdx = 0;
        traceSign = 1;
        traceSteps = 100;
        traceFaceId = 0;
        
        hideVectors = false;
        showSingularities = false;
        wireframe = false;

        targetResolution = 5000;
    
        handleLocation = 0;
        handleParams = Eigen::VectorXd::Zero(6);
        handleParams(0) = 0;
        handleParams(1) = 1;
        handleParams(2) = 1;
        handleParams(3) = 0;
        handleParams(4) = 1;
        handleParams(5) = -1;

        showCoverCuts = true;
        numISOLines = 0;
        
        initSReg = 1e-4;
        globalSScale = 1.0;

        showTraces = true;
        showRatTraces = true;
        extendTrace = 0.1;
        segLen = 0.02;
        maxCurvature = 0.5;
        minRodLen = 1.0;
    }

    virtual void drawGUI(igl::opengl::glfw::imgui::ImGuiMenu &menu);
    virtual bool mouseClicked(igl::opengl::glfw::Viewer &viewer, int button);

    void reassignPermutations();
    void normalizeFields();
    void serializeVectorField();
    void deserializeVectorField();    
    void deserializeVectorFieldOld();
    void augmentField();
    void initializeS();
    void initializeSAlt();
    void computeFunc();
    void drawISOLines();
    void resetCutSelection();
    void addCut();
    void resample();
    void removeSingularities();
    void removePrevCut(); 
    void clearTraces();
    void deleteLastTrace();
    void computeTrace();   
    void rationalizeTraces();
    void saveRods();
    void exportForRendering();
    
    virtual void initSimulation();

    virtual void updateRenderGeometry();

    virtual bool simulateOneStep();    

    virtual void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer);    

    void setFaceColorsWeave(igl::opengl::glfw::Viewer &viewer);
    void setFaceColorsCover(igl::opengl::glfw::Viewer &viewer);
 
    void drawCuts(igl::opengl::glfw::Viewer &viewer);

    void showCutVertexSelection(igl::opengl::glfw::Viewer &viewer);
    void updateSingularVerts(igl::opengl::glfw::Viewer &viewer);
private:
    void clear();
    std::string meshName;
    Weave *weave;
    CoverMesh *cover;
    SolverParams params;

    TraceSet traces;

    std::vector<std::pair<int, int > > selectedVertices; // (face, vert) pairs
    
    double vectorScale;
    double baseLength;

    Eigen::VectorXd handleParams;
    int handleLocation;

    Eigen::MatrixXd curFaceEnergies;
    Eigen::MatrixXd tempFaceEnergies;
    Eigen::MatrixXd renderQWeave;
    Eigen::MatrixXi renderFWeave;
    Eigen::MatrixXd edgePtsWeave;
    Eigen::MatrixXd edgeVecsWeave;
    Eigen::MatrixXi edgeSegsWeave;
    Eigen::MatrixXd edgeColorsWeave;    
    Eigen::MatrixXd edgePtsCover;
    Eigen::MatrixXd edgeVecsCover;
    Eigen::MatrixXi edgeSegsCover;
    Eigen::MatrixXd edgeColorsCover;    
    std::vector<Eigen::Vector3d> renderSelectedVertices; // teal selected vertex spheres
    bool normalizeVectors;
    bool hideVectors;
    bool showCoverCuts;
    bool wireframe;

    Eigen::MatrixXd renderQCover;
    Eigen::MatrixXi renderFCover;
    

    GUIMode_Enum gui_mode;
    WeaveShading_Enum weave_shading_state;
    CoverShading_Enum cover_shading_state;
    Trace_Mode trace_state = Trace_Mode::GEODESIC;
    
    int traceIdx;
    int traceSign;
    int traceFaceId;
    int traceSteps;
    int targetResolution;
    
    bool showSingularities;
    Eigen::MatrixXd singularVerts_topo;
    Eigen::MatrixXd singularVerts_geo;
    Eigen::MatrixXd nonIdentity1Weave;
    Eigen::MatrixXd nonIdentity2Weave;
    Eigen::MatrixXd cutPos1Weave; // endpoints of cut edges
    Eigen::MatrixXd cutPos2Weave;
    Eigen::MatrixXd cutPos1Cover;
    Eigen::MatrixXd cutPos2Cover;
    Eigen::MatrixXd cutColorsCover;

    std::string vectorFieldName;
    std::string exportPrefix;

    bool showTraces;
    bool showRatTraces;
    double extendTrace;
    double segLen;
    double maxCurvature;
    double minRodLen;
    // isolines on the split mesh
    Eigen::MatrixXd pathstarts;
    Eigen::MatrixXd pathends;
    // traces on the single mesh
    Eigen::MatrixXd tracestarts;
    Eigen::MatrixXd traceends;
    Eigen::MatrixXd tracecolors;
    
    std::string rodFilename;

    Eigen::MatrixXd rattracestarts;
    Eigen::MatrixXd rattraceends;
    Eigen::MatrixXd ratcollisions;
    int numISOLines;
    double initSReg;
    double globalSScale;
};

#endif
