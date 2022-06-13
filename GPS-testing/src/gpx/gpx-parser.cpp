#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/algorithm/string.hpp>

#include "xml-parser.h"

#include "gpx-parser.h"

namespace GPS::GPX
{

  void requireElementIs(const XML::Element& element, std::string elementName)
  {
      if (element.getName() != elementName)
      {
          throw std::domain_error("Missing '" + elementName + "' element.");
      }
  }

  void requireSubElementExists(const XML::Element& element, std::string subElementName)
  {
      if (! element.containsSubElement(subElementName) )
      {
          throw std::domain_error("Missing '" + subElementName + "' element.");
      }
  }

  void requireAttributeExists(const XML::Element& element, std::string attributeName)
  {
      if (! element.containsAttribute(attributeName))
      {
          throw std::domain_error("Missing '" + attributeName + "' attribute.");
      }
  }

  std::string extractNameFromOptionalSubElementOf(const XML::Element& ptElement)
  {
      if (ptElement.containsSubElement("name"))
      {
          return boost::algorithm::trim_copy(ptElement.getSubElement("name").getLeafContent());
      }
      else
      {
          return "";
      }
  }

  GPS::Position extractPositionFromPt(const XML::Element& ptElement)
  {
      requireAttributeExists(ptElement,"lat");
      requireAttributeExists(ptElement,"lon");
      degrees lat = std::stod(ptElement.getAttribute("lat"));
      degrees lon = std::stod(ptElement.getAttribute("lon"));

      metres ele = ptElement.containsSubElement("ele") ? std::stod(ptElement.getSubElement("ele").getLeafContent()) : 0;

      return GPS::Position(lat,lon,ele);
  }

  GPS::RoutePoint extractRoutePointFromRtept(const XML::Element& rtept)
  {
      return GPS::RoutePoint{extractPositionFromPt(rtept), extractNameFromOptionalSubElementOf(rtept)};
  }

  std::vector<GPS::RoutePoint> extractRoutePointsFromRte(const XML::Element& rte)
  {
      std::vector<GPS::RoutePoint> routePoints;
      requireSubElementExists(rte,"rtept");
      for (unsigned int i = 0; i < rte.countSubElements("rtept"); ++i)
      {
          XML::Element rtept = rte.getSubElement("rtept",i);
          routePoints.push_back(extractRoutePointFromRtept(rtept));
      }
      return routePoints;
  }

  std::vector<GPS::RoutePoint> parseRoute(std::istream& gpxData)
  {
      XML::Parser parser {gpxData};

      XML::Element gpx = parser.parseRootElement();
      requireElementIs(gpx,"gpx");

      requireSubElementExists(gpx,"rte");
      XML::Element rte = gpx.getSubElement("rte");

      return extractRoutePointsFromRte(rte);
  }

  std::tm parseDateTime(std::string rawDateTime)
  {
      // For documentation on the format specifiers, see: https://en.cppreference.com/w/cpp/io/manip/get_time
      const std::string dateTimeFormat = "%Y-%m-%dT%H:%M:%SZ";

      tm dateTime;

      std::istringstream dateTimeStream{rawDateTime};
      dateTimeStream >> std::get_time(&dateTime, dateTimeFormat.c_str());

      if (dateTimeStream.fail())
      {
          throw std::domain_error("Malformed date/time content: " + rawDateTime);
      }

      return dateTime;
  }

  std::tm extractTimeFromPt(const XML::Element& ptElement)
  {
      requireSubElementExists(ptElement,"time");

      std::string time = ptElement.getSubElement("time").getLeafContent();

      return parseDateTime(time);
  }

  GPS::TrackPoint extractTrackPointFromTrkpt(const XML::Element& trkpt)
  {
      return { extractPositionFromPt(trkpt),
               extractNameFromOptionalSubElementOf(trkpt),
               extractTimeFromPt(trkpt)
             };
  }


  std::vector<GPS::TrackPoint> extractTrackPointsFrom(const XML::Element& element)
  {
      std::vector<GPS::TrackPoint> trackPoints;

      requireSubElementExists(element,"trkpt");
      for (unsigned int i = 0; i < element.countSubElements("trkpt"); ++i)
      {
          XML::Element trkpt = element.getSubElement("trkpt",i);
          trackPoints.push_back(extractTrackPointFromTrkpt(trkpt));
      }

      return trackPoints;
  }

  std::vector<GPS::TrackPoint> extractTrackPointsFromTrk(const XML::Element& trk)
  {
      if (trk.containsSubElement("trkseg"))
      {
          std::vector<GPS::TrackPoint> trackPoints;

          for (unsigned int i = 0; i < trk.countSubElements("trkseg"); ++i)
          {
              XML::Element trkseg = trk.getSubElement("trkseg",i);
              std::vector<GPS::TrackPoint> trackPointsInThisSegment = extractTrackPointsFrom(trkseg);
              trackPoints.insert(trackPoints.end(), trackPointsInThisSegment.begin(), trackPointsInThisSegment.end());
          }

          return trackPoints;
      }
      else
      {
          return extractTrackPointsFrom(trk);
      }
  }

  std::vector<GPS::TrackPoint> parseTrack(std::istream& gpxData)
  {
      XML::Parser parser {gpxData};

      XML::Element gpx = parser.parseRootElement();
      requireElementIs(gpx,"gpx");

      requireSubElementExists(gpx,"trk");
      XML::Element trk = gpx.getSubElement("trk");

      return extractTrackPointsFromTrk(trk);
  }
}
