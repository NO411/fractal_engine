#pragma once
#include "raylib.h"
#include "slider.h"
#include "switch.h"
#include "misc.h"
#include "coordinate_system.h"
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

	float switchYOffset;

public:
	std::vector<Slider> sliders;
	std::vector<Switch> switches;
	CoordinateSystem coordinate_system;

	std::function<void(Texture2D &texture, Font &font, Camera2D &cam)> RenderAdditional;
	std::function<void(RenderTexture2D &canvas)> Update;
	std::function<void()> Reset;

	Fractal(std::string name, std::vector<Slider> &sliders, std::vector<Switch> &switches, CoordinateSystem coordinate_system, std::function<void(Texture2D &texture, Font &font, Camera2D &cam)> RenderAdditional, std::function<void(RenderTexture2D &canvas)> Update, std::function<void()> Reset);

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

Fractal::Fractal(std::string name, std::vector<Slider> &sliders, std::vector<Switch> &switches, CoordinateSystem coordinate_system, std::function<void(Texture2D &texture, Font &font, Camera2D &cam)> RenderAdditional, std::function<void(RenderTexture2D &canvas)> Update, std::function<void()> Reset)
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
	Reset();
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
	DrawTextureEx(canvas.texture, settings::DRAW_OFFSET, 0, 1, WHITE);
	DrawRectangleLinesEx({settings::DRAW_OFFSET.x, settings::DRAW_OFFSET.y, settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT}, 2, {255, 255, 255, 50});

	RenderAdditional(canvas.texture, font, cam);

	for (auto &slider : sliders)
	{
		slider.Render(font);
	}
	for (auto &switch_ : switches)
	{
		switch_.Render(font, switchYOffset);
	}
	coordinate_system.Render(canvas.texture, font);
}

void Fractal::MainUpdate(Camera2D &cam)
{
	std::cout << switchYOffset << std::endl;
	Update(canvas);

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