#include <iostream>
#include <fstream>
#include <filesystem>

#include "dataFiles.h"
#include "gpx-parser.h"
#include "analysis-route.h"

using namespace GPS;

using std::cout;
using std::endl;

int main()
{
    std::string filepath = DataFiles::GPXRoutesDir + "/NorthYorkMoors.gpx";

    if (! std::filesystem::exists(filepath))
    {
        cout << "Could not open log file: " + filepath << endl;
        cout << "(If you're running at the command-line, you need to 'cd' into the 'bin/' directory first.)";
    }

    std::fstream gpxData {filepath};

    Analysis::Route exampleRoute = GPX::parseRoute(gpxData);

    cout << "Number of points: "  << exampleRoute.numPoints()       << endl;
    cout << "Total length: "      << exampleRoute.totalLength()     << endl;
    cout << "Net length: "        << exampleRoute.netLength()       << endl;
    cout << "Total height gain: " << exampleRoute.totalHeightGain() << endl;
    // etc.
}
