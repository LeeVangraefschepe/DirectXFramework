#include "pch.h"

#if defined(_DEBUG)
#include "vld.h"
#endif

#undef main
#include "Renderer.h"
#include "Camera.h"

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	constexpr uint32_t width = 640;
	constexpr uint32_t height = 480;

	SDL_Window* pWindow = SDL_CreateWindow(
		"DirectX - Lee Vangraefschepe 2DAEGD15N",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	const auto pCamera = new Camera{};
	const auto pTimer = new Timer();
	const auto pRenderer = new Renderer(pWindow, pCamera);

	pCamera->Initialize(static_cast<float>(width) / static_cast<float>(height), 45.f, { 0,0,-50 });

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool showFps{};
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_F2)
				{
					pRenderer->ToggleRotation();
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F3)
				{
					pRenderer->ToggleFireMesh();
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F4)
				{
					pRenderer->CycleSampleStates();
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F9)
				{
					pRenderer->CycleCullModes();
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F10)
				{
					pRenderer->ToggleClearCollor();
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F11)
				{
					showFps = !showFps;
				}
				//Test for a key
				//if (e.key.keysym.scancode == SDL_SCANCODE_X)
				break;
			default: ;
			}
		}

		//--------- Update ---------
		pRenderer->Update(pTimer);
		pCamera->Update(pTimer);

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();
		if (showFps)
		{
			printTimer += pTimer->GetElapsed();
			if (printTimer >= 1.f)
			{
				printTimer = 0.f;
				std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
			}
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;
	delete pCamera;

	ShutDown(pWindow);
	return 0;
}