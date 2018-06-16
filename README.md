# tinyini

Tinyini is a tiny cross-platform INI parser.

Usage:
1. Copy "src/tinyini.cpp" and "src/tinyini.hpp" to your project.
2. Enjoy!

# description

An INI document is organized as a tree structure of sections and key-value pairs.
The whole document is considered as a section containing several child sections and
key-value pairs.

Child sections can be accessed using operator(), and key-value pairs using operator[].
By default, the values are considered as std::string.

