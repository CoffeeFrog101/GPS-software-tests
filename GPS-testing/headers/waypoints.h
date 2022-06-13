#ifndef GPS_WAYPOINTS_H
#define GPS_WAYPOINTS_H

#include <string>
#include <ctime>

#include "position.h"

namespace GPS
{
  struct RoutePoint
  {
     GPS::Position position;
     std::string name;
  };

  struct TrackPoint
  {
     GPS::Position position;
     std::string name;
     std::tm dateTime;
  };
}

#endif
