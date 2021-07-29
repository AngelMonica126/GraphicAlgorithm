//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Common.h"

using namespace ElayGraphics;

int ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH = 800;
int ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT = 600;
int ElayGraphics::WINDOW_KEYWORD::VIEWPORT_WIDTH = ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH;
int ElayGraphics::WINDOW_KEYWORD::VIEWPORT_HEIGHT = ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT;
int ElayGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_X = 0;
int ElayGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_Y = 0;
int ElayGraphics::WINDOW_KEYWORD::NUM_SAMPLES = 4;
bool ElayGraphics::WINDOW_KEYWORD::CURSOR_DISABLE = true;
std::string ElayGraphics::WINDOW_KEYWORD::WINDOW_TITLE = "Graphics";

bool ElayGraphics::COMPONENT_CONFIG::IS_ENABLE_GUI = true;