#include "IMGuiUtil.h"
#include "../GameConfig.h"

namespace ImGui
{
	void MobyDickInit(Game* mobyDickGame)
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		//Font
		ImFont* m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 22.0f);

		auto gl_context = SDL_GL_GetCurrentContext();
		const char* glsl_version = "#version 130";

		// Setup Platform/Renderer backends
		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			ImGui_ImplSDL2_InitForOpenGL(mobyDickGame->window(), gl_context);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		else {
			ImGui_ImplSDL2_InitForSDLRenderer(mobyDickGame->window());
			ImGui_ImplSDLRenderer_Init(mobyDickGame->renderer()->sdlRenderer());
		}

	}
	void MobyDickNewFrame()
	{

		{
			if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL2_NewFrame();
			}
			else {
				ImGui_ImplSDLRenderer_NewFrame();
				ImGui_ImplSDL2_NewFrame();
			}
			ImGui::NewFrame();

		}

	}

	void MobyDickRenderFrame()
	{
		ImGui::Render();
		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else {
			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		}

	}

	void MobyDickFPSFrame()
	{

		ImGui::Begin("FrameRate");
		int dwWidth = GameConfig::instance().windowWidth() / 2;
		ImGui::SetWindowPos(ImVec2(dwWidth, 0));
		auto frate = ImGui::GetIO().Framerate;
		ImGui::Value("FPS", frate);
		ImGui::End();

	}

}
