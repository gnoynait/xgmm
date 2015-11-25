#ifndef XGMM_MODEL_MODEL_H
#define XGMM_MODEL_MODEL_H
#include<vector>
#include<cmath>
#include "../utils/math.h"

namespace xgmm{
namespace model{
class IModel {
public:
    virtual float CalcProb(std::vector<float> &bat, std::vector<float> &res) const  = 0;
    virtual float Assign(std::vector<float> &bat, std::vector<int> &cluster) const = 0;
    virtual void GetWeight(std::vector<float> &weight) const = 0;
    virtual void Update() = 0;
    virtual ~IModel();
};

class BaseModel : public IModel {
public:
    BaseModel(int components, int dim):nComp(components), dim(dim) {
        mean.resize(components * dim);
        var.resize(components);
    }
    virtual float CalcProb(const std::vector<float> &bat, std::vector<float> &prob) const {
        int size = bat.size() / dim;
        float likelihood = 0.0f;
        prob.resize(size * nComp);
        for (int i = 0; i < size; i++) {
            for (int c = 0; c < nComp; c++) {
                float p = CalcProb(bat, i, c);
                prob[i * nComp + c] = p;
                likelihood += p;
            }
        }
        return likelihood;
    }
    virtual float Assign(const std::vector<float> &bat, std::vector<int> &res) {
        int size = bat.size() / dim;
        float likelihood = 0.0f;

        res.resize(size);
        for (int i = 0; i < size; i++) {
            int cMax = 0;
            float pMax = -1e-100;
            for (int c = 0; c < nComp; c++) {
                float p = CalcProb(bat, i, c);
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
        utils::Exp(logWeight, weight);
    }
    virtual void Update(const std::vector<float> &bat, std::vector<float> &prob, float rate) {
        int size = bat.size() / dim;
        utils::Fill(mean, 0.0);
        utils::Fill(var, 0.0);
        utils::Fill(logWeight, 0.0);
        for (int i = 0; i < size; i++) {
            for (int c = 0; c < nComp; c++) {
                for (int d = 0; d < dim; d++) {
                    mean[c * dim + d] += bat[i * dim + d] * prob[i * nComp + c];
                }
            }
        }
        for (int c = 0; c < nComp; c++) {
            for (int d = 0; d < dim; d++) {
                mean[c * dim + d] /= size;
            }
        }
        utils::Divide(mean, size);
        for (int i = 0; i < size; i++) {
            for (int c = 0; c < nComp; c++) {
                for (int d = 0; d < nComp; c++) {
                    float diff = bat[i * dim + d] - mean[c * dim + d];
                    var[i] += diff * diff;
                }
            }
        }
        for (int c = 0; c < nComp; c++) {
            var[c] /= size;
        }
        for (int i = 0; i < size; i++) {
            for (int c = 0; c < nComp; c++) {
                logWeight[c] += prob[i * nComp + c];
            }
        }
        utils::Log(logWeight);
    }
private:
    inline float CalcProb(const std::vector<float> &bat, int idx, int comp) const {
        const float constTerm = M_LN2 + std::log(M_PI);
        float sumSq = 0.0f; 
        float diff = 0.0f;
        for (int d = 0; d < dim; d++) {
            diff = bat[idx * dim + d] - mean[comp * dim + d];
            sumSq += diff * diff;
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
    void Update(const std::vector<float>& bat, BaseModel &model) {
        model.CalcProb(bat, prob);        
        model.Update(bat, prob, 1.0);
    }
private:
    std::vector<float> prob;
};

}
}

#endif
