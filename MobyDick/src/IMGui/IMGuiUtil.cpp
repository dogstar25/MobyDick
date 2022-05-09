#include "IMGuiUtil.h"
#include "../GameConfig.h"

namespace ImGui
{
	void MobyDickInit(Game* mobyDickGame)
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.IniFilename = NULL;

		ImGui::StyleColorsDark();

		//Fonts
		font22 = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 22.0f);
		font12 = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 12.0f);

		auto gl_context = SDL_GL_GetCurrentContext();
		const char* glsl_version = "#version 130";

		// Setup Platform/Renderer backends
		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			ImGui_ImplSDL2_InitForOpenGL(mobyDickGame->window(), gl_context);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		else {
			ImGui_ImplSDL2_InitForSDLRenderer(mobyDickGame->window(), mobyDickGame->renderer()->sdlRenderer());
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
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize = ImVec2{ 4,4 };
		style.WindowPadding = ImVec2{ 0,0 };

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
		flags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("FrameRate", nullptr, flags);
		
		auto frate = ImGui::GetIO().Framerate;
		ImGui::Value("FPS", frate);

		ImGui::PushFont(ImGui::font12);
		ImGui::Text("Todd");
		ImGui::PopFont();
		ImGui::Text("Todd2");
		int dwWidth = GameConfig::instance().windowWidth() / 2;
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::End();

	}

}
