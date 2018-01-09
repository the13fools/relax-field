#ifndef WEAVEHOOK_H
#define WEAVEHOOK_H

#include "PhysicsHook.h"
#include "Weave.h"
#include "GaussNewton.h"
#include "Trace.h"
#include <string>

enum Shading_Enum {
    NONE = 0,
    F1_ENERGY,
    F2_ENERGY,
    F3_ENERGY,
    TOT_ENERGY
};

class WeaveHook : public PhysicsHook
{
public:
    WeaveHook() : PhysicsHook(), weave(NULL), vectorScale(1.0), normalizeVectors(true)
    {
        meshName = "meshes/torus.obj";
        params.lambdacompat = 100;
        params.lambdareg = 1e-3;
    }

    virtual void initGUI(igl::viewer::Viewer &viewer)
    {
        viewer.ngui->addVariable("Mesh", meshName);
        viewer.ngui->addGroup("Visualization");
        viewer.ngui->addVariable("Vector Scale", vectorScale);
        viewer.ngui->addVariable("Normalize Vectors", normalizeVectors);
        viewer.ngui->addGroup("Solver Parameters");
        viewer.ngui->addVariable("Compatilibity Lambda", params.lambdacompat);
        viewer.ngui->addVariable("Tikhonov Reg", params.lambdareg);

        viewer.ngui->addVariable("Shading", shading_state, true)
            ->setItems({ "None", "F1 Energy", "F2 Energy", "F3 Energy", "Total Energy" });

        // NOT HOOKED IN YET
        viewer.ngui->addVariable("Trace Field", isTraceField);
        //	viewer.ngui->addVariable("Trace Field", isTraceField);

        viewer.ngui->addButton("Normalize Fields", std::bind(&WeaveHook::normalizeFields, this));

    }

    void reassignPermutations();
    void normalizeFields();

    virtual void initSimulation()
    {
        if (weave)
            delete weave;
        weave = new Weave(meshName, 3);     
        Handle h;
        h.face = 0;
        h.dir << 1, 0;
        h.field = 2;
        weave->addHandle(h);
        h.face = 0;
        h.dir << 0, 1;
        h.field = 1;
        weave->addHandle(h);
        h.face = 0;
        h.dir << 1, -1;
        h.field = 0;
        weave->addHandle(h);
    }

    virtual void updateRenderGeometry()
    {
        renderQ = weave->V;
        renderF = weave->F;        
        weave->createVisualizationEdges(edgePts, edgeVecs, edgeSegs, edgeColors);
        faceColors.resize(weave->nFaces(), 3);
        faceColors.setConstant(0.3);
        baseLength = weave->averageEdgeLength;
        curFaceEnergies = tempFaceEnergies;
    }

    virtual bool simulateOneStep();    

    virtual void renderRenderGeometry(igl::viewer::Viewer &viewer);    

    void setFaceColors(igl::viewer::Viewer &viewer);
private:
    std::string meshName;
    Weave *weave;
    SolverParams params;
    Trace trace;

    double vectorScale;
    double baseLength;

    Eigen::MatrixXd faceColors;
    Eigen::MatrixXd curFaceEnergies;
    Eigen::MatrixXd tempFaceEnergies;
    Eigen::MatrixXd renderQ;
    Eigen::MatrixXi renderF;
    Eigen::MatrixXd edgePts;
    Eigen::MatrixXd edgeVecs;
    Eigen::MatrixXi edgeSegs;
    Eigen::MatrixXd edgeColors;    
    bool normalizeVectors;

    Shading_Enum shading_state = Shading_Enum::NONE;

    bool isTraceField;
};

#endif
