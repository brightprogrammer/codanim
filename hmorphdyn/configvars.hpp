#ifndef CONFIGVARS_H_
#define CONFIGVARS_H_

#include <SFML/Graphics.hpp>

#include "typedefs.hpp"

using namespace std;
using namespace sf;

// colors for linear interpolation
static Color BEGIN_COLOR = Color(200, 100, 200);
static Color END_COLOR = Color(150, 200, 150);
const Color AXIS_COLOR = Color(255, 255, 255, 128);
const float AXIS_THICKNESS = 1;

const int WINDOW_WIDTH = 1350, WINDOW_HEIGHT = 650;
// const int IMAGE_WIDTH = 600, IMAGE_HEIGHT = 600;
const int IMAGE_SCALE = 1;
const int IMAGE_WIDTH = WINDOW_WIDTH*IMAGE_SCALE, IMAGE_HEIGHT = WINDOW_HEIGHT*IMAGE_SCALE;
const u64 MAX_ITERS = 4000;

const Real AXIS_RANGE = 10;
const Real XAXIS_RANGE = AXIS_RANGE;
const Real YAXIS_RANGE = float(AXIS_RANGE)*float(IMAGE_WIDTH)/float(IMAGE_HEIGHT);

const bool SAVE_IMAGES = false;
const string BASE_IMAGE_NAME = "exp(z)*(pow(e,-1)+2i)-" + to_string(time(nullptr)) + "-";

const bool LIMIT_FRAMERATE = false;
const u32 FRAMERATE_LIMIT = 1;

const u32 POINT_SPACING = 1;

#endif // CONFIGVARS_H_
