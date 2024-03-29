#include "GameEngine.h"
#include <chrono>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <utility>
#include <GL/gl.h>

GameEngine *GameEngine::m_gameEngineSingleton{nullptr};

void GameEngine::Error(std::string message)
{
	m_quitting = true;
	std::cout << "Error: " << message << '\n';
	throw;
	exit(-1);
}

void GameEngine::CameraDragUpdate()
{
	Mouse currentMouse{GE->GetMouse()};
	if(currentMouse.left.downThisFrame)
	{
		m_mousePrevFrame = currentMouse.position;
	}
	if(currentMouse.left.holdingDown)
	{
		Vector2d currentPosition{GE->GetCameraPosition()};
		currentPosition -= currentMouse.position - m_mousePrevFrame;
		GE->SetCameraPosition(currentPosition);
		GE->ApplyCamera();

		m_mousePrevFrame = currentMouse.position;
	}
}

GameEngine::GameEngine(): m_quitting(false)
{
	m_gameEngineSingleton = this;
}

GameEngine::~GameEngine()
{
	delete m_game;
}

GameEngine* GameEngine::GetSingleton()
{
	if (m_gameEngineSingleton == nullptr) m_gameEngineSingleton = new GameEngine();
	return m_gameEngineSingleton;
}

void GameEngine::SetGame(AbstractGame *game)
{
	m_game = game;
}

void GameEngine::SetWindowSize(const int width, const int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	if (m_window != nullptr)
	{
		SDL_SetWindowSize(m_window, m_windowWidth, m_windowHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, m_windowWidth, m_windowHeight, 0, -1, 1);

		glViewport(0, 0, m_windowWidth, m_windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

float GameEngine::GetWindowWidth()
{
	return static_cast<float>(m_windowWidth);
}

float GameEngine::GetWindowHeight()
{
	return static_cast<float>(m_windowHeight);
}

void GameEngine::SetTitle(std::string newName)
{
	m_windowTitle = std::move(newName);
	if (m_window != nullptr)
	{
		SDL_SetWindowTitle(m_window, m_windowTitle.c_str());
	}
}

void GameEngine::SetVsync(int setting)
{
	m_windowVsyncMode = setting;
	if (m_window != nullptr)
	{
		if (SDL_GL_SetSwapInterval(m_windowVsyncMode) < 0) Error(SDL_GetError());
	}
}

void GameEngine::Initialize()
{
	if (m_game == nullptr) Error("Use SetGame() before Initialize");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) Error(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	m_window = SDL_CreateWindow(
		m_windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_windowWidth,
		m_windowHeight,
		SDL_WINDOW_OPENGL);

	if (m_window == nullptr) Error(SDL_GetError());

	m_context = SDL_GL_CreateContext(m_window);
	if (m_context == nullptr) Error(SDL_GetError());

	SetVsync(m_windowVsyncMode);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, m_windowWidth, m_windowHeight, 0, -1, 1);
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_keyBoardState = SDL_GetKeyboardState(nullptr);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) Error(IMG_GetError());
	if (TTF_Init() == -1) Error(TTF_GetError());
}

void GameEngine::Run()
{
	SDL_Event e{};
	m_game->Start();

	const std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();

	while (!m_quitting)
	{
		m_mouse.left.downThisFrame = false;
		m_mouse.left.upThisFrame = false;
		m_mouse.right.downThisFrame = false;
		m_mouse.right.upThisFrame = false;
		m_mouse.wheel.y = 0;
		m_mouse.wheel.x = 0;

		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				m_quitting = true;
				break;
			case SDL_MOUSEMOTION:
				m_mouse.position = Vector2d{static_cast<float>(e.motion.x), static_cast<float>(e.motion.y)};
				break;
			case SDL_MOUSEBUTTONDOWN:
				// if(e.button.state)
				if(e.button.button == '\x1')
				{
					m_mouse.left.holdingDown = true;
					m_mouse.left.downThisFrame = true;
				}
				if(e.button.button == '\x3')
				{
					m_mouse.right.holdingDown = true;
					m_mouse.right.downThisFrame = true;
				}

				break;
			case SDL_MOUSEBUTTONUP:
				if(e.button.button == '\x1')
				{
					m_mouse.left.holdingDown = false;
					m_mouse.left.upThisFrame = true;
				}
				if(e.button.button == '\x3')
				{
					m_mouse.right.holdingDown = false;
					m_mouse.right.upThisFrame = true;
				}
				break;
			case SDL_MOUSEWHEEL:
				m_mouse.wheel = Vector2d{float(e.wheel.preciseX), float(e.wheel.preciseY)};
				break;
			default:
				break;
			}
		}

		if (!m_quitting)
		{
			std::chrono::steady_clock::time_point timeNow = std::chrono::steady_clock::now();
			m_currentTime = std::chrono::duration<float>(timeNow - startTime).count();
			m_deltaTime = std::chrono::duration<float>(timeNow - prevTime).count();
			prevTime = timeNow;

			if(m_isCameraDragEnabled)
				CameraDragUpdate();
			m_game->Update();
			m_game->Draw();
			DrawUi();

			SDL_GL_SwapWindow(m_window);
		}
	}
}

void GameEngine::CleanUp()
{
	SDL_GL_DeleteContext(m_context);

	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// I am so smart
void GameEngine::DrawUi()
{
	GLfloat matrix[16];
	Matrix4x4::IdentityMatrix().OpenGlArray(matrix);
	glLoadMatrixf(matrix);
	m_game->DrawUi();

	ApplyCamera();
}

float GameEngine::GetDeltaTime()
{
	return m_deltaTime;
}

float GameEngine::GetCurrentTimer()
{
	return m_currentTime;
}

void GameEngine::ApplyCamera()
{
	GLfloat matrix[16];
	m_camera.OpenGlArray(matrix);
	glLoadMatrixf(matrix);
}

void GameEngine::SetCameraPosition(Vector2d newPosition)
{
	m_camera.m30 = -newPosition.x;
	m_camera.m31 = -newPosition.y;
}

Vector2d GameEngine::GetCameraPosition() const
{
	return Vector2d{-m_camera.m30, -m_camera.m31};
}

void GameEngine::SetCameraZoom(Vector2d newScale)
{
	m_camera.m00 = newScale.x;
	m_camera.m11 = newScale.y;
}

void GameEngine::SetCameraRotationX(float rotation)
{
	Matrix4x4 rotationMatrix{Matrix4x4::IdentityMatrix()};
	rotationMatrix.m00 = cos(rotation);
	rotationMatrix.m10 = -sin(rotation);
	rotationMatrix.m01 = sin(rotation);
	rotationMatrix.m11 = cos(rotation);
	
	m_camera = m_camera * rotationMatrix;
}

void GameEngine::EnableCameraDrag(bool isEnabled)
{
	m_isCameraDragEnabled = isEnabled;
}

const Matrix4x4 & GameEngine::GetCameraMatrix() const
{
	return m_camera;
}

const Mouse& GameEngine::GetMouse()
{
	return m_mouse;
}

const Uint8* GameEngine::GetKeyBoardState()
{
	return m_keyBoardState;
}

float GameEngine::Random(float min, float max)
{
	return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}

int GameEngine::Random(int min, int max)
{
	return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}

void GameEngine::ClearBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GameEngine::SetColor(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
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

void GameEngine::DrawRect(Rect rect)
{
	DrawRect(rect.left, rect.top, rect.width, rect.height);
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
	float dAngle{radX > radY ? static_cast<float>(M_PI / radX) : static_cast<float>(M_PI / radY)};

	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	{
		for (float angle = 0.0; angle < static_cast<float>(2 * M_PI); angle += dAngle)
		{
			glVertex2f(centerX + radX * static_cast<float>(cos(angle)),
			           centerY + radY * static_cast<float>(sin(angle)));
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
	float dAngle{radX > radY ? static_cast<float>(M_PI / radX) : static_cast<float>(M_PI / radY)};

	glBegin(GL_POLYGON);
	{
		for (float angle = 0.0; angle < static_cast<float>(2 * M_PI); angle += dAngle)
		{
			glVertex2f(centerX + radX * static_cast<float>(cos(angle)),
			           centerY + radY * static_cast<float>(sin(angle)));
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
		for (size_t idx{0}; idx < nrVertices; ++idx)
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
		for (size_t idx{0}; idx < nrVertices; ++idx)
		{
			glVertex2f(pVertices[idx].x, pVertices[idx].y);
		}
	}
	glEnd();
}

bool GameEngine::TextureFromFile(const std::string &path, Texture &texture)
{
	//Load file for use as an image in a new surface.
	SDL_Surface* pLoadedSurface = IMG_Load((path).c_str());
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError() << std::endl;
		return false;
	}

	TextureFromSurface(pLoadedSurface, texture);

	//Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

	return true;
}

bool GameEngine::TextureFromString(const std::string &text, TTF_Font *pFont, Texture &texture)
{
	if (pFont == nullptr)
	{
		std::cerr << "Texture::CreateFromString, invalid TTF_Font pointer\n";
		return false;
	}

	//Render text surface
	SDL_Color textColor;
	textColor.r = Uint8(255);
	textColor.g = Uint8(255);
	textColor.b = Uint8(255);
	textColor.a = Uint8(255);

	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
	//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
		return false;
	}

	// copy to video memory
	TextureFromSurface(pLoadedSurface, texture);

	//Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

	return true;
}

bool GameEngine::TextureFromString(const std::string &text, const std::string &fontPath, int ptSize, Texture &texture)
{
	// Create font
	TTF_Font* pFont{};
	pFont = TTF_OpenFont((fontPath).c_str(), ptSize);
	if (pFont == nullptr)
	{
		std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError();
		std::cin.get();
		return false;
	}

	// Create texture using this fontand close font afterwards
	bool textureOk = TextureFromString(text, pFont, texture);
	TTF_CloseFont(pFont);

	return textureOk;
}

void GameEngine::TextureFromSurface(const SDL_Surface *pSurface, Texture &texture)
{
	//Get image dimensions
	texture.width = float(pSurface->w);
	texture.height = float(pSurface->h);

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
	case 3:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGBA;
		}
		else
		{
			pixelFormat = GL_BGRA;
		}
		break;
	default:
		std::cerr << "TextureFromSurface error: Unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";;
		texture.width = 0;
		texture.height = 0;
		return;
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, &texture.id);

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);

	// check for errors.
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		std::cerr << "TextureFromSurface, error binding textures, Error id = " << e << '\n';
		std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static Texture object).\n";
		std::cerr << "There might be a white rectangle instead of the image.\n";
		texture.width = 0;
		texture.height = 0;
		return;
	}

	//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
	//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//         GL_RGBA:    Specifies the number of color components in the texture.
	//                     This is how OpenGL will store the texture internally (kinda)--
	//                     It's essentially the texture's type.
	//      surface->w:    The width of the texture
	//      surface->h:    The height of the texture
	//               0:    The border.  Don't worry about this if you're just starting.
	//     pixelFormat:    The format that the *data* is in--NOT the texture! 
	//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
	// surface->pixels:    The actual data.  As above, SDL's array of bytes.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void GameEngine::DeleteTexture(Texture &texture)
{
	glDeleteTextures(1, &texture.id);
}

void GameEngine::DrawTexture(const Texture &texture, const Vector2d &dstTopLeft, const Rect &srcRect)
{
	Rect dstRect{ dstTopLeft.x, dstTopLeft.y, srcRect.width, srcRect.height };
	DrawTexture(texture, dstRect, srcRect);
}

void GameEngine::DrawTexture(const Texture &texture, const Rect &dstRect, const Rect &srcRect)
{
	// Determine texture coordinates using srcRect and default destination width and height
	float textLeft;
	float textRight;
	float textTop;
	float textBottom;

	float defaultDstWidth;
	float defaultDstHeight;
	if (srcRect.width <= 0.0f && srcRect.height <= 0.0f) // No srcRect specified
	{
		// Use complete texture
		textLeft = 0.0f;
		textRight = 1.0f;
		textTop = 0.0f;
		textBottom = 1.0f;

		defaultDstHeight = texture.height;
		defaultDstWidth = texture.width;
	}
	else // srcRect specified
	{
		textLeft = srcRect.left / texture.width;
		textRight = (srcRect.left + srcRect.width) / texture.width;
		textTop = srcRect.top / texture.height;
		textBottom = (srcRect.top + srcRect.height) / texture.height;

		defaultDstHeight = srcRect.height;
		defaultDstWidth = srcRect.width;
	}

	// Determine vertex coordinates
	float vertexTop{dstRect.top};
	float vertexLeft{dstRect.left};
	float vertexBottom{};
	float vertexRight{};
	if (!(dstRect.width > 0.001f && dstRect.height > 0.001f)) // If no size specified use default size
	{
		vertexRight = vertexLeft + defaultDstWidth;
		vertexBottom = vertexTop + defaultDstHeight;
	}
	else
	{
		vertexRight = vertexLeft + dstRect.width;
		vertexBottom = vertexTop + dstRect.height;
	}

	// Tell opengl which texture we will use
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Draw
	glEnable(GL_TEXTURE_2D);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(textLeft, textBottom);
			glVertex2f(vertexLeft, vertexBottom);

			glTexCoord2f(textLeft, textTop);
			glVertex2f(vertexLeft, vertexTop);

			glTexCoord2f(textRight, textTop);
			glVertex2f(vertexRight, vertexTop);

			glTexCoord2f(textRight, textBottom);
			glVertex2f(vertexRight, vertexBottom);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}