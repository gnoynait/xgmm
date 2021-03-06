#ifndef XGMM_DATA_H
#define XGMM_DATA_H
#include<vector>
namespace xgmm{

struct Iterator {
    virtual void BeforeFirst(void) = 0;
    virtual bool Next(void) = 0;
    virtual const std::vector<float> &Batch(void) const = 0;
    virtual void SetBatchSize(int size) = 0;
};
struct Data {
    virtual int Size() const = 0;
    virtual int Dim() const = 0;
    virtual Iterator *GetIter() = 0;
};

} // end xgmm
#endif
