#pragma once

#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/iterators/DefaultElementIterator.h>
#include <sofa/collisionAlgorithm/proximity/TriangleProximity.h>

namespace sofa {

namespace collisionAlgorithm {

template<class DataTypes>
class TriangleGeometry : public TBaseGeometry<DataTypes> {
public:
    typedef DataTypes TDataTypes;
    typedef TriangleGeometry<DataTypes> GEOMETRY;
    typedef TBaseGeometry<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef core::objectmodel::Data< VecCoord >        DataVecCoord;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;

    typedef size_t TriangleID;
    typedef sofa::core::topology::BaseMeshTopology::Triangle Triangle;
    typedef helper::vector<Triangle> VecTriangles;

    SOFA_CLASS(GEOMETRY,Inherit);

    core::objectmodel::SingleLink<GEOMETRY,core::topology::BaseMeshTopology,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_topology;

    TriangleGeometry()
    : l_topology(initLink("topology", "link to topology")) {
        l_topology.setPath("@.");
    }

    inline BaseElementIterator::UPtr begin(unsigned eid = 0) override {
        return DefaultElementIterator<GEOMETRY, TriangleProximity>::create(this, this->l_topology->getNbTriangles(), eid);
    }

    void draw(const core::visual::VisualParams * vparams) {
        this->drawNormals(vparams);

//        if (! vparams->displayFlags().getShowCollisionModels()) return;
        if (! this->drawCollision.getValue() && ! vparams->displayFlags().getShowCollisionModels()) return ;
        const defaulttype::Vector4 & color = this->d_color.getValue();
        if (color[3] == 0.0) return;

        glDisable(GL_LIGHTING);

        double delta = 0.2;
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());

        if (! vparams->displayFlags().getShowWireFrame()) glBegin(GL_TRIANGLES);
        else glBegin(GL_LINES);

        for (auto it=this->begin();it != this->end(); it++) {
            const Triangle& tri = this->l_topology->getTriangle(it->id());

            glColor4f(fabs(color[0]-delta),color[1],color[2],color[3]);
            glVertex3dv(pos[tri[0]].data());
            if (vparams->displayFlags().getShowWireFrame()) glVertex3dv(pos[tri[1]].data());

            glColor4f(color[0],fabs(color[1]-delta),color[2],color[3]);
            glVertex3dv(pos[tri[1]].data());
            if (vparams->displayFlags().getShowWireFrame()) glVertex3dv(pos[tri[2]].data());

            glColor4f(color[0],color[1],fabs(color[2]-delta),color[3]);
            glVertex3dv(pos[tri[2]].data());
            if (vparams->displayFlags().getShowWireFrame()) glVertex3dv(pos[tri[0]].data());
        }
        glEnd();
    }

    virtual void prepareDetection() override {
        const VecTriangles& triangles = this->l_topology->getTriangles();

        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());

        m_triangle_info.resize(triangles.size());
        m_triangle_normals.resize(triangles.size());

        for (size_t t=0 ; t<triangles.size() ; t++)
        {
            const Triangle& tri = triangles[t];

            //Compute Bezier Positions
            const defaulttype::Vector3 & p0 = pos[tri[0]];
            const defaulttype::Vector3 & p1 = pos[tri[1]];
            const defaulttype::Vector3 & p2 = pos[tri[2]];

            TriangleInfo & tinfo = m_triangle_info[t];
            tinfo.v0 = p1 - p0;
            tinfo.v1 = p2 - p0;

            tinfo.d00 = dot(tinfo.v0,tinfo.v0);
            tinfo.d01 = dot(tinfo.v0,tinfo.v1);
            tinfo.d11 = dot(tinfo.v1,tinfo.v1);

            tinfo.invDenom = 1.0 / (tinfo.d00 * tinfo.d11 - tinfo.d01 * tinfo.d01);

            tinfo.ax1 = tinfo.v0;
            m_triangle_normals[t] = tinfo.v0.cross(tinfo.v1);
            tinfo.ax2 = tinfo.v0.cross(m_triangle_normals[t]);

            tinfo.ax1.normalize();
            m_triangle_normals[t].normalize();
            tinfo.ax2.normalize();
        }
    }

    inline const sofa::core::topology::BaseMeshTopology::Triangle getTriangle(unsigned eid) const {
        return this->l_topology->getTriangle(eid);
    }

    inline defaulttype::Vector3 getNormal(const TriangleProximity & data) const {
        return m_triangle_normals[data.m_eid];
    }

    ////Bezier triangle are computed according to :
    ////http://www.gamasutra.com/view/feature/131389/b%C3%A9zier_triangles_and_npatches.php?print=1
    inline defaulttype::Vector3 getPosition(const TriangleProximity & data, core::VecCoordId v = core::VecCoordId::position()) const {
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(v);

        return pos[data.m_p0] * data.m_f0 +
               pos[data.m_p1] * data.m_f1 +
               pos[data.m_p2] * data.m_f2;
    }

    //Barycentric coordinates are computed according to
    //http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    inline void project(TriangleProximity & data, const defaulttype::Vector3 & P) const {
        const helper::ReadAccessor<DataVecCoord> & pos = this->getState()->read(core::VecCoordId::position());

        const TriangleInfo & tinfo = m_triangle_info[data.m_eid];
        core::topology::BaseMeshTopology::Triangle triangle = getTriangle(data.m_eid);

        defaulttype::Vector3 P0 = pos[triangle[0]];
        defaulttype::Vector3 P1 = pos[triangle[1]];
        defaulttype::Vector3 P2 = pos[triangle[2]];

        defaulttype::Vector3 x1x2 = P - P0;

        //corrdinate on the plane
        double c0 = dot(x1x2,tinfo.ax1);
        double c1 = dot(x1x2,tinfo.ax2);
        defaulttype::Vector3 proj_P = P0 + tinfo.ax1 * c0 + tinfo.ax2 * c1;

        double fact_u,fact_v,fact_w;

        computeBaryCoords(proj_P, tinfo, P0, fact_u,fact_v,fact_w);

        if (fact_u<0)
        {
            defaulttype::Vector3 v3 = P1 - P2;
            defaulttype::Vector3 v4 = proj_P - P2;
            double alpha = dot(v4,v3) / dot(v3,v3);

            if (alpha<0) alpha = 0;
            else if (alpha>1) alpha = 1;

            fact_u = 0;
            fact_v = alpha;
            fact_w = 1.0 - alpha;
        }
        else if (fact_v<0)
        {
            defaulttype::Vector3 v3 = P0 - P2;
            defaulttype::Vector3 v4 = proj_P - P2;
            double alpha = dot(v4,v3) / dot(v3,v3);

            if (alpha<0) alpha = 0;
            else if (alpha>1) alpha = 1;

            fact_u = alpha;
            fact_v = 0;
            fact_w = 1.0 - alpha;
        }
        else if (fact_w<0)
        {
            defaulttype::Vector3 v3 = P1 - P0;
            defaulttype::Vector3 v4 = proj_P - P0;
            double alpha = dot(v4,v3) / dot(v3,v3);

            if (alpha<0) alpha = 0;
            else if (alpha>1) alpha = 1;

            fact_u = 1.0 - alpha;
            fact_v = alpha;
            fact_w = 0;
        }

        data = TriangleProximity(data.m_eid, triangle[0], triangle[1], triangle[2], fact_u, fact_v, fact_w);
    }

protected:
    typedef struct
    {
        defaulttype::Vector3 v0,v1;
        double d00;
        double d01;
        double d11;
        double invDenom;

        defaulttype::Vector3 ax1,ax2;
    } TriangleInfo;

    //proj_P must be on the plane
    void computeBaryCoords(const defaulttype::Vector3 & proj_P,const TriangleInfo & tinfo, const defaulttype::Vector3 & p0, double & fact_u,double & fact_v, double & fact_w) const
    {
        defaulttype::Vector3 v2 = proj_P - p0;

        double d20 = dot(v2,tinfo.v0);
        double d21 = dot(v2,tinfo.v1);

        fact_v = (tinfo.d11 * d20 - tinfo.d01 * d21) * tinfo.invDenom;
        fact_w = (tinfo.d00 * d21 - tinfo.d01 * d20) * tinfo.invDenom;
        fact_u = 1.0 - fact_v  - fact_w;
    }

    std::vector<TriangleInfo> m_triangle_info;
    helper::vector<defaulttype::Vector3> m_triangle_normals;

};


}

}
