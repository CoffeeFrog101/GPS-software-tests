#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <cmath>
#include <algorithm>
#include<string>
#include "types.h"
#include "waypoints.h"
#include "analysis-route.h"
#include "gridworld-route.h"
#include "position.h"
#include "geometry.h"


using namespace GPS;


BOOST_AUTO_TEST_SUITE( TotalLength )
const Position A = Position(0.179964,109.142,1000);//Current Horizental
const Position P = Position(-0.089982,109.142,1000);//Next Horizental
const Position C = Position(0,3,3070);//Current Vertical
const Position D = Position(0,4,7000);//Next Vertical

//Specify Route points
const RoutePoint rtptA = { A,"P1" };
const RoutePoint rtptP = { P, "P2" };
const RoutePoint rtptC = { C, "P3" };
const RoutePoint rtptD = { D, "P4" };

double Subtract(Position p1,Position p2)//function to calculate vertical difference
{
    const degrees elev1 = p1.elevation();//elev1 the elevation in the Position data Type
    const degrees elev2 = p2.elevation();

    double verticalDifference = elev2 - elev1;

    return verticalDifference;//to prevent a end of non-void function and unecessary bugs

};

double VerticalPosition(Position lat1)//Function to see if Position inputted is vertical
{

    const radians VerticalLat1 = lat1.latitude();


    if(VerticalLat1 != 0)throw std::domain_error("Invalid Vertical Position.");
    return VerticalLat1;

}
double HorizontalPosition(Position lon1)//Function to see if Position inputted is vertical
{

    const radians HorizontalLon1 = lon1.longitude();


    if(HorizontalLon1 !=109.142)throw std::domain_error("Invalid Horizontal Position.");

return HorizontalLon1;

}
double MaximumElev(Position X)//Function to see if Position inputted is within elevation range
{
    const degrees Elve = X.elevation();

    if(Elve>=8000)throw std::domain_error("High Elevation value.");
    return Elve;


}
double MinimumElev(Position Y)//Function to see if Position inputted is within elevation range
{
     const degrees Elve = Y.elevation();


     if(Elve<100){throw std::domain_error("Low Elevation value.");}
     return Elve;
}

//Typical Test-  formula for Horizontal Distance between two route points is correct when using the Haversine formula
BOOST_AUTO_TEST_CASE(horizontalDifference)
{
    const std::vector<RoutePoint> routePoints = {rtptA,rtptP};
    const Analysis::Route route {routePoints};

    double Result = route.totalLength();
//    std::cout<<Result;
    double expectedResult = Position::horizontalDistanceBetween(A, P);//result when using the Haversine formula


    BOOST_CHECK( Result = expectedResult);
}
//Typical Test- formula for Vertical Distance between two route points is correct
BOOST_AUTO_TEST_CASE(VerticalDifference)
{

    metres verticalDifference = Subtract(C,D);

   double ExpectedResult = 7000-3070 ;


    BOOST_CHECK_EQUAL( ExpectedResult, verticalDifference);

}

//Edge Case-  Horizontal Values are only Two and are Horizontal
BOOST_AUTO_TEST_CASE(OnlyTwoHorizentalValues)
{

    const std::vector<RoutePoint> routePoints = { rtptA, rtptP };
    const Analysis::Route route {routePoints};

    metres ExpectedHorizontalValue1 = HorizontalPosition(A);
    metres ExpectedHorizontalValue2 = HorizontalPosition(P);

    const unsigned int expectedPoints = 2;
    unsigned int actualPoints = route.numPoints();
    BOOST_CHECK_EQUAL( actualPoints, expectedPoints );
  BOOST_CHECK(ExpectedHorizontalValue1==A.longitude()||ExpectedHorizontalValue2==P.longitude());

}
//Edge Case- Vertical Values are only 2 and are Vertical

BOOST_AUTO_TEST_CASE(OnlyTwoVerticalValues)
{
    const std::vector<RoutePoint> routePoints = { rtptC, rtptD };
    const Analysis::Route route {routePoints};

    metres ExpectedverticalValue1 = VerticalPosition(C);
    metres ExpectedverticalValue2 = VerticalPosition(D);



    const unsigned int expectedPoints = 2;
    unsigned int actualPoints = route.numPoints();
    BOOST_CHECK_EQUAL( actualPoints, expectedPoints );
    BOOST_CHECK(ExpectedverticalValue1==C.latitude()&&ExpectedverticalValue2==D.latitude());

}

//edge case Route Points with no points
BOOST_AUTO_TEST_CASE(nullPoints)
{
    const std::vector<RoutePoint> routePoints = { };
    const Analysis::Route route {routePoints};
    const unsigned int expectedPoints = 0;

    unsigned int actualPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualPoints, expectedPoints );

}
//Boundary case - Maximum elevation of the all Positions
BOOST_AUTO_TEST_CASE(MaximumElevation)
{
   degrees ElevationA = MaximumElev(A);
   degrees ElevationP = MaximumElev(P);
   degrees ElevationC = MaximumElev(C);
   degrees ElevationD = MaximumElev(D);
//   std::cout<<ElevationA<<std::endl;(debugging)
//   std::cout<<A.elevation()<<std::endl;(debugging)
   BOOST_CHECK(ElevationA==A.elevation()&&ElevationP==P.elevation()&&ElevationC==C.elevation()&&ElevationD==D.elevation());


}
//Boundary case - Minimum elevation of the all Positions
BOOST_AUTO_TEST_CASE(MinimumElevation)
{
   degrees ElevA = MinimumElev(A);
   degrees ElevP = MinimumElev(P);
   degrees ElevC = MinimumElev(C);
   degrees ElevD = MinimumElev(D);



   BOOST_CHECK(ElevA==A.elevation()&&ElevP==P.elevation()&&ElevC==C.elevation()&&ElevD==D.elevation());


}
//Typical Test- Evaluating totallength
BOOST_AUTO_TEST_CASE(TotalLength)
{

   double actualLength = 30272.8;

// GPS::pythagoras(HorizontalValue,7);

    double HorizontalDifference = Position::horizontalDistanceBetween(A, P);//result when using the Haversine formula
    double VerticalDifference = Subtract(C,D);//Subtracts the elevation value of Position C and D

    double Length = GPS::pythagoras(HorizontalDifference,VerticalDifference);
//std::cout<<VerticalDifference;

    BOOST_CHECK(Length = actualLength);
}


BOOST_AUTO_TEST_SUITE_END()



