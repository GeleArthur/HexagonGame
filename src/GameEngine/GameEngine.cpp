#include "GameEngine.h"
#include <GL/glu.h>
#include <chrono>
#include <iostream>
#include <utility>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

GameEngine *GameEngine::_gameEngineSingleton{nullptr};

void GameEngine::Error(std::string message)
{
	_quitting = true;
	std::cout << "Error: " << message << '\n';
	throw;
	exit(-1);
}

GameEngine::GameEngine(): _quitting(false)
{
	_gameEngineSingleton = this;
}

GameEngine::~GameEngine()
{
	delete _game;
}

GameEngine* GameEngine::GetSingleton()
{
	if (_gameEngineSingleton == nullptr) _gameEngineSingleton = new GameEngine();
	return _gameEngineSingleton;
}

void GameEngine::SetGame(AbstractGame *game)
{
	_game = game;
}

void GameEngine::SetWindowSize(const int width, const int height)
{
	_windowWidth = width;
	_windowHeight = height;

	if(_window != nullptr)
	{
		SDL_SetWindowSize(_window, _windowWidth, _windowHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluOrtho2D(0, _windowWidth, _windowHeight, 0);
		glViewport(0, 0, _windowWidth, _windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

float GameEngine::GetWindowWidth()
{
	return float(_windowWidth);
}

float GameEngine::GetWindowHeight()
{
	return float(_windowHeight);
}

void GameEngine::SetTitle(std::string newName)
{
	_windowTitle = std::move(newName);
	if(_window != nullptr)
	{
		SDL_SetWindowTitle(_window, _windowTitle.c_str());
	}
}

void GameEngine::SetVsync(int setting)
{
	_windowVsyncMode = setting;
	if(_window != nullptr)
	{
		if (SDL_GL_SetSwapInterval(_windowVsyncMode) < 0)
			Error(SDL_GetError());
	}
}

void GameEngine::Initialize()
{
	if (_game == nullptr) Error("Use SetGame() before Initialize");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) Error(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	_window = SDL_CreateWindow(
		_windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_windowWidth,
		_windowHeight,
		SDL_WINDOW_OPENGL);

	if (_window == nullptr) Error(SDL_GetError());

	_context = SDL_GL_CreateContext(_window);
	if (_context == nullptr) Error(SDL_GetError());
	
	SetVsync(_windowVsyncMode);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, _windowWidth, _windowHeight, 0);
	glViewport(0, 0, _windowWidth, _windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_keyBoardState = SDL_GetKeyboardState(nullptr);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) Error(IMG_GetError());
	if (TTF_Init() == -1) Error(TTF_GetError());
}

void GameEngine::Run()
{
	SDL_Event e{};
	_game->Start();

	const std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();

	while (!_quitting)
	{
		_mouse.downThisFrame = false;
		_mouse.upThisFrame = false;
		
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				_quitting = true;
				break;
			case SDL_MOUSEMOTION:
				_mouse.x = e.motion.x;
				_mouse.y = e.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				// if(e.button.state)
				_mouse.holdingDown = true;
				_mouse.downThisFrame = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_mouse.holdingDown = false;
				_mouse.upThisFrame = true;
				break;
			default:
				break;
			}
		}

		if (!_quitting)
		{
			std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
			_currentTime = std::chrono::duration<float>(timeNow - startTime).count();
			_deltaTime = std::chrono::duration<float>(timeNow - prevTime).count();
			prevTime = timeNow;

			_game->Update();
			_game->Draw();

			SDL_GL_SwapWindow(_window);
		}
	}
}

void GameEngine::CleanUp()
{
	SDL_GL_DeleteContext(_context);

	SDL_DestroyWindow(_window);
	_window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

float GameEngine::GetDeltaTime() 
{
	return _deltaTime;
}

float GameEngine::GetCurrentTimer() 
{
	return _currentTime;
}

void GameEngine::ApplyCamera()
{
	GLfloat matrix[16]; 
	_camera.openGlArray(matrix);
	glLoadMatrixf(matrix);
}

void GameEngine::SetCameraPosition(Vector2d newPosition)
{
	_camera.m30 = -newPosition.x;
	_camera.m31 = -newPosition.y;
}

Vector2d GameEngine::GetCameraPosition()
{
	return Vector2d{-_camera.m30, -_camera.m31};
}

void GameEngine::SetCameraZoom(Vector2d newScale)
{
	_camera.m00 = newScale.x;
	_camera.m11 = newScale.y;
}

void GameEngine::SetCameraRotationX(float rotation)
{
	_camera.m11 = cos(rotation);
	_camera.m22 = cos(rotation);
	_camera.m12 = -sin(rotation);
	_camera.m21 = sin(rotation);
}

const Mouse & GameEngine::GetMouse()
{
	return _mouse;
}

const Uint8 * GameEngine::GetKeyBoardState()
{
	return _keyBoardState;
}

float GameEngine::Random(float min, float max)
{
	return float(rand()) / RAND_MAX * (max - min) + min;
}

int GameEngine::Random(int min, int max)
{
	return float(rand()) / RAND_MAX * (max - min) + min;
}

void GameEngine::ClearBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GameEngine::SetColor(float r, float g, float b, float a)
{
	glColor4f(r,g,b,a);
}

void GameEngine::DrawLine(float x1, float y1, float x2, float y2, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	{
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}
	glEnd();
}

void GameEngine::DrawLine(const Vector2d &p1, const Vector2d &p2, float lineWidth)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y, lineWidth);
}

void GameEngine::DrawTriangle(const Vector2d &p1, const Vector2d &p2, const Vector2d &p3, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void GameEngine::FillTriangle(const Vector2d &p1, const Vector2d &p2, const Vector2d &p3)
{
	glBegin(GL_TRIANGLES);
	{
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void GameEngine::DrawRect(float left, float top, float width, float height, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(left, top);
		glVertex2f(left + width, top);
		glVertex2f(left + width, top + height);
		glVertex2f(left, top + height);
	}
	glEnd();
}

void GameEngine::DrawRect(const Vector2d &topLeft, float width, float height, float lineWidth)
{
	DrawRect(topLeft.x, topLeft.y, width, height, lineWidth);
}

void GameEngine::FillRect(float left, float top, float width, float height)
{
	glBegin(GL_POLYGON);
	{
		glVertex2f(left, top);
		glVertex2f(left + width, top);
		glVertex2f(left + width, top + height);
		glVertex2f(left, top + height);
	}
	glEnd();
}

void GameEngine::FillRect(const Vector2d &topLeft, float width, float height)
{
	FillRect(topLeft.x, topLeft.y, width, height);

}

void GameEngine::DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth)
{
	float dAngle{ radX > radY ? float(M_PI / radX) : float(M_PI / radY) };

	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	{
		for (float angle = 0.0; angle < float(2 * M_PI); angle += dAngle)
		{
			glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
		}
	}
	glEnd();
}

void GameEngine::DrawEllipse(const Vector2d &center, float radX, float radY, float lineWidth)
{
	DrawEllipse(center.x, center.y, radX, radY, lineWidth);
}

void GameEngine::FillEllipse(float centerX, float centerY, float radX, float radY)
{
	float dAngle{ radX > radY ? float(M_PI / radX) : float(M_PI / radY) };

	glBegin(GL_POLYGON);
	{
		for (float angle = 0.0; angle < float(2 * M_PI); angle += dAngle)
		{
			glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
		}
	}
	glEnd();
}

void GameEngine::FillEllipse(const Vector2d &center, float radX, float radY)
{
	FillEllipse(center.x, center.y, radX, radY);
}

void GameEngine::DrawPolygon(const std::vector<Vector2d> &vertices, bool closed, float lineWidth)
{
	DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
}

void GameEngine::DrawPolygon(const Vector2d *pVertices, size_t nrVertices, bool closed, float lineWidth)
{
	glLineWidth(lineWidth);
	closed ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
	{
		for (size_t idx{ 0 }; idx < nrVertices; ++idx)
		{
			glVertex2f(pVertices[idx].x, pVertices[idx].y);
		}
	}
	glEnd();
}

void GameEngine::FillPolygon(const std::vector<Vector2d> &vertices)
{
	FillPolygon(vertices.data(), vertices.size());
}

void GameEngine::FillPolygon(const Vector2d *pVertices, size_t nrVertices)
{
	glBegin(GL_POLYGON);
	{
		for (size_t idx{ 0 }; idx < nrVertices; ++idx)
		{
			glVertex2f(pVertices[idx].x, pVertices[idx].y);
		}
	}
	glEnd();
}

