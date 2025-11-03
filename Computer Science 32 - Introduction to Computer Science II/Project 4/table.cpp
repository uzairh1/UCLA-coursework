#include "table.h"
#include <cstdlib>
#include <algorithm>
using namespace std;

const std::vector<std::string> ALLOWED_OPERATORS = { "<", "<=" , ">" , ">=" , "!=" , "==" , "=", "LT", "Lt", "lT", "lt" , "LE", "Le", "lE", "le", "GT", "gT", "Gt", "gt", "GE", "gE", "Ge", "ge", "NE", "nE", "Ne", "ne", "EQ", "eq", "eQ", "Eq" };
const std::vector<std::string> NUMERICAL_OPERATORS = { "LT", "Lt", "lT", "lt" , "LE", "Le", "lE", "le", "GT", "gT", "Gt", "gt", "GE", "gE", "Ge", "ge", "NE", "nE", "Ne", "ne", "EQ", "eq", "eQ", "Eq" };
// StringParser class

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

#include <string>

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

bool StringParser::getNextField(std::string& fieldText)
{
    m_start = m_text.find_first_not_of(" \t\r\n", m_start);
    if (m_start == std::string::npos)
    {
        m_start = m_text.size();
        fieldText = "";
        return false;
    }
    if (m_text[m_start] != '\'')
    {
        size_t end = m_text.find_first_of(" \t\r\n", m_start + 1);
        fieldText = m_text.substr(m_start, end - m_start);
        m_start = end;
        return true;
    }
    fieldText = "";
    for (;;)
    {
        m_start++;
        size_t end = m_text.find('\'', m_start);
        fieldText += m_text.substr(m_start, end - m_start);
        m_start = (end != std::string::npos ? end + 1 : m_text.size());
        if (m_start == m_text.size() || m_text[m_start] != '\'')
            break;
        fieldText += '\'';
    }
    return true;
}

Table::Table(string keyColumn, const vector<string>& columns) : data(columns.size())
{
    keyField = keyColumn;
    mColumns = columns;
    int keyIndex = -1;
    for (int i = 0; i < columns.size(); i++)
    {
        if (columns[i] == keyColumn)
        {
            keyIndex = i;
        }
    }

    mKeyIndex = keyIndex;
}

Table::~Table()
{
}

bool Table::good() const
{
    if (mColumns.empty())
        return false;

    if (mKeyIndex == -1)
    {
        return false;
    }

    vector<string>::const_iterator it = mColumns.begin();
    while (it != mColumns.end())
    {
        if (*it == "")
            return false;
        it++;
    }

    for (int i = 0; i < mColumns.size(); i++)
    {
        for (int j = i + 1; j < mColumns.size(); j++)
        {
            if (mColumns[i] == mColumns[j])
                return false;
        }
    }

    return true;
}

bool Table::insert(const std::string& recordString)
{
    vector<string> v;
    string s;

    StringParser parser(recordString);
    while (parser.getNextField(s))
    {
        v.push_back(s);
    }

    if (v.size() != mColumns.size())
    {
        return false;
    }


    return data.insert(v, mKeyIndex);





}

void Table::find(std::string key, std::vector<std::vector<std::string>>& records) const
{
    data.find(key, mKeyIndex, records);

}
bool stringToDouble(string s, double& d)
{
    char* end;
    d = std::strtof(s.c_str(), &end);
    return end == s.c_str() + s.size() && !s.empty();
}

int Table::select(std::string query, std::vector<std::vector<std::string>>& records) const
{
    records.clear();


    vector<string> queryCheck;
    string s;

    StringParser parser(query);

    while (parser.getNextField(s))
    {
        queryCheck.push_back(s);
    }

    if (queryCheck.size() != 3)
    {
        return -1;
    }

    if (std::find(mColumns.begin(), mColumns.end(), queryCheck[0]) == mColumns.end())
    {
        return -1;
    }

    if (std::find(ALLOWED_OPERATORS.begin(), ALLOWED_OPERATORS.end(), queryCheck[1]) == ALLOWED_OPERATORS.end())
    {
        return -1;
    }

    if (std::find(NUMERICAL_OPERATORS.begin(), NUMERICAL_OPERATORS.end(), queryCheck[1]) != NUMERICAL_OPERATORS.end())
    {
        double d;
        if (!stringToDouble(queryCheck[2], d))
        {
            return -1;
        }
    }



    int columnIndex = -1;
    for (int i = 0; i < mColumns.size(); i++)
    {
        if (mColumns[i] == queryCheck[0])
        {
            columnIndex = i;
            break;
        }
    }

    if (columnIndex == -1)
    {
        return -1;
    }

    string operation = queryCheck[1];

    if ((std::find(NUMERICAL_OPERATORS.begin(), NUMERICAL_OPERATORS.end(), queryCheck[1]) != NUMERICAL_OPERATORS.end()))
    {
        double given;
        stringToDouble(queryCheck[2], given); // given now stores the double given to me for comparison
        return data.select(columnIndex, given, operation, records);
    }

    else
    {
        return data.select(columnIndex, queryCheck[2], operation, records);
    }
}





