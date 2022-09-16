#include <iostream>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "configvars.hpp"
#include "typedefs.hpp"
#include "complex.hpp"
#include "color.hpp"
#include "math.hpp"
#include <ctime>

#define KB (1024)
#define MB (1024*KB)
#define GB (1024*MB)

using namespace sf;
using namespace std;

u32 currIter;
Real e = exp(1);
Real range = 2;
Real zoom = 0.4;
void genColorMap(ZMap& Z, Image& img, Real zmax){
    const u64 xsz = Z.size();
    const u64 ysz = Z[0].size();

    HSVColor a = {0, 1, 1};
    HSVColor b = {180, 1, 1};

    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Real modz = Z[i][j].mod();

            // double m = currIter + 1 - log(log2(modz));
            // double freqX = 0.2, freqY = 0.2, freqZ = 0.2;
            // double centerX = 0, centerY = 255/2, centerZ = 255/2;
            // double deltaX = 5, deltaY = 5, deltaZ = 5;

            // Color c = img.getPixel(i,j);
            // c.r += (u8)(sin(freqX * m + 0) * centerX + deltaX);
            // c.g += (u8)(sin(freqY * m + 1) * centerY + deltaY);
            // c.b += (u8)(sin(freqZ * m + 2) * centerZ + deltaZ);
            // // c.r += u8(255 * m / MAX_ITERS);
            // // c.g += 255;
            // // c.b += m < MAX_ITERS ? 255 : 0;
            // img.setPixel(i, j, c);

            // get lerped color
            // Color c = lerpHSV(a, b, modz/zmax);

            Color base = img.getPixel(i, j);
            float factor = 0.4;
            base.r *= factor;
            base.g *= factor;
            base.b *= factor;
            base.a *= factor;

            // float t = fabs(sin(modz));
            float t = modz/zmax;

            Color smallPointColor = Color(0, t*255, t*255);
            Color largePointColor = Color(255-t*255, 255-t*255, 255-t*255);
            // Color largePointColor = Color::Transparent;

            // Color c = smallPointColor;
            // cout << "Color {r = " << (int)c.r << ", g = " << (int)c.g << ", b = " << (int)c.b << "}"<< endl;
            // c = largePointColor;
            // cout << "Color {r = " << (int)c.r << ", g = " << (int)c.g << ", b = " << (int)c.b << "}"<< endl;

            Color col = base + lerpHSV(BEGIN_COLOR_HSV, END_COLOR_HSV, t);
            // Color col = base + lerpRGB(BEGIN_COLOR, END_COLOR, t);

            // Color col = base;
            // if(t > 0.01){
            //     col += largePointColor;
            // }else{
            //     col += smallPointColor;
            // }

            img.setPixel(i, j, col);
        }
    }
}

static float PI = 2*asin(1);
void drawPolarAxis(RenderWindow& window){
    const float angleStep = 30;
    const u32 numLines = 360.0 / angleStep;

    RectangleShape line(Vector2f(WINDOW_WIDTH, 0.1));
    line.setPosition(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
    line.setFillColor(AXIS_COLOR);
    line.setOutlineThickness(AXIS_THICKNESS);

    for(u32 i = 0; i < numLines; i++){
        line.rotate(angleStep);
        window.draw(line);
    }

    float radius = WINDOW_WIDTH / (AXIS_RANGE * 2);
    CircleShape contour;
    contour.setRadius(radius);
    contour.setOutlineColor(AXIS_COLOR);
    contour.setOutlineThickness(AXIS_THICKNESS);
    contour.setFillColor(Color::Transparent);
    contour.setPointCount(100);

    static const Real sqrt2 = sqrt(2);
    static const u32 numContours = sqrt2*AXIS_RANGE; //
    for(u32 i = 0; i < numContours; i++){
        contour.setRadius(radius*i);
        contour.setPosition(WINDOW_WIDTH/2-radius*i, WINDOW_HEIGHT/2-radius*i);

        window.draw(contour);
    }


}

int main() {
    // add the flag to the other ones
    Uint32 windowStyle = Style::Titlebar | Style::Close;

    // render window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Holomorphic Dynamics", windowStyle);
    // window.setFramerateLimit(10);

    // create X and Y axis
    const double aspectRatio = float(IMAGE_WIDTH)/float(IMAGE_HEIGHT);

    // initialize axis
    Axis X(IMAGE_WIDTH), Y(IMAGE_HEIGHT);
    linspace(Y, -AXIS_RANGE*aspectRatio, AXIS_RANGE*aspectRatio);
    linspace(X, -AXIS_RANGE, AXIS_RANGE);

    // create zmap
    ZMap Z;
    createZMap(Z, X, Y, identityMap);
    Real zmax = zpow(Z, 2, {0, 0});
    // Real zmax = mandelbrot(Z, X, Y);
    // zmax = mandelbrot(Z, X, Y);

    // initialize image for display
    Image colormap;
    colormap.create(X.size(), Y.size());
    genColorMap(Z, colormap, zmax);
    if(SAVE_IMAGES) colormap.saveToFile(BASE_IMAGE_NAME + "0.png");

    Texture colortex;
    colortex.loadFromImage(colormap);
    colortex.setSmooth(true); // draw smooth textures

    // plotted sprite
    Sprite plot(colortex);
    plot.setScale(double(WINDOW_WIDTH)/IMAGE_WIDTH, float(WINDOW_HEIGHT)/IMAGE_HEIGHT);

    window.draw(plot);
    drawPolarAxis(window);
    window.display();

    currIter = 1;
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        if(currIter < MAX_ITERS){
            window.clear(sf::Color::Black);

            // zmax = mandelbrot(Z, X, Y);
            zmax = zpow(Z, 2, {0, 0});
            genColorMap(Z, colormap, zmax);
            if(SAVE_IMAGES) colormap.saveToFile(BASE_IMAGE_NAME + std::to_string(currIter) + ".png");

            colortex.update(colormap);
            // plot.setTexture(colortex);


            window.draw(plot);
            drawPolarAxis(window);
            window.display();

            currIter++;
            std::cout << currIter << std::endl;
        }
    }

    return 0;
 }
