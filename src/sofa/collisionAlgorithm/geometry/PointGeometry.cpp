#include <sofa/collisionAlgorithm/geometry/PointGeometry.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa {

namespace collisionAlgorithm {

SOFA_DECL_CLASS(PointGeometry)

int PointGeometryClass = core::RegisterObject("PointGeometry")
.add< PointGeometry<sofa::defaulttype::Vec3dTypes> >();

}

}


