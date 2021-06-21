#ifndef V1OOPC_EXAMPLES_CLASS_XYZ_HPP
#define V1OOPC_EXAMPLES_CLASS_XYZ_HPP

class xyz {
public:
    int16_t x, y, z;

    xyz(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {};

    xyz &operator+=(const xyz &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    xyz operator/(const xyz &rhs) const {
        return xyz(x/rhs.x, y/rhs.y,z/rhs.z);
    }
    xyz operator/(const int &rhs) const {
        return xyz(x/rhs, y/rhs,z/rhs);
    }

    xyz & operator=(const xyz &rhs)  = default;
    xyz & operator=(const int &rhs)  {
        x = rhs;
        y = rhs;
        z = rhs;
        return *this;
    }
};
#endif //V1OOPC_EXAMPLES_CLASS_XYZ_HPP
