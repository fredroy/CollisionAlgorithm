#pragma once

#include <memory>
#include <map>
#include <vector>
#include <sofa/core/VecId.h>
#include <sofa/core/MultiVecId.h>
#include <sofa/defaulttype/BaseVector.h>
#include <sofa/core/ConstraintParams.h>
#include <sofa/core/behavior/MechanicalState.h>

namespace sofa
{

namespace collisionAlgorithm
{

class BaseGeometry;

class BaseProximity {
public :
    typedef std::shared_ptr<BaseProximity> SPtr;
    typedef Data<helper::vector<defaulttype::Vector3> > DataVecCoord;

    virtual defaulttype::Vector3 getPosition(core::VecCoordId v = core::VecCoordId::position()) const = 0;

    virtual defaulttype::Vector3 getNormal() const = 0;

    virtual void buildJacobianConstraint(core::MultiMatrixDerivId cId, const helper::vector<defaulttype::Vector3> & dir, double fact, unsigned constraintId) const = 0;

    virtual sofa::core::behavior::BaseMechanicalState * getState() const = 0;

    virtual void storeLambda(const core::ConstraintParams* cParams, core::MultiVecDerivId res, const sofa::defaulttype::BaseVector* lambda) const = 0;

protected:
    template<class DataTypes>
    static void TstoreLambda(const core::ConstraintParams* cParams, Data<typename DataTypes::VecDeriv>& result, const Data<typename DataTypes::MatrixDeriv>& jacobian, const sofa::defaulttype::BaseVector* lambda) {
        auto res = sofa::helper::write(result, cParams);
        const typename DataTypes::MatrixDeriv& j = jacobian.getValue(cParams);
        j.multTransposeBaseVector(res, lambda ); // lambda is a vector of scalar value so block size is one.
    }

};

//Default Proximity for fixed position
template<class DataTypes>
class TBaseProximity : public BaseProximity {
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename DataTypes::Deriv Deriv1;
    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
    typedef core::objectmodel::Data< VecDeriv >        DataVecDeriv;
    typedef core::objectmodel::Data< MatrixDeriv >     DataMatrixDeriv;
    typedef sofa::core::behavior::MechanicalState<DataTypes> State;

    TBaseProximity(State * state)
    : m_state(state) {}

    virtual void addConstributions(MatrixDerivRowIterator & it, const defaulttype::Vector3 & N) const = 0;

    void buildJacobianConstraint(core::MultiMatrixDerivId cId, const helper::vector<defaulttype::Vector3> & normals, double fact, unsigned constraintId) const {
        DataMatrixDeriv & c1_d = *cId[m_state].write();
        MatrixDeriv & c1 = *c1_d.beginEdit();

        for (unsigned j=0;j<normals.size();j++) {
            MatrixDerivRowIterator c_it = c1.writeLine(constraintId+j);
            addConstributions(c_it, normals[j] * fact);
        }

        c1_d.endEdit();
    }

    sofa::core::behavior::BaseMechanicalState * getState() const { return m_state; }

    virtual void storeLambda(const core::ConstraintParams* cParams, core::MultiVecDerivId res, const sofa::defaulttype::BaseVector* lambda) const {
        BaseProximity::TstoreLambda<DataTypes>(cParams, *res[m_state].write(), *cParams->readJ(m_state), lambda);
    }

protected:
    State * m_state;

};

}

}
