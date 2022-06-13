#include <algorithm>
#include <stdexcept>

#include "xml-element.h"
#include "xml-generator.h"
#include "gridworld-model.h"

#include "gridworld-route.h"

namespace GPS::GridWorld
{

Route::Route(std::string routeString, Model gridModel)
  : routeString{routeString},
    gridModel{gridModel}
{
    if (isValidRouteString(routeString))
    {
        constructRoutePoints();
    }
    else
    {
        throw std::invalid_argument("Invalid point sequence, cannot construct Route.");
    }
}

void Route::constructRoutePoints()
{
    for (Model::PointName pointName : routeString)
    {
        routePoints.push_back( {gridModel[pointName], Model::pointToString(pointName)} );
    }
}

std::string Route::toGPX() const
{
    XML::Generator gpx;

    gpx.basicXMLDeclaration();
    gpx.openBasicGPXElement();

    gpx.openElement("rte",{});

    for (const RoutePoint& routePoint : routePoints)
    {
        XML::Attributes attribs =
          { {"lat", std::to_string(routePoint.position.latitude())},
            {"lon", std::to_string(routePoint.position.longitude())}
          };

        gpx.openElement("rtept",attribs);
        gpx.element("name",{},routePoint.name);
        gpx.element("ele",{},std::to_string(routePoint.position.elevation()));
        gpx.closeElement(); // "rtept"
    }

    return gpx.closeAllElementsAndExtractString();
}

std::string Route::toNMEA() const
{
    // Stub definition
    return "";
}

std::string Route::toString() const
{
    return routeString;
}

std::vector<RoutePoint> Route::toRoutePoints() const
{
    return routePoints;
}

bool Route::isValidRouteString(std::string routeStr)
{
    // To be valid, all chars must be in the range 'A'..'Y'.
    return std::all_of(routeStr.begin(), routeStr.end(),
                       [] (char point)->bool {return point >= 'A' && point <= 'Y';});
}

}
