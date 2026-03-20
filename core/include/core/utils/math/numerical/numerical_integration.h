#pragma once

#include <Eigen/Dense>

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>

/**
 * This header provides a variety of methods for solving ODEs depending on the
 * needs of the system. First, second, and fourth order methods are
 * provided, with first order being the fastest and least accurate, and fourth
 * order being the slowest but most accurate.
 *
 * Each method also provides a function for solving time-invariant ODEs with
 * and without an input u:
 * dx/dt = f(x, u)
 * dx/dt = f(x)
 * and time-variant ODEs:
 * dy/dt = f(t, y)
 *
 * Each function here takes a std::function as an input. This must be a vector
 * valued function where x is some Eigen::Vector<double, X>, u is some
 * Eigen::Vector<double, U>, and t is a double.
 *
 * The template arguments are determined by the compiler as long as they are
 * valid, so you do not need to explicity state them when calling a function.
 *
 * To learn about Runge-Kutta methods in general read:
 * https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
 *
 * To see the specific methods used here read:
 * https://en.wikipedia.org/wiki/List_of_Runge%E2%80%93Kutta_methods
 */

template <int X, int U>
using WithInputDerivative =
  std::function<Eigen::Vector<double, X>(const Eigen::Vector<double, X> &, const Eigen::Vector<double, U> &)>;

template <int X>
using WithoutInputDerivative = std::function<Eigen::Vector<double, X>(const Eigen::Vector<double, X> &)>;

template <int Y>
using TimeVariantDerivative = std::function<Eigen::Vector<double, Y>(const double &, const Eigen::Vector<double, Y> &)>;

/**
 * Performs first order numerical integration of the time-invariant differential
 * equation dx/dt = f(x, u) using Euler's method.
 *
 *   0|
 * ---|---
 *    |  1
 *
 * @param f The function to integrate, with two arguments x and u.
 * @param x The initial value of x.
 * @param u The input value u held constant over the integration period.
 * @param h The time over which to integrate.
 */
template <int X, int U>
Eigen::Vector<double, X> euler_with_input(
  const WithInputDerivative<X, U> &f, const Eigen::Vector<double, X> &x, const Eigen::Vector<double, U> &u,
  const double &h
) {
    Eigen::Vector<double, X> k1 = f(x, u);

    return x + h * k1;
}

/**
 * Performs first order numerical integration of the time-invariant differential
 * equation dx/dt = f(x) using Euler's method.
 *
 *   0|
 * ---|---
 *    |  1
 *
 * @param f The function to integrate, with one argument x.
 * @param x The initial value of x.
 * @param h The time over which to integrate.
 */
template <int X>
Eigen::Vector<double, X>
euler_without_input(const WithoutInputDerivative<X> &f, const Eigen::Vector<double, X> &x, const double &h) {
    Eigen::Vector<double, X> k1 = f(x);

    return x + h * k1;
}

/**
 * Performs first order numerical integration of the time-variant differential
 * equation dy/dt = f(t, y) using Euler's method.
 *
 *   0|
 * ---|---
 *    |  1
 *
 * @param f The function to integrate, with two arguments t and y.
 * @param t The initial value of t.
 * @param y The initial value of y.
 * @param h The time over which to integrate.
 */
template <int Y>
Eigen::Vector<double, Y> euler_time_variant(
  const TimeVariantDerivative<Y> &f, const double &t, const Eigen::Vector<double, Y> &y, const double &h
) {
    Eigen::Vector<double, Y> k1 = f(t, y);

    return y + h * k1;
}

/**
 * Performs second order numerical integration of the time-invariant differential
 * equation dx/dt = f(x, u) using the explicit midpoint method.
 *
 *   0|
 * 1/2|1/2
 * ---|-------
 *    |  0   1
 *
 * @param f The function to integrate, with one argument x.
 * @param x The initial value of x.
 * @param u The input value u held constant over the integration period.
 * @param h The time over which to integrate.
 */
template <int X, int U>
Eigen::Vector<double, X> RK2_with_input(
  const WithInputDerivative<X, U> &f, const Eigen::Vector<double, X> &x, const Eigen::Vector<double, U> &u,
  const double &h
) {
    Eigen::Vector<double, X> k1 = f(x, u);
    Eigen::Vector<double, X> k2 = f(x + h * 0.5 * k1, u);

    return x + h * k2;
}

/**
 * Performs second order numerical integration of the time-invariant differential
 * equation dx/dt = f(x) using the explicit midpoint method.
 *
 *   0|
 * 1/2|1/2
 * ---|-------
 *    |  0   1
 *
 * @param f The function to integrate, with one argument x.
 * @param x The initial value of x.
 * @param h The time over which to integrate.
 */
template <int X>
Eigen::Vector<double, X>
RK2_without_input(const WithoutInputDerivative<X> &f, const Eigen::Vector<double, X> &x, const double &h) {
    Eigen::Vector<double, X> k1 = f(x);
    Eigen::Vector<double, X> k2 = f(x + h * 0.5 * k1);

    return x + h * k2;
}

/**
 * Performs second order numerical integration of the time-variant differential
 * equation dy/dt = f(t, y) using the explicit midpoint method.
 *
 *   0|
 * 1/2|1/2
 * ---|-------
 *    |  0   1
 *
 * @param f The function to integrate, with two arguments t and y.
 * @param t The initial value of t.
 * @param y The initial value of y.
 * @param h The time over which to integrate.
 */
template <int Y>
Eigen::Vector<double, Y> RK2_time_variant(
  const TimeVariantDerivative<Y> &f, const double &t, const Eigen::Vector<double, Y> &y, const double &h
) {
    Eigen::Vector<double, Y> k1 = f(t, y);
    Eigen::Vector<double, Y> k2 = f(t + h * 0.5, y + h * 0.5 * k1);

    return y + h * k2;
}

/**
 * Performs fourth order numerical integration of the time-invariant differential
 * equation dx/dt = f(x, u) using the fourth order Runge-Kutta method.
 *
 *   0|
 * 1/2|1/2
 * 1/2|  0 1/2
 *   1|  0   0   1
 * ---|---------------
 *    |1/6 1/3 1/3 1/6
 *
 * @param f The function to integrate, with one argument x.
 * @param x The initial value of x.
 * @param u The input value u held constant over the integration period.
 * @param h The time over which to integrate.
 */
template <int X, int U>
Eigen::Vector<double, X> RK4_with_input(
  const WithInputDerivative<X, U> &f, const Eigen::Vector<double, X> &x, const Eigen::Vector<double, U> &u,
  const double &h
) {
    Eigen::Vector<double, X> k1 = f(x, u);
    Eigen::Vector<double, X> k2 = f(x + h * 0.5 * k1, u);
    Eigen::Vector<double, X> k3 = f(x + h * 0.5 * k2, u);
    Eigen::Vector<double, X> k4 = f(x + h * k3, u);

    return x + h / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
}

/**
 * Performs adaptive Dormand-Prince integration of the time-invariant
 * differential equation dx/dt = f(x, u).
 *
 * This is a 5(4) embedded Runge-Kutta pair where the 5th-order solution is
 * used as the step result and the 4th-order solution estimates local error.
 *
 * @param f The function to integrate, with arguments x and u.
 * @param x The initial value of x.
 * @param u The input value u held constant over the integration period.
 * @param dt The total time over which to integrate.
 * @param max_error Maximum allowed local truncation error norm per step.
 */
template <int X, int U>
Eigen::Vector<double, X> RKDP_with_input(
  const WithInputDerivative<X, U> &f, const Eigen::Vector<double, X> &x, const Eigen::Vector<double, U> &u,
  const double &dt, const double &max_error = 1e-6
) {
    if (dt <= 0.0) {
        return x;
    }

    // Dormand-Prince tableau.
    constexpr double a21 = 1.0 / 5.0;
    constexpr double a31 = 3.0 / 40.0;
    constexpr double a32 = 9.0 / 40.0;
    constexpr double a41 = 44.0 / 45.0;
    constexpr double a42 = -56.0 / 15.0;
    constexpr double a43 = 32.0 / 9.0;
    constexpr double a51 = 19372.0 / 6561.0;
    constexpr double a52 = -25360.0 / 2187.0;
    constexpr double a53 = 64448.0 / 6561.0;
    constexpr double a54 = -212.0 / 729.0;
    constexpr double a61 = 9017.0 / 3168.0;
    constexpr double a62 = -355.0 / 33.0;
    constexpr double a63 = 46732.0 / 5247.0;
    constexpr double a64 = 49.0 / 176.0;
    constexpr double a65 = -5103.0 / 18656.0;

    // 5th-order weights (step result).
    constexpr double b1 = 35.0 / 384.0;
    constexpr double b3 = 500.0 / 1113.0;
    constexpr double b4 = 125.0 / 192.0;
    constexpr double b5 = -2187.0 / 6784.0;
    constexpr double b6 = 11.0 / 84.0;

    // 4th-order weights (error estimate).
    constexpr double bs1 = 5179.0 / 57600.0;
    constexpr double bs3 = 7571.0 / 16695.0;
    constexpr double bs4 = 393.0 / 640.0;
    constexpr double bs5 = -92097.0 / 339200.0;
    constexpr double bs6 = 187.0 / 2100.0;
    constexpr double bs7 = 1.0 / 40.0;

    Eigen::Vector<double, X> state = x;
    double elapsed = 0.0;
    double h = dt;

    while (elapsed < dt) {
        double step = std::min(h, dt - elapsed);
        double truncation_error = std::numeric_limits<double>::infinity();
        Eigen::Vector<double, X> next_state = state;

        while (truncation_error > max_error) {
            const Eigen::Vector<double, X> k1 = f(state, u);
            const Eigen::Vector<double, X> k2 = f(state + step * (a21 * k1), u);
            const Eigen::Vector<double, X> k3 = f(state + step * (a31 * k1 + a32 * k2), u);
            const Eigen::Vector<double, X> k4 = f(state + step * (a41 * k1 + a42 * k2 + a43 * k3), u);
            const Eigen::Vector<double, X> k5 = f(state + step * (a51 * k1 + a52 * k2 + a53 * k3 + a54 * k4), u);
            const Eigen::Vector<double, X> k6 =
              f(state + step * (a61 * k1 + a62 * k2 + a63 * k3 + a64 * k4 + a65 * k5), u);

            next_state = state + step * (b1 * k1 + b3 * k3 + b4 * k4 + b5 * k5 + b6 * k6);
            const Eigen::Vector<double, X> k7 = f(next_state, u);

            const Eigen::Vector<double, X> error = step * ((b1 - bs1) * k1 + (b3 - bs3) * k3 + (b4 - bs4) * k4 +
                                                           (b5 - bs5) * k5 + (b6 - bs6) * k6 - bs7 * k7);

            truncation_error = error.norm();

            if (truncation_error == 0.0) {
                h = dt - elapsed;
                break;
            }

            const double scale = 0.9 * std::pow(max_error / truncation_error, 1.0 / 5.0);
            const double bounded_scale = std::max(0.2, std::min(5.0, scale));

            if (truncation_error > max_error) {
                step *= bounded_scale;
                if (step <= 1e-12) {
                    break;
                }
            } else {
                h = std::max(1e-6, step * bounded_scale);
            }
        }

        state = next_state;
        elapsed += step;
        if (h <= 1e-12) {
            h = std::max(1e-6, dt - elapsed);
        }
    }

    return state;
}

/**
 * Performs fourth order numerical integration of the time-invariant differential
 * equation dx/dt = f(x) using the fourth order Runge-Kutta method.
 *
 *   0|
 * 1/2|1/2
 * 1/2|  0 1/2
 *   1|  0   0   1
 * ---|---------------
 *    |1/6 1/3 1/3 1/6
 *
 * @param f The function to integrate, with one argument x.
 * @param x The initial value of x.
 * @param h The time over which to integrate.
 */
template <int X>
Eigen::Vector<double, X>
RK4_without_input(const WithoutInputDerivative<X> &f, const Eigen::Vector<double, X> &x, const double &h) {
    Eigen::Vector<double, X> k1 = f(x);
    Eigen::Vector<double, X> k2 = f(x + h * 0.5 * k1);
    Eigen::Vector<double, X> k3 = f(x + h * 0.5 * k2);
    Eigen::Vector<double, X> k4 = f(x + h * k3);

    return x + h / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
}

/**
 * Performs second order numerical integration of the time-variant differential
 * equation dy/dt = f(t, y) using the fourth order Runge-Kutta method.
 *
 *   0|
 * 1/2|1/2
 * 1/2|  0 1/2
 *   1|  0   0   1
 * ---|---------------
 *    |1/6 1/3 1/3 1/6
 *
 * @param f The function to integrate, with two arguments t and y.
 * @param t The initial value of t.
 * @param y The initial value of y.
 * @param h The time over which to integrate.
 */
template <int Y>
Eigen::Vector<double, Y> RK4_time_variant(
  const TimeVariantDerivative<Y> &f, const double &t, const Eigen::Vector<double, Y> &y, const double &h
) {
    Eigen::Vector<double, Y> k1 = f(t, y);
    Eigen::Vector<double, Y> k2 = f(t + h * 0.5, y + h * 0.5 * k1);
    Eigen::Vector<double, Y> k3 = f(t + h * 0.5, y + h * 0.5 * k2);
    Eigen::Vector<double, Y> k4 = f(t + h, y + h * k3);

    return y + h / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
}
