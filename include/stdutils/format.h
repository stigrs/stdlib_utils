// Copyright (c) 2018 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef STDUTILS_FORMAT_H
#define STDUTILS_FORMAT_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>

namespace Stdutils {

//------------------------------------------------------------------------------
//
// Stream format methods:

template <typename T>
struct Bound_form;

template <typename T>
std::ostream& operator<<(std::ostream& os, const Bound_form<T>& bf);

// Stroustrup's format class from TC++PL (slightly modified).
template <typename T>
class Format {
public:
    explicit Format(int p = 6)
        : prc(p), wdt(0), ch(' '), fmt(std::ios_base::fmtflags(0))
    {
    }

    Bound_form<T> operator()(T vv) const { return Bound_form<T>(*this, vv); }

    // Set width.
    Format<T>& width(std::size_t w)
    {
        wdt = w;
        return *this;
    }

    // Set fill character.
    Format<T>& fill(char c)
    {
        ch = c;
        return *this;
    }

    // Set precision.
    Format<T>& precision(int p)
    {
        prc = p;
        return *this;
    }

    // Set general format.
    Format<T>& general()
    {
        fmt = std::ios_base::fmtflags(0);
        return *this;
    }

    // Set fixed floating point format.
    Format<T>& fixed()
    {
        fmt = std::ios_base::fixed;
        return *this;
    }

    // Set scientific floating point format using e character.
    Format<T>& scientific()
    {
        fmt = std::ios_base::scientific;
        return *this;
    }

    // Set scientific floating point format using E character.
    Format<T>& scientific_E()
    {
        fmt = std::ios_base::scientific | std::ios_base::uppercase;
        return *this;
    }

private:
    int prc;         // precision, default precision is 6
    std::size_t wdt; // width, 0 means as wide as necessary
    char ch;         // fill character, default is blank

    std::ios_base::fmtflags fmt; // format flag value

    friend std::ostream& operator<<<>(std::ostream&, const Bound_form<T>&);
};

// Form plus value.
template <typename T>
struct Bound_form {
    const Format<T>& f;

    T v;

    Bound_form(const Format<T>& ff, T vv) : f(ff), v(vv) {}

private:
    Bound_form& operator=(const Bound_form&) = delete;
};

template <typename T>
std::ostream& operator<<(std::ostream& to, const Bound_form<T>& bf)
{
    std::ostringstream s;

    s.width(bf.f.wdt);
    s.precision(bf.f.prc);
    s.fill(bf.f.ch);

    s << std::setiosflags(bf.f.fmt) << bf.v;
    return to << s.str();
}

//------------------------------------------------------------------------------
//
// Time format methods:

template <typename Duration, typename Tfirst, typename... Trest>
std::string fmt_duration(Duration d)
{
    auto val = std::chrono::duration_cast<Tfirst>(d);
    std::string res = std::to_string(val.count());

    if (/* constexpr */ sizeof...(Trest) > 0) {
        res += ":" + fmt_duration<Duration, Trest...>(d - val);
    }
    return res;
}

template <typename Duration>
std::string fmt_duration(Duration d)
{
    return {};
}

} // namespace Stdutils

#endif // STDUTILS_FORMAT_H
