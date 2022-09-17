#ifndef MATH_H_
#define MATH_H_

#include <cmath>
#include <cassert>
#include <iostream>
#include <limits>

#include <SFML/Graphics.hpp>

#include "typedefs.hpp"
#include "configvars.hpp"

using namespace std;
using namespace sf;

void linspace(Axis& a, Real start, Real stop){
    assert((a.size() != 0) && "Size of Axis variable must be greater than 0!");

    u64 sz = a.size();
    Real step = (stop-start)/sz;
    for(u64 i = 0; i < sz; i++){
        a[i] = start + step*i;
    }
}

Complex identityMap(Real x, Real y){
    return Complex(x, y);
}

Complex zeroMap(Real x, Real y){
    return Complex(0, 0);
}

/**
 * Create a 2D map of complex numbers with given values of X and Y axis.
 * This will call a mapping function for every matrix element in ZMap
 * with X[i] and Y[j] as real and imaginary parts given to map function.
 *
 * @param[out] Z ZMap to be created.
 * @param[in] X X axis values.
 * @param[in] Y Y axis values.
 * @param[in] map Mapping function to give a complex number for every X[i] & Y[j].
 * */
void createZMap(ZMap& Z, const Axis& X, const Axis& Y, Complex (*map)(Real x, Real y) = identityMap){
    assert(X.size() != 0 && Y.size() != 0 && "Please initialize X and Y before creating Z map");

    u64 xsz = X.size(); // number of cols in map
    u64 ysz = Y.size(); // number of rows in map

    Z.resize(xsz); // create space of holding columns

    // create map
    for(u64 i = 0; i < xsz; i++){
        Z[i].resize(ysz); // create space for holding cols

        // apply map
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = map(X[i], Y[j]);
        }
    }
}

// Real findMaxZ(const ZMap& Z){
//     double max = -numeric_limits<float>::max();
//     for(u64 i = 0; i < IMAGE_WIDTH; i++){
//         for(u64 j = 0; j < IMAGE_HEIGHT; j++){
//             if(Z[i][j].mod() > max) max = Z[i][j].mod();
//         }
//     }

//     return max;
// }

/**
 * Apply mandelbrot map to Z map.
 *
 * @brief[out] Z ZMap to apply the mandelbrot map on.
 * @brief[in] X X axis values.
 * @brief[in] Y Y axis values.
 * */
inline void mandelbrot(ZMap& Z, const Axis& X, const Axis& Y, Real zoom = 0.8){
    u64 xsz = X.size();
    u64 ysz = Y.size();

    // update Z grid
    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = pow(Z[i][j], 2) + Complex{(X[i])/(zoom*AXIS_RANGE), (Y[j])/(zoom*AXIS_RANGE)};
        }
    }
}

/**
 * Apply (Z^n + c) map to Z map.
 *
 * @brief[out] Z Zmap to apply the zpow map upon.
 * @brief[in] n Power by which each z in Z will be raised to.
 * @brief[in] c Complex value to be added to z^n.
 * */
inline void pow(ZMap& Z, u64 n, const Complex& c){
    u64 xsz = Z.size();
    u64 ysz = Z[0].size();

    // update Z grid
    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = std::pow(Z[i][j], n) + static_cast<complex<double>>(c);
        }
    }
}

/**
 * Apply (Z^n + c) map to Z map.
 *
 * @brief[out] Z Zmap to apply the zpow map upon.
 * @brief[in] n Power by which each z in Z will be raised to.
 * @brief[in] c Complex value to be added to z^n.
 * */
inline void exp(ZMap& Z, const Complex& c){
    u64 xsz = Z.size();
    u64 ysz = Z[0].size();

    // update Z grid
    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = exp(Z[i][j])*c;
        }
    }
}

inline void cos(ZMap& Z, const Complex& c){
    u64 xsz = Z.size();
    u64 ysz = Z[0].size();

    // update Z grid
    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = cos(pow(Z[i][j], 2) + c);
        }
    }
}

inline void sin(ZMap& Z, const Complex& c){
    u64 xsz = Z.size();
    u64 ysz = Z[0].size();

    // update Z grid
    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = cos(pow(Z[i][j], 2) + c);
        }
    }
}


#endif // MATH_H_
