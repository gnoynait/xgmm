#ifndef XGMM_IO_DENSE_HPP
#define XGMM_IO_DENSE_HPP
#include<vector>
#include "../data.hpp"
namespace xgmm {
namespace io {
class DenseData : public Data {
public:
    DenseData(int size, int dim): size(size), dim(dim){
        data.resize(size * dim);
    }
    virtual int Size() const { return size; };
    virtual int Dim() const { return dim; };
    virtual Iterator GetIter() = 0;
    std::vector<float> data;
private:
    int size;
    int dim;
};

struct DenseIterator : Iterator {
    DenseIterator(DenseData *data) : data(data) {}
    void BeforeFirst() { offset = -step; }
    bool Next(void) {
        if (offset + step >= data->Size()) return false;
        offset += step;
        return true;
    }
    std::vector<float> &Batch() {
        bat.resize(std::min(step, data->Size() - offset));
        for (int i = 0; offset + i < data->Size() && i < step; i++) {
            bat[i] = data->data[offset + i];
        }
        return bat;
    }
private:
    DenseData *data;
    int step;
    int offset;
    std::vector<float> bat;
};
} // end io
} // end xgmm
#endif
