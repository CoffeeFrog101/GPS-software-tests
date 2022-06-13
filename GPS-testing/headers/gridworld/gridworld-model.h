#ifndef GPS_GRIDWORLD_MODEL_H
#define GPS_GRIDWORLD_MODEL_H

#include <map>

#include "types.h"
#include "position.h"
#include "earth.h"

namespace GPS::GridWorld
{
  /*  Grid World consists of a 5x5 grid of points, where each point is named by a
   *  single character A-Y.
   *
   *  A B C D E
   *  F G H I J
   *  K L M N O
   *  P Q R S T
   *  U V W X Y
   *
   *  The central point M is used as the initial reference point.
   *  Up/down in the grid changes latitude relative to M.
   *  Left/right in the grid changes longitude relative to M.
   *  Note: the closer you get to a pole, the more distorted from a grid this becomes.
   */

  class Model
  {

    public:
      using PointName = char;

      // Defaults to Pontianak, the only equatorial city.
      Model(
        Position posM = Earth::Pontianak, // Position of the central point 'M'.
        metres northSouthGridUnit = 10000, // Horizontal distance between grid points in a north/south direction.
        metres eastWestGridUnit = 10000, // Horizontal distance between grid points in an east/west direction.
        metres verticalGridUnit = 1000); // Vertical distance between grid levels.

      const Position& operator[](PointName) const;

      static std::string pointToString(PointName);

    private:
      static const int gridRadius;
      static const int gridSize;

      const Position posM;
      const degrees deltaLatitude;
      const degrees deltaLongitude;
      const degrees deltaElevation;
      const std::map<PointName,Position> grid;

      degrees calculateLatitudeDelta(metres northSouthGridUnit);
      degrees calculateLongitudeDelta(metres eastWestGridUnit);
      std::map<PointName,Position> generateGrid();
      metres calcElevationFor(PointName pointName);
  };
}

#endif
