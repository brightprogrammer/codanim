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
Real e = exp(1);

u32 currIter;
Real range = 2;
Real zoom = 0.4;

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

void createPointMap(PointMap& pointMap, const ZMap& Z){
    u64 xsz = Z.size()/POINT_SPACING;
    u64 ysz = Z[0].size()/POINT_SPACING;

    pointMap.resize(xsz);

    Point p;
    p.setRadius(2);
    p.setOutlineThickness(0);

    for(u64 i = 0; i < xsz; i++){
        pointMap[i].resize(ysz);
        for(u64 j = 0; j < ysz; j++){
            Complex z = Z[i*POINT_SPACING][j*POINT_SPACING];

            float t = fabs(sin(abs(z)));
            Color col = lerpHSV(BEGIN_COLOR_HSV, END_COLOR_HSV, t);

            p.setFillColor(col);

            // transform to screen coordinates and set position
            Vector2f pos = Vector2f(z.real()*WINDOW_WIDTH/XAXIS_RANGE, z.imag()*WINDOW_HEIGHT/YAXIS_RANGE);
            pos += {WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f};
            p.setPosition(pos);

            pointMap[i][j] = p;
        }
    }
}

void cos(PointMap& pmap, Complex c){
    u64 xsz = pmap.size();
    u64 ysz = pmap[0].size();

    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            // get old position and transform back to original coordinates
            Vector2f pos = pmap[i][j].getPosition() - Vector2f(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
            pos.x *= XAXIS_RANGE/WINDOW_WIDTH;
            pos.y *= YAXIS_RANGE/WINDOW_HEIGHT;

            // update position
            Complex z(pos.x, pos.y);
            z = cos(pow(z,2) + c);

            // transform back to screen coordinates
            pos = {z.real(), z.imag()};
            pos.x *= WINDOW_WIDTH/XAXIS_RANGE;
            pos.y *= WINDOW_HEIGHT/YAXIS_RANGE;
            pos += Vector2f(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);

            // set new position
            pmap[i][j].setPosition(pos);
        }
    }
}

void sin(PointMap& pmap, Complex c){
    u64 xsz = pmap.size();
    u64 ysz = pmap[0].size();

    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            // get old position and transform back to original coordinates
            Vector2f pos = pmap[i][j].getPosition() - Vector2f(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
            pos.x *= XAXIS_RANGE/WINDOW_WIDTH;
            pos.y *= YAXIS_RANGE/WINDOW_HEIGHT;

            // update position
            Complex z(pos.x, pos.y);
            z = sin(pow(z,2) + c);

            // transform back to screen coordinates
            pos = {z.real(), z.imag()};
            pos.x *= WINDOW_WIDTH/XAXIS_RANGE;
            pos.y *= WINDOW_HEIGHT/YAXIS_RANGE;
            pos += Vector2f(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);

            // set new position
            pmap[i][j].setPosition(pos);
       }
    }
}

void exp(PointMap& pmap, Complex c){
    u64 xsz = pmap.size();
    u64 ysz = pmap[0].size();

    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            // get old position and transform back to original coordinates
            Vector2f pos = pmap[i][j].getPosition() - Vector2f(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
            pos.x *= XAXIS_RANGE/WINDOW_WIDTH;
            pos.y *= YAXIS_RANGE/WINDOW_HEIGHT;

            // update position
            Complex z(pos.x, pos.y);
            z = exp(z)*c;

            // transform back to screen coordinates
            pos = {z.real(), z.imag()};
            pos.x *= WINDOW_WIDTH/XAXIS_RANGE;
            pos.y *= WINDOW_HEIGHT/YAXIS_RANGE;
            pos += Vector2f(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);

            // set new position
            pmap[i][j].setPosition(pos);
       }
    }
}

void drawPointMap(RenderWindow& window, const PointMap& pmap){
    u64 xsz = pmap.size();
    u64 ysz = pmap[0].size();

    for(u64 i = 0; i < xsz; i++){
        for(u64 j = 0; j < ysz; j++){
            window.draw(pmap[i][j]);
        }
    }
}

int main() {
    // add the flag to the other ones
    Uint32 windowStyle = Style::Titlebar | Style::Close;

    // render window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Holomorphic Dynamics", windowStyle);
    if(LIMIT_FRAMERATE) window.setFramerateLimit(FRAMERATE_LIMIT);
    window.setPosition(Vector2i(10,50));

    // initialize axis
    Axis X(IMAGE_WIDTH), Y(IMAGE_HEIGHT);
    linspace(X, -YAXIS_RANGE, YAXIS_RANGE);
    linspace(Y, -XAXIS_RANGE, XAXIS_RANGE);

    // create zmap
    ZMap Z;
    createZMap(Z, X, Y, identityMap);

    PointMap pmap;
    createPointMap(pmap, Z);

    Complex c(1/(e), 2);

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

            // cos(Z, c);
            // cos(pmap, c);
            exp(pmap, c);

            drawPointMap(window, pmap);
            drawPolarAxis(window);
            window.display();

            currIter++;
            std::cout << currIter << std::endl;
        }
    }

    return 0;
 }
