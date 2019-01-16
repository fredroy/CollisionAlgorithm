﻿#pragma once

#include <sofa/collisionAlgorithm/BaseElement.h>
#include <sofa/collisionAlgorithm/geometry/PointGeometry.h>
#include <sofa/collisionAlgorithm/proximity/PointProximity.h>

namespace sofa
{

namespace collisionAlgorithm
{

//Internal iterator of elements
class SubsetIterator : public BaseElement {
public:
    SubsetIterator(const BaseGeometry * geometry, const std::set<unsigned> & subsetElements) : m_geometry(geometry), m_subsetElements(subsetElements) {
        m_iterator = m_subsetElements.cbegin();
    }

    BaseProximity::SPtr project(const defaulttype::Vector3 & P) const {
        return m_geometry->begin(id())->project(P);
    }

    BaseProximity::SPtr center() const {
        return m_geometry->begin(id())->center();
    }

    defaulttype::BoundingBox getBBox() const {
        return m_geometry->begin(id())->getBBox();
    }

    void next() {
        m_iterator++;
    }

    unsigned id() const {
        return *m_iterator;
    }

    bool end() const{
        return m_iterator==m_subsetElements.cend();
    }

    const BaseGeometry * m_geometry;
    const std::set<unsigned> m_subsetElements;
    std::set<unsigned>::iterator m_iterator;
};



}

}
