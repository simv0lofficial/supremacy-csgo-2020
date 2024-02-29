#pragma once
#include <iostream>
#include <array>

namespace settings {
	inline float m_stored_alpha{};
	inline float m_menu_color[4]{ 1.f, 45 / 255.f, 0.f, 179 / 255.f };
	inline bool m_is_opened{};
	inline std::array< int, 64 > m_previous_hp{};
}