#include <boost/test/unit_test.hpp>

#include "geometry.h"
#include "position.h"
#include "earth.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE( PositionTests )

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( DDMtoDD )

const double percentageAccuracy = 0.0001;

BOOST_AUTO_TEST_CASE( wholeDegrees )
{
    const std::string ddm = "3700";
    const degrees expectedDD = 37;

    metres actualDD = ddmTodd(ddm);

    BOOST_CHECK_CLOSE( actualDD, expectedDD, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( degreesAndWholeMinutes )
{
    const std::string ddm = "6730";
    const degrees expectedDD = 67.5;

    metres actualDD = ddmTodd(ddm);

    BOOST_CHECK_CLOSE( actualDD, expectedDD, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( degreesAndFractionalMinutes )
{
    const std::string ddm = "7730.90";
    const degrees expectedDD = 77.515;

    metres actualDD = ddmTodd(ddm);

    BOOST_CHECK_CLOSE( actualDD, expectedDD, percentageAccuracy );
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( HorizontalDistanceBetween )

const double percentageAccuracy = 1;
const metres absoluteAccuracy = 1;

BOOST_AUTO_TEST_CASE( oneDegree )
{
    const Position posA = Position(0,0,0);
    const Position posB = Position(0,1,0);
    const metres expectedDistance = Earth::equatorialCircumference / fullRotation;

    metres actualDistance = Position::horizontalDistanceBetween(posA, posB);

    BOOST_CHECK_CLOSE( actualDistance, expectedDistance, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( equatorAndPole )
{
    const Position posA = Earth::EquatorialMeridian;
    const Position posB = Earth::NorthPole;
    const metres expectedDistance = Earth::polarCircumference / 4;

    metres actualDistance = Position::horizontalDistanceBetween(posA, posB);

    BOOST_CHECK_CLOSE( actualDistance, expectedDistance, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( equatorialMeridians )
{
    const Position posA = Earth::EquatorialMeridian;
    const Position posB = Earth::EquatorialAntiMeridian;
    const metres expectedDistance = Earth::equatorialCircumference / 2;

    metres actualDistance = Position::horizontalDistanceBetween(posA, posB);

    BOOST_CHECK_CLOSE( actualDistance, expectedDistance, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NTU_campuses )
{
    const Position posA = Earth::CliftonCampus;
    const Position posB = Earth::CityCampus;
    const metres expectedDistance = 5450;

    metres actualDistance = Position::horizontalDistanceBetween(posA, posB);

    BOOST_CHECK_CLOSE( actualDistance, expectedDistance, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( SamePosition )
{
    const Position pos = Earth::CliftonCampus;

    metres actualDistance = Position::horizontalDistanceBetween(pos, pos);

    BOOST_CHECK_SMALL( actualDistance, absoluteAccuracy );
}

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Constructor )

const double percentageAccuracy = 0.0001;
const double absoluteAccuracy = 0.0001;

const degrees lat = 25.5;
const std::string latDDM = "2530.00";

const degrees lon = 37.25;
const std::string lonDDM = "3715.00";

const metres ele = 4786.2;
const std::string eleStr = std::to_string(ele);
const std::string eleStrNeg = std::to_string(-ele);

const degrees outOfRangeLat = 120;
const std::string outOfRangeLatDDM = "12000.00";

const degrees outOfRangeLon = 250;
const std::string outOfRangeLonDDM = "25000.00";

const degrees latWithinBoundary = 90;
const std::string latWithinBoundaryDDM = "9000.00";

const degrees latBeyondBoundary = 90.01;
const std::string latBeyondBoundaryDDM = "9000.60";

const degrees lonWithinBoundary = 180;
const std::string lonWithinBoundaryDDM = "18000.00";

const degrees lonBeyondBoundary = 180.01;
const std::string lonBeyondBoundaryDDM = "18000.60";

const metres eleWithinBoundary = -6370000;
const std::string eleWithinBoundaryDDM = std::to_string(eleWithinBoundary);

const metres eleBelowBoundary = -6372000;
const std::string eleBelowBoundaryDDM = std::to_string(eleBelowBoundary);

///////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( NumericDD )

BOOST_AUTO_TEST_CASE( PositiveArgs )
{
    Position pos = Position(lat,lon,ele);

    BOOST_CHECK_CLOSE( pos.latitude(), lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NegativeArgs )
{
    Position pos = Position(-lat,-lon,-ele);

    BOOST_CHECK_CLOSE( pos.latitude(), -lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), -lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), -ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LatitudeOutOfRangePositive )
{
    BOOST_CHECK_THROW( Position(outOfRangeLat,lon,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LatitudeOutOfRangeNegative )
{
    BOOST_CHECK_THROW( Position(-outOfRangeLat,lon,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LatitudeWithinPositiveBoundary )
{
    Position pos = Position(latWithinBoundary,lon,ele);

    BOOST_CHECK_CLOSE( pos.latitude(), latWithinBoundary, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LatitudeBeyondPositiveBoundary )
{
   BOOST_CHECK_THROW( Position(latBeyondBoundary,lon,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LatitudeWithinNegativeBoundary )
{
    Position pos = Position(-latWithinBoundary,lon,ele);

    BOOST_CHECK_CLOSE( pos.latitude(), -latWithinBoundary, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LatitudeBeyondNegativeBoundary )
{
   BOOST_CHECK_THROW( Position(-latBeyondBoundary,lon,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LongitudeOutOfRangePositive )
{
    BOOST_CHECK_THROW( Position(lat,outOfRangeLon,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LongitudeOutOfRangeNegative )
{
    BOOST_CHECK_THROW( Position(lat,-outOfRangeLon,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LongitudeWithinPositiveBoundary )
{
    Position pos = Position(lat,lonWithinBoundary,ele);

    BOOST_CHECK_CLOSE( pos.latitude(), lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lonWithinBoundary, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LongitudeBeyondPositiveBoundary )
{
   BOOST_CHECK_THROW( Position(lat,lonBeyondBoundary,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LongitudeWithinNegativeBoundary )
{
    Position pos = Position(lat,-lonWithinBoundary,ele);

    BOOST_CHECK_CLOSE( pos.latitude(), lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), -lonWithinBoundary, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LongitudeBeyondNegativeBoundary )
{
   BOOST_CHECK_THROW( Position(lat,-lonBeyondBoundary,ele) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( ElevationWithinBoundary )
{
    Position pos = Position(lat,lon,eleWithinBoundary);

    BOOST_CHECK_CLOSE( pos.latitude(), lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), eleWithinBoundary, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ElevationBelowBoundary )
{
   BOOST_CHECK_THROW( Position(lat,lon,eleBelowBoundary) , std::invalid_argument );
}

BOOST_AUTO_TEST_SUITE_END()

///////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( DDM )

const char n = 'N';
const char e = 'E';
const char s = 'S';
const char w = 'W';

const std::string ddmNeg = "-5287.65";
const std::string ddmZero = "0000.00";

BOOST_AUTO_TEST_CASE( NE )
{
    Position pos = Position(latDDM,n,lonDDM,e,eleStr);

    BOOST_CHECK_CLOSE( pos.latitude(), lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NW )
{
    Position pos = Position(latDDM,n,lonDDM,w,eleStr);

    BOOST_CHECK_CLOSE( pos.latitude(), lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), -lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( SW )
{
    Position pos = Position(latDDM,s,lonDDM,w,eleStr);

    BOOST_CHECK_CLOSE( pos.latitude(), -lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), -lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( SE )
{
    Position pos = Position(latDDM,s,lonDDM,e,eleStr);

    BOOST_CHECK_CLOSE( pos.latitude(), -lat, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.longitude(), lon, percentageAccuracy );
    BOOST_CHECK_CLOSE( pos.elevation(), ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NegativeLatitude )
{
    BOOST_CHECK_THROW( Position(ddmNeg,n,lonDDM,e,eleStr) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( NegativeLongitude )
{
    BOOST_CHECK_THROW( Position(latDDM,n,ddmNeg,e,eleStr) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( NegativeElevation )
{
    Position pos = Position(latDDM,n,lonDDM,e,eleStrNeg);

    BOOST_CHECK_CLOSE( pos.elevation(), -ele, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LatitudeOutOfRange )
{
    BOOST_CHECK_THROW( Position(outOfRangeLatDDM,n,lonDDM,e,eleStr) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LatitudeWithinBoundary )
{
    Position pos = Position(latWithinBoundaryDDM,n,lonDDM,e,eleStr);

    BOOST_CHECK_CLOSE( pos.latitude(), latWithinBoundary, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LatitudeBeyondBoundary )
{
   BOOST_CHECK_THROW( Position(latBeyondBoundaryDDM,n,lonDDM,e,eleStr) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LatitudeZero )
{
    Position pos = Position(ddmZero,n,lonDDM,e,eleStr);

    BOOST_CHECK_SMALL( pos.latitude(), absoluteAccuracy );
}

BOOST_AUTO_TEST_CASE( LongitudeOutOfRange )
{
    BOOST_CHECK_THROW( Position(latDDM,n,outOfRangeLonDDM,e,eleStr) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LongitudeWithinBoundary )
{
    Position pos = Position(latDDM,n,lonWithinBoundaryDDM,e,eleStr);

    BOOST_CHECK_CLOSE( pos.longitude(), lonWithinBoundary, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( LongitudeBeyondBoundary )
{
   BOOST_CHECK_THROW( Position(latDDM,n,lonBeyondBoundaryDDM,e,eleStr) , std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( LongitudeZero )
{
    Position pos = Position(latDDM,n,ddmZero,e,eleStr);

    BOOST_CHECK_SMALL( pos.longitude(), absoluteAccuracy );
}

BOOST_AUTO_TEST_CASE( ElevationWithinBoundary )
{
    Position pos = Position(latDDM,n,lonDDM,e,eleWithinBoundaryDDM);

    BOOST_CHECK_CLOSE( pos.elevation(), eleWithinBoundary, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ElevationBelowBoundary )
{
   BOOST_CHECK_THROW( Position(latDDM,n,lonDDM,e,eleBelowBoundaryDDM) , std::invalid_argument );
}

BOOST_AUTO_TEST_SUITE_END()

///////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

/////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
