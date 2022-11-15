#pragma once

#include "raylib.h"
#include "settings.h"
#include "misc.h"
#include <string>

class Switch
{
private:
	static const int textDist;
	static const Vector2 size;
	
	Rectangle box;
	Vector2 pos;

	std::string name;

	bool *setConstant;
	bool value;
	bool selected;

public:
	Switch();
	Switch(int number, std::string name, bool *setConstant, bool initVar);

	void Render(Font &font, int yOffset);
	bool Selected(float yOffset, Camera2D &cam);
	void SetConstant();
	void Update(float yOffset, Camera2D &cam);
	bool GetValue();
};

const int Switch::textDist = 5;
const Vector2 Switch::size = {100, 25};

Switch::Switch()
{
}

Switch::Switch(int number, std::string name, bool *setConstant, bool initVar)
	: name(name), setConstant(setConstant), value(initVar)
{
	pos = {settings::PARTING + 50, (float)50 + (number - 1) * 60};
	box = {pos.x, pos.y, size.x / 2.0f, size.y};
	if (initVar)
	{
		box.x += size.x / 2.0f;
	}
}

void Switch::Render(Font &font, int yOffset)
{
	DrawRectangleRounded({pos.x, yOffset + pos.y, size.x, size.y}, 0.5, 10, settings::BG_COLOR);

	Color boxColor = GRAY;
	if (value)
	{
		boxColor = settings::DEFAULT_COLOR;
	}
	if (selected)
	{
		boxColor = LIGHTGRAY;
	}
	DrawRectangleRounded({box.x, box.y + yOffset, box.width, box.height}, 0.5, 10, boxColor);
	DrawTextEx(font, name.c_str(), {pos.x, pos.y + yOffset - textDist - MeasureTextEx(font, name.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING).y}, settings::FONT_SIZE_2, settings::FONT_SPACING, GRAY);
}

bool Switch::Selected(float yOffset, Camera2D &cam)
{
	return CheckCollisionPointRec(GetMousePositionScaled(cam), {box.x, yOffset + box.y, box.width, box.height});
}

void Switch::SetConstant()
{
	*setConstant = GetValue();
}

void Switch::Update(float yOffset, Camera2D &cam)
{
	selected = false;
	if (!Selected(yOffset, cam))
	{
		return;
	}
	selected = true;
	if (IsMouseButtonPressed(0))
	{
		value = !value;
		box.x = pos.x;
		if (value)
		{
			box.x += size.x / 2.0f;
		}
	}
}

bool Switch::GetValue()
{
	return value;
}
