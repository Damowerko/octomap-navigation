#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

using namespace matlab::engine;

int main() {
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();
}