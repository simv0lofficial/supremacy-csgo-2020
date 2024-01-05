#pragma once

#include "../shots.hpp"

namespace supremacy::hacks {
	__forceinline std::deque< shot_t >& c_shots::elements() { return m_elements; }

	__forceinline shot_t* c_shots::last_unprocessed() {
		if (m_elements.empty())
			return nullptr;

		const auto shot = std::find_if(
			m_elements.rbegin(), m_elements.rend(),
			[](const shot_t& shot) {
				return !shot.m_processed
				&& shot.m_server_info.m_fire_tick
			&& shot.m_server_info.m_fire_tick == valve::g_client_state->m_server_tick;
			}
		);

		return shot == m_elements.rend() ? nullptr : &*shot;
	}

	__forceinline void c_shots::add(
		const vec3_t& src, const aim_last_target_t* const target, const int next_shift_amount
	) {
		m_elements.emplace_back(src, target, next_shift_amount);

		if (m_elements.size() < 128)
			return;

		m_elements.pop_front();
	}
}