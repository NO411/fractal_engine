#pragma once

#include "raylib.h"
#include <string>
#include "settings.h"

class Slider
{
private:
	static const int length;
	static const int textDist;
	static const int dragRadius;

	Vector2 pos;
	float sliderPercentage;

	std::string name;

	long double min;
	long double max;

	long double *setConstant;
	long double value;

	bool moving = false;
	bool int_ = false;

	int number;
	int linkedSlider;

public:
	static const float ySize;

	Slider();
	Slider(int number, std::string name, long double min, long double max, long double *setConstant, long double initVar, int linkedSlider, bool int_);

	std::string DoubleToString(long double n);
	void Render(Font &font);
	bool Selected();
	void SetConstant();
	void UpdateValue();
	void Update();
	long double GetValue();

	void SetMin(long double m);
	int GetLinked();
	bool IsLinked();
	int GetNumber();
};

const float Slider::ySize = 80;

const int Slider::length = 150;
const int Slider::textDist = 5;
const int Slider::dragRadius = 8;

Slider::Slider()
{
}

Slider::Slider(int number, std::string name, long double min, long double max, long double *setConstant, long double initVar, int linkedSlider, bool int_)
	: name(name), min(min), max(max), setConstant(setConstant), value(initVar), linkedSlider(linkedSlider), number(number), int_(int_)
{
	sliderPercentage = (initVar - min) / (max - min);
	pos = {settings::PARTING + 50, (float)50 + (number - 1) * ySize};
	UpdateValue();
}

std::string Slider::DoubleToString(long double n)
{
	std::string ret = std::to_string(n);
	ret.resize(5);
	if (ret.back() == '.')
	{
		ret.resize(4);
	}

	if (int_)
	{
		std::size_t found = ret.find('.');
		if (found != std::string::npos)
		{
			ret.erase(found, ret.length());
		}
	}

	return ret;
}

void Slider::Render(Font &font)
{
	Color dragColor = WHITE;
	if (moving)
	{
		dragColor = GRAY;
	}
	DrawLineEx(pos, {pos.x + length, pos.y}, 4, settings::BG_COLOR);
	DrawCircle(pos.x + length * sliderPercentage, pos.y, dragRadius, dragColor);
	DrawTextEx(font, name.c_str(), {pos.x + length / 2 - MeasureTextEx(font, name.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING).x / 2, pos.y - MeasureTextEx(font, name.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING).y - (dragRadius + textDist)}, settings::FONT_SIZE_2, settings::FONT_SPACING, GRAY);

	std::string minString = DoubleToString(min);
	Vector2 minMeasure = MeasureTextEx(font, minString.c_str(), settings::FONT_SIZE, settings::FONT_SPACING);
	DrawTextEx(font, minString.c_str(), {pos.x - (dragRadius + textDist) - minMeasure.x, pos.y - minMeasure.y / 2}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);

	DrawTextEx(font, DoubleToString(max).c_str(), {pos.x + length + dragRadius + textDist, pos.y - minMeasure.y / 2}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);

	std::string valueString = DoubleToString(value);
	DrawTextEx(font, valueString.c_str(), {pos.x + sliderPercentage * length - MeasureText(valueString.c_str(), settings::FONT_SIZE) / 2, pos.y + (dragRadius + textDist)}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);
}

bool Slider::Selected()
{
	return CheckCollisionPointCircle(GetMousePosition(), {pos.x + length * sliderPercentage, pos.y}, dragRadius);
}

void Slider::SetConstant()
{
	*setConstant = GetValue();
}

void Slider::UpdateValue()
{
	value = sliderPercentage * (max - min) + min;
}

void Slider::Update()
{
	if (IsMouseButtonPressed(0) && Selected())
	{
		moving = true;
	}

	if (IsMouseButtonReleased(0))
	{
		moving = false;
	}

	if (moving)
	{
		sliderPercentage = (GetMouseX() - pos.x) / length;
		if (sliderPercentage < 0)
		{
			sliderPercentage = 0;
		}
		else if (sliderPercentage > 1)
		{
			sliderPercentage = 1;
		}
	}

	UpdateValue();
}

long double Slider::GetValue()
{
	if (int_)
	{
		return (int) value;
	}
	return value;
}

void Slider::SetMin(long double m)
{
	min = m;
}

int Slider::GetLinked()
{
	return linkedSlider;
}

int Slider::GetNumber()
{
	return number;
}

bool Slider::IsLinked()
{
	return GetLinked() > 0;
}
