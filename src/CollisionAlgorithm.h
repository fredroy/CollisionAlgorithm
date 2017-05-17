#ifndef SOFA_COMPONENT_CONSTRAINT_COLLISIONALGORITHM_H
#define SOFA_COMPONENT_CONSTRAINT_COLLISIONALGORITHM_H

#include <sofa/defaulttype/SolidTypes.h>
#include <sofa/core/behavior/BaseController.h>
#include <sofa/core/behavior/MechanicalState.h>
#include "ConstraintGeometry.h"
#include "ConstraintProximity.h"

namespace sofa {

namespace core {

namespace behavior {

class CollisionAlgorithm {
public:

    //
    static ConstraintProximity findClosestProximity(const defaulttype::Vector3 & T, BaseGeometry * geo);

};

} // namespace controller

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_CONTROLLER_NeedleConstraint_H
