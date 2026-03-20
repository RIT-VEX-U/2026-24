#pragma once

#include "core/utils/math/eigen_interface.h"

#include <cmath>
#include <iostream>
#include <vector>

/**
 * Class representing a difference between two poses,
 * more specifically a distance along an arc from a pose.
 *
 * Assumes conventional cartesian coordinate system:
 * Looking down at the coordinate plane,
 * +X is right
 * +Y is up
 * +Theta is counterclockwise
 */
class Twist2d {
  public:
    /**
     * Default Constructor for Twist2d
     */
    constexpr Twist2d();

    /**
     * Constructs a twist with given translation and angle deltas.
     *
     * @param dx the linear dx component.
     * @param dy the linear dy component.
     * @param dtheta the angular dtheta component.
     */
    Twist2d(const double &dx, const double &dy, const double &dtheta);

    /**
     * Constructs a twist with given translation and angle deltas.
     *
     * @param twist_vector vector of the form [dx, dy, dtheta]
     */
    Twist2d(const EVec<3> &twist_vector);

    /**
     * Returns the linear dx component.
     *
     * @return the linear dx component.
     */
    double dx() const;

    /**
     * Returns the linear dy component.
     *
     * @return the linear dy component.
     */
    double dy() const;

    /**
     * Returns the angular dtheta component.
     *
     * @return the angular dtheta component.
     */
    double dtheta() const;

    /**
     * Compares this to another twist.
     *
     * @param other the other twist to compare to.
     *
     * @return true if each of the components are within 1e-9 of each other.
     */
    bool operator==(const Twist2d &other) const;

    /**
     * Multiplies this twist by a scalar.
     *
     * @param scalar the scalar value to multiply by.
     */
    Twist2d operator*(const double &scalar) const;

    /**
     * Divides this twist by a scalar.
     *
     * @param scalar the scalar value to divide by.
     */
    Twist2d operator/(const double &scalar) const;

    /**
     * Sends a twist to an output stream.
     * Ex.
     * std::cout << twist;
     *
     * prints "Twist2d[dx: (value), dy: (value), drad: (radians)]"
     */
    friend std::ostream &operator<<(std::ostream &os, const Twist2d &twist);

  private:
    double m_dx;
    double m_dy;
    double m_dtheta;
};

/**
 * Default Constructor for Twist2d
 */
constexpr Twist2d::Twist2d() : m_dx(0), m_dy(0), m_dtheta(0) {}

/**
 * Constructs a twist with given translation and angle deltas.
 *
 * @param dx the linear dx component.
 * @param dy the linear dy component.
 * @param dtheta the angular dtheta component.
 */
inline Twist2d::Twist2d(const double &dx, const double &dy, const double &dtheta) : m_dx{dx}, m_dy{dy}, m_dtheta{dtheta} {}

/**
 * Constructs a twist with given translation and angle deltas.
 *
 * @param twist_vector vector of the form [dx, dy, dtheta]
 */
inline Twist2d::Twist2d(const EVec<3> &twist_vector)
    : m_dx{twist_vector(0)}, m_dy{twist_vector(1)}, m_dtheta{twist_vector(2)} {}

/**
 * Returns the linear dx component.
 *
 * @return the linear dx component.
 */
inline double Twist2d::dx() const { return m_dx; }

/**
 * Returns the linear dy component.
 *
 * @return the linear dy component.
 */
inline double Twist2d::dy() const { return m_dy; }

/**
 * Returns the angular dtheta component.
 *
 * @return the angular dtheta component.
 */
inline double Twist2d::dtheta() const { return m_dtheta; }

/**
 * Compares this to another twist.
 *
 * @param other the other twist to compare to.
 *
 * @return true if each of the components are within 1e-9 of each other.
 */
inline bool Twist2d::operator==(const Twist2d &other) const {
    return std::abs(dx() - other.dx()) < 1e-9 && std::abs(dy() - other.dy()) < 1e-9 &&
           std::abs(dtheta() - other.dtheta()) < 1e-9;
}

/**
 * Multiplies this twist by a scalar.
 *
 * @param scalar the scalar value to multiply by.
 */
inline Twist2d Twist2d::operator*(const double &scalar) const {
    return Twist2d{dx() * scalar, dy() * scalar, dtheta() * scalar};
}

/**
 * Divides this twist by a scalar.
 *
 * @param scalar the scalar value to divide by.
 */
inline Twist2d Twist2d::operator/(const double &scalar) const { return *this * (1. / scalar); }

/**
 * Sends a twist to an output stream.
 * Ex.
 * std::cout << twist;
 *
 * prints "Twist2d[x: (value), y: (value), rad: (radians), deg: (degrees)]"
 */
inline std::ostream &operator<<(std::ostream &os, const Twist2d &twist) {
    os << "Twist2d[dx: " << twist.dx() << ", dy: " << twist.dy() << ", dtheta: " << twist.dtheta() << "]";
    return os;
}
