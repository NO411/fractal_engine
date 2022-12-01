#pragma once
#include "../src/fractal.h"
#include <cmath>

namespace mandelbrot
{
#define number long double

	number ITERATIONS_MAX = 500;

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

	void Iterate(RenderTexture2D &canvas, ComplexNumber c)
	{
		ComplexNumber z(0.0L, 0.0L);

		int iterations;
		for (iterations = 0; iterations <= ITERATIONS_MAX; iterations++)
		{
			if (pow(z.im, 2) + pow(z.re, 2) > 4)
			{
				break;
			}

			z = z.Pow() + c;
		}

		BeginTextureMode(canvas);
		BeginBlendMode(BLEND_ADDITIVE);
		if (iterations >= ITERATIONS_MAX)
		{
			DrawPixelV({(float)c.xImage, (float)c.yImage}, settings::DEFAULT_COLOR);
		}
		else if (RENDER_SURROUNDING)
		{
			DrawPixelV({(float)c.xImage, (float)c.yImage}, {settings::DEFAULT_COLOR.r, settings::DEFAULT_COLOR.g, settings::DEFAULT_COLOR.b, (unsigned char)(iterations * 2)});
		}
		EndBlendMode();
		EndTextureMode();
	}

	bool Update(RenderTexture2D &canvas)
	{
		if (currentPixel <= settings::IMAGE_WIDTH)
		{
			for (int i = 0; i < 2; i++)
			{
				for (int y = 0; y <= settings::IMAGE_HEIGHT; y++)
				{
					c = {currentPixel, y};
					Iterate(canvas, c);
				}
				currentPixel++;
			}
			return true;
		}
		return false;
	}

	void Reset(RenderTexture2D &canvas)
	{
		currentPixel = 0;
	}

	// this passes the fractal to the main program
	Fractal fractal("Mandelbrot", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
