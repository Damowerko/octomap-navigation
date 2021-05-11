#include <iostream>
#include <vector>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "octonav/visualize.hpp"

using namespace std;
using namespace octomap;

void visualize(OcTree &octree, const std::vector<point3d> &path)
{
    std::unique_ptr<matlab::engine::MATLABEngine> mlp = matlab::engine::startMATLAB();

    octree.writeBinary("./out/tmp.bt");
    mlp->eval(u"map = importOccupancyMap3D('./out/tmp.bt');");

    vector<double> x;
    vector<double> y;
    vector<double> z;
    for (auto p : path)
    {
        x.push_back(p.x());
        y.push_back(p.y());
        z.push_back(p.z());
    }

    matlab::data::ArrayFactory f;
    matlab::data::Array xp_arr = f.createArray({x.size()}, x.begin(), x.end());
    matlab::data::Array yp_arr = f.createArray({y.size()}, y.begin(), y.end());
    matlab::data::Array zp_arr = f.createArray({z.size()}, z.begin(), z.end());

    mlp->setVariable(u"x", std::move(xp_arr));
    mlp->setVariable(u"y", std::move(yp_arr));
    mlp->setVariable(u"z", std::move(zp_arr));   

    mlp->eval(u"figure;");
    mlp->eval(u"show(map);");
    mlp->eval(u"hold on;");

    mlp->eval(u"q = plot3(x,y,z, '-', 'Color', 'b');");

    mlp->eval(u"axis tight");
    mlp->eval(u"camlight headlight");
    mlp->eval(u"saveas(gcf,'./out/viz.png')");

    cout << "Press any key to continue..." << endl;
    cin.get();
}