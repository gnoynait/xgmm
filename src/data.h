#ifndef XGMM_DATA_H
#define XGMM_DATA_H
#include<vector>
namespace xgmm{
struct IData {
	virtual int Size() = 0;
	virtual int Dim() = 0;
	virtual ~IData(void) = 0;
};

class DenseData : public IData {
public:
	DenseData(int size, int dim): size(size), dim(dim){
		data.resize(size * dim);
	}
	virtual int Size() const { return size };
	virtual int Dim() const { return dim; };
private:
	std::vector<float> data;
	int size;
	int dim;
};
struct Iterator {
	virtual void BeforeFirst(void) = 0;
	virtual bool Next(void) = 0;
	virtual const DType &Value(void) const = 0;
};

#endif