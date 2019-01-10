#pragma once

#include <sofa/collisionAlgorithm/BaseGeometry.h>
#include <sofa/collisionAlgorithm/BaseElementFilter.h>
#include <sofa/core/collision/Pipeline.h>
#include <sofa/collisionAlgorithm/proximity/FixedProximity.h>

namespace sofa
{

namespace collisionAlgorithm
{

class DetectionOutput {
public:
    DetectionOutput(BaseProximity::SPtr p1, BaseProximity::SPtr p2)
    : m_firstProx(p1)
    , m_secondProx(p2) {}

    inline BaseProximity::SPtr getFirstProximity() const {
        return m_firstProx;
    }

    inline BaseProximity::SPtr getSecondProximity() const {
        return m_secondProx;
    }

protected:
    BaseProximity::SPtr m_firstProx;
    BaseProximity::SPtr m_secondProx;
};

class BaseCollisionAlgorithm : public core::collision::Pipeline
{
public :
    SOFA_ABSTRACT_CLASS(BaseCollisionAlgorithm, core::collision::Pipeline);

    BaseCollisionAlgorithm()
//        : l_filter(initLink("filter", "Link to an optional filter"))
    {

    }
    virtual ~BaseCollisionAlgorithm() override {}

    void draw(const core::visual::VisualParams * vparams) override
    {
        if (! vparams->displayFlags().getShowCollisionModels()) return;

        for (unsigned i=0;i<m_output.size();i++) {
            vparams->drawTool()->drawLine(m_output[i].getFirstProximity()->getPosition(),
                                          m_output[i].getSecondProximity()->getPosition(),
                                          defaulttype::Vector4(0,1,0,1));
        }
     }

    virtual void reset() override {}

    virtual void computeCollisionReset() override {
        m_output.clear();
    }

    virtual void computeCollisionResponse() override {}

    void computeCollisionDetection() override {
        processAlgorithm();
    }

    virtual std::set< std::string > getResponseList() const override
    {
        std::set< std::string > res;
        return res;
    }

    inline void addDetectionOutput(BaseProximity::SPtr p1,defaulttype::Vector3 & P) {
        m_output.push_back(DetectionOutput(p1,std::shared_ptr<FixedProximity>(new FixedProximity(P))));
    }

    inline void addDetectionOutput(BaseProximity::SPtr p1, BaseProximity::SPtr p2) {
        m_output.push_back(DetectionOutput(p1,p2));
    }

    const helper::vector<DetectionOutput> & getOutput() {
        return m_output;
    }

protected:
//    core::objectmodel::SingleLink<BaseCollisionAlgorithm,BaseElementFilter,BaseLink::FLAG_STRONGLINK|BaseLink::FLAG_STOREPATH> l_filter;

    virtual void doCollisionReset() override {}

    virtual void doCollisionDetection(const sofa::helper::vector<core::CollisionModel*>& /*collisionModels*/) override {}

    virtual void doCollisionResponse() override {}

    virtual void processAlgorithm() = 0;

private:
    helper::vector<DetectionOutput> m_output;

};

}

}
