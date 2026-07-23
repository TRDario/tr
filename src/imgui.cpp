///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements imgui.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/tr/imgui.hpp"
#include "../include/tr/sysgfx/graphics_context.hpp"
#include "../include/tr/sysgfx/texture.hpp"
#include "../include/tr/sysgfx/texture_ref.hpp"
#include "../include/tr/sysgfx/window.hpp"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

struct SDL_Event;

////////////////////////////////////////////////////////////////// IMGUI //////////////////////////////////////////////////////////////////

void tr::ImGui::Init(graphics_context& context)
{
	ImGui_ImplSDL3_InitForOpenGL(context.window().m_ptr, context.m_ptr.get());
	ImGui_ImplOpenGL3_Init("#version 150");
}

void tr::ImGui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
}

//

ImTextureID tr::ImGui::GetTextureID(const texture_ref& texture)
{
	return GetTextureID(texture.m_ref);
}

ImTextureID tr::ImGui::GetTextureID(const texture& texture)
{
	return texture.m_handle;
}

//

void tr::ImGui::ProcessEvent(const event& event)
{
	ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<const SDL_Event*>(&event));
}

//

void tr::ImGui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
}

void tr::ImGui::Draw(graphics_context& context)
{
	(void)context.should_setup_renderer(renderer_id::imgui_renderer);
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}