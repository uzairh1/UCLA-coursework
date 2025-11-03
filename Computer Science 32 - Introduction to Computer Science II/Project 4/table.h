#ifndef TABLE_H
#define TABLE_H

#include <string>
using std::string;
#include <vector>
#include <functional>

bool stringToDouble(string s, double& d);



// HashTable declaration and definition
class HashTable {
public:
    HashTable(unsigned int initialSize) : m_numberOfBuckets(initialSize)
    {

    }

    bool insert(std::vector<std::string>& fields, int keyFieldIndex)
    {

        unsigned int h = std::hash<std::string>()(fields[keyFieldIndex]) % m_numberOfBuckets;
        m_Buckets[h].push_back(fields);
        return true;
    }

    void find(std::string key, int keyFieldIndex, std::vector<std::vector<std::string>>& records) const
    {
        unsigned int rehash = std::hash<std::string>()(key) % m_numberOfBuckets;
        const std::vector<std::vector<std::string>>& bucket = m_Buckets[rehash];
        std::vector<std::vector<std::string>>::const_iterator it = bucket.begin();
        while (it != bucket.end())
        {
            std::vector<std::string> currFields = *it;
            if (currFields[keyFieldIndex] == key)
            {
                records.push_back(currFields);
                it++;
            }
        }
    }

    int select(int columnIndex, double given, std::string operation, std::vector<std::vector<std::string>>& records) const // will overload for string comparison
    {
        int count = 0;

        if (operation == "LT" || operation == "Lt" || operation == "lT" || operation == "lt")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    double d;



                    if (!stringToDouble(currFields[columnIndex], d))
                    {
                        count++;
                        it++;
                        continue;
                    }

                    if (d < given)
                    {
                        records.push_back(currFields);
                    }


                    it++;


                }


            }
        }

        if (operation == "LE" || operation == "Le" || operation == "lE" || operation == "le")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    double d;


                    if (!stringToDouble(currFields[columnIndex], d))
                    {
                        count++;
                        continue;
                    }
                    if (d <= given)
                    {
                        records.push_back(currFields);
                    }


                    it++;


                }


            }
        }


        if (operation == "GT" || operation == "Gt" || operation == "gT" || operation == "gt")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    double d;

                    if (!stringToDouble(currFields[columnIndex], d))
                    {
                        count++;
                        continue;
                    }
                    if (d > given)
                    {
                        records.push_back(currFields);
                    }


                    it++;


                }


            }
        }


        if (operation == "GE" || operation == "Ge" || operation == "gE" || operation == "ge")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    double d;

                    if (!stringToDouble(currFields[columnIndex], d))
                    {
                        count++;
                        continue;
                    }
                    if (d >= given)
                    {
                        records.push_back(currFields);
                    }


                    it++;


                }


            }
        }

        if (operation == "NE" || operation == "Ne" || operation == "nE" || operation == "ne")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    double d;

                    if (!stringToDouble(currFields[columnIndex], d))
                    {
                        count++;
                        continue;
                    }
                    if (d != given)
                    {
                        records.push_back(currFields);
                    }


                    it++;


                }


            }
        }

        if (operation == "EQ" || operation == "Eq" || operation == "eQ" || operation == "eq")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    double d;

                    if (!stringToDouble(currFields[columnIndex], d))
                    {
                        count++;
                        continue;
                    }
                    if (d == given)
                    {
                        records.push_back(currFields);
                    }


                    it++;


                }


            }
        }
        return count;
    }

    int select(int columnIndex, std::string given, std::string operation, std::vector<std::vector<std::string>>& records) const
    {
        if (operation == "<")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    if (currFields[columnIndex] < given)
                    {
                        records.push_back(currFields);
                    }
                    ++it;
                }
            }
        }

        else if (operation == "<=")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    if (currFields[columnIndex] <= given)
                    {
                        records.push_back(currFields);
                    }
                    ++it;
                }
            }
        }

        else if (operation == ">")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    if (currFields[columnIndex] > given)
                    {
                        records.push_back(currFields);
                    }
                    ++it;
                }
            }
        }

        else if (operation == ">=")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    if (currFields[columnIndex] >= given)
                    {
                        records.push_back(currFields);
                    }
                    ++it;
                }
            }
        }

        else if (operation == "!=")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    if (currFields[columnIndex] != given)
                    {
                        records.push_back(currFields);
                    }
                    ++it;
                }
            }
        }

        else  if (operation == "==" || operation == "=")
        {
            for (unsigned int i = 0; i < m_numberOfBuckets; i++)
            {
                std::vector<std::vector<std::string>>::const_iterator it = m_Buckets[i].begin();
                while (it != m_Buckets[i].end())
                {
                    std::vector<std::string> currFields = *it;
                    if (currFields[columnIndex] == given)
                    {
                        records.push_back(currFields);
                    }
                    ++it;
                }
            }
        }


        return 0;
    }


private:
    unsigned int m_numberOfBuckets;
    std::vector<std::vector<std::string>> m_Buckets[1000];
    // each bucket holds VECTORS of fields....
    // I want the keyField bucket to hold a vector of whatever is in a given RECORD
    // i.e., if we're hashing on customer name, then for this example:
    // " Patel 12345 42.54 Westwood" (the RECORD is parsed into a vector of strings by Table::insert w/ the help of StringParser class.)
    // the Patel bucket should hold: "Patel 12345 42.54 Westwood"


};

class Table {
public:
    Table(std::string keyColumn, const std::vector<std::string>& columns);
    ~Table();
    bool good() const;
    bool insert(const std::string& recordString);
    void find(std::string key, std::vector<std::vector<std::string>>& records) const;
    int select(std::string query, std::vector<std::vector<std::string>>& records) const;

    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;

private:
    HashTable data;
    std::string keyField;
    int mKeyIndex;
    std::vector<std::string> mColumns;
}; // Added missing closing brace and semicolon


#endif