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
	ImGui_ImplSDL3_InitForOpenGL(sys::g_window.ptr(), sys::g_window.gfx_context().ptr());
	ImGui_ImplOpenGL3_Init("#version 150");
}

void tr::ImGui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
}

ImTextureID tr::ImGui::GetTextureID(const texture& texture)
{
	return texture.m_handle;
}

void tr::ImGui::ProcessEvent(const event& event)
{
	ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<const SDL_Event*>(&event));
}

void tr::ImGui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
}

void tr::ImGui::Draw()
{
	(void)sys::g_window.gfx_context().should_setup_context(renderer_id::imgui_renderer);
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}