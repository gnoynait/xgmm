#ifndef XGMM_MODEL_MODEL_H
#define XGMM_MODEL_MODEL_H
#include<vector>

namespace xgmm{
namespace model{
template<typename TMean, typename TVar>
class IModel {
public:
	virtual float likelyhood(IData data)=0;
	virtual float likelyHood(IData data, int i)=0;
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
	void Likelyhood(IData &data, std::vector<float> &likelyhood) {
		likelyhood.resize(data.size() * components);
		for (int i = 0; i < n; i++) {
			for (int c = 0; c < components; c++) {
				for (int d = 0; d < dim; d++) {
					float n = data[i * dim + d] - mean[ c * dim + d];
					float r = n * n;
					likelyhood[i] = -0.5 * (log(PI*2.0) + log(var[c]) + r * r / var[c]);
				}
			}
		}
	}
	void Responsibility(IData &data, std::vector<float> &res) {
	}
	void Cluster(IData &data, std::vector<float> &cluster) {
	}
private:
	std::vector<float> mean;
	std::vector<float> var;
	int components;
	int dim;
};
}
}
















#endif
