#pragma once

#include "../visuals.hpp"

namespace supremacy::hacks {
	__forceinline float& c_visuals::last_hurt_time() { return m_last_hurt_time; }

	__forceinline std::string& c_visuals::surface_font_name() { return m_surface_font_name; }

	__forceinline std::vector< vec3_t >& c_visuals::player_sounds() { return m_player_sounds; }

	__forceinline std::vector< c_visuals::bullet_impact_t >& c_visuals::bullet_impacts() { return m_bullet_impacts; }

	__forceinline float& c_visuals::next_update() { return m_next_update; }
}