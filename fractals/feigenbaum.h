#pragma once
#include "../src/fractal.h"

namespace feigenbaum
{
#define START_VALUE 0.2
	long double MIN_A = 2.9;
	long double MAX_A = 4;
	long double SETTLING_TIME = 500;
	long double PLOT_VALUES = 1000;

	int currentPixel = 0;

	// coordinate system
	const int dist = 15;
	const float lineLength = 10;

	void RenderAdditional(Texture2D &texture, Font &font, Camera2D &cam)
	{
		if (currentPixel <= texture.width)
		{
			DrawLine(settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y, settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y + texture.height, {255, 255, 255, 50});
		}

		Vector2 endPoint1 = {settings::DRAW_OFFSET.x - dist, settings::DRAW_OFFSET.y};
		Vector2 endPoint2 = {settings::DRAW_OFFSET.x + texture.width, settings::DRAW_OFFSET.y + texture.height + dist};

		DrawLineEx(endPoint1, {settings::DRAW_OFFSET.x - dist, settings::DRAW_OFFSET.y + texture.height + dist + lineLength / 2.0f}, 2, WHITE);
		DrawLineEx({0, settings::DRAW_OFFSET.y + texture.height + dist}, endPoint2, 2, WHITE);
		DrawTriangle(endPoint1, {endPoint1.x - 4, endPoint1.y + 7}, {endPoint1.x + 4, endPoint1.y + 7}, WHITE);
		DrawTriangle(endPoint2, {endPoint2.x - 7, endPoint2.y - 4}, {endPoint2.x - 7, endPoint2.y + 4}, WHITE);

		for (int i = 0; i <= 10; i++)
		{
			// y - axis
			float y = texture.height + settings::DRAW_OFFSET.y - (float)i / 10.0f * texture.height;
			float x = settings::DRAW_OFFSET.x - dist;
			DrawLineV({x - lineLength / 2, y}, {x + lineLength / 2, y}, WHITE);

			std::string text = std::to_string(i / 10.0f);
			text.resize(5);
			Vector2 measureText = MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING);
			float yText = y - measureText.y / 2.0f;
			DrawTextEx(font, text.c_str(), {settings::DRAW_OFFSET.x - measureText.x - 2 * dist, yText}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);

			// x - axis
			y = texture.height + settings::DRAW_OFFSET.y + dist;
			x = settings::DRAW_OFFSET.x + (float)i / 10.0f * texture.width;
			DrawLineV({x, y - lineLength / 2}, {x, y + lineLength / 2}, WHITE);

			text = std::to_string(MIN_A + (MAX_A - MIN_A) * (float)i / 10.0f);
			text.resize(5);
			float xText = x - MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING).x / 2.0f;
			DrawTextEx(font, text.c_str(), {xText, y + dist}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);
		}

		std::string x = "x";
		Vector2 measureX = MeasureTextEx(font, x.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
		DrawTextEx(font, x.c_str(), {settings::DRAW_OFFSET.x - dist - measureX.x / 2.0f, endPoint1.y - measureX.y}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);

		std::string a = "a";
		Vector2 measureA = MeasureTextEx(font, a.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
		DrawTextEx(font, a.c_str(), {endPoint2.x + 5, texture.height + settings::DRAW_OFFSET.y + dist - measureA.y / 2.0f}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);
	}

	long double GetNewValue(long double a, long double xn)
	{
		return a * xn * (1 - xn);
	}

	void Update(RenderTexture2D &canvas)
	{
		if (currentPixel > canvas.texture.width)
		{
			return;
		}
		currentPixel++;

		// get a for every pixel from 0 to screen width
		long double a = MIN_A + currentPixel / (long double)canvas.texture.width * (MAX_A - MIN_A);
		long double x = START_VALUE;

		for (int i = 0; i < SETTLING_TIME + PLOT_VALUES; i++)
		{
			x = GetNewValue(a, x);

			if (i > SETTLING_TIME)
			{
				BeginTextureMode(canvas);
				BeginBlendMode(BLEND_ADDITIVE);

				// convert y value to drawing coordinate system
				long double y = canvas.texture.height * x;
				DrawPixelV({(float)currentPixel, (float)y}, {settings::DEFAULT_COLOR.r, settings::DEFAULT_COLOR.g, settings::DEFAULT_COLOR.b, 20});

				EndBlendMode();
				EndTextureMode();
			}
		}
	}

	void Reset()
	{
		currentPixel = 0;
	}

	std::vector<Slider> sliders = {
		{1, "min a", 0, 4, &MIN_A, MIN_A, 0, false},
		{2, "max a", 2, 4, &MAX_A, MAX_A, 1, false},
		{3, "settling time", 0, 500, &SETTLING_TIME, SETTLING_TIME, 0, true},
		{4, "plot values", 0, 3000, &PLOT_VALUES, PLOT_VALUES, 0, true},
	};

	std::vector<Switch> switches = {
	};

	// this passes the fractal to the main program
	Fractal fractal("Feigenbaum", sliders, switches, RenderAdditional, Update, Reset);
}
