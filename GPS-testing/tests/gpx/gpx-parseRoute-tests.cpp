#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "dataFiles.h"
#include "gpx-parser.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE( GPX_parseRoute )

const metres percentageTolerance = 0.000001;
const std::string routePointNotNamed = "";

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( PointPositions )

// Check that a simple one-point route is correctly parsed.
BOOST_AUTO_TEST_CASE( onePoint )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"20\" lon=\"70\"><ele>250</ele></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);

    BOOST_CHECK_CLOSE(routePoints[0].position.latitude() , 20 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.longitude() , 70 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.elevation() , 250 , percentageTolerance);
}

// Check that a simple two-point route is correctly parsed.
BOOST_AUTO_TEST_CASE( twoPoints )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"20\" lon=\"70\"><ele>250</ele></rtept><rtept lat=\"60\" lon=\"120\"><ele>5</ele></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 2);

    BOOST_CHECK_CLOSE(routePoints[0].position.latitude() , 20 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.longitude() , 70 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.elevation() , 250 , percentageTolerance);

    BOOST_CHECK_CLOSE(routePoints[1].position.latitude() , 60 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[1].position.longitude() , 120 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[1].position.elevation() , 5 , percentageTolerance);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( PointNames )

// Check that an absent name element is parsed correctly.
BOOST_AUTO_TEST_CASE( NoNameElement )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , routePointNotNamed);
}

// Check that an empty name element is parsed correctly.
BOOST_AUTO_TEST_CASE( EmptyNameElement )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name></name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , routePointNotNamed);
}

// Check that the name of a point is parsed correctly.
BOOST_AUTO_TEST_CASE( oneNamedPoint )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name>MyPoint</name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , "MyPoint");
}

// Check that names are parsed correctly when there are two named points.
BOOST_AUTO_TEST_CASE( twoNamedPoints )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name>Start</name></rtept><rtept lat=\"1\" lon=\"1\"><ele>1</ele><name>Finish</name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 2);
    BOOST_CHECK_EQUAL(routePoints[0].name , "Start");
    BOOST_CHECK_EQUAL(routePoints[1].name , "Finish");
}

// Check that names are parsed correctly when one of two points is named.
BOOST_AUTO_TEST_CASE( oneOfTwoNamedPoints )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele></rtept><rtept lat=\"1\" lon=\"1\"><ele>1</ele><name>Destination</name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 2);
    BOOST_CHECK_EQUAL(routePoints[0].name , routePointNotNamed);
    BOOST_CHECK_EQUAL(routePoints[1].name , "Destination");
}

// Check that leading whitespace is correctly trimmed from point names.
BOOST_AUTO_TEST_CASE( TrimLeadingWhitespace )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name>   MyPoint</name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , "MyPoint");
}

// Check that trailing whitespace is correctly trimmed from point names.
BOOST_AUTO_TEST_CASE( TrimTrailingWhitespace )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name>MyPoint       </name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , "MyPoint");
}

// Check that internal whitespace is preserved within point names.
BOOST_AUTO_TEST_CASE( AllowInternalWhitespace )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name>My Point</name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , "My Point");
}

// Check that all whitespace is trimmed in a point name element consisting entirely of whitespace.
BOOST_AUTO_TEST_CASE( TrimEntirelyWhitespace )
{
    std::stringstream gpxData
      {"<gpx><rte><rtept lat=\"0\" lon=\"0\"><ele>0</ele><name>  </name></rtept></rte></gpx>"};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1);
    BOOST_CHECK_EQUAL(routePoints[0].name , routePointNotNamed);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( FileSources )

// To produce a better error message if the file used in the test isn't found.
void requireFileExists(std::string filepath)
{
    BOOST_REQUIRE_MESSAGE(
      std::filesystem::exists(filepath),
      ("Could not open log file: " + filepath + "\n(If you're running at the command-line, you need to 'cd' into the 'bin/' directory first.)")
    );
}

// Check when the input contains various whitespace layout.
BOOST_AUTO_TEST_CASE( whitespaceLayout )
{
    const std::string filepath = DataFiles::GPXRoutesDir + "ThreePointRoute.gpx";
    requireFileExists(filepath);
    std::fstream gpxData {filepath};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 3);

    BOOST_CHECK_CLOSE(routePoints[0].position.latitude() , 0.4 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.longitude() , 23.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.elevation() , 10 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints[0].name , routePointNotNamed);

    BOOST_CHECK_CLOSE(routePoints[1].position.latitude() , 0.5 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[1].position.longitude() , 33.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[1].position.elevation() , 20 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints[1].name , routePointNotNamed);

    BOOST_CHECK_CLOSE(routePoints[2].position.latitude() , 0.6 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[2].position.longitude() , 43.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[2].position.elevation() , 30 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints[2].name , routePointNotNamed);
}

// Check that the parser can handle additional data beyond the minimum.
BOOST_AUTO_TEST_CASE( extraData )
{
    const std::string filepath = DataFiles::GPXRoutesDir + "ThreePointRoute-ExtraData.gpx";
    requireFileExists(filepath);
    std::fstream gpxData {filepath};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 3);

    BOOST_CHECK_CLOSE(routePoints[0].position.latitude() , 0.4 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.longitude() , 23.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[0].position.elevation() , 10 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints[0].name , "A");

    BOOST_CHECK_CLOSE(routePoints[1].position.latitude() , 0.5 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[1].position.longitude() , 33.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[1].position.elevation() , 20 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints[1].name , "B");

    BOOST_CHECK_CLOSE(routePoints[2].position.latitude() , 0.6 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[2].position.longitude() , 43.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints[2].position.elevation() , 30 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints[2].name , "C");
}

// Check there's no errors for a large file of real GPX route data.
BOOST_AUTO_TEST_CASE( realGPXData )
{
    const std::string filepath = DataFiles::GPXRoutesDir + "NorthYorkMoors.gpx";
    requireFileExists(filepath);
    std::fstream gpxData {filepath};

    std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData);

    BOOST_REQUIRE_EQUAL(routePoints.size() , 1091);

    BOOST_CHECK_CLOSE(routePoints.front().position.latitude() , 54.42204773426058 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints.front().position.longitude() , -1.118760108947754 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints.front().position.elevation() , 309 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints.front().name , routePointNotNamed);

    BOOST_CHECK_CLOSE(routePoints.back().position.latitude() , 54.50918734073639 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints.back().position.longitude() , -1.120192408561707 , percentageTolerance);
    BOOST_CHECK_CLOSE(routePoints.back().position.elevation() , 148 , percentageTolerance);
    BOOST_CHECK_EQUAL(routePoints.back().name , routePointNotNamed);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Exceptions )

// An exception is thrown if the 'gpx' element is missing.
BOOST_AUTO_TEST_CASE( missing_gpx_element )
{
    std::stringstream gpxData
        {"<rte><rtept lat=\"0\" lon=\"0\"><ele>1</ele></rtept></rte>"};

    BOOST_CHECK_THROW( GPX::parseRoute(gpxData), std::domain_error);
}

// An exception is thrown if the 'rte' element is missing.
BOOST_AUTO_TEST_CASE( missing_rte_element )
{
    std::stringstream gpxData
        {"<gpx><rtept lat=\"0\" lon=\"0\"><ele>1</ele></rtept></gpx>"};

    BOOST_CHECK_THROW( GPX::parseRoute(gpxData) , std::domain_error);
}

// An exception is thrown if the 'rtept' element is missing.
BOOST_AUTO_TEST_CASE( missing_rtept_element )
{
    std::stringstream gpxData
        {"<gpx><rte></rte></gpx>"};

    BOOST_CHECK_THROW( GPX::parseRoute(gpxData) , std::domain_error);
}

// An exception is thrown if a 'lat' attribute is missing.
BOOST_AUTO_TEST_CASE( missing_lat_attribute )
{
    std::stringstream gpxData
        {"<gpx><rte><rtept lon=\"0\"><ele>1</ele></rtept></rte></gpx>"};

    BOOST_CHECK_THROW( GPX::parseRoute(gpxData) , std::domain_error);
}

// An exception is thrown if a 'lon' attribute is missing.
BOOST_AUTO_TEST_CASE( missing_lon_attribute )
{
    std::stringstream gpxData
        {"<gpx><rte><rtept lat=\"0\"><ele>1</ele></rtept></rte></gpx>"};

    BOOST_CHECK_THROW( GPX::parseRoute(gpxData) , std::domain_error);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
