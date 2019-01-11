#pragma once

#include <sofa/collisionAlgorithm/BaseCollisionAlgorithm.h>

namespace sofa
{

namespace collisionAlgorithm
{

class CollisionDetectionAlgorithm : public BaseCollisionAlgorithm
{
public:
    SOFA_CLASS(CollisionDetectionAlgorithm, BaseCollisionAlgorithm);

    Data<double> d_minDist;
    Data<double> d_minAngle;

    CollisionDetectionAlgorithm()
        : BaseCollisionAlgorithm()
        , d_minDist(initData(&d_minDist, std::numeric_limits<double>::max(), "dist", "this"))
        , d_minAngle(initData(&d_minAngle, -1.0, "angle","this"))
        , l_from(initLink("from", "Link to from geometry"))
        , l_dest(initLink("dest", "Link to dest geometry"))
    {}

    void processAlgorithm() override;

private:
//    template<class ElementIterator>
    void findClosestPoint(BaseElement::Iterator & efrom);

    core::objectmodel::SingleLink<CollisionDetectionAlgorithm,BaseGeometry,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_from;
    core::objectmodel::SingleLink<CollisionDetectionAlgorithm,BaseGeometry,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_dest;

};

}

}
