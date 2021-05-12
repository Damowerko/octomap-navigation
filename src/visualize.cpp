#include <iostream>
#include <vector>
#include <tuple>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "octonav/visualize.hpp"

using namespace std;
using namespace octomap;
using matlab::data::Array;

std::unique_ptr<matlab::engine::MATLABEngine> mlp = matlab::engine::startMATLAB();

tuple<Array, Array, Array> vectorToXYZ(const std::vector<point3d> &path){
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
    return {xp_arr, yp_arr, zp_arr};
}

void visualize(OcTree &octree, const std::vector<point3d> &path1, const std::vector<point3d> &path2)
{
    octree.writeBinary("/home/damow/octomap-navigation/out/tmp.bt");
    mlp->eval(u"map = importOccupancyMap3D('/home/damow/octomap-navigation/out/tmp.bt');");


    auto [x1, y1, z1] = vectorToXYZ(path1);
    auto [x2, y2, z2] = vectorToXYZ(path2);
    mlp->setVariable(u"x1", std::move(x1));
    mlp->setVariable(u"y1", std::move(y1));
    mlp->setVariable(u"z1", std::move(z1));
    mlp->setVariable(u"x2", std::move(x2));
    mlp->setVariable(u"y2", std::move(y2));
    mlp->setVariable(u"z2", std::move(z2));

    mlp->eval(u"figure;");
    mlp->eval(u"show(map);");
    mlp->eval(u"hold on;");

    mlp->eval(u"q = plot3(x1,y1,z1, 'r-', 'LineWidth', 5);");
    mlp->eval(u"q = plot3(x2,y2,z2, 'm-', 'LineWidth', 5);");

    mlp->eval(u"axis tight");
    mlp->eval(u"camlight headlight");
    mlp->eval(u"saveas(gcf,'/home/damow/octomap-navigation/out/viz.png')");
}