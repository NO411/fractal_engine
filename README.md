# Fractal Engine
Made with Raylib.

Installed fractals:
- Dragon curve
- Mandelbrot set
- Bifurcation diagram
- Buddhabrot

<p float="left">
  <img src="https://github.com/NO411/fractal_engine/blob/main/pictures/Dragon/Dragon_0.png?raw=true" width="300" />
  <img src="https://github.com/NO411/fractal_engine/blob/main/pictures/Mandelbrot/Mandelbrot_0.png" width="300" />
</p>

<p float="left">
  <img src="https://github.com/NO411/fractal_engine/blob/main/pictures/Dragon/Dragon_1.png" width="200" />
  <img src="https://github.com/NO411/fractal_engine/blob/main/pictures/Feigenbaum/Feigenbaum_0.png" width="200" />
  <img src="https://github.com/NO411/fractal_engine/blob/main/pictures/Feigenbaum/Feigenbaum_1.png" width="200" />
</p>

<p float="left">
  <img src="https://raw.githubusercontent.com/NO411/fractal_engine/main/pictures/Buddhabrot/Buddhabrot_0.png" width="600" />
</p>

## Installation

### Linux

#### Ubuntu

Navigate to the folder where you want to install fractal engine.

```
cd <path to your installation folder>
```

Then run:

```
sudo apt install g++ git make libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev && git clone https://github.com/NO411/fractal_engine && git clone https://github.com/raysan5/raylib && cd raylib/src && make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED && sudo make install RAYLIB_LIBTYPE=SHARED && cd ../../fractal_engine/src && g++ main.cpp -o ../fractal_engine -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -std=c++2a && ./../fractal_engine
```

To run the program after installing it in your installation folder, type:
```
cd ./fractal_engine/fractal_engine
```

### Windows

Download the fractal_enigne source code and extract the .zip file.
In the extracted folder, create a folder named `include` and a folder named `lib` in the `src` folder.

Download and compile Raylib:

Download [w64devkit](https://github.com/skeeto/w64devkit/releases/download/v1.11.0/w64devkit-1.11.0.zip) and unzip the folder.
On your Windows system, type `env` in the search box and click Enter. Click on `Environment Variables`. Under `System Variables`, click the Path variable. In the new dialog window, copy the pathname of your extracted `w64devkit/bin` folder and paste it into a new path.

You should now have a finished g++ compiler. To prove this, run the `w64devkit.exe` and search for `cmd` in the Windows search box and press Enter. Run `g++ --version`.

Download and unzip [Raylib](https://github.com/raysan5/raylib).

From the command line, navigate to your raylib folder:
```
cd <path to your Raylib folder>/src
```
and run
```
mingw32-make PLATFORM=PLATFORM_DESKTOP
```

Copy the `libraylib.a` file from the `raylib/src` folder to your initially created `lib` folder and copy the `raylib.h` file to your `include` folder.

To create the final fractal_engine.exe, navigate to your `fractal_engine/src` folder on the command line and run:

```
g++ main.cpp -o ../fractal_engine.exe -O1 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
```

Or just follow this [tutorial](https://www.youtube.com/watch?v=HPDLTQ4J_zQ).

## Make your own fractal

All Raylib functions available.
Use `GetMousePositionScaled(Camera2D &cam)` to get the mouse position (the window is scaled to the screen).

Template `name.h`:
```
#pragma once
#include "../src/fractal.h"

namespace name
{
	// constants / variables here

	std::vector<Slider> sliders = {
		// sliders depending on variables above
	};

	std::vector<Switch> switches = {
		// switches depending on variables above
	};

	CoordinateSystem coordinate_system;

	void RenderAdditional(Font &font, Camera2D &cam)
	{
		// draw to hole screen
	}

	bool Update(RenderTexture2D &canvas, Image &image) // will update timer if true returned
	{
		// iterate and draw to the image!
		// only update canvas.texture if needed (see dragon.h example) using:
		/*
		Color *pixels = LoadImageColors(image);
		UpdateTexture(canvas.texture, pixels);
		UnloadImageColors(pixels);
		*/
	}

	void Reset(Image &image)
	{
		// called when STRG + R is pressed
	}

	// this passes the fractal to the main program
	Fractal fractal("name", sliders, switches, coordinate_system, RenderAdditional, Update, Reset);
}
```
- move the created header file to `fractal_engine/fractals`
- add include to `fractal_engine/src/main.cpp`, e.g. `#include "../fractals/name.h"`
- recompile with installation command

## TODO:
- [ ] autozoom box
- [ ] only save file when it doesnt already exist
- [ ] optimize buddhabrot