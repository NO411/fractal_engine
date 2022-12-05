#pragma once
#include "raylib.h"
#include "slider.h"
#include "switch.h"
#include "misc.h"
#include "coordinate_system.h"
#include "complex_number.h"
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

class Fractal;
std::vector<Fractal> fractals;

class Fractal
{
private:
	bool initialized = false;
	std::string name;
	RenderTexture2D canvas;
	Image image;

	float switchYOffset;
	float timer;
	bool converted = false; // for rendering (from canvas to image)

public:
	std::vector<Slider> sliders;
	std::vector<Switch> switches;
	CoordinateSystem coordinate_system;

	std::function<void(Font &font, Camera2D &cam)> RenderAdditional;
	std::function<bool(RenderTexture2D &canvas, Image &image)> Update;
	std::function<void(Image &image)> Reset;

	Fractal(std::string name, std::vector<Slider> &sliders, std::vector<Switch> &switches, CoordinateSystem coordinate_system, std::function<void(Font &font, Camera2D &cam)> RenderAdditional, std::function<bool(RenderTexture2D &canvas, Image &image)> Update, std::function<void(Image &image)> Reset);

	void SetConstants();
	bool IsInitialized();
	std::string &GetName();
	void Clear();
	RenderTexture2D &GetCanvas();
	void SaveImage();
	void Init();
	int GetSliderPos(int number);

	void Render(Font &font, Camera2D &cam);
	void MainUpdate(Camera2D &cam);
};

Fractal::Fractal(std::string name, std::vector<Slider> &sliders, std::vector<Switch> &switches, CoordinateSystem coordinate_system, std::function<void(Font &font, Camera2D &cam)> RenderAdditional, std::function<bool(RenderTexture2D &canvas, Image &image)> Update, std::function<void(Image &image)> Reset)
	: name(name), sliders(sliders), switches(switches), coordinate_system(coordinate_system), RenderAdditional(RenderAdditional), Update(Update), Reset(Reset)
{
	// init, then save fractal in global vector
	fractals.push_back(*this);
}

void Fractal::SetConstants()
{
	for (auto &slider : sliders)
	{
		slider.SetConstant();
	}
	for (auto &switch_ : switches)
	{
		switch_.SetConstant();
	}
}

bool Fractal::IsInitialized()
{
	return initialized;
}

std::string &Fractal::GetName()
{
	return name;
}

void Fractal::Clear()
{
	BeginTextureMode(canvas);
	ClearBackground(settings::BG_COLOR);
	EndTextureMode();
	SetConstants();
	image = GenImageColor(settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT, settings::BG_COLOR);
	Reset(image);
	ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	timer = 0;

	initialized = true;
}

RenderTexture2D &Fractal::GetCanvas()
{
	return canvas;
}

void Fractal::SaveImage()
{
	if (!DirectoryExists(("pictures/" + name).c_str()))
	{
		std::filesystem::create_directories((GetWorkingDirectory() + std::string("/pictures/") + name).c_str());
	}
	int numFiles = std::distance(std::filesystem::directory_iterator("pictures/" + name), std::filesystem::directory_iterator{});
	ExportImage(LoadImageFromTexture(canvas.texture), ("pictures/" + name + "/" + name + "_" + std::to_string(numFiles) + ".png").c_str());
}

void Fractal::Init()
{
	canvas = LoadRenderTexture(settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT);
	switchYOffset = sliders.size() * Slider::ySize;
}

int Fractal::GetSliderPos(int number)
{
	int i = 0;
	for (auto &slider : sliders)
	{
		if (slider.GetNumber() == number)
		{
			return i;
		}
		i++;
	}
	return -1;
}

void Fractal::Render(Font &font, Camera2D &cam)
{
	SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);
	DrawTextureRec(canvas.texture, (Rectangle){0, 0, (float)settings::IMAGE_WIDTH, (float)-settings::IMAGE_HEIGHT}, settings::DRAW_OFFSET, WHITE);
	DrawRectangleLinesEx({settings::DRAW_OFFSET.x, settings::DRAW_OFFSET.y, settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT}, 2, {255, 255, 255, 50});

	RenderAdditional(font, cam);

	for (auto &slider : sliders)
	{
		slider.Render(font);
	}
	for (auto &switch_ : switches)
	{
		switch_.Render(font, switchYOffset);
	}
	std::string text = "Timer: " + std::to_string(timer);
	float yMeasureTimer = MeasureTextEx(font, text.c_str(), settings::FONT_SIZE, settings::FONT_SPACING).y;
	DrawTextEx(font, text.c_str(), {settings::PARTING + 20, GetScreenHeight() / cam.zoom - (yMeasureTimer + 10)}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);
	coordinate_system.Render(canvas.texture, font);
}

void Fractal::MainUpdate(Camera2D &cam)
{
	if (Update(canvas, image))
	{
		timer += GetFrameTime();
		converted = false;
	}
	else if (!converted)
	{
		Color *pixels = LoadImageColors(image);
		UpdateTexture(canvas.texture, pixels);
		UnloadImageColors(pixels);

		converted = true;
	}

	for (auto &slider : sliders)
	{
		slider.Update(cam);
		if (slider.IsLinked())
		{
			slider.SetMin(sliders[GetSliderPos(slider.GetLinked())].GetValue());
		}
	}

	for (auto &switch_ : switches)
	{
		switch_.Update(switchYOffset, cam);
	}

	if (IsKeyDown(KEY_LEFT_CONTROL))
	{
		if (IsKeyPressed(KEY_R))
		{
			Clear();
		}
		else if (IsKeyPressed(KEY_S))
		{
			SaveImage();
		}
	}
}

Color AddColor(Color srcColor, Color color)
{
	return {
		(unsigned char)(std::min((int)(srcColor.r + (color.a / 255.0f) * color.r), 255)),
		(unsigned char)(std::min((int)(srcColor.g + (color.a / 255.0f) * color.g), 255)),
		(unsigned char)(std::min((int)(srcColor.b + (color.a / 255.0f) * color.b), 255)),
	255};
}

Color AddColor(Image *image, int x, int y, Color color)
{
	return AddColor(GetImageColor(*image, x, y), color);
}
