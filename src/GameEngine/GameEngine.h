#pragma once
#include <SDL_ttf.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <SDL_video.h>
#include <unordered_map>
#include <vector>

#include "AbstractGame.h"
#include "../Utils/Matrix.h"
#include "../Utils/Mouse.h"
#include "../Utils/Texture.h"
#include "../Utils/Vector2d.h"
#include "../Utils/Rect.h"
#define GE (GameEngine::GetSingleton())

class GameEngine
{
	bool m_quitting;
	SDL_Window *m_window{};
	AbstractGame *m_game{};
	SDL_GLContext m_context{};
	static GameEngine *m_gameEngineSingleton;
	
	float m_deltaTime;
	float m_currentTime;
	Mouse m_mouse{};
	Matrix4x4 m_camera{Matrix4x4::IdentityMatrix()};
	bool m_isCameraDragEnabled{};

	std::string m_windowTitle{"GAME!!!"};
	int m_windowWidth{400};
	int m_windowHeight{400};
	int m_windowVsyncMode{1};
	const Uint8* m_keyBoardState{};

	void Error(std::string message);

	Vector2d m_mousePrevFrame;
	void CameraDragUpdate();

public:
	GameEngine();
	~GameEngine();

	static GameEngine* GetSingleton();

	void Initialize();
	void Run();
	void CleanUp();
	void DrawUi();

	void SetGame(AbstractGame *game);
	void SetWindowSize(int width, int height);
	float GetWindowWidth();
	float GetWindowHeight();
	void SetTitle(std::string newName);
	void SetVsync(int setting);
	
	float GetDeltaTime();
	float GetCurrentTimer();

	void ApplyCamera();
	void SetCameraPosition(Vector2d newPosition);
	Vector2d GetCameraPosition() const;
	void SetCameraZoom(Vector2d newScale);
	void SetCameraRotationX(float rotation);
	void EnableCameraDrag(bool isEnabled);
	const Matrix4x4& GetCameraMatrix() const;

	const Mouse& GetMouse();
	const Uint8* GetKeyBoardState();

	float Random(float min, float max);
	int Random(int min, int max);

#pragma region Drawing
	void ClearBackground(float r, float g, float b);
	void SetColor(float r, float g, float b, float a = 1);
	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth = 1.0f);
	void DrawLine(const Vector2d& p1, const Vector2d& p2, float lineWidth = 1.0f);

	void DrawTriangle(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3, float lineWidth = 1);
	void FillTriangle(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3);
	
	void DrawRect(float left, float top, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Vector2d& topLeft, float width, float height, float lineWidth = 1.0f);
	void DrawRect(Rect rect);

	void FillRect(float left, float top, float width, float height);
	void FillRect(const Vector2d& topLeft, float width, float height);

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Vector2d& center, float radX, float radY, float lineWidth = 1.0f);
	void FillEllipse(float centerX, float centerY, float radX, float radY);
	void FillEllipse(const Vector2d& center, float radX, float radY);

	void DrawPolygon(const std::vector<Vector2d>& vertices, bool closed = true, float lineWidth = 1.0f);
	void DrawPolygon(const Vector2d* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f);
	void FillPolygon(const std::vector<Vector2d>& vertices);
	void FillPolygon(const Vector2d* pVertices, size_t nrVertices);

	bool TextureFromFile(const std::string& path, Texture& texture);
	bool TextureFromString(const std::string& text, TTF_Font* pFont, /*const Color4f& textColor,*/ Texture& texture);
	bool TextureFromString(const std::string& text, const std::string& fontPath, int ptSize, /*const Color4f& textColor,*/ Texture& texture);
	void TextureFromSurface(const SDL_Surface* pSurface, Texture& textureData);
	void DrawTexture(const Texture& texture, const Vector2d& dstTopLeft, const Rect& srcRect = Rect{0,0,0,0});
	void DrawTexture(const Texture& texture, const Rect& dstRect, const Rect& srcRect = {});
	void DeleteTexture(Texture& texture);

#pragma endregion  Drawing

};
