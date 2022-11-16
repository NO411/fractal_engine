#pragma once
#include "../src/fractal.h"
#include <cmath>
#include <map>
#include <iostream>

namespace buddhabrot
{
#define number long double

	number ITERATIONS_MAX = 2000;

	// imaginärer Anteil von c
	number IM_MIN = -1; // y-axis
	number IM_MAX = 1;

	// reeller Anteil von c
	number RE_MIN = -2; // x-axis
	number RE_MAX = 1;

	std::vector<Slider> sliders = {
		{1, "min y (im)", -1, 1, &IM_MIN, IM_MIN, 0, false},
		{2, "max y (im)", -1, 1, &IM_MAX, IM_MAX, 1, false},
		{3, "min x (re)", -2, 1, &RE_MIN, RE_MIN, 0, false},
		{4, "max x (re)", -2, 1, &RE_MAX, RE_MAX, 3, false},
		{5, "max iterations", 0, 500, &ITERATIONS_MAX, ITERATIONS_MAX, 0, true},
	};

	std::vector<Switch> switches = {
	};

	CoordinateSystem coordinate_system = {"re", "im", 5, &RE_MIN, &RE_MAX, &IM_MIN, &IM_MAX};
	
	std::map<int, std::map<int, int>> counters;

	int currentPixel = 0;
	bool rendered = false;

	class ComplexNumber
	{
	private:
	public:
		number re;
		number im;

		int xImage;
		int yImage;

		ComplexNumber() {}

		ComplexNumber(number re, number im) : re(re), im(im) {}

		ComplexNumber(int xImage, int yImage) : xImage(xImage), yImage(yImage)
		{
			re = RE_MIN + xImage / (number)settings::IMAGE_WIDTH * (RE_MAX - RE_MIN);
			im = IM_MIN + yImage / (number)settings::IMAGE_HEIGHT * (IM_MAX - IM_MIN);
		}

		int GetXImage()
		{
			return (int)((re - RE_MIN) * settings::IMAGE_WIDTH / (RE_MAX - RE_MIN));
		}

		int GetYImage()
		{
			return (int)((im - IM_MIN) * settings::IMAGE_HEIGHT / (IM_MAX - IM_MIN));
		}

		ComplexNumber Pow()
		{
			return {pow(re, 2) - pow(im, 2), 2 * re * im};
		}

		ComplexNumber operator+(ComplexNumber c)
		{
			return {c.re + re, c.im + im};
		}
	};

	ComplexNumber c;

	void RenderAdditional(Texture2D &texture, Font &font, Camera2D &cam)
	{
		if (currentPixel <= texture.width)
		{
			DrawLine(settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y, settings::DRAW_OFFSET.x + currentPixel, settings::DRAW_OFFSET.y + texture.height, {255, 255, 255, 50});
		}
	}
	int currentw = 0;
	void Iterate(RenderTexture2D &canvas, ComplexNumber c, int currentPixel, int y, bool diverged)
	{
		ComplexNumber z(0.0L, 0.0L);
		int iterations;
		for (iterations = 0; iterations <= ITERATIONS_MAX; iterations++)
		{
			if (pow(z.im, 2) + pow(z.re, 2) > 4)
			{
				break; // diverged
			}

			z = z.Pow() + c;
		currentw++;

			if (diverged)
			{
				counters[z.GetXImage()][z.GetYImage()]++;
				BeginTextureMode(canvas);
				BeginBlendMode(BLEND_ADDITIVE);
				DrawPixelV({(float)z.GetXImage(), (float)z.GetYImage()}, {0, 255, 255, 2});
				EndBlendMode();
				EndTextureMode();
			}
		}

		if (iterations < ITERATIONS_MAX && !diverged)
		{
			Iterate(canvas, c, currentPixel, y, true);
		}
	}

	void Update(RenderTexture2D &canvas)
	{
		if (currentPixel <= settings::IMAGE_WIDTH)
		{
			for (int y = 0; y <= settings::IMAGE_HEIGHT; y++)
			{
				c = {currentPixel, y};
				Iterate(canvas, c, currentPixel, y, false);
			}
			currentPixel++;
		}
		else
		{
			if (!rendered)
			{
				rendered = true;
				for (int x = 0; x <= settings::IMAGE_WIDTH; x++)
				{
					for (int y = 0; y <= settings::IMAGE_HEIGHT; y++)
					{
						
					}
				}
			}
		}
	}

	void Reset()
	{
		currentPixel = 0;
		rendered = false;
	}

	// this passes the fractal to the main program
	Fractal fractal("Buddhabrot", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
