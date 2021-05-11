#include "octonav/visualize.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

// #include <octomap/octomap.h>
// #include <octomap/OcTree.h>

using std::cout;
using std::ios_base;
using std::ofstream;
using std::string;
using std::vector;

using namespace octomap;

// class Sinewave
// {
// public:
//     vector<double> yvals;
//     vector<double> xvals;
//     // yvals = sin(a*t + b) and ownership of t is transferred to this object (as xvals) if it's an R-value
//     // a = angular frequency; the value resulting from 2*pi*f
//     // b = initial phase, in radians
//     Sinewave(double a, double b, vector<double> &&t) : xvals(std::move(t))
//     {
//         yvals = vector<double>(xvals.size());
//         // Compute the angle values
//         std::transform(xvals.begin(), xvals.end(), yvals.begin(), [a, b](double xval) -> double { return a * xval + b; });

//         // Compute the sin-values
//         std::transform(yvals.begin(), yvals.end(), yvals.begin(), [](double angle) -> double { return sin(angle); });
//     }
//     // Transfer ownership of other SineSignal object's yvals, xvals
//     Sinewave(const Sinewave &&other) noexcept
//         : yvals(std::move(yvals)), xvals(std::move(xvals)) {}

//     // Output to csv file for quick plotting in matlab
//     bool ToCsvFile(const string path)
//     {
//         ofstream csvFile(path, ios_base::trunc);
//         if (!csvFile)
//             return false; // Failure
//         vector<string> valPairs;
//         for (size_t i = 0; i < xvals.size(); i++)
//         {
//             std::stringstream ss;
//             ss << xvals[i] << "," << yvals[i] << '\n';
//             valPairs.push_back(ss.str());
//         }
//         for (auto pair : valPairs)
//             csvFile << pair.c_str();
//         csvFile.close();
//         return true;
//     }

//     // Uses the Matlab Engine API for C++ to invoke Matlab's plot command and open a figure window
//     void PlotInMatlab()
//     {
//         std::unique_ptr<matlab::engine::MATLABEngine> MLPtr = matlab::engine::startMATLAB();
//         matlab::data::ArrayFactory factory;
//         // Create arguments for the call to Matlab's plot function
//         std::vector<matlab::data::Array> args({factory.createArray({xvals.size(), 1}, xvals.begin(), xvals.end()),
//                                                factory.createArray({yvals.size(), 1}, yvals.begin(), yvals.end()),
//                                                factory.createCharArray(std::string("ro"))});
//         // Invoke the plot command
//         size_t numArguments(1);
//         std::vector<matlab::data::Array> figureHandle = MLPtr->feval(u"figure", numArguments, {});
//         MLPtr->feval(u"plot", args);
//         MLPtr->eval(u"saveas(gcf,'sine.png')");
//         // Pause to display the figure
//         // MLPtr->eval(u"pause(5)");

//         // // Set the Color property to red
//         // MLPtr->setProperty(figureHandle[0], u"Color", factory.createCharArray("red"));
//         // MLPtr->eval(u"pause(10)");
//     }
// };
// /* Returns multiples of Ts, the sampling period in seconds, beginning at tStart. */
// vector<double> SampledTime(int nSamples, double Ts, double tStart = 0)
// {
//     vector<double> tvals(nSamples);
//     std::iota(tvals.begin(), tvals.end(), tStart);
//     std::transform(tvals.begin(), tvals.end(), tvals.begin(), [Ts](double sampleVal) -> double { return sampleVal * Ts; });
//     return tvals;
// }
// int main()
// {
//     std::cout << "Visualize!!"
//               << "\n";
//     double fs = 300;                                         // Sampling frequency, in Hz
//     int nCycles = 2, nSamples = (int)((double)nCycles * fs); // Number of sine-wave cycles
//     vector<double> tvals = SampledTime(nSamples, 1 / fs);

//     const double pi = 3.141592653589793;
//     double w = 2 * pi * 1, poff = pi; // 1 Hz sinewave, 180 deg. out of phase
//     auto sw = Sinewave(w, poff, std::move(tvals));
//     sw.PlotInMatlab();
// }