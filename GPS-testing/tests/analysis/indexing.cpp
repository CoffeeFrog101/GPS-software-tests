#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "types.h"
#include "waypoints.h"
#include "analysis-route.h"
#include "gridworld-route.h"

using namespace GPS;

/* The Route.operator[] uses an index number to select a particular point in the Route.
 *
 * There are therefore two main aspects to test:
 *   - the length of the Route;
 *   - the index number used.
 *
 * The actual data stored in the routes (Position and name) isn't particularly
 * significant for this function, as the function shouldn't need to process that data at
 * all when locating a point by index number. However, we should ensure that each test
 * point is different, as otherwise a bug that returns the wrong point might go undetected
 * if the other point returned is another copy of the point that should have been returned.
 *
 * We should test both valid and invalid indices. A valid index is less than the length
 * of the route, and returns the RoutePoint. An invalid index is greater than the length
 * of the route, and we should check that this throws the expected exception.
 *
 * Edges and boundaries:
 *  - For the index number there is one edge case to test: index number 0.
 *  - There is a boundary between valid and invalid indices - we should test the largest
 *    valid index and the smallest invalid index.
 *  - The route length itself has a single edge case: a route containing zero points.
 *
 * Note: We do not need to consider negative index numbers because the parameter type is
 * 'unsigned int'. If, in future, this was changed to a (signed) int, then there would
 * be another boundary between 0 and -1, and we would need to test that negative indices
 * throw an exception.
 */

BOOST_AUTO_TEST_SUITE( Route_Indexing )

const double percentageAccuracy = 0.2;
const double epsilon = 0.0001;

const metres northSouthGridUnit = 100000;
const metres eastWestGridUnit = 100000;
const metres verticalGridUnit = 1000;
const GridWorld::Model gwNearEquator {Earth::Pontianak,northSouthGridUnit,eastWestGridUnit,verticalGridUnit};


// Typical input - indexing in the middle of a route containing multiple points.
BOOST_AUTO_TEST_CASE( MiddleOfRoute )
{
    const Analysis::Route route {GridWorld::Route("KLMNO",gwNearEquator).toRoutePoints()};
    const unsigned int index = 2;
    const std::string expectedName = "M";
    const Position expectedPosition = gwNearEquator['M'];

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
    BOOST_CHECK_CLOSE( actualPoint.position.elevation(), expectedPosition.elevation(), percentageAccuracy);
}

// Edge case - the first point in the route.
BOOST_AUTO_TEST_CASE( StartOfRoute )
{
    const Analysis::Route route {GridWorld::Route("ABHNT",gwNearEquator).toRoutePoints()};
    const unsigned int index = 0;
    const std::string expectedName = "A";
    const Position expectedPosition = gwNearEquator['A'];

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
    BOOST_CHECK_CLOSE( actualPoint.position.elevation(), expectedPosition.elevation(), percentageAccuracy);
}

// Boundary case - the last valid index in the route.
BOOST_AUTO_TEST_CASE( EndOfRoute )
{
    const Analysis::Route route {GridWorld::Route("AYXW",gwNearEquator).toRoutePoints()};
    const unsigned int index = 3;
    const std::string expectedName = "W";
    const Position expectedPosition = gwNearEquator['W'];;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
    BOOST_CHECK_CLOSE( actualPoint.position.elevation(), expectedPosition.elevation(), percentageAccuracy);
}

// Corner case - only one valid index, and it is both the first and last index in the route.
BOOST_AUTO_TEST_CASE( OnlyPointOnRoute )
{
    const Analysis::Route route {GridWorld::Route("Q",gwNearEquator).toRoutePoints()};
    const unsigned int index = 0;
    const std::string expectedName = "Q";
    const Position expectedPosition = gwNearEquator['Q'];;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
    BOOST_CHECK_CLOSE( actualPoint.position.elevation(), expectedPosition.elevation(), percentageAccuracy);
}

// Error case - index out of range.
BOOST_AUTO_TEST_CASE( OutOfRange )
{
    const Analysis::Route route {GridWorld::Route("ABCDE",gwNearEquator).toRoutePoints()};
    const unsigned int index = 20;

    BOOST_CHECK_THROW( route[index], std::out_of_range );
}

// Boundary error case - first index that is out-of-range.
BOOST_AUTO_TEST_CASE( OutOfRangeByOne )
{
    const Analysis::Route route {GridWorld::Route("ABCDE",gwNearEquator).toRoutePoints()};
    const unsigned int index = 5;

    BOOST_CHECK_THROW( route[index], std::out_of_range );
}

// Corner error case - an empty route, so any index is out of range.
BOOST_AUTO_TEST_CASE( EmptyRoute )
{
    const Analysis::Route route {std::vector<RoutePoint>()};
    const unsigned int index = 0;

    BOOST_CHECK_THROW( route[index], std::out_of_range );
}

BOOST_AUTO_TEST_SUITE_END()
