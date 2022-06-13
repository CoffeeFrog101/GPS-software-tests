#ifndef GPS_GPX_PARSER_H
#define GPS_GPX_PARSER_H

#include <vector>
#include <istream>

#include "waypoints.h"

namespace GPS::GPX
{
  //  Parse GPX data containing a route.
  std::vector<GPS::RoutePoint> parseRoute(std::istream&);

  // Parse GPX data containing a track.
  std::vector<GPS::TrackPoint> parseTrack(std::istream&);
}

#endif
