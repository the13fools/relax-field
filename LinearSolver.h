#ifndef LinearSolver_H
#define LinearSolver_H

#include <Eigen/Core>
#include <vector>
#include <Eigen/Sparse>

class Weave;
struct SolverParams;

struct Handle;
// {
//     int face;
//     Eigen::Vector2d dir;
// };

class LinearSolver
{
public:
 //   LinearSolver(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F);
    void addHandle(const Handle &h);
    void clearHandles();
    const std::vector<Handle> &getHandles() { return handles; }

    // void generateRandomField(Eigen::VectorXd &primalVars, Eigen::VectorXd &dualVars);
    // void generateHarmonicField(Eigen::VectorXd &primalVars, Eigen::VectorXd &dualVars);
    void curlOperator(const Weave &weave, Eigen::SparseMatrix<double> &curlOp);
    void differentialOperator(const Weave &weave, Eigen::SparseMatrix<double> &D);
    void unconstrainedProjection(const Weave &weave, Eigen::SparseMatrix<double> &proj);

    void updatePrimalVars(const Weave &weave, Eigen::VectorXd &primalVars, const Eigen::VectorXd &dualVars, double smoothingCoeff);
    void updateDualVars(const Weave &weave, const Eigen::VectorXd &primalVars, Eigen::VectorXd &dualVars);

    // Problem dimensions
    // int numPrimalDOFs(); // involved in GN part of optimization
    // int numDualDOFs(); // involved in eigenvector problem part of optimization
   // void setFaceEnergies(const Eigen::VectorXd &primalVars, const Eigen::VectorXd &dualVars, Eigen::VectorXd &faceEnergies);

private:
    // Eigen::MatrixXd V;
    // Eigen::MatrixXi F;
    // Eigen::MatrixXi E;
    // Eigen::MatrixXd centroids;

    std::vector<Handle> handles;

};

#endif