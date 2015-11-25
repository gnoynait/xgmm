#ifndef XGMM_MODEL_PARAM_H
#define XGMM_MODEL_PARAM_H
namespace xgmm {
namespace model {

class IParam {
public:
    virtual float CalProb(const Inst& inst) = 0;
    virtual float Update(IParam &nParam, float rate) = 0;
};



struct IDist {
public:
    virtual float Likely(const Inst &inst) = 0;
    virtual float Prior() = 0;
};




struct IMixture {
public:
    virtual float Likely(const Batch &bat) = 0;
    virtual float Prior() = 0;
    virtual int Components() = 0;
    virtual void Weight() = 0;
    virtual int Weight(int i) = 0;
};
















}
}
#endif

