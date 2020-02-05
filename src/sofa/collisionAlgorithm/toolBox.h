#pragma once


#include <sofa/defaulttype/defaulttype.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/SolidTypes.h>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <iostream>


namespace sofa
{

namespace collisionAlgorithm
{

typedef defaulttype::Vector3 Vec3d;

struct TriangleInfo
{
    Vec3d v0,v1;
    double d00;
    double d01;
    double d11;
    double invDenom;

    Vec3d ax1,ax2;
    Vec3d P0,P1,P2;
};


struct TetraInfo
{
    double V0;
    Vec3d P0,P1,P2,P3;
    Vec3d ax1,ax2,ax3,ax2Cax3;
};

namespace toolBox
{
//------------ Edges methods ------------//

inline void projectOnEdge(const Vec3d & projP, const Vec3d & e1, const Vec3d & e2, double & fact_u, double & fact_v);


//------------ Triangle methods ------------//

inline TriangleInfo computeTriangleInfo(const Vec3d & t0,const Vec3d & t1,const Vec3d & t2);

inline void computeTriangleBaryCoords(const Vec3d & proj_P, const TriangleInfo & tinfo, double & fact_u, double & fact_v, double & fact_w);

inline void projectOnTriangle(const Vec3d projectP, const TriangleInfo & tinfo, double & fact_u, double & fact_v, double & fact_w);


//------------ Tetrahedra methods ------------//

inline TetraInfo computeTetraInfo(const Vec3d & p0, const Vec3d & p1, const Vec3d & p2, const Vec3d & p3);

inline void computeTetraBaryCoords(const Vec3d & P, const TetraInfo & tinfo, double & fact_u,double & fact_v, double & fact_w, double & fact_x);

inline void projectOnTetra(const Vec3d & projectP,  const TetraInfo & tinfo, double & fact_u, double & fact_v, double & fact_w, double & fact_x);


}

} // namespace component

} // namespace sofa

