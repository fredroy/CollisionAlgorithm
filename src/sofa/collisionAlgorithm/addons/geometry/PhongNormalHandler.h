#pragma once

#include <sofa/collisionAlgorithm/BaseGeometryModifier.h>

namespace sofa
{

namespace collisionAlgorithm
{

template<typename TGeometry>
class PhongNormalHandler : public TNormalHandler<TGeometry>
{
protected:
    defaulttype::Vector3 internalGetNormal(const BaseGeometry*, const size_t, const double* fact) const ;
};

}

}
