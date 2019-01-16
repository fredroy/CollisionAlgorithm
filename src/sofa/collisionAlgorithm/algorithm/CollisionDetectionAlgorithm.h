#pragma once

#include <sofa/collisionAlgorithm/BaseGeometryAlgorithm.h>
#include <sofa/collisionAlgorithm/BroadPhase.h>

namespace sofa
{

namespace collisionAlgorithm
{

class CollisionDetectionAlgorithm : public BaseGeometryAlgorithm
{
public:
    SOFA_CLASS(CollisionDetectionAlgorithm, BaseGeometryAlgorithm);

    Data<DetectionOutput> d_output;

    CollisionDetectionAlgorithm()
    : d_output(initData(&d_output, "output" , "this"))
    , l_from(initLink("from", "Link to from geometry"))
    , l_dest(initLink("dest", "Link to dest geometry"))
    {}

    virtual void computeCollisionReset() override;

    virtual void computeCollisionDetection() override;

private:
//    template<class ElementIterator>
    static DetectionOutput::PairDetection findClosestPoint(const BaseElement *itfrom, BaseElement::Iterator &itdest,const std::set<BaseFilter*> & filters = std::set<BaseFilter*>());

    core::objectmodel::SingleLink<CollisionDetectionAlgorithm,BaseGeometry,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_from;
    core::objectmodel::SingleLink<CollisionDetectionAlgorithm,BaseGeometry,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_dest;

    BaseElement::Iterator broadPhaseIterator(const defaulttype::Vector3 & P) const;

};

}

}
