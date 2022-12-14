#pragma once
#include "../src/fractal.h"

namespace dragon
{
	long double FOLDS = 10;
	long double WAIT = 0.5;

	int nFolds = 1;
	long double nWait = 0;

	bool RENDER_OLD_LINES = false;
	long double ALPHA_OLD_LINES = 20;

	std::vector<Slider> sliders = {
		{1, "folds", 1, 22, &FOLDS, FOLDS, 0, true},
		{2, "wait (in seconds)", 0, 5, &WAIT, WAIT, 0, false},
		{3, "old lines aplha", 0, 255, &ALPHA_OLD_LINES, ALPHA_OLD_LINES, 0, true},
	};

	std::vector<Switch> switches = {
		{1, "render old lines", &RENDER_OLD_LINES, RENDER_OLD_LINES},
	};

	CoordinateSystem coordinate_system;

	int squareSideLength = settings::IMAGE_WIDTH - 400; // 200 dist per side
	Vector2 startCorner = {settings::IMAGE_WIDTH / 2.0f, settings::IMAGE_HEIGHT - 110};
	std::vector<Vector2> points = {
		{startCorner.x - squareSideLength / 2.0f, startCorner.y - squareSideLength / 2.0f},
		startCorner,
		{startCorner.x + squareSideLength / 2.0f, startCorner.y - squareSideLength / 2.0f},
	};

	std::vector<Vector2> startPoints = points;
	std::vector<Vector2> oldPoints;

	void RenderAdditional(Font &font, Camera2D &cam)
	{
		DrawTextEx(font, ("folds: " + std::to_string(nFolds - 1)).c_str(), {settings::DRAW_OFFSET.x, settings::DRAW_OFFSET.y + settings::IMAGE_HEIGHT + 5}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);
	}

	bool SameFloat(float a, float b)
	{
		if (std::abs(a - b) <= __FLT_EPSILON__)
		{
			return true;
		}
		return std::abs(a - b) <= __FLT_EPSILON__ * std::max(std::abs(a), std::abs(b));
	}

	bool Update(RenderTexture2D &canvas, Image &image)
	{
		// iterate and draw to canvas.texture
		if (nFolds > FOLDS)
		{
			return false;
		}

		// wait iterating for `WAIT` seconds but return true for timer
		nWait += GetFrameTime();
		if (nWait < WAIT && nFolds != 1)
		{
			return true;
		}
		nWait = 0;

		std::vector<Vector2> newPoints;

		for (size_t i = 0; i < points.size() - 1; i++) // without last element, will be pushed after loop
		{
			Vector2 point1 = points[i];
			Vector2 point2 = points[i + 1];
			newPoints.push_back(point1);
			
			if (nFolds % 4 == 0)
			{
				newPoints.push_back({point2.x, point1.y});
			}
			else if ((nFolds - 1) % 4 == 0)
			{
				if (SameFloat(point1.y, point2.y))
				{
					if (point1.x < point2.x)
					{
						newPoints.push_back({point1.x + (point2.x - point1.x) / 2.0f, point1.y + (point2.x - point1.x) / 2.0f});
					}
					else
					{
						newPoints.push_back({point2.x + (point1.x - point2.x) / 2.0f, point1.y - (point1.x - point2.x) / 2.0f});
					}
				}
				else if (SameFloat(point1.x, point2.x))
				{
					if (point1.y < point2.y)
					{
						newPoints.push_back({point1.x + (point2.y - point1.y) / 2.0f, point1.y + (point2.y - point1.y) / 2.0f});
					}
					else
					{
						newPoints.push_back({point2.x - (point1.y - point2.y) / 2.0f, point2.y + (point1.y - point2.y) / 2.0f});
					}
				}
			}
			else if ((nFolds - 2) % 4 == 0)
			{ 
				newPoints.push_back({point1.x, point2.y});
			}
			else if ((nFolds - 3) % 4 == 0)
			{
				if (SameFloat(point1.y, point2.y))
				{
					if (point1.x < point2.x)
					{
						newPoints.push_back({point1.x + (point2.x - point1.x) / 2.0f, point1.y - (point2.x - point1.x) / 2.0f});
					}
					else
					{
						newPoints.push_back({point2.x + (point1.x - point2.x) / 2.0f, point1.y + (point1.x - point2.x) / 2.0f});
					}
				}
				else if (SameFloat(point1.x, point2.x))
				{
					if (point1.y < point2.y)
					{
						newPoints.push_back({point1.x - (point2.y - point1.y) / 2.0f, point1.y + (point2.y - point1.y) / 2.0f});
					}
					else
					{
						newPoints.push_back({point2.x + (point1.y - point2.y) / 2.0f, point2.y + (point1.y - point2.y) / 2.0f});
					}
				}
			}
		}
		newPoints.push_back(points.back());

		points = newPoints;

		image = GenImageColor(settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT, settings::BG_COLOR);
		if (RENDER_OLD_LINES && oldPoints.size() != 0 && nFolds > 1)
		{
			for (size_t i = 0; i < oldPoints.size() - 1; i++)
			{
				ImageDrawLineV(&image, oldPoints[i], oldPoints[i + 1], AddColor(settings::BG_COLOR, ColorAlpha(settings::DEFAULT_COLOR, ALPHA_OLD_LINES / 255.0f)));
			}
		}

		for (size_t i = 0; i < points.size() - 1; i++)
		{
			ImageDrawLineV(&image, points[i], points[i + 1], settings::DEFAULT_COLOR);
		}

		Color *pixels = LoadImageColors(image);
		UpdateTexture(canvas.texture, pixels);
		UnloadImageColors(pixels);

		oldPoints = points;

		nFolds++;
		return true;
	}

	void Reset(Image &image)
	{
		nFolds = 1;
		points = startPoints;
	}

	Fractal fractal("Dragon", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
