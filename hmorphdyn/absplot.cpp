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
#include "activations.hpp"
#include "color.hpp"
#include "math.hpp"
#include <ctime>

#define KB (1024)
#define MB (1024*KB)
#define GB (1024*MB)

using namespace sf;
using namespace std;

static const float PI = 2.f*asin(1);

u32 currIter;
Real e = exp(1);
Real range = 2;
Real zoom = 0.4;
void genColorMap(ZMap& Z, Image& img){
    const u64 xsz = Z.size();
    const u64 ysz = Z[0].size();

    HSVColor a = {0, 1, 1};
    HSVColor b = {180, 1, 1};

    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            Z[i][j] = exp(Z[i][j])*Complex(1/e, 2);
            // Z[i][j] = pow(Z[i][j],10) + Complex(-0.5, 0.5);
            // Z[i][j] = sin(pow(Z[i][j],2) + Complex(-2/e, 1/(e*e)));
            Real modz = abs(Z[i][j]);

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

            // Color base = img.getPixel(i, j);
            // float factor = 0.0;
            // base.r *= factor;
            // base.g *= factor;
            // base.b *= factor;
            // base.a *= factor;

            float t = fabs(sin(modz));
            // float t = modz/zmax;
            // float t = (sigmoid(modz/1e2)-0.5)*2;
            // float t = 1 - tanh(modz)*tanh(modz);

            Color col = lerpHSV(BEGIN_COLOR_HSV, END_COLOR_HSV, t);
            // Color col = base + lerpHSV(BEGIN_COLOR_HSV, END_COLOR_HSV, t);
            // Color col = base + lerpRGB(BEGIN_COLOR, END_COLOR, t);
            // Color c = col;
            // cout << "zmax=" << zmax << "\tColor {r = " << (int)c.r << ", g = " << (int)c.g << ", b = " << (int)c.b << "}"<< endl;

            img.setPixel(i, j, col);
        }
    }
}

void drawPolarAxis(RenderWindow& window){
    const float angleStep = 30;
    const u32 numLines = 360.0 / angleStep;

    RectangleShape line(Vector2f(WINDOW_WIDTH, 1));
    line.setPosition(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
    line.setFillColor(AXIS_COLOR);
    line.setOutlineColor(Color::Transparent);
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
    static const u32 numContours = 2*AXIS_RANGE; //
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
    if(LIMIT_FRAMERATE) window.setFramerateLimit(FRAMERATE_LIMIT);
    window.setPosition(Vector2i(10,50));

    // create X and Y axis
    const double aspectRatio = float(IMAGE_WIDTH)/float(IMAGE_HEIGHT);

    // initialize axis
    Axis X(IMAGE_WIDTH), Y(IMAGE_HEIGHT);
    linspace(X, -AXIS_RANGE*aspectRatio, AXIS_RANGE*aspectRatio);
    linspace(Y, -AXIS_RANGE, AXIS_RANGE);

    // create zmap
    ZMap Z;
    createZMap(Z, X, Y, identityMap);
    // Complex c(-0.833333, 0.0416667);
    // Real zmax = zpow(Z, 2, c);
    Real c = 1;
    // Real zmax = lambdaExponential(Z, X, Y, Complex(1/c, 0));
    // Real zmax = cosMap(Z, c);
    // Real zmax = mandelbrot(Z, X, Y);
    // zmax = mandelbrot(Z, X, Y);

    // initialize image for display
    Image colormap;
    colormap.create(X.size(), Y.size());
    genColorMap(Z, colormap);
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
            // window.clear(sf::Color::Black);

            // zmax = mandelbrot(Z, X, Y);
            // lambdaExponential(Z, X, Y, Complex(1/c, 0));
            // zmax = cosMap(Z, c);
            // zmax = zpow(Z, 2, c);

            genColorMap(Z, colormap);
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

    // genColorMap(Z, colormap);
    colormap.saveToFile("final.png");

    return 0;
 }
