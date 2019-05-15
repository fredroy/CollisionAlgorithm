#pragma once

#include <sofa/core/VecId.h>
#include <sofa/core/MultiVecId.h>
#include <sofa/defaulttype/BaseVector.h>
#include <sofa/core/ConstraintParams.h>
#include <sofa/core/behavior/MechanicalState.h>

namespace sofa
{

namespace collisionAlgorithm
{

/*!
 * \brief The BaseProximity class is the basic abstract proximity class
 */
class BaseProximity {
public :
    typedef std::shared_ptr<BaseProximity> SPtr;

    /// return proximiy position in a vector3
    virtual defaulttype::Vector3 getPosition(core::VecCoordId v = core::VecCoordId::position()) const = 0;

    /// return normal in a vector3
    virtual defaulttype::Vector3 getNormal() const = 0;

    virtual void buildJacobianConstraint(core::MultiMatrixDerivId cId, const helper::vector<defaulttype::Vector3> & dir, double fact, unsigned constraintId) const = 0;

    virtual void storeLambda(const core::ConstraintParams* cParams, core::MultiVecDerivId res, unsigned cid, const sofa::defaulttype::BaseVector* lambda) const = 0;

    virtual unsigned getElementId() const = 0;
};

/*!
 * Template implementation of BaseProximity
 */
template<class CONTAINER, class PROXIMITYDATA>
class TBaseProximity : public BaseProximity {
public:
    typedef typename CONTAINER::TDataTypes DataTypes;
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

    TBaseProximity(const CONTAINER * container, const PROXIMITYDATA & data)
    : m_container(container)
    , m_data(data) {}

    defaulttype::Vector3 getPosition(core::VecCoordId v = core::VecCoordId::position()) const override {
        return m_container->getPosition(m_data,v);
    }

    defaulttype::Vector3 getNormal() const override {
        return m_container->getNormal(m_data);
    }

    void buildJacobianConstraint(core::MultiMatrixDerivId cId, const helper::vector<defaulttype::Vector3> & normals, double fact, unsigned constraintId) const {
        m_container->buildJacobianConstraint(m_data,cId,normals,fact,constraintId);
    }

    void storeLambda(const core::ConstraintParams* cParams, core::MultiVecDerivId resId, unsigned cid, const sofa::defaulttype::BaseVector* lambda) const {
        m_container->storeLambda(cParams,resId,cid,lambda);
    }

    inline unsigned getElementId() const override {
        return m_data.getElementId();
    }

    inline BaseProximity::SPtr createProximity(const PROXIMITYDATA & data) const {
        return BaseProximity::SPtr(new TBaseProximity<CONTAINER,PROXIMITYDATA>(m_container, data));
    }

protected:
    const CONTAINER * m_container;
    const PROXIMITYDATA m_data;
};

template<class CONTAINER, class PROXIMITYDATA>
static BaseProximity::SPtr createProximity(const CONTAINER * container, const PROXIMITYDATA & data) {
    return BaseProximity::SPtr(new TBaseProximity<CONTAINER,PROXIMITYDATA>(container, data));
}

}

}
