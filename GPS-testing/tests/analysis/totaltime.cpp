#include <boost/test/unit_test.hpp>

#include "types.h"
#include "waypoints.h"
#include "analysis-track.h"
#include "gridworld-track.h"

using namespace GPS;


BOOST_AUTO_TEST_SUITE( Track_totalTime )

const metres gridUnit = 1000;
const GridWorld::Model gwNearEquator {Earth::Pontianak,gridUnit,gridUnit,gridUnit};


// Typical case: A track without rests, containing a variety of different travel times.
BOOST_AUTO_TEST_CASE( NoRests )
{
   const std::vector<TrackPoint> trackPoints = GridWorld::Track("A1B5Q1W6E",gwNearEquator).toTrackPoints();
   const metres restingRange = gridUnit * 0.1; // Ensure no rests.
   const Analysis::Track track {trackPoints,restingRange};
   const std::chrono::seconds expectedTime {13}; // 1 + 5 + 1 + 6

   std::chrono::seconds actualTime = track.totalTime();

   BOOST_CHECK_EQUAL( actualTime.count(), expectedTime.count() );
}


BOOST_AUTO_TEST_SUITE_END()
