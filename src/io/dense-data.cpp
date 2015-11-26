#include "dense-data.hpp"
namespace xgmm {
namespace io {
Iterator *DenseData::GetIter() {
    return new DenseIterator(this,  Size() ,dim);
}
} // end io
} // end xgmm
