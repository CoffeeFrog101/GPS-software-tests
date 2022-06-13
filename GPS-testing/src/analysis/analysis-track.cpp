#include <cassert>
#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "geometry.h"

#include "analysis-track.h"

using std::chrono::seconds;
using std::chrono::duration_cast;

namespace GPS::Analysis
{

Track::Track(std::vector<TrackPoint> trackPoints, metres restingRange)
    : Route{trackPointsToRoutePoints(trackPoints)},
      timeStamps{trackPointsToTimeStamps(trackPoints)},
      restingRange{restingRange}
{
    assert (routePoints.size() == timeStamps.size());
}

void Track::setRestingRange(metres newRestingRange)
{
    restingRange = newRestingRange;
}

seconds Track::totalTime() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot compute the duration time of an empty track.");

    if (timeStamps.back() < timeStamps.front()) throw std::domain_error("Track takes negative time duration overall.");

    return duration_cast<seconds>(timeStamps.back() - timeStamps.front());
}

seconds Track::restingTime() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot compute the resting time of an empty track.");

    seconds total = seconds::zero();

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains rests of negative time duration.");

            total += duration_cast<seconds>(timeStamps[next] - timeStamps[current]);
        }
    }
    return total;
}

seconds Track::travellingTime() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot compute the travelling time of an empty track.");

    seconds total = seconds::zero();

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains travelling with negative time duration.");

            total += duration_cast<seconds>(timeStamps[next] - timeStamps[current]);
        }
    }
    return total;
}

seconds Track::longestRestingPeriod() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot find resting periods in an empty track.");

    seconds maxRest = seconds::zero();

    seconds currentRest = seconds::zero();

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains rests of negative time duration.");

            currentRest += duration_cast<seconds>(timeStamps[next] - timeStamps[current]);
        }
        else
        {
            // We may have just finished a resting period, so check and reset.
            maxRest = std::max(maxRest,currentRest);
            currentRest = seconds::zero();
        }
    }

    // If there is a resting period at the end of the Track, then that period will not have been checked in the loop, so we check it here.
    maxRest = std::max(maxRest,currentRest);

    return maxRest;
}

std::chrono::seconds Track::longestTravellingPeriod() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot find travelling periods in an empty track.");

    seconds maxTravelling = seconds::zero();

    seconds currentTravelling = seconds::zero();

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains travelling of negative time duration.");

            currentTravelling += duration_cast<seconds>(timeStamps[next] - timeStamps[current]);
        }
        else
        {
            // We may have just finished a travelling period, so check and reset.
            maxTravelling = std::max(maxTravelling,currentTravelling);
            currentTravelling = seconds::zero();
        }
    }

    // If there is a travelling period at the end of the Track, then that period will not have been checked in the loop, so we check it here.
    maxTravelling = std::max(maxTravelling,currentTravelling);

    return maxTravelling;
}

seconds Track::averageRestingPeriod() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot find resting periods in an empty track.");

    int numRestingPeriods = 0;
    bool currentlyResting = false;

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (withinRestingRange(routePoints[current], routePoints[next]))
        {
            currentlyResting = true;
        }
        else if (currentlyResting)
        {
            ++numRestingPeriods;
            currentlyResting = false;
        }
    }

    if (currentlyResting) ++numRestingPeriods;

    return restingTime() / numRestingPeriods;
}

seconds Track::averageTravellingPeriod() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot find travelling periods in an empty track.");

    int numTravellingPeriods = 0;
    bool currentlyTravelling = false;

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            currentlyTravelling = true;
        }
        else if (currentlyTravelling)
        {
            ++numTravellingPeriods;
            currentlyTravelling = false;
        }
    }

    if (currentlyTravelling) ++numTravellingPeriods;

    return travellingTime() / numTravellingPeriods;
}

speed Track::maxSpeed() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot measure speed in an empty track.");

    assert( routePoints.size() == timeStamps.size() );

    speed maximumSpeed = 0;

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains travelling of negative time duration.");

            seconds timeDifference = duration_cast<seconds>(timeStamps[next] - timeStamps[current]);

            if (timeDifference == seconds::zero()) throw std::domain_error("Cannot compute maximum speed in m/s when there is travelling of zero seconds duration.");

            Position currentPos = routePoints[current].position;
            Position nextPos = routePoints[next].position;

            metres horizontalDifference = Position::horizontalDistanceBetween(currentPos,nextPos);
            metres verticalDifference = nextPos.elevation() - currentPos.elevation();

            metres distance = pythagoras(horizontalDifference,verticalDifference);

            speed currentSpeed = distance / timeDifference.count();

            maximumSpeed =  std::max(currentSpeed,maximumSpeed);
        }
    }

    return maximumSpeed;
}

speed Track::averageSpeed() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot measure speed in an empty track.");

    seconds timeTaken = totalTime();

    if (timeTaken == seconds::zero()) throw std::domain_error("Cannot speed in m/s when the travel-time has zero seconds duration.");

    return totalLength() / timeTaken.count();
}

speed Track::averageTravellingSpeed() const
{
    assert( routePoints.size() == timeStamps.size() );

    metres distanceTravelled = 0;
    seconds timeTravelling = seconds::zero();

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains travelling of negative time duration.");

            seconds timeDifference = duration_cast<seconds>(timeStamps[next] - timeStamps[current]);

            if (timeDifference == seconds::zero()) throw std::domain_error("Cannot compute average speed in m/s when there is travelling of zero seconds duration.");

            Position currentPos = routePoints[current].position;
            Position nextPos = routePoints[next].position;

            metres horizontalDifference = Position::horizontalDistanceBetween(currentPos,nextPos);
            metres verticalDifference = nextPos.elevation() - currentPos.elevation();

            distanceTravelled += pythagoras(horizontalDifference,verticalDifference);

            timeTravelling += timeDifference;
        }
    }

    if (timeTravelling == seconds::zero()) throw std::domain_error("Cannot calculate average travelling speed in a track with no travelling periods.");

    return distanceTravelled / timeTravelling.count();
}

speed Track::maxRateOfAscent() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot measure rate of ascent in an empty track.");

    assert( routePoints.size() == timeStamps.size() );

    speed maxAscentRate = 0;

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains travelling of negative time duration.");

            seconds timeDifference = duration_cast<seconds>(timeStamps[next] - timeStamps[current]);

            if (timeDifference == seconds::zero()) throw std::domain_error("Cannot compute maximum rate of ascent in m/s when there is travelling of zero seconds duration.");

            metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();

            if (verticalDifference > 0)
            {
                speed currentAscentRate = verticalDifference / timeDifference.count();
                maxAscentRate = std::max(currentAscentRate,maxAscentRate);
            }
        }
    }

    return maxAscentRate;
}

speed Track::maxRateOfDescent() const
{
    if (timeStamps.empty()) throw std::domain_error("Cannot measure rate of ascent in an empty track.");

    assert( routePoints.size() == timeStamps.size() );

    speed maxDescentRate = 0;

    for (unsigned int current = 0, next = 1; next < timeStamps.size() ; ++current, ++next)
    {
        if (! withinRestingRange(routePoints[current], routePoints[next]))
        {
            if (timeStamps[next] < timeStamps[current]) throw std::domain_error("Track contains travelling of negative time duration.");

            seconds timeDifference = duration_cast<seconds>(timeStamps[next] - timeStamps[current]);

            if (timeDifference == seconds::zero()) throw std::domain_error("Cannot compute maximum rate of descent in m/s when there is travelling of zero seconds duration.");

            metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();

            if (verticalDifference < 0)
            {
                speed currentDescentRate = (-verticalDifference) / timeDifference.count();
                maxDescentRate = std::max(currentDescentRate,maxDescentRate);
            }
        }
    }

    return maxDescentRate;
}

std::vector<RoutePoint> Track::trackPointsToRoutePoints(std::vector<TrackPoint> trackPoints)
{
    std::vector<RoutePoint> routePoints;
    routePoints.reserve(trackPoints.size());

    for (const TrackPoint& trackPoint : trackPoints)
    {
        routePoints.push_back({trackPoint.position,trackPoint.name});
    }

    return routePoints;
}

std::vector<Track::TimeStamp> Track::trackPointsToTimeStamps(std::vector<TrackPoint> trackPoints)
{
    std::vector<TimeStamp> timeStamps;
    timeStamps.reserve(trackPoints.size());

    for (const TrackPoint& trackPoint : trackPoints)
    {
        timeStamps.push_back(tmToTimeStamp(trackPoint.dateTime));
    }

    return timeStamps;
}

bool Track::withinRestingRange(RoutePoint pt1, RoutePoint pt2) const
{
    metres horizontalDifference = Position::horizontalDistanceBetween(pt1.position,pt2.position);
    metres verticalDifference = pt2.position.elevation() - pt1.position.elevation();
    metres distance = pythagoras(horizontalDifference,verticalDifference);
    return (distance < restingRange);
}

Track::TimeStamp Track::tmToTimeStamp(std::tm dateTime)
{
    return std::chrono::system_clock::from_time_t(std::mktime(&dateTime));
}

}
