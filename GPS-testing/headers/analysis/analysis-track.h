#ifndef GPS_ANALYSIS_TRACK_H
#define GPS_ANALYSIS_TRACK_H

#include <string>
#include <vector>
#include <ctime>
#include <chrono>

#include "types.h"
#include "position.h"
#include "waypoints.h"
#include "analysis-route.h"

namespace GPS::Analysis
{
  class Track : public Route
  {
    protected:
      /* For each track point, we store a corresponding time stamp.
       */
      using TimeStamp = std::chrono::system_clock::time_point;

      const std::vector<TimeStamp> timeStamps;
      /* Class Invariant:
       *   The length of the 'timeStamps' list is the same as that of the 'routePoints' list.
       */


      /* During analysis of a Track, if two adjacent track points are less than the 'restingRange'
       * distance apart, then the time spent between them is considered to be time spent 'resting'.
       * One or more adjacent 'rests' together form a 'resting period'.  Time not spent resting is
       * considered to be 'travelling', and one or more adjacent non-rests together form a
       * 'travelling period'.
       */
      metres restingRange;


    public:
      // By default, the resting range is 0m. That is, nothing is considered to be resting.
      Track(std::vector<TrackPoint>, metres restingRange = 0);


      /* Update the distance (permitted between adjacent points) that defines a rest.
       */
      void setRestingRange(metres);


      /* Total elapsed time between start and finish of track.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time elapsed between the start and finish of the track
       * is negative (but negative time periods between individual points will not trigger an
       * exception, provided that the overall time is not negative).
       */
      std::chrono::seconds totalTime() const;


      /* Total elapsed time between start and finish of the track that is spent resting.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * resting period is negative (but negative time while travelling will not trigger an exception).
       */
      std::chrono::seconds restingTime() const;


      /* Total elapsed time between start and finish of the track that is spent travelling (not resting).
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * travelling period is negative (but negative time while resting will not trigger an exception).
       */
      std::chrono::seconds travellingTime() const;


      /* The duration of the longest resting period spent during the track.
       * (A resting period is the sum of all adjacent rests.)
       * Returns zero if the track contains no resting periods.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * resting period is negative (but negative time while travelling will not trigger an exception).
       */
      std::chrono::seconds longestRestingPeriod() const;


      /* The duration of the longest travelling period during the track.
       * (A travelling period is the sum of all adjacent non-rests.)
       * Returns zero if the track contains no travelling periods.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * travelling period is negative (but negative time while resting will not trigger an exception).
       */
      std::chrono::seconds longestTravellingPeriod() const;


      /* The mean duration of the resting periods within the track.
       * (A resting period is the sum of all adjacent rests.)
       * Throws a std::domain_error if the track contains no resting periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * resting period is negative (but negative time while travelling will not trigger an exception).
       */
      std::chrono::seconds averageRestingPeriod() const;


      /* The mean duration of the travelling periods within the track.
       * (A travelling period is the sum of all adjacent non-rests.)
       * Throws a std::domain_error if the track contains no travelling periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * travelling period is negative (but negative time while resting will not trigger an exception).
       */
      std::chrono::seconds averageTravellingPeriod() const;


      /* The fastest speed between successive track points during travelling periods, taking into
       * account both vertical and horizontal distance travelled.
       * Returns zero if the track only contains resting periods.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * travelling period is zero or negative (but negative time while resting will not trigger an
       * exception).
       */
      speed maxSpeed() const;


      /* The mean speed over the course of the whole track,
       * taking into account both vertical and horizontal distance travelled.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the total elapsed time is zero or negative (but negative
       * time periods between individual points will not trigger an exception, provided that the
       * overall time is not negative).
       */
      speed averageSpeed() const;


      /* The mean speed of all the time spent travelling over the course of the track,
       * taking into account both vertical and horizontal distance travelled.
       * Throws a std::domain_error if the track does not contain any travelling periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points during a
       * travelling period is zero or negative (but negative time while resting will not trigger an
       * exception).
       */
      speed averageTravellingSpeed() const;


      /* The fastest rate of positive elevation change between successive track points while
       * travelling.
       * Returns zero if there are no positive elevation changes while travelling.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time between two adjacent points in a travelling
       * period is zero or negative (but negative time while resting will not trigger an
       * exception).
      */
      speed maxRateOfAscent() const;


      /* The fastest rate of negative elevation change between successive track points (returned
       * as a positive number).
       * Returns zero if there are no negative elevation changes while travelling.
       * Throws a std::domain_error if the track contains zero track points.
       * Throws a std::domain_error if the time between two adjacent points in a travelling
       * period is zero or negative (but negative time while resting will not trigger an
       * exception).
       */
      speed maxRateOfDescent() const;


    private:

      static std::vector<RoutePoint> trackPointsToRoutePoints(std::vector<TrackPoint>);

      static std::vector<TimeStamp> trackPointsToTimeStamps(std::vector<TrackPoint>);

      bool withinRestingRange(RoutePoint,RoutePoint) const;

      static TimeStamp tmToTimeStamp(std::tm);
  };
}

#endif
