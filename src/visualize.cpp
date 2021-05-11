#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>
#include "octonav/visualize.hpp"
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

void visualize(const OcTree &octree, const std::vector<point3d> &path)
{
    vector<double> x;
    vector<double> y;
    vector<double> z;

    vector<double> xp;
    vector<double> yp;
    vector<double> zp;
    for (auto p : path)
    {
        xp.push_back(p.x());
        yp.push_back(p.y());
        zp.push_back(p.z());
    }

    //
    for (auto it = octree.begin_leafs(); it != octree.end_leafs(); ++it)
    {
        if (octree.isNodeOccupied(octree.search(it.getIndexKey())))
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

    matlab::data::Array xp_arr = f.createArray({xp.size()}, xp.begin(), xp.end());
    matlab::data::Array yp_arr = f.createArray({yp.size()}, yp.begin(), yp.end());
    matlab::data::Array zp_arr = f.createArray({zp.size()}, zp.begin(), zp.end());

    mlp->setVariable(u"x", std::move(x_arr));
    mlp->setVariable(u"y", std::move(y_arr));
    mlp->setVariable(u"z", std::move(z_arr));

    mlp->setVariable(u"xp", std::move(xp_arr));
    mlp->setVariable(u"yp", std::move(yp_arr));
    mlp->setVariable(u"zp", std::move(zp_arr));

    mlp->eval(u"figure;");
    mlp->eval(u"p = plot3(x,y,z, '.','Color', 'r');");
    mlp->eval(u"hold on;");

    mlp->eval(u"q = plot3(xp,yp,zp, '-', 'Color', 'b');");

    mlp->eval(u"view(30,-15);");
    mlp->eval(u"axis tight");
    mlp->eval(u"camlight");
    mlp->eval(u"lighting gouraud");
    mlp->eval(u"saveas(gcf,'viz.png')");
}