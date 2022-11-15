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

			std::string text = std::to_string(IM_MIN + (IM_MAX - IM_MIN) * (float)i / 10.0f);
			text.resize(5);
			Vector2 measureText = MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING);
			float yText = y - measureText.y / 2.0f;
			DrawTextEx(font, text.c_str(), {settings::DRAW_OFFSET.x - measureText.x - 2 * dist, yText}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);

			// x - axis
			y = texture.height + settings::DRAW_OFFSET.y + dist;
			x = settings::DRAW_OFFSET.x + (float)i / 10.0f * texture.width;
			DrawLineV({x, y - lineLength / 2}, {x, y + lineLength / 2}, WHITE);

			text = std::to_string(RE_MIN + (RE_MAX - RE_MIN) * (float)i / 10.0f);
			text.resize(5);
			float xText = x - MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING).x / 2.0f;
			DrawTextEx(font, text.c_str(), {xText, y + dist}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);
		}

		std::string y = "im";
		Vector2 measureX = MeasureTextEx(font, y.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
		DrawTextEx(font, y.c_str(), {settings::DRAW_OFFSET.x - dist - measureX.x / 2.0f, endPoint1.y - measureX.y}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);

		std::string x = "re";
		Vector2 measureA = MeasureTextEx(font, x.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
		DrawTextEx(font, x.c_str(), {endPoint2.x + 5, texture.height + settings::DRAW_OFFSET.y + dist - measureA.y / 2.0f}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);
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

	void Update(RenderTexture2D &canvas)
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
		}
	}

	void Reset()
	{
		currentPixel = 0;
	}

	// this passes the fractal to the main program
	Fractal fractal("Mandelbrot", sliders, switches, RenderAdditional, Update, Reset);
}
