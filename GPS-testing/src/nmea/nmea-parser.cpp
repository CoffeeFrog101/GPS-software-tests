// #include <iostream> // For debugging
#include <stdexcept>

#include "nmea-parser.h"

namespace GPS::NMEA
{
  bool isSupportedFormat(std::string)
  {
      // Stub definition, needs implementing
      return false;
  }

  bool hasValidSentenceStructure(std::string s)
  {
      char c;
      unsigned int i;
      if (s.empty()) return false; // Need this or next line could crash
      if (s[0] != '$') return false;
      if (s.size() < 3) return false; // Need this or next line could crash
      if (s[1] != 'G') return false;
      if (s[2] != 'P') return false;
      // cout << "Debug A" << endl;
      if (s.size() < 6) return false; // Need this or next line could crash
      c = s[3];
      if (c < 65 || c > 90) return false;
      c = s[4];
      if (c < 65 || c > 90) return false;
      c = s[5];
      if (c < 'A' || c > 'Z') return false; // Hey Ken: we can do it like this instead of using the ASCII codes.  Shall I change the previous two like this as well?
      // cout << "Debug B" << endl;
      if (s.size() < 7) return false; // Need this or next line could crash.
      if (s[6] != ',') return false;
      // cout << "Debug C" << endl;
      /*
      i = 7;
      while (true)
      {
          if (i == potentialSentence.size()) return false;
          if (potentialSentence[i] == '$') return false; // Not allowed here.
          if (potentialSentence[i] == '*') break;
          ++i;
      }
      */
      // Nadia: for-loop is better.
      for (i = 7; true; ++i)
      {
          if (i == s.size()) return false;
          if (s[i] == '$') return false; // Not allowed here.
          if (s[i] == '*') break;
      }
      // cout << "Debug D" << endl;
      if (s.size() < i+3) return false; // Note: i+3 NOT i+2.  This is confusing.
      c = s[i+1];
      if ((c < 48) || (c > 57 && c < 65) || (c > 70 && c < 97) || (c > 102)) return false;
      c = s[i+2];
      if ((c < 48) || (c > 57 && c < 65) || (c > 70 && c < 97) || (c > 102)) return false;
      // cout << "Debug E" << endl;
      // cout << potentialSentence.size() << endl;
      // cout << i << endl;
      if (s.size() != i+3) return false; // Note: i+3 NOT i+2.  This is confusing.
      // Alice: It's because indexing starts at 0, so the index number is one less.
      // Nadia: No, it's because we're using '<' rather than '<='.  We need a char at i+2, so we say < i+3.
      // Ken: But indexing normally starts at 0, and we normally use <. So why is this different?
      return true;
  }

  bool checksumMatches(std::string)
  {
      // Stub definition, needs implementing
      return false;
  }

  SentenceData parseSentence(std::string)
  {
      // Stub definition, needs implementing
      return {"",{}};
  }

  bool hasCorrectNumberOfFields(SentenceData)
  {
      // Stub definition, needs implementing
      return true;
  }

  Position positionFromSentenceData(SentenceData d)
  {
      using namespace std; // Ken: Writing 'std::' everywhere is irritating.
      string la, lo, ns, ew, e;
      char ns0, ew0;
      Position p = Position(0,0,0); // Dummy object becasue there's no default constructor available for Postion
      if (d.format == "GLL")
      {
          la = d.dataFields[0];
          lo = d.dataFields[2];
          ns = d.dataFields[1];
          ew = d.dataFields[3];
          e = "0"; // no ele
          if (ns.size() == 1) {
              ns0 = ns[0];
          } else {
              throw domain_error(string("Ill-formed bearing in GLL sentence field: ") + ns + ". Bearings must be a single character.");
          }
          if (ew.size() == 1) {
              ew0 = ew[0];
          } else {
              throw domain_error(string("Ill-formed bearing in GLL sentence field: ") + ew + ". Bearings must be a single character.");
          }
          try
          {
                p = Position(la,ns0,lo,ew0,e);
          }
          catch (const invalid_argument& e)
          {
                throw domain_error(string("Ill-formed GLL sentence field: ") + e.what());
          }
      }
      else if (d.format == "RMC")
      {
          la = d.dataFields[2];
          lo = d.dataFields[4];
          ns = d.dataFields[3];
          ew = d.dataFields[5];
          e = "0"; // no ele
          if (ns.size() != 1) {
              throw domain_error(string("Ill-formed bearing in RMC sentence field: ") + ns + ". Bearings must be a single character.");
          } else {
              ns0 = ns[0];
          }
          if (ew.size() != 1) {
              throw domain_error(string("Ill-formed bearing in RMC sentence field: ") + ew + ". Bearings must be a single character.");
          } else {
              ew0 = ew[0];
          }
          try {
            p = Position(la,ns0,lo,ew0,e);
          }
          catch (const invalid_argument& e) {
            throw domain_error(string("Ill-formed RMC sentence field: ") + e.what());
          }
      } else if (d.format == "GGA")
      {
          la = d.dataFields[1];
          lo = d.dataFields[3];
          ns = d.dataFields[2];
          ew = d.dataFields[4];
          e = d.dataFields[8];
          if (ns.size() != 1) {
              throw domain_error(string("Ill-formed bearing in GGA sentence field: ") + ns + ". Bearings must be a single character.");
          }
          ns0 = ns[0];
          if (ew.size() != 1) {
              throw domain_error(string("Ill-formed bearing in GGA sentence field: ") + ew + ". Bearings must be a single character.");
          }
          ew0 = ew[0];
          try {
            p = Position(la,ns0,lo,ew0,e);
          } catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed GGA sentence field: ") + e.what());
          }
      }
      return p;
  }

  std::vector<Position> readSentences(std::istream &)
  {
      // Stub definition, needs implementing
      return {};
  }
}
