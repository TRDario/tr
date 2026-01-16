///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements imgui.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/tr/imgui.hpp"
#include "../include/tr/sysgfx/impl.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

////////////////////////////////////////////////////////////////// IMGUI //////////////////////////////////////////////////////////////////

void tr::ImGui::Init()
{
	ImGui_ImplSDL3_InitForOpenGL(sys::g_sdl_window, gfx::g_ogl_context);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void tr::ImGui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
}

ImTextureID tr::ImGui::GetTextureID(const gfx::texture& texture)
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
	gfx::g_active_renderer = gfx::renderer_id::imgui_renderer;
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}