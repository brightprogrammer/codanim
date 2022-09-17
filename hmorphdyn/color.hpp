#ifndef COLOR_H_
#define COLOR_H_

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "typedefs.hpp"

using namespace sf;
using namespace std;

struct HSVColor {
    double H = 0;
    double S = 0;
    double V = 0;
};

HSVColor BEGIN_COLOR_HSV{0, 1, 1};
HSVColor END_COLOR_HSV{270, 1, 1};

struct RGBColor {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
};

Color HSVtoRGB(HSVColor& hsv){
    int hue = hsv.H;
    float sat = hsv.S;
    float val = hsv.V;

    hue %= 360;
    while(hue<0) hue += 360;

    if(sat<0.f) sat = 0.f;
    if(sat>1.f) sat = 1.f;

    if(val<0.f) val = 0.f;
    if(val>1.f) val = 1.f;

    int h = hue/60;
    float f = float(hue)/60-h;
    float p = val*(1.f-sat);
    float q = val*(1.f-sat*f);
    float t = val*(1.f-sat*(1-f));

    switch(h)
    {
    default:
    case 0:
    case 6: return sf::Color(val*255, t*255, p*255);
    case 1: return sf::Color(q*255, val*255, p*255);
    case 2: return sf::Color(p*255, val*255, t*255);
    case 3: return sf::Color(p*255, q*255, val*255);
    case 4: return sf::Color(t*255, p*255, val*255);
    case 5: return sf::Color(val*255, p*255, q*255);
    }
}

HSVColor RGBtoHSV(const sf::Color& color)
{
    double R = color.r / 255.f;
    double G = color.g / 255.f;
    double B = color.b / 255.f;

    double M = std::max({ R, G, B });
    double m = std::min({ R, G, B });
    double C = M - m; // Chroma

    double H = 0.f; // Hue
    double S = 0.f; // Saturation
    double V = 0.f; // Value

    if (C != 0.f)
    {
        if (M == R)
            H = std::fmod(((G - B) / C), 6.f);
        else if (M == G)
            H = ((B - R) / C) + 2;
        else if (M == B)
            H = ((R - G) / C) + 4;

        H *= 60;
    }

    if (H < 0.f)
        H += 360;

    V = M;

    if (V != 0.f)
        S = C / V;

    return { H, S, V };
}

double lerp(float a, float b, float t){
    return a + t*(b-a);
}

// take HSV color values, interpolate them, convert to RGB space and return
Color lerpHSV(HSVColor a, HSVColor b, double t){
    if(t > 1 || t < 0) std::cerr << "WARNING[lerpHSV] : value of t is outside range! t=" << t << "\n";

    // // Hue interpolation
    // double h;
    // double d = b.H - a.H;

    // // swap c1.x and c2.x
    // if(d < 0) {
    //     double h3 = b.H;
    //     b.H = a.H;
    //     a.H = h3;

    //     d = -d;
    //     t = 1 - t;
    // }

    // if (d > 0.5) {
    //     a.H += 1;
    //     h = std::fmod(lerp(a.H, b.H, t), 1);
    // } else h = lerp(a.H, b.H, t);


    // HSVColor lerpedhsv = {h*360.f, lerp(a.S, b.S, t), lerp(a.V, b.V, t)};
    // Color c = HSVtoRGB(lerpedhsv);
    // cout << "Color {r = " << (int)c.r << ", g = " << (int)c.g << ", b = " << (int)c.b << "}"<< endl;

    HSVColor c;
    c.H = a.H + (b.H-a.H)*t;
    c.S = 1; c.V = 1;
    // // c.H = a.S + (b.S-a.S)*t;
    // // c.H = a.V + (b.V-a.V)*t;

    return HSVtoRGB(c);
}

Color lerpRGB(Color a, Color b, double t){
    if(t > 1 || t < 0) std::cerr << "WARNING[lerpRGB] : value of t is outside range! t=" << t << "\n";

    // new lerped color
    Color col;

    const float delta = 0.01;

    /* When lerping between two color channels, moment comes then the color is black for t=0.5 */

    col.r = u8(a.r + t*(b.r-a.r));
    col.g = u8(a.g + t*(b.g-a.g));
    col.b = u8(a.b + t*(b.b-a.b));

    // Color c = col;
    // cout << "Color {r = " << (int)c.r << ", g = " << (int)c.g << ", b = " << (int)c.b << "}"<< endl;

    return col;
}

#endif // COLOR_H_
