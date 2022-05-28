#pragma once

#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/proximity/PointProximity.h>
#include <sofa/collisionAlgorithm/elements/PointElement.h>
#include <sofa/collisionAlgorithm/toolbox/PointToolBox.h>
#include <sofa/collisionAlgorithm/proximity/TopologyProximity.h>

namespace sofa {

namespace collisionAlgorithm {

template<class DataTypes>
class PointGeometry : public TBaseGeometry<DataTypes> {
public:
    typedef DataTypes TDataTypes;
    typedef PointElement ELEMENT;
    typedef PointGeometry<DataTypes> GEOMETRY;
    typedef TBaseGeometry<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
    typedef std::function<BaseProximity::SPtr(const PointElement * elmt)> ProximityCreatorFunc;

    SOFA_CLASS(GEOMETRY,Inherit);


    PointGeometry() {}

    void buildPointElements() override {
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());
        for (unsigned i=0;i<pos.size();i++) {
            auto prox = BaseProximity::SPtr(new TopologyProximity(this->getState(),i));
            this->insert(new PointElement(prox));
        }
    }

    ElementIterator::SPtr begin(unsigned id = 0) const override {
        return pointBegin(id);
    }

    inline ElementIterator::SPtr pointBegin(unsigned id = 0) const {
        return ElementIterator::SPtr(new TDefaultElementIteratorSPtr(this->pointElements(),id));
    }

};

}

}
