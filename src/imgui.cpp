#include "../include/tr/imgui.hpp"
#include "../include/tr/dependencies/imgui_impl_opengl3.h"
#include "../include/tr/dependencies/imgui_impl_sdl3.h"
#include "../include/tr/sysgfx/impl.hpp"

void tr::imgui::initialize()
{
	ImGui_ImplSDL3_InitForOpenGL(system::sdl_window, system::ogl_context);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void tr::imgui::shut_down()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
}

std::uint64_t tr::imgui::get_texture_id(gfx::texture_ref texture)
{
	return texture._id;
}

void tr::imgui::process_event(const system::event& event)
{
	ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<const SDL_Event*>(&event));
}

void tr::imgui::new_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
}

void tr::imgui::draw()
{
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}