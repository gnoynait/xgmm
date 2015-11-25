#ifndef XGMM_MODEL_MODEL_H
#define XGMM_MODEL_MODEL_H
#include<vector>
#include<math>

namespace xgmm{
namespace model{
template<typename TMean, typename TVar>
class IModel {
public:
    virtual float CalcProb(Batch &bat, std::vector<float> &res) const  = 0;
    virtual float Assign(Batch &bat, std::vector<int> &cluster) const = 0;
    virtual void GetWeight(std::vector<float> &weight) const = 0;
    virtual Update() = 0;
    virtual ~IModel();
};

struct IsoVar;
template<float, IsoVar>
class BaseModel : public IModel {
public:
    BaseModel(int components, int dim):components(components), dim(dim) {
        mean.resize(components * dim);
        var.resize(conpunents);
    }
    float CalcProb(const Batch &data, std::vector<float> &prob) const {
        float likelihood = 0.0f;
        prob.resize(Data.size() * components);
        for (int i = 0; i < n; i++) {
            for (int c = 0; c < components; c++) {
                float p = CalcProb(data, i, c);
                prob[i * components + c] = p;
                likelihood += p;
            }
        }
        return likelihood;
    }
    float Assign(IData &data, std::vector<int> &res) {
        float likelihood = 0.0f;
        res.ressize(data.size());
        for (int i = 0; i < data.Size(); i++) {
            int cMax = 0;
            float pMax = -1e-100;
            for (int c = 0; c < nComp; c++) {
                float p = CalcProb(data, i, c);
                likelihood += p;
                if (p > pMax) {
                    cMax = c;
                    pMax = p;
                }
            }
            res[i] = cMax;
        }
        return likelihood;
    }
    void GetWeight(std::vector<float> &weight) {
        weight.resize(nComp);
        for (int i = 0; i < nComp; i++) {
            weight[i] = std::exp(logWeight[i]);
        }
    }
private:
    inline float CalcProb(Batch &data, int idx, int comp) const {
        const float constTerm = M_LN2 + std::log(M_PI);
        float sumSq = 0.0f; 
        float diff = 0.0f;
        for (int d = 0; d < dim; d++) {
            diff = data[idx * dim + d] - mean[comp * dim + d];
            sum += diff * diff;
        }
        return -0.5 * (constTerm + dim * std::log(var[comp]) + sumSq / var[comp]) 
                + logWeight[comp];
    }
    std::vector<float> mean;
    std::vector<float> var;
    std::vector<float> logWeight;
    int nComp;
    int dim;
};

class EmUpdater {
public:
    void Update(const Batch& bat, BaseModel &model) {
        model.CalProb(bat, prob);        
    }
};

}
}

#endif
