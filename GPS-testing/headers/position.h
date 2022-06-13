#ifndef GPS_POSITION_H
#define GPS_POSITION_H

#include <string>

#include "types.h"

namespace GPS
{
  /* A Position object represents a location on, within, or above the Earth's surface.
   * The location is identifed by degrees latitude, degrees longitude, and elevation.
   * Elevation is relative to the Earth's mean radius, i.e. roughly, but not exactly, the Earth's surface.
   */
  class Position
  {
    public:
      /* Both constructors throw a std::invalid_argument exception for geometrically invalid latitude and longitude values,
       * and for negative elevation values that exceed the Earth's mean radius, i.e. that would be below the centre of the Earth.
       */

      /* Construct a Position from numeric degrees latitude, degrees longitude, and elevation in metres.
       */
      Position(degrees lat, degrees lon, metres ele);

      /* Construct a Position from strings containing a DDM (degrees and decimal minutes) representation of latitude and
       * longitude, with 'N'/'S' and 'E'/'W' characters to indicate bearing (positive or negative), and elevation in metres.
       */
      Position(std::string ddmLatStr, char latBearing,
               std::string ddmLonStr, char lonBearing,
               std::string eleStr);

      degrees latitude() const;
      degrees longitude() const;
      metres  elevation() const;

      /* Computes an approximation of the horizontal distance between two Positions on the Earth's surface.
       * Does NOT take into account elevation.
       */
      static metres horizontalDistanceBetween(Position, Position);

    private:
      degrees lat;
      degrees lon;
      metres  ele;
  };

  /* Convert a DDM (degrees and decimal minutes) string representation of an angle to a numeric DD (decimal degrees) value.
   */
  degrees ddmTodd(std::string);
}

#endif
