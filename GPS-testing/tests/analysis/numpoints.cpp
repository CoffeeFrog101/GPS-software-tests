#include <boost/test/unit_test.hpp>

#include "types.h"
#include "waypoints.h"
#include "analysis-route.h"

using namespace GPS;

/* The Route.numPoints() functions counts the number of route points within the Route.
 *
 * The main test consideration is therefore to test routes with different numbers of
 * route points. The actual data stored in those routes (Position and name) isn't
 * significant for this function, as the function shouldn't need to process that data at
 * all when counting points.
 *
 * There is one major edge case for this function: an empty route containing no points.
 *
 * An atypical case to test is when the same Position occurs multiple times in the route,
 * to ensure each duplicate is also counted.
 *
 * Even though the name data shouldn't affect the the function result, it might be worth
 * testing a Route containing unnamed route points, as a possible bug is that *names*
 * are being counted rather than points.
 */

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Route_numPoints )

const Position pos1 = Position(10,1,100);
const Position pos2 = Position(20,2,200);
const Position pos3 = Position(30,3,300);
const Position pos4 = Position(40,4,400);
const Position pos5 = Position(50,5,500);

const RoutePoint rtpt1 = { pos1, "P1" };
const RoutePoint rtpt2 = { pos2, "P2" };
const RoutePoint rtpt3 = { pos3, "P3" };

// Typical case: a small route with multiple points.
BOOST_AUTO_TEST_CASE( MultiplePoints )
{
    const std::vector<RoutePoint> routePoints = { rtpt1, rtpt2, rtpt3 };
    const Analysis::Route route {routePoints};
    const unsigned int expectedNumPoints = 3;

    unsigned int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// Edge case: an empty route with no points.
BOOST_AUTO_TEST_CASE( EmptyRoute )
{
    const std::vector<RoutePoint> routePoints = {};
    const Analysis::Route route {routePoints};
    const unsigned int expectedNumPoints = 0;

    unsigned int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// Atypical case: a route containing duplicated points.
BOOST_AUTO_TEST_CASE( DuplicatedPoints )
{
    const std::vector<RoutePoint> routePoints = { rtpt1, rtpt2, rtpt1, rtpt3 };
    const Analysis::Route route {routePoints};
    const unsigned int expectedNumPoints = 4;

    unsigned int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// Typical case: A route containing unnamed points.
BOOST_AUTO_TEST_CASE( UnnamedPoints )
{
    const std::vector<RoutePoint> routePoints = { {pos1,""}, {pos2,""}, {pos3,""}, {pos4,""}, {pos5,""} };
    const Analysis::Route route {routePoints};
    const unsigned int expectedNumPoints = 5;

    unsigned int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

BOOST_AUTO_TEST_SUITE_END()

///////////////////////////////////////////////////////////////////////////////
