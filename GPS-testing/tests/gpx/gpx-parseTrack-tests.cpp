#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "dataFiles.h"
#include "gpx-parser.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE( GPX_parseTrack )

const metres percentageTolerance = 0.000001;
const std::string trackPointNotNamed = "";

/////////////////////////////////////////////////////////////////////////////////////////

// Utilities

// Useful in the tests because the 'tm_year' data member counts years from 1900 CE rather than 0 CE.
int ce_year_to_tm_year(int year)
{
  return year - 1900;
}

// Useful in the tests because the 'tm_mon' data member counts months from 0 (January).
int month_number_to_tm_mon(int month)
{
  return month - 1;
}

// To produce a better error message if the file used in the test isn't found.
void requireFileExists(std::string filepath)
{
    BOOST_REQUIRE_MESSAGE(
      std::filesystem::exists(filepath),
      ("Could not open log file: " + filepath + "\n(If you're running at the command-line, you need to 'cd' into the 'bin/' directory first.)")
    );
}

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( PointPositions )

// Check that a simple one-point track is correctly parsed.
BOOST_AUTO_TEST_CASE( onePoint )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"20\" lon=\"70\"><ele>250</ele><time>2020-03-23T13:00:01Z</time></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);

    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 20 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 70 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 250 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_year , ce_year_to_tm_year(2020));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mon , month_number_to_tm_mon(03));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mday , 23);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_hour , 13);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_min , 00);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_sec , 01);
}

// Check that a simple two-point track is correctly parsed.
BOOST_AUTO_TEST_CASE( twoPoints )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"20\" lon=\"70\"><ele>250</ele><time>2020-03-23T13:00:01Z</time></trkpt><trkpt lat=\"60\" lon=\"120\"><ele>5</ele><time>2020-11-05T00:01:59Z</time></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 2);

    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 20 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 70 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 250 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_year , ce_year_to_tm_year(2020));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mon , month_number_to_tm_mon(03));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mday , 23);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_hour , 13);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_min , 00);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_sec , 01);

    BOOST_CHECK_CLOSE(trackPoints[1].position.latitude() , 60 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.longitude() , 120 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.elevation() , 5 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_year , ce_year_to_tm_year(2020));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mon , month_number_to_tm_mon(11));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mday , 05);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_hour , 00);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_min , 01);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_sec , 59);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( PointNames )

// Check that an absent name element is parsed correctly.
BOOST_AUTO_TEST_CASE( NoNameElement )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , trackPointNotNamed);
}

// Check that an empty name element is parsed correctly.
BOOST_AUTO_TEST_CASE( EmptyNameElement )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name></name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , trackPointNotNamed);
}

// Check that the name of a point is parsed correctly.
BOOST_AUTO_TEST_CASE( oneNamedPoint )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name>MyPoint</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , "MyPoint");
}

// Check that names are parsed correctly when there are two named points.
BOOST_AUTO_TEST_CASE( twoNamedPoints )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name>Start</name></trkpt><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:02Z</time><name>Finish</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 2);
    BOOST_CHECK_EQUAL(trackPoints[0].name , "Start");
    BOOST_CHECK_EQUAL(trackPoints[1].name , "Finish");
}

// Check that names are parsed correctly when one of two points is named.
BOOST_AUTO_TEST_CASE( oneOfTwoNamedPoints )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time></trkpt><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-02T00:00:01Z</time><name>Destination</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 2);
    BOOST_CHECK_EQUAL(trackPoints[0].name , trackPointNotNamed);
    BOOST_CHECK_EQUAL(trackPoints[1].name , "Destination");
}

// Check that leading whitespace is correctly trimmed from point names.
BOOST_AUTO_TEST_CASE( TrimLeadingWhitespace )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name>   MyPoint</name><name>MyPoint</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , "MyPoint");
}

// Check that trailing whitespace is correctly trimmed from point names.
BOOST_AUTO_TEST_CASE( TrimTrailingWhitespace )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name>MyPoint       </name><name>MyPoint</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , "MyPoint");
}

// Check that internal whitespace is preserved within point names.
BOOST_AUTO_TEST_CASE( AllowInternalWhitespace )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name>My Point</name><name>MyPoint</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , "My Point");
}

// Check that all whitespace is trimmed in a point name element consisting entirely of whitespace.
BOOST_AUTO_TEST_CASE( TrimEntirelyWhitespace )
{
    std::stringstream gpxData
      {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>0</ele><time>1970-01-01T00:00:01Z</time><name>  </name><name>MyPoint</name></trkpt></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_EQUAL(trackPoints[0].name , trackPointNotNamed);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( FileSources )

// Check when the input is a file rather than a GPX string.
BOOST_AUTO_TEST_CASE( threeUnnamedPoints )
{
    const std::string filepath = DataFiles::GPXTracksDir + "ThreePointTrack.gpx";
    requireFileExists(filepath);
    std::fstream gpxData {filepath};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 3);

    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 0.4 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 23.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 10 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[0].name , trackPointNotNamed);

    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_year , ce_year_to_tm_year(2000));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mon , month_number_to_tm_mon(01));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mday , 11);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_hour , 01);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_min , 10);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_sec , 05);

    BOOST_CHECK_CLOSE(trackPoints[1].position.latitude() , 0.5 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.longitude() , 33.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.elevation() , 20 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[1].name , trackPointNotNamed);

    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_year , ce_year_to_tm_year(2001));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mon , month_number_to_tm_mon(02));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mday , 12);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_hour , 02);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_min , 20);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_sec , 15);

    BOOST_CHECK_CLOSE(trackPoints[2].position.latitude() , 0.6 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[2].position.longitude() , 43.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[2].position.elevation() , 30 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[2].name , trackPointNotNamed);

    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_year , ce_year_to_tm_year(2002));
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_mon , month_number_to_tm_mon(03));
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_mday , 13);
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_hour , 03);
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_min , 30);
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_sec , 25);
}

// Check when the parser can handle GPX files containing additional data beyond the minimum.
BOOST_AUTO_TEST_CASE( fileContainingExtraData )
{
    const std::string filepath = DataFiles::GPXTracksDir + "ThreePointTrack-ExtraData.gpx";
    requireFileExists(filepath);
    std::fstream gpxData {filepath};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 3);

    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 0.4 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 23.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 10 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[0].name , "A");

    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_year , ce_year_to_tm_year(2000));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mon , month_number_to_tm_mon(01));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mday , 11);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_hour , 01);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_min , 10);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_sec , 05);

    BOOST_CHECK_CLOSE(trackPoints[1].position.latitude() , 0.5 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.longitude() , 33.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.elevation() , 20 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[1].name , "B");

    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_year , ce_year_to_tm_year(2001));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mon , month_number_to_tm_mon(02));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mday , 12);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_hour , 02);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_min , 20);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_sec , 15);

    BOOST_CHECK_CLOSE(trackPoints[2].position.latitude() , 0.6 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[2].position.longitude() , 43.1 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[2].position.elevation() , 30 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[2].name , "C");

    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_year , ce_year_to_tm_year(2002));
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_mon , month_number_to_tm_mon(03));
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_mday , 13);
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_hour , 03);
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_min , 30);
    BOOST_CHECK_EQUAL(trackPoints[2].dateTime.tm_sec , 25);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Exceptions )

// An exception is thrown if the 'gpx' element is missing.
BOOST_AUTO_TEST_CASE( missing_gpx_element )
{
    std::stringstream gpxData
        {"<trk><trkpt lat=\"0\" lon=\"0\"><ele>1</ele><time>1970-01-01T00:00:01Z</time></trkpt></trk>"};

    BOOST_CHECK_THROW( GPX::parseTrack(gpxData) , std::domain_error);
}

// An exception is thrown if the 'trk' element is missing.
BOOST_AUTO_TEST_CASE( missing_trk_element )
{
    std::stringstream gpxData
        {"<gpx><trkpt lat=\"0\" lon=\"0\"><ele>1</ele><time>1970-01-01T00:00:01Z</time></trkpt></gpx>"};

    BOOST_CHECK_THROW( GPX::parseTrack(gpxData) , std::domain_error);
}

// An exception is thrown if the 'trkpt' element is missing.
BOOST_AUTO_TEST_CASE( missing_trkpt_element )
{
    std::stringstream gpxData {"<gpx><trk></trk></gpx>"};

    BOOST_CHECK_THROW( GPX::parseTrack(gpxData) , std::domain_error);
}

// An exception is thrown if a 'lat' attribute is missing.
BOOST_AUTO_TEST_CASE( missing_lat_attribute )
{
    std::stringstream gpxData
        {"<gpx><trk><trkpt lon=\"0\"><ele>1</ele><time>1970-01-01T00:00:01Z</time></trkpt></trk></gpx>"};

    BOOST_CHECK_THROW( GPX::parseTrack(gpxData) , std::domain_error);
}

// An exception is thrown if a 'lon' attribute is missing.
BOOST_AUTO_TEST_CASE( missing_lon_attribute )
{
    std::stringstream gpxData
        {"<gpx><trk><trkpt lat=\"0\"><ele>1</ele><time>1970-01-01T00:00:01Z</time></trkpt></trk></gpx>"};

    BOOST_CHECK_THROW( GPX::parseTrack(gpxData) , std::domain_error);
}

// An exception is thrown if a 'time' element is missing.
BOOST_AUTO_TEST_CASE( missing_time_element )
{
    std::stringstream gpxData
        {"<gpx><trk><trkpt lat=\"0\" lon=\"0\"><ele>1</ele></trkpt></trk></gpx>"};

    BOOST_CHECK_THROW( GPX::parseTrack(gpxData) , std::domain_error);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Segments )

// Check that the parser can handle a segment element.
BOOST_AUTO_TEST_CASE( onePointSegment )
{
    std::stringstream gpxData
        {"<gpx><trk><trkseg><trkpt lat=\"20\" lon=\"70\"><ele>250</ele><time>2020-03-23T13:00:01Z</time><name>Here</name></trkpt></trkseg></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 1);
    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 20 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 70 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 250 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[0].name , "Here");

    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_year , ce_year_to_tm_year(2020));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mon , month_number_to_tm_mon(03));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mday , 23);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_hour , 13);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_min , 00);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_sec , 01);
}

// Check that the parser can handle a segment element containing multiple points.
BOOST_AUTO_TEST_CASE( twoPointSegment )
{
    std::stringstream gpxData
        {"<gpx><trk><trkseg><trkpt lat=\"20\" lon=\"70\"><ele>250</ele><time>2020-03-23T13:00:01Z</time><name>Here</name></trkpt><trkpt lat=\"60\" lon=\"120\"><ele>5</ele><time>2020-11-05T00:01:59Z</time><name>There</name></trkpt></trkseg></trk></gpx>"};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 2);

    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 20 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 70 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 250 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[0].name , "Here");

    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_year , ce_year_to_tm_year(2020));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mon , month_number_to_tm_mon(03));
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_mday , 23);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_hour , 13);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_min , 00);
    BOOST_CHECK_EQUAL(trackPoints[0].dateTime.tm_sec , 01);

    BOOST_CHECK_CLOSE(trackPoints[1].position.latitude() , 60 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.longitude() , 120 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.elevation() , 5 , percentageTolerance);

    BOOST_CHECK_EQUAL(trackPoints[1].name , "There");

    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_year , ce_year_to_tm_year(2020));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mon , month_number_to_tm_mon(11));
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_mday , 05);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_hour , 00);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_min , 01);
    BOOST_CHECK_EQUAL(trackPoints[1].dateTime.tm_sec , 59);
}

// Check that the parser can handle multiple segment elements.
BOOST_AUTO_TEST_CASE( multipleSegments )
{
    const std::string filepath = DataFiles::GPXTracksDir + "MultipleSegments.gpx";
    requireFileExists(filepath);
    std::fstream gpxData {filepath};

    std::vector<TrackPoint> trackPoints = GPX::parseTrack(gpxData);

    BOOST_REQUIRE_EQUAL(trackPoints.size() , 9);

    BOOST_CHECK_CLOSE(trackPoints[0].position.latitude() , 7 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.longitude() , 5 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[0].position.elevation() , 200 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[1].position.latitude() , 8 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.longitude() , 15 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[1].position.elevation() , 300 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[2].position.latitude() , 9 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[2].position.longitude() , 25 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[2].position.elevation() , 400 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[3].position.latitude() , 10 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[3].position.longitude() , 35 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[3].position.elevation() , 500 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[4].position.latitude() , 11 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[4].position.longitude() , 45 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[4].position.elevation() , 600 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[5].position.latitude() , 12 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[5].position.longitude() , 55 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[5].position.elevation() , 700 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[6].position.latitude() , 13 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[6].position.longitude() , 65 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[6].position.elevation() , 800 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[7].position.latitude() , 14 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[7].position.longitude() , 75 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[7].position.elevation() , 900 , percentageTolerance);

    BOOST_CHECK_CLOSE(trackPoints[8].position.latitude() , 15 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[8].position.longitude() , 85 , percentageTolerance);
    BOOST_CHECK_CLOSE(trackPoints[8].position.elevation() , 1000 , percentageTolerance);
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
