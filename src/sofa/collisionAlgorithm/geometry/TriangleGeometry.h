#pragma once

#include <sofa/collisionAlgorithm/geometry/EdgeGeometry.h>
#include <sofa/collisionAlgorithm/elements/DataTriangleElement.h>

namespace sofa {

namespace collisionAlgorithm {

template<class DataTypes>
class TriangleGeometry : public BaseGeometry<DataTypes> {
public:
    typedef DataTypes TDataTypes;
    typedef BaseGeometry<DataTypes> Inherit;
    typedef TriangleGeometry<DataTypes> GEOMETRY;

    SOFA_CLASS(GEOMETRY,Inherit);

    DataTriangleContainer<GEOMETRY> d_triangles;

    TriangleGeometry()
    : d_triangles(initData(&d_triangles, "triangles", "Triangles Container" )) {}

    void init() override {
        Inherit::init();

        ///To remove if we think every input has to be explicit
        if(d_triangles.getValue().empty())
        {
            msg_warning(this) << "Triangles are not set (data is empty). Will set from topology if present in the same context";
            sofa::core::topology::BaseMeshTopology* topology{nullptr};
            this->getContext()->get(topology);
            if(!topology)
            {
                msg_error(this) << "No topology to work with ; giving up.";
            }
            else
            {
                if(topology->getTriangles().empty())
                {
                    msg_error(this) << "No topology with triangles to work with ; giving up.";
                }
                else
                {
                    d_triangles.setParent(topology->findData("triangles"));
                }
            }
        }
    }

};


}

}
