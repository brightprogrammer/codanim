#include <iostream>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <limits>

#include <SFML/Graphics.hpp>

#include "typedefs.h"

#define KB (1024)
#define MB (1024*KB)
#define GB (1024*MB)

using namespace sf;
using namespace std;

struct HSVColor {
    float H = 0;
    float S = 0;
    float V = 0;
};

struct Complex {
    float x = 0;
    float y = 0;

    // conjugate
    Complex conjugate() const{
        return {x, -y};
    }

    // magnitude of this complex number
    float mod() const{
        return sqrt(x*x+y*y);
    }

    // add operator overload
    Complex operator + (const Complex& a){
        return {x+a.x, y+a.y};
    }

    Complex operator + (float p){
        return {x+p, y};
    }

    // subtract operator overload
    Complex operator - (const Complex& a){
        return {x-a.x, y-a.y};
    }

    Complex operator - (float p){
        return {x-p, y};
    }

    // product operator overload
    Complex operator * (const Complex& a){
        return {x*a.x - y*a.y, x*a.y + y*a.x};
    }

    Complex operator * (float p){
        return {x*p, y*p};
    }

    // division operator overload
    Complex operator / (const Complex& a){
        Complex w = {x/a.mod(), y/a.mod()};
        w = w * a.conjugate();
        return w;
    }

    Complex operator / (float p){
        return {x/p, y/p};
    }
};

const int WINDOW_WIDTH = 650, WINDOW_HEIGHT = 650;
float X[WINDOW_WIDTH], Y[WINDOW_HEIGHT];
Complex Z[WINDOW_WIDTH][WINDOW_HEIGHT];

// e^z function
Complex exp(const Complex& z) {
    Complex w;

    // e^(x+iy) = (e^x)*(e^iy) = (e^x)(cos[y] + isin[y]);
    w.x = exp(z.x)*cos(z.y);
    w.y = exp(z.x)*sin(z.y);

    return w;
}

// code copied from : https://gist.github.com/marukrap/7c361f2c367eaf40537a8715e3fd952a
Color HSVtoRGB(HSVColor& hsv){
    float C = hsv.S * hsv.V; // Chroma
    float HPrime = std::fmod(hsv.H / 60, 6.f); // hsv.H
    float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
    float M = hsv.V - C;

    float R = 0.f;
    float G = 0.f;
    float B = 0.f;

    switch (static_cast<int>(HPrime)){
    case 0: R = C; G = X;        break; // [0, 1)
    case 1: R = X; G = C;        break; // [1, 2)
    case 2:        G = C; B = X; break; // [2, 3)
    case 3:        G = X; B = C; break; // [3, 4)
    case 4: R = X;        B = C; break; // [4, 5)
    case 5: R = C;        B = X; break; // [5, 6)
    }

    R += M;
    G += M;
    B += M;

    sf::Color color;
    color.r = static_cast<u8>(std::round(R * 255));
    color.g = static_cast<u8>(std::round(G * 255));
    color.b = static_cast<u8>(std::round(B * 255));

    return color;
}

float lerp(float a, float b, float t){
    return a + t*(b-a);
}

// take HSV color values, interpolate them, convert to RGB space and return
Color lerpHSV(HSVColor a, HSVColor b, float t){
    // if(t > 1 || t < 0) std::cerr << "WARNING[lerpHSV] : value of t is outside range! t=" << t << "\n";

    // Hue interpolation
    float h;
    float d = b.H - a.H;

    // swap c1.x and c2.x
    if(d < 0) {
        float h3 = b.H;
        b.H = a.H;
        a.H = h3;

        d = -d;
        t = 1 - t;
    }

    if (d > 0.5) {
        a.H += 1;
        h = std::fmod(lerp(a.H, b.H, t), 1);
    } else h = lerp(a.H, b.H, t);

    HSVColor lerpedhsv = { h*360.f, lerp(a.S, b.S, t), lerp(a.V, b.V, t) };
    return HSVtoRGB(lerpedhsv);
}

Color lerpRGB(Color a, Color b, float t){
    // if(t > 1 || t < 0) std::cerr << "WARNING[lerpRGB] : value of t is outside range! t=" << t << "\n";

    // new lerped color
    Color col;

    col.r = a.r + t*(b.r-a.r);
    col.g = a.g + t*(b.g-a.g);
    col.b = a.b + t*(b.b-a.b);

    return col;
}


void fillX(float start, float stop){
    float step = (stop-start)/WINDOW_WIDTH;
    for(u64 i = 0; i < WINDOW_WIDTH; i++){
        X[i] = start + step*i;
    }
}

void fillY(float start, float stop){
    float step = (stop-start)/WINDOW_HEIGHT;
    for(u64 i = 0; i < WINDOW_HEIGHT; i++){
        Y[i] = start + step*i;
    }
}

void fillZ(){
    for(u64 i = 0; i < WINDOW_WIDTH; i++){
        for(u64 j = 0; j < WINDOW_HEIGHT; j++){
            Z[i][j] = {X[i], Y[j]};
        }
    }
}

float findMaxZ(){
    float max = -numeric_limits<float>::max();
    for(u64 i = 0; i < WINDOW_WIDTH; i++){
        for(u64 j = 0; j < WINDOW_HEIGHT; j++){
            if(Z[i][j].mod() > max) max = Z[i][j].mod();
        }
    }

    return max;
}

float f = exp(4);
float fstep = 10;
float e = exp(1);
void genImage(Image& img){
    static HSVColor a = {0, 1, 1};
    static HSVColor b = {359, 1, 1};

     // update Z grid
    for(u64 i = 0; i < WINDOW_WIDTH; i++){
        for(u64 j = 0; j < WINDOW_HEIGHT; j++){
            Z[i][j] = exp(Z[i][j])*(1/(e+1));
        }
    }

    // normalize and generate map
    float max = findMaxZ();
    for(u64 i = 0; i < WINDOW_WIDTH; i++){
        for(u64 j = 0; j < WINDOW_HEIGHT; j++){
            float modz = Z[i][j].mod()/max;
                img.setPixel(i, j, lerpHSV(a, b, modz));
        }
    }

    // f += fstep;
}

int main() {
    // render window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Holomorphic Dynamics", Style::Titlebar | Style::Close);
    window.setPosition(Vector2i(0, 50));
    window.setFramerateLimit(4);

    // create X and Y axis
    fillX(-10, 10);
    fillY(-10, 10);
    fillZ();

    Image gridimg;
    gridimg.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    Color p(255, 0, 0);
    Color q(0, 0, 255);

    Vertex xAxis[] = {
        Vertex(Vector2f(0, WINDOW_HEIGHT/2), Color::Black),
        Vertex(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT/2), Color::Black),
    };

    Vertex yAxis[] = {
        Vertex(Vector2f(WINDOW_WIDTH/2, 0), Color::Black),
        Vertex(Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT), Color::Black),
    };

    genImage(gridimg);

    Texture gridtex;
    gridtex.loadFromImage(gridimg);

    Sprite plot(gridtex);

    u64 maxIters = 50;
    u64 currIter = 0;
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        while(currIter < maxIters){
            window.clear(sf::Color::Black);
            genImage(gridimg);
            gridtex.loadFromImage(gridimg);
            plot.setTexture(gridtex);

            window.draw(plot);

            // draw axis
            window.draw(yAxis, 2, Lines);
            window.draw(xAxis, 2, Lines);


            window.display();
            currIter++;
            std::cout << currIter << std::endl;
        }
    }

    return 0;
 }
