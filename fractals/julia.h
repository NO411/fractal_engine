#pragma once
#include "../src/fractal.h"
#include <iostream>

namespace julia
{
#define number long double

	number ITERATIONS_MAX = 100;

	// imaginärer Anteil von c
	number IM_MIN = -2; // y-axis
	number IM_MAX = 2;

	// reeller Anteil von c
	number RE_MIN = -3; // x-axis
	number RE_MAX = 3;

	number C_RE = 0; // x-axis
	number C_IM = 0;

	bool RENDER_SURROUNDING = true;

	std::vector<Slider> sliders = {
		{1, "c im", -1, 1, &C_IM, C_IM, 0, false},
		{2, "c re", -1, 1, &C_RE, C_RE, 0, false},
		{3, "max iterations", 0, 500, &ITERATIONS_MAX, ITERATIONS_MAX, 0, true},
	};

	std::vector<Switch> switches = {
		{1, "render surrounding", &RENDER_SURROUNDING, RENDER_SURROUNDING},
	};

	CoordinateSystem coordinate_system = {"re", "im", 5, &RE_MIN, &RE_MAX, &IM_MIN, &IM_MAX};

	int currentPixel = 0;

	ComplexNumber c;
	ComplexNumber z;

	void RenderAdditional(Font &font, Camera2D &cam)
	{
		if (currentPixel <= settings::IMAGE_WIDTH)
		{
			DrawLine(settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y, settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y + settings::IMAGE_HEIGHT, {255, 255, 255, 50});
		} 
	}
	void Iterate(Image &image, ComplexNumber &z)
	{
		int iterations;
		for (iterations = 0; iterations <= ITERATIONS_MAX; iterations++)
		{
			if (z.im * z.im + z.re * z.re > 4)
			{
				break;
			}
			z = z.Pow() + c;
		}

		if (iterations >= ITERATIONS_MAX)
		{
			ImageDrawPixel(&image, (float)z.xImage, (float)z.yImage, settings::DEFAULT_COLOR);
		}
		else if (RENDER_SURROUNDING)
		{
			ImageDrawPixel(&image, (float)z.xImage, (float)z.yImage, AddColor(settings::BG_COLOR, {settings::DEFAULT_COLOR.r, settings::DEFAULT_COLOR.g, settings::DEFAULT_COLOR.b, (unsigned char)(iterations * 2)}));
		}
	}

	bool Update(RenderTexture2D &canvas, Image &image)
	{
		if (currentPixel <= settings::IMAGE_WIDTH)
		{
			for (int y = 0; y <= settings::IMAGE_HEIGHT; y++)
			{
				z = {currentPixel, y, RE_MIN, RE_MAX, IM_MIN, IM_MAX};
				Iterate(image, z);
			}
			currentPixel++;
			return true;
		}
		return false;
	}

	void Reset(Image &image)
	{
		currentPixel = 0;
		c = {C_RE, C_IM};
	}

	// this passes the fractal to the main program
	Fractal fractal("Julia", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
