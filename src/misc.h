#pragma once

#include "raylib.h"

Vector2 GetMousePositionScaled(Camera2D &cam)
{
	return {GetScreenToWorld2D(GetMousePosition(), cam)};
}
