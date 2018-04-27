#pragma once

#include <commonTypes.h>
#include <memory>
#include <map>
#include <vector>

namespace collisionAlgorithm {

class ConstraintElement;
class BaseGeometry;

class ConstraintProximity {
public :

    ConstraintProximity(ConstraintElement * elmt);

    virtual Vector3 getPosition(VecID v = VecCoordId::position()) const = 0;

    virtual Vector3 getNormal() const = 0;

    virtual std::map<unsigned,Vector3> getContribution(const Vector3 & N) = 0;

    inline ConstraintElement * element() const {
        return (ConstraintElement*) m_element;
    }

protected:
    ConstraintElement * m_element;
    State * m_state; // direct access to the state;
};

typedef std::shared_ptr<ConstraintProximity> ConstraintProximityPtr;

class ConstraintElement {
    friend class ConstraintProximity;
public:

    ConstraintElement(BaseGeometry * geo,unsigned nc)
    : m_geometry(geo)
    , m_controlPoints(nc) {}

    //this function returns a vector with all the control points of the element
    virtual ConstraintProximityPtr getControlPoint(const int i) = 0;

    //this function project the point P on the element and return the corresponding proximity
    virtual ConstraintProximityPtr project(Vector3 P) = 0;

    // return the number of control points
    unsigned getNbControlPoints() {
        return m_controlPoints;
    }

    template<class T = BaseGeometry>
    T * geometry() const {
        return (T*) m_geometry;
    }

protected:
    BaseGeometry * m_geometry;
    unsigned m_controlPoints;
};

typedef std::shared_ptr<ConstraintElement> ConstraintElementPtr;

class BaseGeometry : public BaseObject {
public:

    Port<Topology,REQUIRED> p_topology;
    Port<BaseObject> p_type;

    BaseGeometry()
    : p_topology("topology",LEFT,this)
    , p_type("any",LEFT,this) {
        m_dirty = true;
    }

    void init() {
        m_elements.clear();
    }

    void newStep() {
        if (! m_dirty) return;
        m_dirty = false;
        prepareDetection();
    }

    void handleEvent(Event * e) {
        if (dynamic_cast<AnimateBeginEvent *>(e)) newStep();
        else if (dynamic_cast<AnimateEndEvent *>(e)) m_dirty = true;
    }

    unsigned getNbElements() {
        newStep();
        return m_elements.size();
    }

    ConstraintElementPtr getElement(unsigned i) const {
        return m_elements[i];
    }

    virtual void prepareDetection() {}

    ConstraintProximityPtr project(const Vector3 & P) {
        double min_dist = std::numeric_limits<double>::max();
        ConstraintProximityPtr min_prox = NULL;

        for (unsigned i=0;i<m_elements.size();i++) {
            ConstraintProximityPtr pdest = m_elements[i]->project(P);
            double dist = (P - pdest->getPosition()).norm();
            if (dist<min_dist) {
                min_dist = dist;
                min_prox = pdest;
            }
        }

        return min_prox;
    }


protected:
    std::vector<ConstraintElementPtr> m_elements;
    bool m_dirty;

};




} // namespace controller
