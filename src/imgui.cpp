#include "../include/tr/imgui.hpp"
#include "../include/tr/sysgfx/impl.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

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
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}