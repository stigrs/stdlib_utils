// Copyright (c) 2018 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef STDUTILS_STREAM_H
#define STDUTILS_STREAM_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace Stdutils {

//------------------------------------------------------------------------------
//
// Stream utility methods:

// Wrapper function for opening file stream for input.
inline void fopen(std::ifstream& from,
                  const std::string& filename,
                  std::ios_base::openmode mode = std::ios_base::in)
{
    from.open(filename.c_str(), mode);
    if (!from.is_open()) {
        throw std::runtime_error("cannot open " + filename);
    }
}

// Wrapper function for opening file stream for output.
inline void fopen(std::ofstream& to,
                  const std::string& filename,
                  std::ios_base::openmode mode = std::ios_base::out)
{
    to.open(filename.c_str(), mode);
    if (!to.is_open()) {
        throw std::runtime_error("cannot open " + filename);
    }
}

// Find section in input stream.
inline int find_section(std::istream& from, std::string key)
{
    int pos = 0;
    std::string buf;
    while (from >> buf) {
        if (buf == key) {
            pos = from.tellg();
            break;
        }
    }
    return pos;
}

// Get token value from input stream.
template <typename T>
inline void get_token_value(std::istream& from,
                            std::string token,
                            T& value,
                            const T& def = T{},
                            int pos)
{
    value = def;     // assign default value
    from.seekg(pos); // search from starting pos

    std::string buf;
    while (from >> buf) {
        if (buf == token) {
            from >> value;
            break;
        }
    }
}

// Lexical cast (type must be able to stream into and/or out of a string).
template <typename Target, typename Source>
inline Target lexical_cast(Source arg)
{
    std::stringstream interpreter;
    Target result;

    if (!(interpreter << arg) || !(interpreter >> result) ||
        !(interpreter >> std::ws).eof()) { // stuff left in stream?
        throw std::runtime_error("bad lexical cast");
    }
    return result;
}

} // namespace Stdutils

#endif // STDUTILS_STREAM_H
