#include "octonav/visualize.hpp"
#include <vector>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

using std::vector;
using namespace octomap;
/**
     * @brief Visualize the point cloud from an octograph and optionally a path. PNG saved in results/
     * 
     * @param graph Octograph to visualize
     * @param path Path to visualize
     */
void visualize(const OctoGraph &graph, const vector<OctoNode> &path)
{

    vector<double> x;
    vector<double> y;
    vector<double> z;

    vector<double> xp;
    vector<double> yp;
    vector<double> zp;
    for (auto p : path)
    {
        xp.push_back(graph.octree.keyToCoord(p.key).x());
        yp.push_back(graph.octree.keyToCoord(p.key).y());
        zp.push_back(graph.octree.keyToCoord(p.key).z());
    }

    //
    for (auto it = graph.octree.begin_leafs(); it != graph.octree.end_leafs(); ++it)
    {
        if (graph.octree.isNodeOccupied(graph.octree.search(it.getIndexKey())))
        {
            double sz = it.getSize();
            x.push_back(it.getX());
            y.push_back(it.getY());
            z.push_back(it.getZ());

            x.push_back(it.getX() + sz / 2);
            y.push_back(it.getY() + sz / 2);
            z.push_back(it.getZ() + sz / 2);

            x.push_back(it.getX() - sz / 2);
            y.push_back(it.getY() - sz / 2);
            z.push_back(it.getZ() - sz / 2);
        }
    }

    std::unique_ptr<matlab::engine::MATLABEngine> mlp = matlab::engine::startMATLAB();
    matlab::data::ArrayFactory f;

    matlab::data::Array x_arr = f.createArray({x.size()}, x.begin(), x.end());
    matlab::data::Array y_arr = f.createArray({y.size()}, y.begin(), y.end());
    matlab::data::Array z_arr = f.createArray({z.size()}, z.begin(), z.end());

    mlp->setVariable(u"x", std::move(x_arr));
    mlp->setVariable(u"y", std::move(y_arr));
    mlp->setVariable(u"z", std::move(z_arr));

    mlp->eval(u"figure;");
    mlp->eval(u"p = plot3(x,y,z, '.','Color', 'r');");

    if (xp.size() > 0)
    {
        mlp->eval(u"hold on;");
        matlab::data::Array xp_arr = f.createArray({xp.size()}, xp.begin(), xp.end());
        matlab::data::Array yp_arr = f.createArray({yp.size()}, yp.begin(), yp.end());
        matlab::data::Array zp_arr = f.createArray({zp.size()}, zp.begin(), zp.end());

        mlp->setVariable(u"xp", std::move(xp_arr));
        mlp->setVariable(u"yp", std::move(yp_arr));
        mlp->setVariable(u"zp", std::move(zp_arr));
        mlp->eval(u"q = plot3(xp,yp,zp, '-', 'Color', 'b');");
        mlp->eval(u"legend('Point Cloud','Optimal Path');");
    }
    else
    {
        mlp->eval(u"legend('Point Cloud');");
    }

    mlp->eval(u"view(30,-15);");
    mlp->eval(u"axis tight");
    mlp->eval(u"camlight");
    mlp->eval(u"lighting gouraud");
    mlp->eval(u"saveas(gcf,'../results/viz.png')");
    //Pause to display the figure
    mlp->eval(u"pause(5)");
}
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
//         std::unique_ptr<matlab::engine::MATLABEngine> mlp = matlab::engine::startMATLAB();
//         matlab::data::ArrayFactory factory;
//         // Create arguments for the call to Matlab's plot function
//         std::vector<matlab::data::Array> args({factory.createArray({xvals.size(), 1}, xvals.begin(), xvals.end()),
//                                                factory.createArray({yvals.size(), 1}, yvals.begin(), yvals.end()),
//                                                factory.createCharArray(std::string("ro"))});
//         // Invoke the plot command
//         size_t numArguments(1);
//         std::vector<matlab::data::Array> figureHandle = mlp->feval(u"figure", numArguments, {});
//         mlp->feval(u"plot", args);
//         mlp->eval(u"saveas(gcf,'sine.png')");
//         // Pause to display the figure
//         // mlp->eval(u"pause(5)");

//         // // Set the Color property to red
//         // mlp->setProperty(figureHandle[0], u"Color", factory.createCharArray("red"));
//         // mlp->eval(u"pause(10)");
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