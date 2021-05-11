#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include <iostream>

using namespace matlab::engine;

int main()
{
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();
    matlab::data::ArrayFactory factory;
    std::vector<matlab::data::Array> args({
        factory.createArray<double>({1, 10}, {4, 8, 6, -1, -2, -3, -1, 3, 4, 5})
    });
    matlabPtr->feval(u"plot", args);
    std::cin.get();
}