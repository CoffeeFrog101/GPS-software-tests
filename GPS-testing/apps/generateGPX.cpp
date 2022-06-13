#include <iostream>
#include <fstream>

#include "dataFiles.h"

#include "gridworld-model.h"
#include "gridworld-route.h"
#include "gridworld-track.h"

using namespace GPS;

using std::cout;
using std::endl;

int main()
{
   /////////////////////////////////////////////////////////////

   /* Basic Usage Examples - Using default Grid World settings */

   /* A simple route using the default Grid World model. */
   cout << GridWorld::Route("P").toGPX() << endl;

   /* // A simple track using the default GridWorld model. */
   // cout << GridWorld::Track("A1G3M").toGPX() << endl;

   /////////////////////////////////////////////////////////////

   /* Advanced Usage Examples - Configuring GridWorld settings  */

    Position posM = Position(50,30,2000); // Position of the central point 'M'.
    metres northSouthGridUnit = 100000; // North/south distance between grid points.
    metres eastWestGridUnit = 100000; // East/west distance between grid points.
    metres verticalGridUnit = 1000; // Vertical distance between grid levels.

//    GridWorld::Model gwModel {posM, northSouthGridUnit, eastWestGridUnit, verticalGridUnit};

    /* A simple route using the specified GridWorld model. */
    // cout << GridWorld::Route("AGM",gwModel).toGPX() << endl;

    /* A simple track using the specified GridWorld model. */
    // cout << GridWorld::Track("A1G3M",gwModel).toGPX() << endl;

    /////////////////////////////////////////////////////////////

    /* Writing output to File Example */

    std::string filepath = DataFiles::GPXRoutesDir + "ExampleGPXRoute.gpx";
    // std::ofstream file {filepath};
    // file << GridWorld::Route("AGM").toGPX() << endl;

    /////////////////////////////////////////////////////////////
}
