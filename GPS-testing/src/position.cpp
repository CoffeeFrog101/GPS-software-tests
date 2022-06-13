#include <cassert>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "geometry.h"
#include "earth.h"
#include "position.h"

namespace GPS
{
  Position::Position(degrees lat, degrees lon, metres ele)
  {
      if (! isValidLatitude(lat))
          throw std::invalid_argument("Latitude values must not exceed " + std::to_string(poleLatitude) + " degrees.");

      if (! isValidLongitude(lon))
          throw std::invalid_argument("Longitude values must not exceed " + std::to_string(antiMeridianLongitude) + " degrees.");

      if (! Earth::isValidElevation(ele))
          throw std::invalid_argument("Negative elevation values must not exceed the Earth's mean radius (i.e. must not be below the centre of the Earth).");

      this->lat = lat;
      this->lon = lon;
      this->ele = ele;
  }

  Position::Position(std::string ddmLatStr, char latBearing,
                     std::string ddmLonStr, char lonBearing,
                     std::string eleStr)
      : Position(ddmTodd(ddmLatStr), ddmTodd(ddmLonStr), std::stod(eleStr))
  {
      if (lat < 0)
          throw std::invalid_argument("Latitude values must be positive when accompanied by a N/S bearing.");

      if (lon < 0)
          throw std::invalid_argument("Longitude values must be positive when accompanied by an E/W bearing.");

      switch (latBearing)
      {
          case 'N': break;             // 'N' means positive angle, so no change
          case 'S': lat = -lat; break; // 'S' means negative angle
          default: throw std::invalid_argument(latBearing + std::string(" is an invalid North/South bearing character in DDM format.  Only 'N' or 'S' accepted."));
      }

      switch (lonBearing)
      {
          case 'E': break;             // 'E' means positive angle, so no change
          case 'W': lon = -lon; break; // 'W' means negative angle
          default: throw std::invalid_argument(lonBearing + std::string(" is an invalid East/West bearing character in DDM format.  Only 'E' or 'W' accepted."));
      }

  }

  degrees Position::latitude() const
  {
      return lat;
  }

  degrees Position::longitude() const
  {
      return lon;
  }

  metres Position::elevation() const
  {
      return ele;
  }

  metres Position::horizontalDistanceBetween(Position p1, Position p2)
  /*
   * See: https://en.wikipedia.org/wiki/Haversine_formula
   */
  {
      const radians lat1 = degToRad(p1.latitude());
      const radians lat2 = degToRad(p2.latitude());
      const radians lon1 = degToRad(p1.longitude());
      const radians lon2 = degToRad(p2.longitude());

      double h = sinSqr((lat2-lat1)/2) + std::cos(lat1)*std::cos(lat2)*sinSqr((lon2-lon1)/2);
      return 2 * Earth::meanRadius * std::asin(std::sqrt(h));
  }

  degrees ddmTodd(std::string ddmStr)
  {
      double ddm  = std::stod(ddmStr);
      double degs = std::floor(ddm / 100);
      double mins = ddm - 100 * degs;
      return degs + mins / minutesPerDegree; // converts minutes to decimal fractions of a degree
  }
}
