#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdint.h>
#include <vector>
#include <complex>

#include <SFML/Graphics.hpp>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef float Real;
typedef std::complex<Real> Complex;

typedef std::vector<Real> Axis;
typedef std::vector<std::vector<Complex>> ZMap;

typedef sf::CircleShape Point;
typedef std::vector<std::vector<Point>> PointMap;

typedef struct RGBColor RGBColor;
typedef RGBColor* ColorMap;

#endif // TYPEDEFS_H_
