//
//  Table.cpp
//  Project 4
//
//  Created by Benjamin Goldblatt on 6/3/21.
//

#include "Table.h"
#include <iostream>
#include <functional>
#include <string>

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
        size_t end = m_text.find_first_of(" \t\r\n", m_start+1);
        fieldText = m_text.substr(m_start, end-m_start);
        m_start = end;
        return true;
    }
    fieldText = "";
    for (;;)
    {
        m_start++;
        size_t end = m_text.find('\'', m_start);
        fieldText += m_text.substr(m_start, end-m_start);
        m_start = (end != std::string::npos ? end+1 : m_text.size());
        if (m_start == m_text.size()  ||  m_text[m_start] != '\'')
            break;
        fieldText += '\'';
    }
    return true;
}



Table::Table(std::string keyColumn, const std::vector<std::string>& columns) {
    cols = columns;
    auto index = std::find(cols.begin(), cols.end(), keyColumn); //finding where key string is in col
    keycol = (int)(index - cols.begin());
    for (int i = 0; i < len; i++) { //initialize each table vector, these are the buckets of hash table
        std::list<Record> li {};
        table.push_back(li);
    }
}

Table::~Table() {
    
}

bool Table::good() const {
    if (keycol < 0 || keycol>= cols.size()) //if key not in columns range table is not good
        return false;
    std::vector<std::string> test; //separate vector that tests for duplicates
    for (int i = 0; i < cols.size(); i++) {
        if (cols[i] == "")
            return false;
        if (!std::count(test.begin(), test.end(), cols[i]))
            test.push_back(cols[i]); //if column element already in test set it will not be inserted again
    }
    if (test.size() != cols.size()) //if test has less elements, columns has duplicates
        return false;
    return true;
}

int hash(std::string key, int len) {
    unsigned long h = std::hash<std::string>()(key); //hash function from functional library
    return h%len; //makes a hash within the bounds of the number of buckets
}


bool Table::insert(const std::string& recordString) {
    StringParser parser(recordString); //this part processes the string with the table contents
    std::string s;
    std::vector<std::string> values;
    while (parser.getNextField(s)) {
        values.push_back(s); //first string will be first column element, second will be second, push_back orders values correctly
    }
    if (values.size() != cols.size()) //if new values size less than cols, invalid
        return false;
    int bucket = hash(values[keycol], len); //get bucket from hash function
    Record tmp(values); //create instance of record
    table[bucket].push_back(tmp); //push back new record in list at bucket index in vector
    return true;
}

void Table::find(std::string key, std::vector<std::vector<std::string>>& records) const {
    std::vector<std::vector<std::string>> recs;
    int bucket = hash(key, len); //get bucket that key element would be in
    std::list<Record> li = table[bucket]; //get list at bucket index
    while(!li.empty()) {
        Record record = li.back(); //get record from list
        li.pop_back();
        if (record.values[keycol] == key) //in case of collisions, still need to check whether record has same key
            recs.push_back(record.values);
    }
    records = recs; //return all records with same key
}

bool stringToDouble(std::string s, double& d)
{
    char* end;
    d = std::strtof(s.c_str(), &end);
    return end == s.c_str() + s.size()  &&  !s.empty();
}

int Table::select(std::string query, std::vector<std::vector<std::string>>& records) const {
    std::vector<std::vector<std::string>> recs;
    int count = 0; //this int tracks number of records that don't satisfy int requirement for numerical comparisons
    StringParser parser(query);
    std::string s;
    if (!parser.getNextField(s)) {
        records.clear();
        return -1;
    }
    std::string column = s; //first word in query is the column that will be compared
    double c;
    if (!parser.getNextField(s)) {
        records.clear();
        return -1;
    }
    std::string oper = s; //second field is the operator
    if (!parser.getNextField(s)) {
        records.clear();
        return -1;
    }
    std::string value = s; //third field is the value the column will be compared against
    double val = -1;
    
    if (parser.getNextField(s)) { //if more words, then query string is invalid
        records.clear();
        return -1;
    }
    
    auto index = std::find(cols.begin(), cols.end(), column);
    if (index == cols.end()) { //this suggests column is not actually in cols
        records.clear();
        return -1;
    }
    int ind = (int)(index - cols.begin()); //get index where query column is in cols
    
    for (auto & c: oper) c = toupper(c); //make operator uppercase
    if (oper == "LT" || oper == "LE" || oper == "GT" || oper == "GE" || oper == "NE" || oper == "EQ") {
        if (!stringToDouble(value, val)) { //if oper is numerical comparator, value must be int, this tests whether value is int
            records.clear();
            return -1;
        }
    }
    
    for (int i = 0; i < len; i++) { //iterates through each bucket in hash table
        std::list<Record> li = table[i]; //get list of records in bucket
        while(!li.empty()) {
            Record record = li.back();
            if (oper == "<") {if (record.values[ind] < value) recs.push_back(record.values);} //if element in record fulfills query, add it to records vector
            else if (oper == "<=") {if (record.values[ind] <= value) recs.push_back(record.values);}
            else if (oper == ">") {if (record.values[ind] > value) recs.push_back(record.values);}
            else if (oper == ">=") {if (record.values[ind] >= value) recs.push_back(record.values);}
            else if (oper == "!=") {if (record.values[ind] != value) recs.push_back(record.values);}
            else if (oper == "=" || oper == "==") {if (record.values[ind] == value) recs.push_back(record.values);}
            //the next statements deal with numerical operators
            //first if statement checks whether record element is int and compares this int to value in query string
            else if (oper == "LT") {if (stringToDouble(record.values[ind], c) && c < val) recs.push_back(record.values);
                if(!stringToDouble(record.values[ind], c)) count++;}
            else if (oper == "LE") {if (stringToDouble(record.values[ind], c) && c <= val) recs.push_back(record.values);
                if(!stringToDouble(record.values[ind], c)) count++;} //if records has value at col that is not valid, increment count
            else if (oper == "GT") {if (stringToDouble(record.values[ind], c) && c > val) recs.push_back(record.values);
                if(!stringToDouble(record.values[ind], c)) count++;}
            else if (oper == "GE") {if (stringToDouble(record.values[ind], c) && c >= val) recs.push_back(record.values);
                if(!stringToDouble(record.values[ind], c)) count++;}
            else if (oper == "NE") {if (stringToDouble(record.values[ind], c) && c != val) recs.push_back(record.values);
                if(!stringToDouble(record.values[ind], c)) count++;}
            else if (oper == "EQ") {if (stringToDouble(record.values[ind], c) && c == val) recs.push_back(record.values);
                if(!stringToDouble(record.values[ind], c)) count++;}
            else { //oper not valid operator
                records.clear();
                return -1;
            }
            li.pop_back(); //shrink list
        }
    }
    records = recs;
    return count; //return number of invalid records for numerical comparisons, if none return 0
}
