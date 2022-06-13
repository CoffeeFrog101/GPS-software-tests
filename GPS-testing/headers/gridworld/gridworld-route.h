#ifndef GPS_GRIDWORLD_ROUTE_H
#define GPS_GRIDWORLD_ROUTE_H

#include <vector>
#include <string>

#include "waypoints.h"
#include "gridworld-model.h"

namespace GPS::GridWorld
{
  /* This class generates routes in either:
   *  - GPX format
   *  - NMEA format (currently unimplemented)
   *  - C++ data structure format (std::vector<RoutePoint>)
   *
   * To use this class, the user must provide a string of GridWorld::Points
   * specifying the route path, e.g. "ABCD".
   * The locations of these points are interpreted in a GridWorld::Model (see gridworld/model.h).
   * The user may provide a GridWorld::Model object; if not the default GridWorld::Model is used.
   */
  class Route
  {
    public:
      Route(std::string routeString, // A sequence of GridWorld::PointNames.
            GridWorld::Model gridModel = GridWorld::Model()); // The default Model can be overwritten if desired.

      // Produce a GPX representation of the route.
      std::string toGPX() const;

      // Produce a NMEA representation of the route.
      std::string toNMEA() const; // unimplemented

      // Produce a string representation of the route.
      std::string toString() const;

      // Produce a vector of RoutePoints representation of the route.
      std::vector<GPS::RoutePoint> toRoutePoints() const;

      // Check whether the argument would be a valid string for creating a GridWorld::Route.
      static bool isValidRouteString(std::string);

    private:
      const std::string routeString; // The PointNames used to construct the Route.
      const Model gridModel;

      std::vector<GPS::RoutePoint> routePoints;

      void constructRoutePoints();
  };
}
#endif
