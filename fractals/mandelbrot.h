#pragma once
#include "../src/fractal.h"

namespace mandelbrot
{
#define number long double

	number ITERATIONS_MAX = 100;

	// imaginärer Anteil von c
	number IM_MIN = -1; // y-axis
	number IM_MAX = 1;

	// reeller Anteil von c
	number RE_MIN = -2; // x-axis
	number RE_MAX = 1;

	bool RENDER_SURROUNDING = true;

	std::vector<Slider> sliders = {
		{1, "min y (im)", -1, 1, &IM_MIN, IM_MIN, 0, false},
		{2, "max y (im)", -1, 1, &IM_MAX, IM_MAX, 1, false},
		{3, "min x (re)", -2, 1, &RE_MIN, RE_MIN, 0, false},
		{4, "max x (re)", -2, 1, &RE_MAX, RE_MAX, 3, false},
		{5, "max iterations", 0, 500, &ITERATIONS_MAX, ITERATIONS_MAX, 0, true},
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

	void Iterate(Image &image, ComplexNumber c)
	{
		z = {0.0L, 0.0L};

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
			ImageDrawPixel(&image, (float)c.xImage, (float)c.yImage, settings::DEFAULT_COLOR);
		}
		else if (RENDER_SURROUNDING)
		{
			ImageDrawPixel(&image, (float)c.xImage, (float)c.yImage, {(unsigned char)(settings::DEFAULT_COLOR.r * (1.5 * iterations / ITERATIONS_MAX)), (unsigned char)(settings::DEFAULT_COLOR.g * (1.5 * iterations / ITERATIONS_MAX)), (unsigned char)(settings::DEFAULT_COLOR.b * (1.5 * iterations / ITERATIONS_MAX)), 255});
		}
	}

	bool Update(RenderTexture2D &canvas, Image &image)
	{
		if (currentPixel <= settings::IMAGE_WIDTH)
		{
			for (int y = 0; y <= settings::IMAGE_HEIGHT; y++)
			{
				c = {currentPixel, y, RE_MIN, RE_MAX, IM_MIN, IM_MAX};
				Iterate(image, c);
			}
			currentPixel++;
			return true;
		}
		return false;
	}

	void Reset(Image &image)
	{
		currentPixel = 0;
	}

	// this passes the fractal to the main program
	Fractal fractal("Mandelbrot", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
