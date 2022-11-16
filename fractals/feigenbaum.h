#pragma once
#include "../src/fractal.h"

namespace feigenbaum
{
#define START_VALUE 0.2
	long double MIN_A = 2.9;
	long double MAX_A = 4;

	long double MIN_X = 0;
	long double MAX_X = 1;

	long double SETTLING_TIME = 500;
	long double PLOT_VALUES = 1000;

	int currentPixel = 0;
	CoordinateSystem coordinate_system = {"a", "x", 5, &MIN_A, &MAX_A, &MIN_X, &MAX_X};

	void RenderAdditional(Texture2D &texture, Font &font, Camera2D &cam)
	{
		if (currentPixel <= texture.width)
		{
			DrawLine(settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y, settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y + texture.height, {255, 255, 255, 50});
		}
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
	Fractal fractal("Feigenbaum", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
