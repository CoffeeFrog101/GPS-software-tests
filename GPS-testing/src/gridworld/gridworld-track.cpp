#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>

#include "geometry.h"
#include "xml-generator.h"
#include "gridworld-model.h"
#include "gridworld-route.h"

#include "gridworld-track.h"

namespace GPS::GridWorld
{

Track::Track(std::string trackString, Model gridModel, system_clock::time_point startTime)
  : trackString{trackString},
    gridModel{gridModel},
    startTime{startTime}
{
    if (isValidTrackString(trackString))
    {
        constructTrackPoints();
    }
    else
    {
        throw std::invalid_argument("Invalid point sequence, cannot construct Track.");
    }
}

void Track::constructTrackPoints()
{
    std::istringstream tss(trackString);

    system_clock::time_point currentTime = startTime;
    Model::PointName pointName;

    while (tss >> pointName)
    {
        trackPoints.push_back( {gridModel[pointName], Model::pointToString(pointName), time_pointTotm(currentTime)} );

        int numberOfSeconds;
        tss >> numberOfSeconds;
        currentTime += std::chrono::seconds(numberOfSeconds);
        // If there are no trailing time digits then this final input operation fails,
        // but that's not a problem as then the loop terminates anyway.
    }
}

std::string Track::toGPX() const
{
    XML::Generator gpx;

    gpx.basicXMLDeclaration();
    gpx.openBasicGPXElement();

    gpx.openElement("trk",{});

    for (const TrackPoint& trackPoint : trackPoints)
    {
        XML::Attributes attribs =
          { {"lat", std::to_string(trackPoint.position.latitude())},
            {"lon", std::to_string(trackPoint.position.longitude())}
          };

        gpx.openElement("trkpt", attribs);
        gpx.element("ele",{},std::to_string(trackPoint.position.elevation()));
        gpx.element("time",{},dateTimeToString(trackPoint.dateTime));
        gpx.element("name",{},trackPoint.name);
        gpx.closeElement(); // "trkpt"
    }

    return gpx.closeAllElementsAndExtractString();
}

std::string Track::toString() const
{
    return trackString;
}

std::vector<TrackPoint> Track::toTrackPoints() const
{
    return trackPoints;
}

bool Track::isValidTrackString(std::string trackStr)
{
    bool validChars = std::all_of(trackStr.begin(), trackStr.end(),
                                  [] (char c) {return std::isupper(c) || std::isdigit(c) || c == '-';});
    bool noLeadingOrTrailingTime = trackStr.empty() || (isupper(trackStr.front()) && isupper(trackStr.back()));
    return validChars && noLeadingOrTrailingTime && Route::isValidRouteString(routeStringFromTrackString(trackStr));
}

std::string Track::routeStringFromTrackString(std::string trackStr)
{
    std::string routeStr = trackStr;
    routeStr.erase(std::remove_if(routeStr.begin(),routeStr.end(),[] (char c) {return std::isdigit(c) || c == '-';}),
                   routeStr.end());
    return routeStr;
}

std::tm Track::time_pointTotm(system_clock::time_point tp)
{
    std::time_t calendarTime = system_clock::to_time_t(tp);
    return *localtime(&calendarTime);
}

std::string Track::dateTimeToString(const tm& dateTime)
{
    // For documentation on the format specifiers, see: https://en.cppreference.com/w/cpp/io/manip/put_time
    const std::string dateTimeFormat = "%Y-%m-%dT%H:%M:%SZ";

    std::ostringstream dateTimeStream;
    dateTimeStream << std::put_time(&dateTime, dateTimeFormat.c_str());

    return dateTimeStream.str();
}

}
