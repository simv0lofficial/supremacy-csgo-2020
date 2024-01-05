#pragma once

#include "../movement.hpp"

namespace supremacy::hacks {
	__forceinline bool& c_movement::should_fake_duck() {
		return m_should_fake_duck;
	}

	__forceinline float& c_movement::prev_view_yaw() {
		return m_prev_view_yaw;
	}

	__forceinline int& c_movement::stop_type() {
		return m_stop_type;
	}
}