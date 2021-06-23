
//          Copyright Daniel Hoegee 2020 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef V1OOPC_EXAMPLES_CLASS_XYZ_HPP
#define V1OOPC_EXAMPLES_CLASS_XYZ_HPP

///\brief
/// Class to easily store the xyz values
///\details
/// This class stores the xyz values and has a couple of custom operators
class xyz {
public:
    int16_t x;
    int16_t y;
    int16_t z;

    xyz(int16_t x,int16_t y, int16_t z) : x(x), y(y), z(z) {};

    xyz &operator+=(const xyz &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    xyz operator/(const xyz &rhs) const {
        return xyz(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    xyz operator/(const int16_t &rhs) const {
        return xyz(x / rhs, y / rhs, z / rhs);
    }

    xyz &operator=(const xyz &rhs) = default;

    xyz &operator=(const int16_t &rhs) {
        x = rhs;
        y = rhs;
        z = rhs;
        return *this;
    }
};


#endif //V1OOPC_EXAMPLES_CLASS_XYZ_HPP
