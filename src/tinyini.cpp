#include "tinyini.hpp"

namespace tinyini
{

// Exceptions
KeyError::KeyError(std::string key)
{
    std::stringstream ss;
    ss << "unknown key: " << key;
    error(ss.str());
}

ExpectFailedError::ExpectFailedError(char expect)
{
    std::stringstream ss;
    ss << "expected " << expect;
    error(ss.str());
}


// string utilities
bool notBlank(char c)
{
    return !std::isblank(c);
}

std::string util::trim(const std::string &str)
{
    auto right = std::find_if(str.rbegin(), str.rend(), notBlank).base();
    auto left = std::find_if(str.begin(), right, notBlank);
    return std::string(left, right);
}

DataNode util::split(const std::string &str, char delim)
{
    int cnt = str.find_first_of(delim);
    return std::make_pair(str.substr(0, cnt), str.substr(cnt + 1));
}

std::string util::unbrace(const std::string &str, char left, char right)
{
    if (str[0] != left) throw ExpectFailedError(left);
    if (str[str.size() - 1] != right) throw ExpectFailedError(right);
    return str.substr(1, str.size() - 2);
}

// class Section
bool Section::hasKey(const std::string &key) const
{
    return _data.count(key) > 0;
}

bool Section::hasSection(const std::string &name) const
{
    return _children.count(name) > 0;
}

std::string &Section::value(const std::string &key)
{
    if (hasKey(key))
        return _data[key];
    else
        throw KeyError(key);
}

int Section::valueAsInt(const std::string &key)
{
    int result;
    std::stringstream ss;
    ss << value(key);
    ss >> result;
    return result;
}

Section &Section::child(const std::string &name)
{
    if (hasSection(name))
        return *_children[name];
    else
        throw KeyError(name);
}

std::string &Section::operator[](const std::string &key)
{
    return value(key);
}

Section &Section::operator()(const std::string &name)
{
    return child(name);
}

void Section::write(std::ostream &out)
{
    for (auto it : _data)
        out << it.first << "=" << it.second << std::endl;
    for (auto it : _children)
    {
        out << "[" << it.first << "]" << std::endl;
        it.second->write(out);
    }
}

void Section::writeToFile(const std::string &file_name)
{
    std::ofstream fout(file_name);
    write(fout);
}

void Section::writeToString(std::string &str)
{
    std::stringstream ss;
    write(ss);
    ss >> str;
}

void Section::dealloc()
{
    for (auto it : _children)
        delete it.second;
}

Section::~Section()
{
    dealloc();
}

// class Parser
Section Parser::parse(std::istream &stream)
{
    Section root;
    auto current_child = &root;
    std::string line;
    while (std::getline(stream, line))
    {
        line = util::trim(line);
        if (line.empty() || line[0] == ';')
            continue;
        else if (line[0] == '[')
        {
            auto section_name = util::unbrace(line, '[', ']');
            current_child = root._children[section_name] = new Section();
        }
        else
        {
            auto node = util::split(line, '=');
            current_child->_data[node.first] = node.second;
        }
    }
    return root;
}

Section Parser::parseFile(const std::string &file_name)
{
    std::ifstream fin(file_name);
    auto section = parse(fin);
    fin.close();
    return section;
}

Section Parser::parseString(const std::string &str)
{
    std::stringstream ss(str);
    auto section = parse(ss);
    return section;
}

Section Parser::sectionFromFile(const std::string &file_name)
{
    Parser parser;
    return parser.parseFile(file_name);
}

Section Parser::sectionFromString(const std::string &str)
{
    Parser parser;
    return parser.parseString(str);
}

} // namespace tinyini