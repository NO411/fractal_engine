#pragma once

#include "raylib.h"
#include <string>

namespace settings
{
	const std::string PROJECT_NAME = "Fractal Engine";

	constexpr int IMAGE_WIDTH = 900;
	constexpr int IMAGE_HEIGHT = 600;
	constexpr Vector2 DRAW_OFFSET = {80, 80};

	constexpr Color BG_COLOR = {20, 40, 50, 255};
	constexpr Color BG_COLOR_2 = {10, 30, 40, 255};
	constexpr Color DEFAULT_COLOR = {0, 255, 255, 255};

	constexpr int FONT_SIZE = 11;
	constexpr int FONT_SIZE_2 = 18;
	constexpr float FONT_SPACING = 0.5;

	float PARTING = IMAGE_WIDTH + DRAW_OFFSET.x + 50;
	constexpr Vector2 TABSIZE = {150, 40};
}
