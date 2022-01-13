#pragma once

#include <sofa/collisionAlgorithm/BaseAlgorithm.h>
#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/BaseOperation.h>
#include <sofa/collisionAlgorithm/operations/CreateCenterProximity.h>
#include <sofa/collisionAlgorithm/operations/Project.h>

namespace sofa::collisionAlgorithm {

class FindClosestProximityAlgorithm_BroadPhase : public Operations::GenericOperation<std::function<BaseElement::Iterator(type::Vector3,BaseGeometry *) > > {
public:

    using Inherit = GenericOperation;

    //By default no broadPhase so we loop over all elements
    GenericOperation::FUNC getDefault() const override {
        return [=](type::Vector3 ,BaseGeometry * geo) -> BaseElement::Iterator {
            return geo->begin();
        };
    }

};

class FindClosestProximityAlgorithm : public BaseAlgorithm {
public:
    SOFA_CLASS(FindClosestProximityAlgorithm, BaseAlgorithm);

    core::objectmodel::SingleLink<FindClosestProximityAlgorithm,BaseGeometry,BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> l_from;
    core::objectmodel::SingleLink<FindClosestProximityAlgorithm,BaseGeometry,BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> l_dest;
    Data<bool> d_drawCollision ;
    Data<DetectionOutput> d_output;
    Data<sofa::type::vector<double> > d_outputDist;

    FindClosestProximityAlgorithm()
    : l_from(initLink("from", "link to from geometry"))
    , l_dest(initLink("dest", "link to dest geometry"))
    , d_drawCollision (initData(&d_drawCollision, true, "drawcollision", "draw collision"))
    , d_output(initData(&d_output,"output", "output of the collision detection"))
    , d_outputDist(initData(&d_outputDist,"outputDist", "Distance of the outpu pair of detections"))
    , m_distance([=](BaseProximity::SPtr a,BaseProximity::SPtr b){ return (a->getPosition()-b->getPosition()).norm(); })
    {}

    void draw(const core::visual::VisualParams* vparams) {
        if (! vparams->displayFlags().getShowCollisionModels() && ! d_drawCollision.getValue()) return;
        glDisable(GL_LIGHTING);
        glColor4f(0,1,0,1);

        glBegin(GL_LINES);
        DetectionOutput output = d_output.getValue() ;
        for (unsigned i=0;i<output.size();i++) {
            glVertex3dv(output[i].first->getPosition().data());
            glVertex3dv(output[i].second->getPosition().data());
        }
        glEnd();
    }

    void doDetection() {
        if (l_from == NULL) return;
        if (l_dest == NULL) return;

        DetectionOutput & output = *d_output.beginEdit();
        output.clear();

        auto createProximityOp = Operations::CreateCenterProximity::func(l_from);
        auto projectOp = Operations::Project::func(l_dest);
        auto broadPhaseOp = FindClosestProximityAlgorithm_BroadPhase::func(l_dest);

        for (auto itfrom=l_from->begin();itfrom!=l_from->end();itfrom++) {
            auto pfrom = createProximityOp(itfrom->element());
            if (pfrom == nullptr) continue;

            double min_dist = std::numeric_limits<double>::max();
            PairDetection min_pair;

            for (auto itdest = broadPhaseOp(pfrom->getPosition(),l_dest);itdest!=l_dest->end();itdest++) {
                auto edest = *itdest;
                if (edest == nullptr) continue;

                BaseProximity::SPtr pdest = projectOp(pfrom->getPosition(),edest);

                double d = m_distance(pfrom,pdest);
                if (d < min_dist) min_pair = PairDetection(pfrom,pdest);
            }

            if (min_pair.first != nullptr && min_pair.second != nullptr) output.push_back(min_pair);
        }

        d_output.endEdit();
    }

private:
    std::function<double(BaseProximity::SPtr,BaseProximity::SPtr)> m_distance;

};

}

