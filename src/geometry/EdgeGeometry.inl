﻿#pragma once

#include <geometry/EdgeGeometry.h>
#include <GL/gl.h>

namespace collisionAlgorithm {

/**************************************************************************/
/******************************PROXIMITY***********************************/
/**************************************************************************/

EdgeProximity::EdgeProximity(EdgeElement * elmt,double f1,double f2) : ConstraintProximity(elmt) {
    m_fact[0] = f1;
    m_fact[1] = f2;
}

Vector3 EdgeProximity::getPosition(TVecId v) const {
    const std::vector<Vector3> & pos = element()->geometry()->getPos(v);
    return pos[element()->m_pid[0]] * m_fact[0] + pos[element()->m_pid[1]] * m_fact[1];
}

Vector3 EdgeProximity::getNormal() const {
    return Vector3(1,0,0);
}

std::map<unsigned,Vector3> EdgeProximity::getContribution(const Vector3 & N) {
    std::map<unsigned,Vector3> res;

    res[element()->m_pid[0]] = N * 1.0/2.0;
    res[element()->m_pid[1]] = N * 1.0/2.0;

    return res;
}

/**************************************************************************/
/******************************ELEMENT*************************************/
/**************************************************************************/

EdgeElement::EdgeElement(EdgeGeometry * geo,unsigned eid) : ConstraintElement(geo) {
    m_eid = eid;

    const std::vector<TEdge> & edges = geometry()->p_topology->getEdges();

    m_pid[0] = edges[eid][0];
    m_pid[1] = edges[eid][1];
}

ConstraintProximityPtr EdgeElement::getControlPoint(const int cid) {
    if (cid == 0) return std::make_shared<EdgeProximity>(this,1,0);
    else if (cid == 1) return std::make_shared<EdgeProximity>(this,0,1);
    return NULL;
}

unsigned EdgeElement::getNbControlPoints() {
    return 2;
}

//this function project the point P on the element and return the corresponding proximity
ConstraintProximityPtr EdgeElement::project(Vector3 P) {
    double fact_u,fact_v;

    const std::vector<Vector3> & pos = geometry()->getPos(TVecId::position);

    Vector3 P1 = pos[m_pid[0]];
    Vector3 P2 = pos[m_pid[1]];

    Vector3 v = P2-P1;
    fact_v = dot(P - P1,v) / dot(v,v);

    if (fact_v<0.0) fact_v = 0.0;
    else if (fact_v>1.0) fact_v = 1.0;

    fact_u = 1.0-fact_v;

    return std::make_shared<EdgeProximity>(this,fact_u,fact_v);
}

void EdgeElement::draw(const std::vector<Vector3> & X) {
    glBegin(GL_LINES);
        glVertex3dv(X[m_pid[0]].data());
        glVertex3dv(X[m_pid[1]].data());
    glEnd();
}

/**************************************************************************/
/******************************GEOMETRY************************************/
/**************************************************************************/

void EdgeGeometry::prepareDetection() {
    if (! m_elements.empty()) return;

    for (unsigned i=0;i<p_topology->getEdges().size();i++) {
        m_elements.push_back(std::make_shared<EdgeElement>(this,i));
    }
}

}
