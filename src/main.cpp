#include "raylib.h"
#include "settings.h"
#include "slider.h"
#include "fractal.h"
#include "misc.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

// include fractal headers from "../fractals/*" here
#include "../fractals/feigenbaum.h"
#include "../fractals/mandelbrot.h"
#include "../fractals/dragon.h"

float GetYSwitchOffset(int currentFractal);

int main()
{
	// init
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTraceLogLevel(LOG_NONE);
	InitWindow(settings::MIN_ORIG_WINDOW_WIDTH, settings::MIN_ORIG_WINDOW_HEIGHT, settings::PROJECT_NAME.c_str());
	Camera2D cam = {0};
	cam.zoom = std::min(std::min((float)(GetMonitorWidth(GetCurrentMonitor()) - 100.0f) / (float)GetScreenWidth(), (float)(GetMonitorHeight(GetCurrentMonitor()) - 100.0f)/ (float)GetScreenHeight()), 1.0f);
	SetWindowMinSize((settings::MIN_ORIG_WINDOW_WIDTH) * cam.zoom, (settings::MIN_ORIG_WINDOW_HEIGHT) * cam.zoom);
	SetWindowSize((settings::MIN_ORIG_WINDOW_WIDTH) * cam.zoom, (settings::MIN_ORIG_WINDOW_HEIGHT) * cam.zoom);
	SetExitKey(0);
	ChangeDirectory(GetApplicationDirectory());
	SetTargetFPS(120);
	//

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
		if (IsWindowResized())
		{
			cam.zoom = std::min(std::min((float)GetScreenWidth() / (float)(settings::MIN_ORIG_WINDOW_WIDTH), GetScreenHeight() / (float)(settings::MIN_ORIG_WINDOW_HEIGHT)), 1.0f);
		}

		fractals[currentFractal].Update(fractals[currentFractal].GetCanvas());

		for (auto &slider : fractals[currentFractal].sliders)
		{
			slider.Update(cam);
			if (slider.IsLinked())
			{
				slider.SetMin(fractals[currentFractal].sliders[fractals[currentFractal].GetSliderPos(slider.GetLinked())].GetValue());
			}
		}

		for (auto &switch_ : fractals[currentFractal].switches)
		{
			switch_.Update(GetYSwitchOffset(currentFractal), cam);
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
			if (CheckCollisionPointRec(GetMousePositionScaled(cam), {i * settings::TABSIZE.x, 0, settings::TABSIZE.x, settings::TABSIZE.y}))
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
		BeginMode2D(cam);

		ClearBackground(settings::BG_COLOR);

		SetTextureFilter(fractals[currentFractal].GetCanvas().texture, TEXTURE_FILTER_BILINEAR);
		DrawTextureEx(fractals[currentFractal].GetCanvas().texture, settings::DRAW_OFFSET, 0, 1, WHITE);
		DrawRectangleLinesEx({settings::DRAW_OFFSET.x, settings::DRAW_OFFSET.y, settings::IMAGE_WIDTH, settings::IMAGE_HEIGHT}, 2, {255, 255, 255, 50});

		fractals[currentFractal].RenderAdditional(fractals[currentFractal].GetCanvas().texture, font, cam);

		for (int i = 0; i < fractalNumber; i++)
		{
			Color tabColor = settings::BG_COLOR_2;
			if (selectedTab == i + 1 && selectedTab - 1 != currentFractal)
			{
				tabColor = GRAY;
			}
			else if (i == currentFractal)
			{
				tabColor = settings::BG_COLOR;
			}
			
			DrawRectangle(i * settings::TABSIZE.x, 0, settings::TABSIZE.x, settings::TABSIZE.y, tabColor);
			DrawLine((i + 1) * settings::TABSIZE.x - 1, 0, (i + 1) * settings::TABSIZE.x - 1, settings::TABSIZE.y, WHITE);
			std::string name = fractals[i].GetName();
			Vector2 measureName = MeasureTextEx(font, name.c_str(), settings::FONT_SIZE_2, settings::FONT_SPACING);
			DrawTextEx(font, name.c_str(), {(float)(0.5 + i) * settings::TABSIZE.x - measureName.x / 2.0f, settings::TABSIZE.y / 2.0f - measureName.y / 2.0f}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);
		}

		DrawLine(0, settings::TABSIZE.y, currentFractal * settings::TABSIZE.x - 0.5, settings::TABSIZE.y, WHITE);
		DrawLine(currentFractal * settings::TABSIZE.x + settings::TABSIZE.x - 1.5, settings::TABSIZE.y, settings::PARTING, settings::TABSIZE.y, WHITE);

		DrawRectangle(settings::PARTING, 0, GetScreenWidth() / cam.zoom - settings::PARTING, GetScreenHeight() / cam.zoom, settings::BG_COLOR_2);
		DrawLine(settings::PARTING, 0, settings::PARTING, GetScreenHeight() / cam.zoom, WHITE);

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
		DrawTextEx(font, hints.c_str(), {settings::PARTING  + 20, GetScreenHeight() / cam.zoom - (hintsMeasure + 50)}, settings::FONT_SIZE_2, settings::FONT_SPACING, WHITE);

		std::string fps = (std::to_string(GetFPS()) + " FPS");
		Vector2 measureFPS = MeasureTextEx(font, fps.c_str(), settings::FONT_SIZE, settings::FONT_SPACING);
		DrawTextEx(font, fps.c_str(), {GetScreenWidth() / cam.zoom - measureFPS.x - 10, GetScreenHeight() / cam.zoom - measureFPS.y - 5}, settings::FONT_SIZE, settings::FONT_SPACING, WHITE);

		EndMode2D();
		EndDrawing();
	}

	for (auto &fractal : fractals)
	{
		UnloadRenderTexture(fractal.GetCanvas());
	}
	CloseWindow();
	return 0;
}

float GetYSwitchOffset(int currentFractal)
{
	return fractals[currentFractal].sliders.size() * Slider::ySize;
}
