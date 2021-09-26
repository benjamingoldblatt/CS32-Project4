#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <list>
#include <vector>
class Table
    {
      public:
        Table(std::string keyColumn, const std::vector<std::string>& columns);
        ~Table();
        bool good() const;
        bool insert(const std::string& recordString);
        void find(std::string key, std::vector<std::vector<std::string>>& records) const;
        int select(std::string query, std::vector<std::vector<std::string>>& records) const;

          // We prevent a Table object from being copied or assigned by
          // making the copy constructor and assignment operator unavailable.
        Table(const Table&) = delete;
        Table& operator=(const Table&) = delete;
      private:
        struct Record {
            std::vector<std::string> values;
            Record(std::vector<std::string> values): values(values) {}
        };
        int keycol = -1;
        int len = 997;
        std::vector<std::string> cols;
        std::vector<std::list<Record>> table;
    };
// StringParser

// A StringParser object is constructed from a string.  Successive calls
// to the getNextField member function extract the next field from the
// string, returning true if it got one and false if there was no next field.
// Fields are separated by blank, tab, carriage return, or newline, and
// may be surrounded by single quotes, which are stripped off in the
// extracted field.  Two consecutive single quotes in a quoted field are
// converted to one.
//
// Here's an example of what it does:
//     StringParser parser("  abc  'def ghi'  'Don''t cry'  ''  jk ");
//     string s;
//     assert(parser.getNextField(s)  &&  s == "abc");
//     assert(parser.getNextField(s)  &&  s == "def ghi");
//     assert(parser.getNextField(s)  &&  s == "Don't cry");
//     assert(parser.getNextField(s)  &&  s == "");
//     assert(parser.getNextField(s)  &&  s == "jk");
//     assert(!parser.getNextField(s));


class StringParser
{
  public:
    StringParser(std::string text = "")
    {
        setString(text);
    }

    void setString(std::string text)
    {
        m_text = text;
        m_start = 0;
    }

    bool getNextField(std::string& field);

  private:
    std::string m_text;
    size_t m_start;
};

#endif //TABLE_H
