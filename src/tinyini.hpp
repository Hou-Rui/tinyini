#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <exception>

namespace tinyini
{

class Section;
class Parser;

typedef std::map<std::string, std::string> DataMap;
typedef std::map<std::string, Section *> ChildrenMap;
typedef std::pair<std::string, std::string> DataNode;

namespace util
{
std::string trim(const std::string &str);
DataNode split(const std::string &str, char delim);
std::vector<std::string> seqsplit(const std::string &str, char delim);
std::string unbrace(const std::string &str, char left, char right);
} // namespace util

class BaseError : public std::exception
{
  public:
    void error(std::string message) { _msg = message; }
    virtual const char *what() { return _msg.c_str(); }
  protected:
    std::string _msg;
};

struct KeyError : public BaseError
{ KeyError(std::string key); };

struct SyntaxError : public BaseError
{ };

struct ExpectFailedError : public SyntaxError
{ ExpectFailedError(char expect); };

class Section
{
  public:
    bool hasKey(const std::string &key) const;
    bool hasSection(const std::string &name) const;
    std::string &value(const std::string &key);
    int valueAsInt(const std::string &key);
    Section &child(const std::string &name);
    std::string &operator[](const std::string &key);
    Section &operator()(const std::string &name);
    void write(std::ostream &out);
    void writeToFile(const std::string &file_name);
    void writeToString(std::string &str);
    ~Section();

  private:
    friend class Parser;
    DataMap _data;
    ChildrenMap _children;
    void dealloc();
};

class Parser
{
  public:
    Section parse(std::istream &stream);
    Section parseFile(const std::string &file_name);
    Section parseString(const std::string &str);
    static Section sectionFromFile(const std::string &file_name);
    static Section sectionFromString(const std::string &str);
};

} // namespace tinyini
