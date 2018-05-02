#pragma once

#include <BaseGeometry.h>

namespace collisionAlgorithm {


class AABBGeometry : public BaseGeometry {
    friend class AABBElement;

public:
    Data<Vec3i> d_nbox;
    Port<BaseGeometry,REQUIRED> p_geometry;

    AABBGeometry();

    ~AABBGeometry(){}

    void init();

    void prepareDetection();

    void draw(const VisualParams * vparams);

    virtual void handleEvent(Event * e) {
        p_geometry->handleEvent(e);
        BaseGeometry::handleEvent(e);
    }

    inline ReadAccessor<Vector3> read(VecID v) {
        return p_geometry->read(v);
    }

protected:
    Vector3 m_Bmin,m_Bmax,m_cellSize;
    Vec3i m_nbox;
    std::vector<std::vector<std::vector<std::vector<unsigned> > > >  m_indexedElement;
};

class AABBElement : public ConstraintElement {
public:
    AABBElement(AABBGeometry *geo);

    ConstraintProximityPtr getControlPoint(const int i);

    ConstraintProximityPtr project(Vector3 /*P*/);

    inline AABBGeometry * geometry() const {
        return (AABBGeometry*) m_geometry;
    }

private:
    void fillElementSet(Vec3i cbox,int d, std::set<int> & selectElements);
};

}
