#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <istream>
#include <set>

#include "xml-element.h"

namespace XML
{

class Parser
{
  public:
    Parser(std::istream&);
    Element parseRootElement();

  private:
    std::istream& xmlSource;

    struct OpeningTag;

    Element parseElement();
    OpeningTag parseOpeningTag();
    Attributes parseAttributes();
    std::string parseAttributeValue();
    SubElements parseSubElements();
    LeafContent parseLeafContent();
    void parseClosingTag(ElementName);

    std::string parseName();
    void parseWhitespace();

    void tryparseProlog(); // currently we discard the Prolog

    bool tryParseChar(char);
    bool tryParseString(std::string);

    std::string parseBetweenDelimiters(char delimiter);
    std::string parseUntil(char delimiter);
    std::string parseUntilAnyOf(std::set<char> delimiters);
    std::string parseWhileAnyOf(std::set<char> validChars);

    bool nameNext();
    bool closingTagNext();

    void require(bool condition, std::string errorMessage);

    const std::set<char> whitespaceChars {' ','\t','\n','\v','\f','\r'};
    const std::set<char> nameDelimiters {' ','\t','\n','\v','\f','\r','/','>','=',std::char_traits<char>::eof()};
};

}

#endif
