#ifndef XGMM_IO_DENSE_HPP
#define XGMM_IO_DENSE_HPP
#include<vector>
#include<random>
#include<algorithm>
#include "../data.hpp"
namespace xgmm {
namespace io {
class DenseIterator;
class DenseData : public Data {
public:
    DenseData(int size, int dim): size(size), dim(dim){
        data.resize(size * dim);
    }
    virtual int Size() const { return size; };
    virtual int Dim() const { return dim; };
    virtual Iterator *GetIter();
    virtual ~DenseData() {};
    std::vector<float> data;
protected:
    int size;
    int dim;
};

class SimpleRandomData : public DenseData {
public:
    SimpleRandomData(int size) : DenseData(size, 2) {
        int c1 = size * 0.4;
        int c2 = size - c1;
        std::default_random_engine generator;
        std::normal_distribution<float>d1(0.3, 0.5);
        std::normal_distribution<float>d2(-0.5, 0.5);

        std::normal_distribution<float>d3(-1.0, 0.2);
        std::normal_distribution<float>d4(1.0, 0.2);
        for (int i = 0; i < c1; i++) {
            data[i * 2] = d1(generator);
            data[i * 2 + 1] = d2(generator);
        }
        for (int i = c1; i < size; i++) {
            data[i * 2] = d3(generator);
            data[i * 2 + 1] = d4(generator);
        }
    }
    virtual int Size() const { return size; }
    virtual int Dim() const { return dim; }
    virtual ~SimpleRandomData() {}
};

struct DenseIterator : public Iterator {
    DenseIterator(DenseData *dat, int size, int dim) : data(dat), step(size * dim) {}
    void BeforeFirst() { offset = 0; }
    bool Next(void) {
        if (offset >= data->Size()) return false;
        int size = std::min(step, data->Size() - offset);
        bat.resize(size);
        for (int i = 0; i < size; i++) {
            bat[i] = data->data[offset + i];
        }
        offset += size;
        return true;
    }
    const std::vector<float> &Batch() const {
        return bat;
    }
    virtual void SetBatchSize(int size) {
        step = size * dim;
    }
private:
    DenseData *data;
    int step;
    int dim;
    int offset;
    std::vector<float> bat;
};
} // end io
} // end xgmm
#endif
