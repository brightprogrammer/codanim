#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <cmath>
#include "typedefs.hpp"

typedef float Real;

struct Complex {
    Real x = 0;
    Real y = 0;

    // conjugate
    Complex conjugate() const{
        return {x, -y};
    }

    // magnitude of this complex number
    Real mod() const{
        return sqrt(x*x+y*y);
    }

    // add operator overload
    Complex operator + (const Complex& a){
        return {x+a.x, y+a.y};
    }

    Complex operator + (Real p){
        return {x+p, y};
    }

    // subtract operator overload
    Complex operator - (const Complex& a){
        return {x-a.x, y-a.y};
    }

    Complex operator - (Real p){
        return {x-p, y};
    }

    // product operator overload
    Complex operator * (const Complex& a){
        return {x*a.x - y*a.y, x*a.y + y*a.x};
    }

    Complex operator * (Real p){
        return {x*p, y*p};
    }

    // division operator overload
    Complex operator / (const Complex& a){
        Complex w = {x/a.mod(), y/a.mod()};
        w = w * a.conjugate();
        return w;
    }

    Complex operator / (Real p){
        return {x/p, y/p};
    }
};

inline Complex pow(const Complex& z, u64 n){
    Complex w = z;
    for(u64 i = 1; i < n; i++){
        w = w*z;
    }

    return w;
}

// e^z function
inline Complex exp(const Complex& z) {
    Complex w;

    // e^(x+iy) = (e^x)*(e^iy) = (e^x)(cos[y] + isin[y]);
    w.x = exp(z.x)*cos(z.y);
    w.y = exp(z.x)*sin(z.y);

    return w;
}


#endif // COMPLEX_H_
