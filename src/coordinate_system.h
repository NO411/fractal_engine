#pragma once
#include "settings.h"

class CoordinateSystem
{
private:
    static const int dist;
    static const int lineLength;

    std::string x;
    std::string y;
    int digits;

    long double *minX;
    long double *maxX;

    long double *minY;
    long double *maxY;

    bool exists;

public:
    CoordinateSystem();
    CoordinateSystem(std::string x, std::string y, int digits, long double *minX, long double *maxX, long double *minY, long double *maxY);

    void Render(Texture2D &texture, Font &font);
};

const int CoordinateSystem::dist = 15;
const int CoordinateSystem::lineLength = 10;

CoordinateSystem::CoordinateSystem()
{
    exists = false;
}

CoordinateSystem::CoordinateSystem(std::string x, std::string y, int digits, long double *minX, long double *maxX, long double *minY, long double *maxY)
    : x(x), y(y), digits(digits), minX(minX), maxX(maxX), minY(minY), maxY(maxY)
{
    exists = true;
}

void CoordinateSystem::Render(Texture2D &texture, Font &font)
{
    if (!exists)
    {
        return;
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
		std::string text = std::to_string(*minY + (*maxY - *minY) * (float)i / 10.0f);
		text.resize(digits);
		Vector2 measureText = MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING);
		float yText = y - measureText.y / 2.0f;
		DrawTextEx(font, text.c_str(), {settings::DRAW_OFFSET.x - measureText.x - 2 * dist, yText}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);
		// x - axis
		y = texture.height + settings::DRAW_OFFSET.y + dist;
		x = settings::DRAW_OFFSET.x + (float)i / 10.0f * texture.width;
		DrawLineV({x, y - lineLength / 2}, {x, y + lineLength / 2}, WHITE);
		text = std::to_string(*minX + (*maxX - *minX) * (float)i / 10.0f);
		text.resize(digits);
		float xText = x - MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING).x / 2.0f;
		DrawTextEx(font, text.c_str(), {xText, y + dist}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);
	}
    
	Vector2 measureY = MeasureTextEx(font, y.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
	DrawTextEx(font, y.c_str(), {settings::DRAW_OFFSET.x - dist - measureY.x / 2.0f, endPoint1.y - measureY.y}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);

	Vector2 measureX = MeasureTextEx(font, x.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
	DrawTextEx(font, x.c_str(), {endPoint2.x + 5, texture.height + settings::DRAW_OFFSET.y + dist - measureX.y / 2.0f}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);
}
