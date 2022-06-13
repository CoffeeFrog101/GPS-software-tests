#ifndef GPS_GRIDWORLD_TRACK_H
#define GPS_GRIDWORLD_TRACK_H

#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "waypoints.h"

#include "gridworld-model.h"

using std::chrono::system_clock;

namespace GPS::GridWorld
{
  /* This class generates tracks in either:
   *  - GPX format
   *  - NMEA format (currently unimplemented)
   *  - C++ data structure format (std::vector<TrackPoint>)
   *
   * To use this class, the user must provide a string of GridWorld::Points,
   * interspersed with time durations (in seconds), specifying the track path and times.
   * E.g. "A1B3C" means that it takes 1 second to travel from Point A to Point B,
   * then 3 seconds to travel from Point B to Point C.
   *
   * The locations of these points are interpreted in a GridWorldModel (see GridWorld.h).
   * The user may provide a GridWorldModel object; if not the default GridWorldModel is used.
   */
  class Track
  {      
    public:
      Track(std::string trackString, // A string containing GridWorld::PointNames and time units.
            GridWorld::Model gridModel = GridWorld::Model(), // The default Model can be overwritten if desired.
            system_clock::time_point startTime = {}); // The time point of the first track point.

      // Produce a GPX representation of the track.
      std::string toGPX() const;

      // Produce a NMEA representation of the track.
      std::string toNMEA() const; // unimplemented

      // Produce a string representation of the track.
      std::string toString() const;

      // Produce a vector of TrackPoints representation of the route.
      std::vector<GPS::TrackPoint> toTrackPoints() const;

      // Check whether the argument would be a valid string for creating a GridWorld::Track.
      static bool isValidTrackString(std::string);

      // Convert a track string into a route string (by discarding the timing information).
      static std::string routeStringFromTrackString(std::string);

    private:
      const std::string trackString; // The PointNames and times used to construct the Track.
      const GridWorld::Model gridModel;
      const system_clock::time_point startTime;

      std::vector<GPS::TrackPoint> trackPoints;

      void constructTrackPoints();

      static std::tm time_pointTotm(system_clock::time_point);
      static std::string dateTimeToString(const std::tm&);
  };
}

#endif
