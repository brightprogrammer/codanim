#ifndef ACTIVATIONS_H_
#define ACTIVATIONS_H_

#include "typedefs.hpp"
#include <cmath>

Real sigmoid(Real x){
    return 1.0/(1+exp(-x));
}

#endif // ACTIVATIONS_H_
