#pragma once
#include "pch.h"
#include "Core.h"
#include <iostream>

#include <chrono>

// Variables
SDL_Window* g_pWindow; // The window we'll be rendering to
SDL_GLContext g_pContext; // OpenGL context
Uint32 g_MilliSeconds{};
const Uint32 g_MaxElapsedTime{ 100 }; // limit in case of breakpoint

#pragma region windowInformation

bool g_IsVSyncOn{ true };
#pragma endregion windowInformation

#pragma region coreImplementations


void Initialize()
{
	
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		QuitOnSDLError();
	}

	//Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create window
	g_pWindow = SDL_CreateWindow(
		g_WindowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int(g_WindowWidth),
		int(g_WindowHeight),
		SDL_WINDOW_OPENGL);

	if (g_pWindow == nullptr)
	{
		QuitOnSDLError();
	}

	// Create an opengl context and attach it to the window 
	g_pContext = SDL_GL_CreateContext(g_pWindow);
	if (g_pContext == nullptr)
	{
		QuitOnSDLError();
	}

	if (g_IsVSyncOn)
	{
		// Synchronize buffer swap with the monitor's vertical refresh
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			QuitOnSDLError();
		}
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}

	// Initialize Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping (viewing) area's left, right, top and top
	gluOrtho2D(0, g_WindowWidth, g_WindowHeight, 0);// y from top to top

	// The viewport is the rectangular region of the window where the image is drawn.
	// Set it to the entire client area of the created window
	glViewport(0, 0, int(g_WindowWidth), int(g_WindowHeight));

	//Initialize Modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		QuitOnImageError();
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		QuitOnTtfError();
	}

}

extern void Start();
extern void Update(float elapsedSec);
extern void Draw();
extern void End();

extern void OnKeyDownEvent(SDL_Keycode key);
extern void OnKeyUpEvent(SDL_Keycode key);
extern void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
extern void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
extern void OnMouseUpEvent(const SDL_MouseButtonEvent& e);



void Run()
{
	//Main loop flag
	bool quit{ false };

	// Set start time
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	Start();

	//The event loop
	SDL_Event e{};
	while (!quit)
	{
		// Poll next event from queue
		while (SDL_PollEvent(&e) != 0)
		{
			// Handle the polled event
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				OnKeyDownEvent(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				OnKeyUpEvent(e.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				OnMouseMotionEvent(e.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				OnMouseDownEvent(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				OnMouseUpEvent(e.button);
				break;
			default:
				//std::cout << "\n Warning: This event is not captured. \n";
				break;
			}
		}

		if (!quit)
		{

			// Get current time
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

			// Calculate elapsed time
			float elapsedSeconds = std::chrono::duration<float>(t2 - t1).count();

			// Update current time
			t1 = t2;

			// Prevent jumps in time caused by break points
			if (elapsedSeconds > g_MaxElapsedTime)
			{
				elapsedSeconds = g_MaxElapsedTime;
			}

			// Call update function, using time in seconds (!)
			Update(elapsedSeconds);

			// Draw in the back buffer
			Draw();

			// Update the window: swap back and front buffer
			SDL_GL_SwapWindow(g_pWindow);
		}
	}
	End();
}

void Cleanup()
{
	SDL_GL_DeleteContext(g_pContext);

	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void QuitOnSDLError()
{
	std::cout << "Problem during SDL initialization: ";
	std::cout << SDL_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}

void QuitOnOpenGlError()
{
	std::cout << "Problem during OpenGL initialization: ";
	std::cout << SDL_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}

void QuitOnImageError()
{
	std::cout << "Problem during SDL_image initialization: ";
	std::cout << IMG_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}

void QuitOnTtfError()
{
	std::cout << "Problem during SDL_ttf initialization: ";
	std::cout << TTF_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}

#pragma endregion coreImplementations