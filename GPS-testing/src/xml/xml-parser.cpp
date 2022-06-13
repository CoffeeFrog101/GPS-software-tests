#include <cassert>
#include <map>
#include <vector>
#include <cctype>
#include <iterator>
#include <sstream>

#include "xml-element.h"

#include "xml-parser.h"

namespace XML
{

Parser::Parser(std::istream& xml)
    : xmlSource{xml}
{}

struct Parser::OpeningTag
{
    ElementName name;
    std::map<AttributeName, AttributeValue> attributes;
    bool isSelfClosing;
};

Element Parser::parseRootElement()
{
    parseWhitespace();
    tryparseProlog();
    parseWhitespace();
    return parseElement();
}

void Parser::tryparseProlog()
{
    if (tryParseString("<?xml"))
    {
        parseWhitespace();
        parseAttributes();
        parseWhitespace();
        require(tryParseString("?>"), "prolog not terminated correctly.");
    }
}

Element Parser::parseElement()
{
    OpeningTag openingTag = parseOpeningTag();

    if (openingTag.isSelfClosing)
    {
        return SelfClosingElement(openingTag.name,openingTag.attributes);
    }

    LeafContent potentialLeafContent = parseLeafContent();

    if (closingTagNext())
    {
        parseClosingTag(openingTag.name);
        return LeafElement(openingTag.name,openingTag.attributes,potentialLeafContent);
    }
    else
    {
        SubElements subElements = parseSubElements();
        parseClosingTag(openingTag.name);
        return InternalNodeElement(openingTag.name,openingTag.attributes,subElements);
    }
}

Parser::OpeningTag Parser::parseOpeningTag()
{
    OpeningTag tag;
    require(tryParseChar('<'), "opening tag not started correctly.");
    tag.name = parseName();
    parseWhitespace();
    tag.attributes = parseAttributes();

    if (tryParseChar('>'))
    {
        tag.isSelfClosing = false;
    }
    else
    {
        require(tryParseString("/>"), "opening tag not terminated correctly.");
        tag.isSelfClosing = true;
    }

    return tag;
}

void Parser::parseClosingTag(ElementName tagName)
{
    std::string closingTag = "</" + tagName + ">";
    require(tryParseString(closingTag), "missing closing tag: " + closingTag);
}

Attributes Parser::parseAttributes()
{
    std::map<AttributeName, AttributeValue> attributes;
    while (nameNext())
    {
        AttributeName name = parseName();
        parseWhitespace();
        require(tryParseChar('='), "attribute missing '='");
        parseWhitespace();
        AttributeValue value = parseAttributeValue();
        parseWhitespace();
        attributes[name] = value;
    }
    return attributes;
}

SubElements Parser::parseSubElements()
{
    std::map<ElementName, std::vector<Element>> subElements;

    parseWhitespace();
    while (! closingTagNext())
    {
        Element element = parseElement();
        subElements[element.getName()].push_back(element);

        parseWhitespace();
    }

    return subElements;
}

AttributeValue Parser::parseAttributeValue()
{
    return parseBetweenDelimiters('\"');
}

LeafContent Parser::parseLeafContent()
{
    return parseUntil('<');
}

std::string Parser::parseName()
{
    assert (nameNext());
    return parseUntilAnyOf(nameDelimiters);
}

void Parser::parseWhitespace()
{
    parseWhileAnyOf(whitespaceChars);
}

bool Parser::tryParseChar(char charToMatch)
{
    if (xmlSource.get() == charToMatch)
    {
        return true;
    }
    else
    {
        xmlSource.unget();
        return false;
    }
}

bool Parser::tryParseString(std::string stringToMatch)
{
    for (std::string::const_iterator it = stringToMatch.begin(); it != stringToMatch.end(); ++it)
    {
        if (xmlSource.get() != *it)
        {
             // Rewind the XML stream back to the start of the string.
             for (; it != std::prev(stringToMatch.begin()); --it)
             {
                 xmlSource.unget();
             }
             return false;
        }
    }
    return true;
}

std::string Parser::parseBetweenDelimiters(char delimiter)
{
    const std::string delimiterAsText = std::string(1,delimiter) + " delimiter.";

    require(tryParseChar('\"'), "missing opening " + delimiterAsText);

    std::string xmlBetweenDelimiters;
    std::getline(xmlSource, xmlBetweenDelimiters, delimiter);
    require(! xmlSource.eof(), "missing closing " + delimiterAsText);

    return xmlBetweenDelimiters;
}

std::string Parser::parseUntil(char delimiter)
{
    std::string xmlUptoDelimiter;
    std::getline(xmlSource, xmlUptoDelimiter, delimiter);
    xmlSource.unget(); // The delimiter or end-of-file character.
    return xmlUptoDelimiter;
}

std::string Parser::parseUntilAnyOf(std::set<char> delimiters)
{
    std::ostringstream parsedText;
    while (! delimiters.count(xmlSource.peek()))
    {
        parsedText << char(xmlSource.get());
    }
    return parsedText.str();

    // From C++ 20 we can replace .count() with .contains(), which will be clearer.
}

std::string Parser::parseWhileAnyOf(std::set<char> validChars)
{
    std::ostringstream parsedText;
    for (char nextChar = xmlSource.peek(); validChars.count(nextChar); nextChar = xmlSource.peek())
    {
        parsedText << xmlSource.get();
    }
    return parsedText.str();

    // From C++ 20 we can replace .count() with .contains(), which will be clearer.
}

bool Parser::nameNext()
{
    return isalpha(xmlSource.peek());
}

bool Parser::closingTagNext()
{
    if (tryParseString("</"))
    {
        xmlSource.unget();
        xmlSource.unget();
        return true;
    }
    return false;
}

void Parser::require(bool condition, std::string errorMessage)
{
    if (! condition) throw std::domain_error("Malformed XML: " + errorMessage);
}

}
