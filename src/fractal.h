#pragma once
#include "raylib.h"
#include "slider.h"
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

public:
	std::vector<Slider> sliders;
	std::function<void(Texture2D &texture, Font &font)> RenderAdditional;
	std::function<void(RenderTexture2D &canvas)> Update;
	std::function<void()> Reset;

	Fractal(std::string name, std::vector<Slider> &sliders, std::function<void(Texture2D &texture, Font &font)> RenderAdditional, std::function<void(RenderTexture2D &canvas)> Update, std::function<void()> Reset);

	void SetConstants();
	bool IsInitialized();
	std::string &GetName();
	void Clear();
	RenderTexture2D &GetCanvas();
	void SaveImage();
	void Init();
	int GetSliderPos(int number);
};

Fractal::Fractal(std::string name, std::vector<Slider> &sliders, std::function<void(Texture2D &texture, Font &font)> RenderAdditional, std::function<void(RenderTexture2D &canvas)> Update, std::function<void()> Reset)
	: name(name), sliders(sliders), RenderAdditional(RenderAdditional), Update(Update), Reset(Reset)
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
