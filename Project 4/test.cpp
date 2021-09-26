#include "Table.h"
#include <iostream>
#include <cassert>
using namespace std;
 
void testConstructor()
{
    string key = "a";
    vector<string> a1;
    a1.push_back("a");
    assert(a1.size() == 1);
    Table t1(key, a1);
    assert(t1.good());
    key = "";
    Table t2(key, a1);
    assert(!t2.good());
    a1.clear();
    Table t3(key, a1);
    assert(!t3.good());
    key = "bob";
    Table t4(key, a1);
    assert(!t4.good());
    a1.push_back("bob");
    Table t5(key, a1);
    assert(t5.good());
    a1.push_back("");
    Table t6(key, a1);
    assert(!t6.good());
    a1.clear();
    a1.push_back("bob");
    a1.push_back("bob");
    Table t7(key, a1);
    assert(!t7.good());
    key = "aaa";
    Table t8(key, a1);
    assert(!t8.good());
}
 
void testInsert(){
    string key = "name";
    vector<string> a1;
    a1.push_back("name");
    a1.push_back("age");
    Table t(key, a1);
    assert(t.good());
    assert(t.insert("Anna 12"));
    assert(t.insert("Anna ''"));
    assert(!t.insert("Bob"));
    assert(!t.insert(""));
    assert(!t.insert("A B C"));
}
 
void testFind(){
    string key = "name";
    vector<string> a1;
    a1.push_back("name");
    a1.push_back("age");
    Table t(key, a1);
    assert(t.good());
    assert(t.insert("Anna 12"));
    assert(t.insert("Anna ''"));
    t.insert("Sam 18");
    
    vector<vector<string>> records;
    t.find("Anna", records);
    assert(records.size() == 2);
 
    assert(records[0][0] == "Anna");
    assert(records[1][0] == "Anna");
    assert(records[1][1] == "");
    
    t.find("Bob", records);
    assert(records.size() == 0);
    
    t.find("", records);
    assert(records.size() == 0);
    
    t.find("Sam", records);
    assert(records.size() == 1);
    
    t.insert("'' S");
    t.find("", records);
    assert(records.size() == 1);
}
 
void testSelect(){
    string key = "name";
    vector<string> a1;
    a1.push_back("name");
    a1.push_back("age");
    Table t(key, a1);
    assert(t.good());
    assert(t.insert("Anna 12"));
    assert(t.insert("Anna ''"));
    t.insert("Sam 18");
    
    vector<vector<string>> records;
    assert(t.select("name < A", records) == 0);
    assert(records.size() == 0);
    assert(t.select("name <= A", records) == 0);
    assert(records.size() == 0);
    assert(t.select("name > Abe", records) == 0);
    assert(records.size() == 3);
    assert(t.select("name >= Anna", records) == 0);
    assert(records.size() == 3);
    assert(t.select("name != Anna", records) == 0);
    assert(records.size() == 1);
    assert(t.select("name == Sam", records) == 0);
    assert(records.size() == 1);
    
    assert(t.select("age LT 12", records) == 1);
    assert(records.size() == 0);
    assert(t.select("age LE 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age GT 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age GE 12", records) == 1);
    cerr << records.size() << endl;
    assert(records.size() == 2);
    assert(t.select("age NE 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age EQ 12", records) == 1);
    assert(records.size() == 1);
    
    assert(t.select("age lt 12", records) == 1);
    assert(records.size() == 0);
    assert(t.select("age le 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age gt 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age ge 12", records) == 1);
    assert(records.size() == 2);
    assert(t.select("age ne 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age eq 12", records) == 1);
    assert(records.size() == 1);
    
    assert(t.select("age Lt 12", records) == 1);
    assert(records.size() == 0);
    assert(t.select("age Le 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age Gt 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age Ge 12", records) == 1);
    assert(records.size() == 2);
    assert(t.select("age Ne 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age Eq 12", records) == 1);
    assert(records.size() == 1);
    
    assert(t.select("age lT 12", records) == 1);
    assert(records.size() == 0);
    assert(t.select("age lE 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age gT 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age gE 12", records) == 1);
    assert(records.size() == 2);
    assert(t.select("age nE 12", records) == 1);
    assert(records.size() == 1);
    assert(t.select("age eQ 12", records) == 1);
    assert(records.size() == 1);
    
    //bad queries
    assert(t.select("age eq", records) == -1);
    assert(t.select("age", records) == -1);
    assert(t.select("age eq sdf sd", records) == -1);
    assert(t.select("", records) == -1);
    assert(t.select("age is 12", records) == -1);
    assert(t.select("location > Marina", records) == -1);
    assert(t.select("   ", records) == -1);
    assert(t.select("age eq BOB", records) == -1);
}
 
int main()
{
    testFind();
    testInsert();
    testConstructor();
    testSelect();
 
    cout << "all passed" << endl;
}
