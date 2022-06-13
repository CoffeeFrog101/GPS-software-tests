#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "geometry.h"

#include "analysis-route.h"

namespace GPS::Analysis
{

Route::Route(std::vector<RoutePoint> routePoints)
    : routePoints{routePoints}
{}

unsigned int Route::numPoints() const
{
    return routePoints.size();
}

metres Route::totalLength() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot compute the length of an empty route.");

    metres lengthSoFar = 0;

    for (unsigned int current = 0, next = 1; next < routePoints.size() ; ++current, ++next)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(routePoints[current].position, routePoints[next].position);
        metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();
        lengthSoFar += pythagoras(horizontalDifference,verticalDifference);
    }

    return lengthSoFar;
}

metres Route::netLength() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot compute the length of an empty route.");

    metres horizontalDifference = Position::horizontalDistanceBetween(routePoints.front().position, routePoints.back().position);
    metres verticalDifference = routePoints.back().position.elevation() - routePoints.front().position.elevation();
    return pythagoras(horizontalDifference,verticalDifference);
}

metres Route::totalHeightGain() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot compute the height gain of an empty route.");

    metres total = 0;

    for (unsigned int current = 0, next = 1; next < routePoints.size() ; ++current, ++next)
    {
        metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();
        if (verticalDifference > 0) total += verticalDifference; // ignore negative height differences
    }

    return total;
}

metres Route::netHeightGain() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot compute the height gain of an empty route.");

    metres verticalDifference = routePoints.back().position.elevation() - routePoints.front().position.elevation();
    return std::max(verticalDifference,0.0); // ignore negative height differences
}

degrees Route::maxGradient() const
{
    if (routePoints.size() < 2) throw std::domain_error("Cannot compute gradients on a route of fewer than two points.");

    degrees maxGrad = -halfRotation/2; // minimum possible gradient value

    for (unsigned int current = 0, next = 1; next < routePoints.size() ; ++current, ++next)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(routePoints[current].position, routePoints[next].position);
        metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();
        degrees grad = radToDeg(std::atan(verticalDifference/horizontalDifference));
        maxGrad = std::max(maxGrad,grad);
    }

    return maxGrad;
}

degrees Route::minGradient() const
{
    if (routePoints.size() < 2) throw std::domain_error("Cannot compute gradients on a route of fewer than two points.");

    degrees minGrad = halfRotation/2; // maximum possible gradient value

    for (unsigned int current = 0, next = 1; next < routePoints.size() ; ++current, ++next)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(routePoints[current].position, routePoints[next].position);
        metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();
        degrees grad = radToDeg(std::atan(verticalDifference/horizontalDifference));
        minGrad = std::min(minGrad,grad);
    }

    return minGrad;
}

degrees Route::steepestGradient() const
{
    if (routePoints.size() < 2) throw std::domain_error("Cannot compute gradients on a route of fewer than two points.");

    degrees steepestGrad = 0; // minimum possible gradient value

    for (unsigned int current = 0, next = 1; next < routePoints.size() ; ++current, ++next)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(routePoints[current].position, routePoints[next].position);
        metres verticalDifference = routePoints[next].position.elevation() - routePoints[current].position.elevation();
        degrees grad = radToDeg(std::atan(verticalDifference/horizontalDifference));
        if (std::abs(grad) > std::abs(steepestGrad))
        {
            steepestGrad = grad;
        }
    }
    return steepestGrad;
}

RoutePoint Route::highestPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint highestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.elevation() > highestPointSoFar.position.elevation())
        {
            highestPointSoFar = currentPoint;
        }
    }
    return highestPointSoFar;
}

RoutePoint Route::lowestPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint lowestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.elevation() < lowestPointSoFar.position.elevation())
        {
            lowestPointSoFar = currentPoint;
        }
    }
    return lowestPointSoFar;
}

RoutePoint Route::mostNorthelyPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint northmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.latitude() > northmostPointSoFar.position.latitude())
        {
            northmostPointSoFar = currentPoint;
        }
    }
    return northmostPointSoFar;
}

RoutePoint Route::mostSoutherlyPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint southmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.latitude() < southmostPointSoFar.position.latitude())
        {
            southmostPointSoFar = currentPoint;
        }
    }
    return southmostPointSoFar;
}

RoutePoint Route::mostEasterlyPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint eastmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.longitude() > eastmostPointSoFar.position.longitude())
        {
            eastmostPointSoFar = currentPoint;
        }
    }
    return eastmostPointSoFar;
}

RoutePoint Route::mostWesterlyPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint westmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.longitude() < westmostPointSoFar.position.longitude())
        {
            westmostPointSoFar = currentPoint;
        }
    }
    return westmostPointSoFar;
}

RoutePoint Route::mostEquatorialPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint nearestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (std::abs(currentPoint.position.latitude()) < std::abs(nearestPointSoFar.position.latitude()))
        {
            nearestPointSoFar = currentPoint;
        }
    }
    return nearestPointSoFar;
}

RoutePoint Route::leastEquatorialPoint() const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate a point in an empty route.");

    RoutePoint farthestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (std::abs(currentPoint.position.latitude()) > std::abs(farthestPointSoFar.position.latitude()))
        {
            farthestPointSoFar = currentPoint;
        }
    }
    return farthestPointSoFar;
}

RoutePoint Route::nearestPointTo(Position targetPosition) const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate nearest point in an empty route.");

    RoutePoint nearestPointSoFar = routePoints.front();
    metres shortestDistanceSoFar = Position::horizontalDistanceBetween(nearestPointSoFar.position, targetPosition);

    for (const RoutePoint& currentPoint : routePoints)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(currentPoint.position, targetPosition);
        metres verticalDifference = currentPoint.position.elevation() - targetPosition.elevation();
        metres currentDistance = pythagoras(horizontalDifference,verticalDifference);
        if (currentDistance < shortestDistanceSoFar)
        {
            nearestPointSoFar = currentPoint;
            shortestDistanceSoFar = currentDistance;
        }
    }

    return nearestPointSoFar;
}

RoutePoint Route::farthestPointFrom(Position avoidedPosition) const
{
    if (routePoints.empty()) throw std::domain_error("Cannot locate farthest point in an empty route.");

    RoutePoint farthestPointSoFar = routePoints.front();
    metres longestDistanceSoFar = Position::horizontalDistanceBetween(farthestPointSoFar.position, avoidedPosition);

    for (const RoutePoint& currentPoint : routePoints)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(currentPoint.position, avoidedPosition);
        metres verticalDifference = currentPoint.position.elevation() - avoidedPosition.elevation();
        metres currentDistance = pythagoras(horizontalDifference,verticalDifference);
        if (currentDistance > longestDistanceSoFar)
        {
            farthestPointSoFar = currentPoint;
            longestDistanceSoFar = currentDistance;
        }
    }

    return farthestPointSoFar;
}

unsigned int Route::numPointsNear(Position targetPosition, metres nearDistance) const
{
    unsigned int numNear = 0;

    for (const RoutePoint& routePoint : routePoints)
    {
        metres horizontalDifference = Position::horizontalDistanceBetween(routePoint.position, targetPosition);
        metres verticalDifference = routePoint.position.elevation() - targetPosition.elevation();
        metres currentDistance = pythagoras(horizontalDifference,verticalDifference);
        if (currentDistance < nearDistance) ++numNear;
    }

    return numNear;
}

unsigned int Route::timesVisited(std::string soughtName) const
{
    if (soughtName.empty()) throw std::invalid_argument("The name of the route point to be visited must be provided.");

    unsigned int numberOfVisits = 0;

    for (const RoutePoint& routePoint : routePoints)
    {
        if (routePoint.name == soughtName) ++numberOfVisits;
    }

    return numberOfVisits;
}

Position Route::findPosition(std::string soughtName) const
{
    if (soughtName.empty()) throw std::invalid_argument("Cannot find the position of an empty name.");

    for (const RoutePoint& routePoint : routePoints)
    {
        if (routePoint.name == soughtName) return routePoint.position;
    }

    throw std::domain_error("No position with that name found in the route.");
}

RoutePoint Route::operator[](unsigned int index) const
{
    if (index >= routePoints.size())
    {
        throw std::out_of_range("Position index out-of-range.");
    }

    return routePoints[index];
}


}
