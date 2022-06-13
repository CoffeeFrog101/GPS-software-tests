#ifndef GPS_ANALYSIS_ROUTE_H
#define GPS_ANALYSIS_ROUTE_H

#include <string>
#include <vector>

#include "types.h"
#include "position.h"
#include "waypoints.h"

namespace GPS::Analysis
{
  class Route
  {
    protected:
      const std::vector<RoutePoint> routePoints;

    public:
      Route(std::vector<RoutePoint>);


      // Returns the number of stored route points.
      unsigned int numPoints() const;


      /* The total length of the Route. This is the sum of the distances between
       * successive route points, including both vertical and horizontal distance.
       * Throws a std::domain_error if the route contains zero route points.
       */
      metres totalLength() const;


      /* The distance between the first and last points on the Route.
       * This includes both vertical and horizontal distance.
       * Throws a std::domain_error if the route contains zero route points.
       */
      metres netLength() const;


      /* The sum of all the positive (>0) height differences between successive route points.
       * That is, downhill changes are ignored.
       * Throws a std::domain_error if the route contains zero route points.
       */
      metres totalHeightGain() const;


      /* The increase in height from the start point to the finishing point.
       * Returns zero if the height difference is negative.
       * Throws a std::domain_error if the route contains zero route points.
       */
      metres netHeightGain() const;


      /* The steepest uphill gradient (in degrees) between successive points on the route.
       * If the entire route is downhill, then this is the least steep downhill gradient (i.e. negative).
       * Throws a std::domain_error if the route contains fewer than two points.
       */
      degrees maxGradient() const;


      /* The steepest downhill gradient (in degrees) between successive points on the Route.
       * If the entire route is uphill, then this is the least steep uphill gradient (i.e. positive).
       * Throws a std::domain_error if the route contains fewer than two points.
       */
      degrees minGradient() const;


      /* The steepest gradient between successive points on the Route, whether uphill or downhill
       * (i.e. either positive or negative).
       * Throws a std::domain_error if the route contains fewer than two points.
       */
      degrees steepestGradient() const;


      /* The point on the Route with the highest elevation.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint highestPoint() const;


      /* The point on the Route with the lowest elevation.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint lowestPoint() const;


      /* The point on the Route that is farthest north.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint mostNorthelyPoint() const;


      /* The point on the Route that is farthest south.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint mostSoutherlyPoint() const;


      /* The point on the Route that is farthest east.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint mostEasterlyPoint() const;


      /* The point on the Route that is farthest west.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint mostWesterlyPoint() const;


      /* The point on the Route that is nearest to the equator.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint mostEquatorialPoint() const;


      /* The point on the Route that is farthest from the equator.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint leastEquatorialPoint() const;


      /* Return the route point that is nearest to the specified Position.
       * This takes into account both vertical and horizontal distance.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint nearestPointTo(Position) const;


      /* Return the route point that is farthest from the specified Position.
       * This takes into account both vertical and horizontal distance.
       * Throws a std::domain_error if the route contains zero route points.
       */
      RoutePoint farthestPointFrom(Position) const;


      /* Compute how many route points are within the specified distance of the target Position.
       * This takes into account both vertical and horizontal distance.
       * Throws a std::invalid_argument exception if the distance is negative.
       */
      unsigned int numPointsNear(Position targetPosition, metres nearDistance) const;


      /* Compute how many times the named route point is visited during the route.
       * Throws a std::invalid_argument exception if the argument string is zero length.
       */
      unsigned int timesVisited(std::string soughtName) const;


      /* Find the Position bearing the specified name.
       * Throws a std::invalid_argument exception if the argument is an empty string.
       * Throws a std::domain_error exception if the name is not found.
       */
      Position findPosition(std::string soughtName) const;


      /* Return the route point at the specified index.
       * Throws a std::out_of_range exception if the index is out-of-range.
       */
      RoutePoint operator[](unsigned int) const;
  };
}

#endif
