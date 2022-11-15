#include "raylib.h"
#include "settings.h"
#include "slider.h"
#include "fractal.h"
#include <iostream>
#include <string>
#include <vector>

// include fractal headers from "../fractals/*" here
#include "../fractals/feigenbaum.h"
#include "../fractals/mandelbrot.h"
#include "../fractals/dragon.h"

void Init();
float GetYSwitchOffset(int currentFractal);

int main()
{
	Init();

	for (auto &fractal : fractals)
	{
		fractal.Init();
	}

	int currentFractal = 0;
	int fractalNumber = fractals.size();
	int selectedTab = 0;

	auto updateWindowTitle = [&currentFractal]()
	{
		SetWindowTitle((settings::PROJECT_NAME + " - " + fractals[currentFractal].GetName()).c_str());
	};
	updateWindowTitle();

	Font font = LoadFont("resources/consola.ttf");
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

	fractals[currentFractal].Clear();

	while (!WindowShouldClose())
	{
		fractals[currentFractal].Update(fractals[currentFractal].GetCanvas());

		for (auto &slider : fractals[currentFractal].sliders)
		{
			slider.Update();
			if (slider.IsLinked())
			{
				slider.SetMin(fractals[currentFractal].sliders[fractals[currentFractal].GetSliderPos(slider.GetLinked())].GetValue());
			}
		}

		for (auto &switch_ : fractals[currentFractal].switches)
		{
			switch_.Update(GetYSwitchOffset(currentFractal));
		}

		if (IsKeyDown(KEY_LEFT_CONTROL))
		{
			if (IsKeyPressed(KEY_R))
			{
				fractals[currentFractal].Clear();
			}
			else if (IsKeyPressed(KEY_S))
			{
				fractals[currentFractal].SaveImage();
			}
		}

		selectedTab = 0;
		for (int i = 0; i < fractalNumber; i++)
		{
			if (CheckCollisionPointRec(GetMousePosition(), {i * settings::TABSIZE.x, 0, settings::TABSIZE.x, settings::TABSIZE.y}))
			{
				selectedTab = i + 1;
				if (IsMouseButtonPressed(0))
				{
					currentFractal = i;
					SetWindowTitle((settings::PROJECT_NAME + " - " + fractals[currentFractal].GetName()).c_str());

					if (!fractals[currentFractal].IsInitialized())
					{
						fractals[currentFractal].Clear();
					}
				}
				break;
			}
		}

		BeginDrawing();
		ClearBackground(settings::BG_COLOR);

		DrawTextureRec(fractals[currentFractal].GetCanvas().texture, {0, 0, (float)fractals[currentFractal].GetCanvas().texture.width, (float)fractals[currentFractal].GetCanvas().texture.height}, settings::DRAW_OFFSET, WHITE);
		DrawRectangleLinesEx({settings::DRAW_OFFSET.x, settings::DRAW_OFFSET.y, settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT}, 2, {255, 255, 255, 50});

		fractals[currentFractal].RenderAdditional(fractals[currentFractal].GetCanvas().texture, font);

		for (int i = 0; i < fractalNumber; i++)
		{
			Color tabColor = settings::BG_COLOR_2;
			if (selectedTab == i + 1)
			{
				tabColor = GRAY;
			}
			else if (i == currentFractal)
			{
				tabColor = settings::BG_COLOR;
			}
			
			DrawRectangle(i * settings::TABSIZE.x, 0, settings::TABSIZE.x, settings::TABSIZE.y, tabColor);
			DrawLine((i + 1) * settings::TABSIZE.x, 0, (i + 1) * settings::TABSIZE.x, settings::TABSIZE.y, WHITE);
			std::string name = fractals[i].GetName();
			Vector2 measureName = MeasureTextEx(font, name.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
			DrawTextEx(font, name.c_str(), {(float)(0.5 + i) * settings::TABSIZE.x - measureName.x / 2.0f, settings::TABSIZE.y / 2.0f - measureName.y / 2.0f}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);
		}
		DrawLine(0, settings::TABSIZE.y, currentFractal * settings::TABSIZE.x, settings::TABSIZE.y, WHITE);
		DrawLine(currentFractal * settings::TABSIZE.x + settings::TABSIZE.x, settings::TABSIZE.y, settings::PARTING, settings::TABSIZE.y, WHITE);

		DrawLine(settings::PARTING, 0, settings::PARTING, GetScreenHeight(), WHITE);
		DrawRectangle(settings::PARTING, 0, GetScreenWidth() - settings::PARTING, GetScreenHeight(), settings::BG_COLOR_2);

		for (auto &slider : fractals[currentFractal].sliders)
		{
			slider.Render(font);
		}

		for (auto &switch_ : fractals[currentFractal].switches)
		{
			switch_.Render(font, GetYSwitchOffset(currentFractal));
		}

		std::string hints = "Press STRG + R\nto re-plot the graph!\n\nPress STRG + S\nto save the image!";
		float hintsMeasure = MeasureTextEx(font, hints.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING).y;
		DrawTextEx(font, hints.c_str(), {settings::PARTING + 20, GetScreenHeight() - (hintsMeasure + 50)}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);

		std::string fps = (std::to_string(GetFPS()) + " FPS");
		Vector2 measureFPS = MeasureTextEx(font, fps.c_str(), settings::FONT_SIZE, settings::FONT_SPACING);
		DrawTextEx(font, fps.c_str(), {GetScreenWidth() - measureFPS.x - 10, GetScreenHeight() - measureFPS.y - 5}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);

		EndDrawing();
	}

	for (auto &fractal : fractals)
	{
		UnloadRenderTexture(fractal.GetCanvas());
	}
	CloseWindow();
	return 0;
}

void Init()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTraceLogLevel(LOG_NONE);
	InitWindow(settings::IMAGE_WIDTH + 395, settings::IMAGE_HEIGHT + settings::DRAW_OFFSET.y * 2, settings::PROJECT_NAME.c_str());
	SetWindowMinSize(settings::IMAGE_WIDTH + 395, settings::IMAGE_HEIGHT + settings::DRAW_OFFSET.y * 2);
	SetExitKey(0);
	ChangeDirectory(GetApplicationDirectory());
	SetTargetFPS(120);
}

float GetYSwitchOffset(int currentFractal)
{
	return fractals[currentFractal].sliders.size() * Slider::ySize;
}