#include <boost/test/unit_test.hpp>

#include <sstream>

#include "xml-element.h"
#include "xml-parser.h"

using namespace XML;

BOOST_AUTO_TEST_SUITE( XML_Parser )

BOOST_AUTO_TEST_CASE( ParseRootElement )
{
    std::stringstream xmlData {"<a></a>"};

    XML::Parser parser(xmlData);

    BOOST_CHECK_NO_THROW(parser.parseRootElement());
}

BOOST_AUTO_TEST_CASE( ElementWithClosingTag )
{
    std::stringstream xmlData { "<a></a>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getName(), "a");
    BOOST_CHECK_EQUAL(element.getLeafContent() , "");
}

BOOST_AUTO_TEST_CASE( SelfClosingElement )
{
    std::stringstream xmlData { "<a/>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getName(), "a");
    BOOST_CHECK_EQUAL(element.getLeafContent() , "");
}

BOOST_AUTO_TEST_CASE( SingleAttribute )
{
    const std::string attributeName = "at";
    const std::string attributeValue = "data";

    std::stringstream xmlData { "<a " + attributeName + "=\"" + attributeValue +"\"/>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getAttribute(attributeName), attributeValue);
}

BOOST_AUTO_TEST_CASE( MultipleAttributes )
{
    std::stringstream xmlData { "<a at1=\"data1\" at2=\"data2\" />" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getAttribute("at1"), "data1");
    BOOST_CHECK_EQUAL(element.getAttribute("at2"), "data2");
}

BOOST_AUTO_TEST_CASE( AttributeInOpeningTag )
{
    const std::string attributeName = "at";
    const std::string attributeValue = "data";

    std::stringstream xmlData { "<a " + attributeName + "=\"" + attributeValue +"\"></a>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getAttribute(attributeName), attributeValue);
}

BOOST_AUTO_TEST_CASE( LeafContent )
{
    const std::string leafContent = "Hello";
    std::stringstream xmlData { "<a>" + leafContent + "</a>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getLeafContent() , leafContent);
}

BOOST_AUTO_TEST_CASE( LeafContentNotTrimming )
{
    const std::string leafContent = "  Hello World   ";
    std::stringstream xmlData { "<a>" + leafContent + "</a>" };

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getLeafContent() , leafContent);
}

BOOST_AUTO_TEST_CASE( ContainsSubElements )
{
    std::stringstream xmlData { "<root><a></a><b/></root>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.countSubElements("a"), 1);
    BOOST_CHECK_EQUAL(element.countSubElements("b"), 1);
}

BOOST_AUTO_TEST_CASE( CountSubElements )
{
    std::stringstream xmlData { "<root><a></a><b/><a></a><b/><b></b></root>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.countSubElements("a"), 2);
    BOOST_CHECK_EQUAL(element.countSubElements("b"), 3);
}

BOOST_AUTO_TEST_CASE( SubElementNameIsName )
{
    const std::string subElementName = "a";
    std::stringstream xmlData { "<root><" + subElementName + "></" + subElementName + "></root>" };

    XML::Parser parser(xmlData);
    Element element = parser.parseRootElement();
    BOOST_REQUIRE_EQUAL(element.countSubElements(subElementName),1);
    Element subElement = element.getSubElement(subElementName);

    BOOST_CHECK_EQUAL(subElement.getName(), subElementName);
}

BOOST_AUTO_TEST_CASE( SubElementLeafContent )
{
    const std::string subElementName = "a";
    const std::string leafContent = "Hello";
    std::stringstream xmlData { "<root><" + subElementName + ">" + leafContent + "</" + subElementName + "></root>" };

    XML::Parser parser(xmlData);
    Element parsedElement = parser.parseRootElement();
    BOOST_REQUIRE_EQUAL(parsedElement.countSubElements(subElementName),1);
    Element subElement = parsedElement.getSubElement(subElementName);

    BOOST_CHECK_EQUAL(subElement.getLeafContent() , leafContent);
}


BOOST_AUTO_TEST_SUITE_END()
