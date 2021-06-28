
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

    ///constructor
    xyz(int16_t x,int16_t y, int16_t z) : x(x), y(y), z(z) {};

    ///default constructor (sets all to 0)
    xyz(): x{0},y{0},z{0} {}

    ///adds rhs to lhs
    xyz &operator+=(const xyz &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    ///devide lhs bij rhs
    xyz operator/(const xyz &rhs) const {
        return xyz(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    ///devide lhs bij integer
    xyz operator/(const int16_t &rhs) const {
        return xyz(x / rhs, y / rhs, z / rhs);
    }

    ///Sets lhs to default
    xyz &operator=(const xyz &rhs) = default;

    ///Sets lhs to integer
    xyz &operator=(const int16_t &rhs) {
        x = rhs;
        y = rhs;
        z = rhs;
        return *this;
    }
};


#endif //V1OOPC_EXAMPLES_CLASS_XYZ_HPP
