﻿#pragma once

#include <geometry/PointGeometry.h>

namespace collisionAlgorithm {

/**************************************************************************/
/******************************ELEMENT*************************************/
/**************************************************************************/

class PointElement : public ConstraintElement {
    friend class PointProximity;
    friend class PointGeometry;

public:

    inline ConstraintProximityPtr createProximity(const int i);

    PointElement(PointGeometry *geo, unsigned pid) : ConstraintElement(geo,1) {
        m_pid = pid;
    }

    ConstraintProximityPtr getControlPoint(const int i) {
        return createProximity(i);
    }

    //this function project the point P on the element and return the corresponding proximity
    ConstraintProximityPtr project(Vector3 /*P*/) {
        return getControlPoint(0);
    }

    PointGeometry * geometry() {
        return (PointGeometry * ) m_geometry;
    }

protected:
    unsigned m_pid;
};

/**************************************************************************/
/******************************PROXIMITY***********************************/
/**************************************************************************/

class PointProximity : public ConstraintProximity {
public :
    PointProximity(PointElement * elmt) : ConstraintProximity(elmt) {}

    Vector3 getPosition(VecID v) const {
        const ReadAccessor<Vector3> & pos = element()->geometry()->read(v);
        return pos[element()->m_pid];
    }

    Vector3 getNormal() const {
        return Vector3(1,0,0);
    }

    std::map<unsigned,double> getContributions() {
        std::map<unsigned,double> res;

        res[element()->m_pid] = 1.0;

        return res;
    }

    inline PointElement * element() const {
        return (PointElement*) m_element;
    }
};

ConstraintProximityPtr PointElement::createProximity(const int i) {
    return std::make_shared<PointProximity>(this);
}

}
