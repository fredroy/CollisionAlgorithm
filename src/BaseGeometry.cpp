#include <BaseGeometry.h>
//#include <Eigen/Dense>
//#include <Eigen/Core>
//#include <Eigen/SVD>
#include <GL/gl.h>

namespace collisionAlgorithm {

ConstraintProximity::ConstraintProximity(ConstraintElement * elmt)
: m_element(elmt) {
    m_state = m_element->geometry()->p_topology->p_state();
}

//namespace core {

//namespace behavior {

////void ConstraintProximity::getControlPoints(helper::vector<Vector3> & controlPoints) {
////    helper::vector<double> prev = m_fact;
////    m_fact.clear();
////    m_fact.resize(prev.size(),0.0);
////    for (unsigned i=0;i<m_fact.size();i++) {
////        m_fact[i] = 1.0;
////        controlPoints.push_back(getPosition());
////        m_fact[i] = 0.0;
////    }
////    m_fact = prev;
////}

////void ConstraintProximity::inc(const helper::vector<double> & dir) {
////    //apply dx
////    for (unsigned i=0;i<dir.size();i++) m_fact[i]+=dir[i];

////    //clamp the bary coord to the element
//////            double sum = 0.0;
//////            for (unsigned i=0;i<m_fact.size();i++) sum += m_fact[i];
//////            if (sum == 0.0) return;
//////            for (unsigned j=0;j<m_fact.size();j++) m_fact[j] *= 1.0/sum;

////    for (unsigned i=0;i<m_fact.size();i++) {
////        if (m_fact[i] < 0) {
////            double remove = m_fact[i];
////            m_fact[i] = 0;
////            for (unsigned j=0;j<m_fact.size();j++) m_fact[j] /= (1.0 - remove);
//////                    usePoints[i] = false;
//////                    printf("CLAMP %d\n",i);
////        }
////    }

////    double sum = 0.0;
////    for (unsigned i=0;i<m_fact.size();i++) sum += m_fact[i];
////    if (sum == 0.0) return;
////    for (unsigned j=0;j<m_fact.size();j++) m_fact[j] *= 1.0/sum;

//////            std::cout << "FACT (" << sum << ") = " << m_fact << std::endl;
////}


//inline Eigen::MatrixXd pinv(const Eigen::MatrixXd & m) {
//    double epsilon= 1e-15;
//    Eigen::JacobiSVD<Eigen::MatrixXd> svd(m, Eigen::ComputeThinU | Eigen::ComputeThinV);
//    const Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType singVals = svd.singularValues();
//    Eigen::JacobiSVD<Eigen::MatrixXd>::SingularValuesType invSingVals = singVals;
//    for(int i=0; i<singVals.rows(); i++) {
//        if(singVals(i)*singVals(i) <= epsilon*epsilon) invSingVals(i) = 0.0;
//        else invSingVals(i) = 1.0 / invSingVals(i);
//    }
//    Eigen::MatrixXd S_inv = invSingVals.asDiagonal();
//    Eigen::MatrixXd m_inverse = svd.matrixV()*S_inv* svd.matrixU().transpose();
//    return m_inverse;
//}

//ConstraintProximityPtr BaseGeometry::ConstraintElement::project(Vector3 Q) {
//    const int maxIt = 1;
//    const double tolerance = 0.0001;
//    const double threshold = 0.0000001;
//    double delta = 0.001;

//    helper::vector<ConstraintProximityPtr> controlPoints = getConstrolPoints();

//    ConstraintProximityPtr res = getDefaultProximity();

//    if (controlPoints.size() <= 1) return res;

//    //check the control points that are necessary to activate
//    helper::vector<bool> usePoints(controlPoints.size(), true);
//    helper::vector<double> inc(controlPoints.size(), 0.0);

//    int it = 0;

//    while (it< maxIt) {
//        Vector3 P = res->getPosition();


////        for (unsigned i=0;i<m_fact.size();i++) {
////            if (m_fact[i] == 0) usePoints[i] = dot(Q - P,controlPoints[i] - P) > 0;
////            else usePoints[i] = true;

////            if (usePoints[i]) normals.push_back((controlPoints[i] - P)*delta);
////        }

//        unsigned JLin = controlPoints.size();

////        if (JLin == 0) break;

//        Eigen::VectorXd e0(JLin);
//        Eigen::MatrixXd J = Eigen::MatrixXd::Zero(JLin,JLin);

//        Vector3 PQ = Q-P;

//        helper::vector<Vector3> normals;

//        double err=0.0;
//        for (unsigned j=0;j<JLin;j++) {
////            helper::vector<double> inc(controlPoints.size(), 0.0);
////            inc[j] = delta;
////            ConstraintProximityPtr sigma = move(inc.ptr());
//            normals.push_back(controlPoints[j]->getPosition() - P);
//            double e = dot(PQ,normals[j]);
//            e0(j) = e;
//            err += e*e;
//        }

//        if (sqrt(err)<tolerance) break;

//        for (unsigned j=0;j<JLin;j++) {
//            const Vector3 R = P + normals[j] * delta;
//            for (unsigned i=0;i<JLin;i++) {
//                const double fxdx = dot(R, normals[i]);
//                J(i,j) = (fxdx - e0(i))/ delta;
//            }
//        }

//        std::cout << "e0=\n" << e0 << std::endl;
//        std::cout << "J=\n" << J << std::endl;

//        Eigen::MatrixXd invJ = pinv(J);
//        Eigen::VectorXd dx = -invJ * e0;

//        std::cout << "dx=\n" << dx << std::endl;

//        res->moveToControlPoints(dx.data());

////        helper::vector<double> Dx;

////        int k=0;
////        for (unsigned i=0;i<usePoints.size();i++) {
////            if (usePoints[i]) Dx.push_back(dx(k++));
////            else Dx.push_back(0.0);
////        }

////        helper::vector<double> prev = m_fact;

//        it++;
////        this->inc(Dx);


////        double res = 0.0;
////        for (unsigned i=0;i<m_fact.size();i++) res += std::pow((prev[i] - m_fact[i]),2);
////        if (sqrt(res) < threshold) break;

////                std::cout << "DX(" << sqrt(res) << ")=" << Dx << "    |||||| " << normals << std::endl;
//    }

//    return res;


////            double sum = 0.0;
////            for (unsigned i=0;i<m_fact.size();i++) sum += m_fact[i];
////            std::cout << "NEWTON ITERATIONS " << it << " sum=" << sum << " fact=" << m_fact << std::endl;
//}

//} // namespace controller

//} // namespace component




}
