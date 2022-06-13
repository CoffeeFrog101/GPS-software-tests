#include <string>
#include <sstream>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "geometry.h"

#include "gridworld-model.h"

namespace GPS::GridWorld
{

const int Model::gridRadius = 2;
const int Model::gridSize = gridRadius*2 + 1;

Model::Model(Position posM, metres northSouthGridUnit, metres eastWestGridUnit, metres verticalGridUnit)
    : posM{posM},
      deltaLatitude{calculateLatitudeDelta(northSouthGridUnit)},
      deltaLongitude{calculateLongitudeDelta(eastWestGridUnit)},
      deltaElevation{verticalGridUnit},
      grid{generateGrid()}
{}

const Position& Model::operator[](PointName pointName) const
{
    return grid.at(pointName);
}

std::string Model::pointToString(PointName pointName)
{
    return std::string(1,pointName);
}

degrees Model::calculateLatitudeDelta(metres northSouthGridUnit)
{
    if ((northSouthGridUnit * gridRadius * 2) > (Earth::polarCircumference/2))
    {
        throw std::domain_error("North/South grid units too large - Grid World must fit between the North and South poles on the Earth's surface.");
    }

    degrees deltaLat = Earth::latitudeSubtendedBy(northSouthGridUnit);

    if (! isValidLatitude(std::abs(posM.latitude()) + deltaLat*gridRadius))
    {
        throw std::domain_error("Latitude of Position M too close to a pole for North/South grid units of this magnitude - Grid World cannot extend beyond the poles.");
    }

    return deltaLat;
}
degrees Model::calculateLongitudeDelta(metres eastWestGridUnit)
{
    if (eastWestGridUnit * gridRadius * 2 > Earth::circumferenceAtLatitude(posM.latitude()))
    {
        throw std::domain_error("East/West grid units too large for the latitude of Position M - Grid World must fit on the Earth's surface.");
    }

    return Earth::longitudeSubtendedBy(eastWestGridUnit,posM.latitude());
}

std::map<Model::PointName,Position> Model::generateGrid()
{
    /* A B C D E
     * F G H I J
     * K L M N O
     * P Q R S T
     * U V W X Y
     */

    std::map<Model::PointName,Position> grid;

    Model::PointName currentPoint = 'A';
    degrees lat = posM.latitude() + gridRadius*deltaLatitude;
    for (int i = 0; i < gridSize; ++i)
    {
        degrees lon = posM.longitude() - gridRadius*deltaLongitude;
        for (int j = 0; j < gridSize; ++j)
        {
            metres ele = calcElevationFor(currentPoint);
            grid.insert({currentPoint,Position(lat,normaliseDegrees(lon),ele)}); // We normalise the longitude in case it has crossed the antimeridean.
            lon += deltaLongitude;
            ++currentPoint;
        }
        lat -= deltaLatitude;
    }

    return grid;
}

metres Model::calcElevationFor(PointName pointName)
{
    /* Neighbours of M are 1 level lower than M.
     * All other Points are 2 levels lower than M.
     */
    switch (pointName)
    {
        case 'M':
            return posM.elevation();
        case 'G': case 'H': case 'I': case 'L': case 'N': case 'Q': case 'R': case 'S':
            return posM.elevation() - deltaElevation;
        default:
            return posM.elevation() - (2 * deltaElevation);
    }
}

}
