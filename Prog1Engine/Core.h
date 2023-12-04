#pragma once

extern float g_WindowWidth;
extern float g_WindowHeight;
extern std::string g_WindowTitle;

#pragma region coreDeclarations
// Functions
void Initialize();
void Run();
void Cleanup();
void QuitOnSDLError();
void QuitOnOpenGlError();
void QuitOnImageError();
void QuitOnTtfError();

#pragma endregion coreDeclarations
