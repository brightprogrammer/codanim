#ifndef CONFIGVARS_H_
#define CONFIGVARS_H_

#include <SFML/Graphics.hpp>

#include "typedefs.hpp"

using namespace std;
using namespace sf;

// colors for linear interpolation
static Color BEGIN_COLOR = Color(200, 100, 200);
static Color END_COLOR = Color(150, 200, 150);
const Color AXIS_COLOR = Color::White;
const float AXIS_THICKNESS = 1;

const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;
const int IMAGE_WIDTH = 2000, IMAGE_HEIGHT = 2000;
const u64 MAX_ITERS = 16;

const Real AXIS_RANGE = 5;

const bool SAVE_IMAGES = false;
const string BASE_IMAGE_NAME = "image" + to_string(time(nullptr)) + "-";


#endif // CONFIGVARS_H_
