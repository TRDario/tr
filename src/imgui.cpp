#include "../include/tr/imgui.hpp"
#include "../include/tr/sysgfx/impl.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

namespace tr::ImGui {
	constexpr u32 IMGUI_RENDERER_ID{4};

	int string_callback(ImGuiInputTextCallbackData* data);
} // namespace tr::ImGui

int tr::ImGui::string_callback(ImGuiInputTextCallbackData* data)
{
	std::string& str{*(std::string*)data->UserData};

	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
		str.resize(data->BufTextLen);
	}
	else if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
		str.resize(data->BufTextLen);
	}

	return 0;
}

//

void tr::ImGui::Init()
{
	ImGui_ImplSDL3_InitForOpenGL(sys::sdl_window, gfx::ogl_context);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void tr::ImGui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
}

bool tr::ImGui::InputText(const char* label, std::string& str, ImGuiInputTextFlags flags)
{
	return ::ImGui::InputText(label, str.data(), str.size(), flags | ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackEdit,
							  string_callback, &str);
}

tr::u64 tr::ImGui::GetTextureID(const gfx::texture& texture)
{
	return texture.m_handle;
}

void tr::ImGui::ProcessEvent(const sys::event& event)
{
	ImGui_ImplSDL3_ProcessEvent((const SDL_Event*)&event);
}

void tr::ImGui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
}

void tr::ImGui::Draw()
{
	tr::gfx::active_renderer = IMGUI_RENDERER_ID;
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}