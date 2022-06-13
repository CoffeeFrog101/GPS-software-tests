#include <boost/test/unit_test.hpp>

#include "geometry.h"

using namespace GPS;

BOOST_AUTO_TEST_SUITE( Geometry )

////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( NormaliseDegrees )

const double percentageAccuracy = 0.0001;
const double absoluteAccuracy = 0.0001;

BOOST_AUTO_TEST_CASE( alreadyNormalisedPositive )
{
    const degrees initialDegrees = 135;
    const degrees expectedDegrees = initialDegrees;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( alreadyNormalisedNegative )
{
    const degrees initialDegrees = -170;
    const degrees expectedDegrees = initialDegrees;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( alreadyNormalisedZero )
{
    const degrees initialDegrees = 0;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_SMALL( actualDegrees, absoluteAccuracy );
}

BOOST_AUTO_TEST_CASE( oneReductionPositive )
{
    const degrees initialDegrees = 410;
    const degrees expectedDegrees = initialDegrees - fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( oneReductionPositiveToNegative )
{
    const degrees initialDegrees = 205;
    const degrees expectedDegrees = initialDegrees - fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( oneIncreaseNegative )
{
    const degrees initialDegrees = -380;
    const degrees expectedDegrees = initialDegrees + fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( oneIncreaseNegativeToPositive )
{
    const degrees initialDegrees = -250;
    const degrees expectedDegrees = initialDegrees + fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( multipleReductions )
{
    const degrees initialDegrees = 1340;
    const degrees expectedDegrees = initialDegrees - 4*fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( multipleIncreases )
{
    const degrees initialDegrees = -2345;
    const degrees expectedDegrees = initialDegrees + 7*fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( multipleReductionsToZero )
{
    const degrees initialDegrees = 720;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_SMALL( actualDegrees, absoluteAccuracy );
}

BOOST_AUTO_TEST_CASE( boundaryLargestAlreadyNormalised )
{
    const degrees initialDegrees = 180;
    const degrees expectedDegrees = initialDegrees;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( boundaryAboveLargestAlreadyNormalised )
{
    const degrees initialDegrees = 180.1;
    const degrees expectedDegrees = initialDegrees - fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( boundaryLargestNotNormalised )
{
    const degrees initialDegrees = -180;
    const degrees expectedDegrees = initialDegrees + fullRotation;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( boundaryBelowLargestNotNormalised )
{
    const degrees initialDegrees = -179.9;
    const degrees expectedDegrees = initialDegrees;

    degrees actualDegrees = normaliseDegrees(initialDegrees);

    BOOST_CHECK_CLOSE( actualDegrees, expectedDegrees, percentageAccuracy );
}

BOOST_AUTO_TEST_SUITE_END()

////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
