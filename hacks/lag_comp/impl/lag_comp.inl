#pragma once

#include "../lag_comp.hpp"

namespace supremacy::hacks {
	__forceinline lag_backup_t::lag_backup_t(valve::c_player* const player) {
		m_origin = player->origin();
		m_abs_origin = player->abs_origin();

		m_obb_min = player->obb_min();
		m_obb_max = player->obb_max();

		m_abs_angles = player->abs_angles();

		const auto& bone_accessor = player->bone_accessor();

		m_readable_bones = bone_accessor.m_readable_bones;
		m_writable_bones = bone_accessor.m_writable_bones;

		const auto& bone_cache = player->bone_cache();

		std::memcpy(
			m_bones.data(),
			bone_cache.m_mem.m_ptr,
			bone_cache.m_size * sizeof(mat3x4_t)
		);

		m_bones_count = bone_cache.m_size;

		m_mdl_bone_counter = player->mdl_bone_counter();
	}

	__forceinline void lag_backup_t::restore(valve::c_player* const player) const {
		player->origin() = m_origin;

		player->set_abs_origin(m_abs_origin);

		player->set_collision_bounds(m_obb_min, m_obb_max);

		player->set_abs_angles(m_abs_angles);

		auto& bone_accessor = player->bone_accessor();

		bone_accessor.m_readable_bones = m_readable_bones;
		bone_accessor.m_writable_bones = m_writable_bones;

		std::memcpy(
			player->bone_cache().m_mem.m_ptr,
			m_bones.data(), m_bones_count * sizeof(mat3x4_t)
		);

		player->mdl_bone_counter() = m_mdl_bone_counter;
	}

	__forceinline void lag_record_t::restore(valve::c_player* const player, const int anim_index, const bool only_anim) const {
		if (!only_anim) {
			player->origin() = m_origin;

			player->set_abs_origin(m_origin);

			player->set_collision_bounds(m_obb_min, m_obb_max);
		}

		const auto& anim_side = anim_index < 3 ? m_sides.at(anim_index) : m_low_sides.at(anim_index - 3);

		player->set_abs_angles(anim_side.m_abs_angles);

		std::memcpy(
			player->bone_cache().m_mem.m_ptr,
			anim_side.m_bones.data(), anim_side.m_bones_count * sizeof(mat3x4_t)
		);

		player->mdl_bone_counter() = **reinterpret_cast<unsigned long**>(
			g_context->addresses().m_invalidate_bone_cache + 0xau
			);
	}

	__forceinline bool lag_record_t::valid() const {
		if (!g_context->cvars().m_cl_lagcompensation->get_bool())
			return true;

		if (m_shifting)
			return false;

		if (g_lag_comp->calc_time_delta(m_sim_time) > 0.2f)
			return false;

		const auto& net_info = g_context->net_info();

		int delay{};
		if (g_movement->should_fake_duck())
			delay = 15 - valve::g_client_state->m_choked_cmds;

		return m_sim_time >= static_cast<int>(valve::to_time(net_info.m_server_tick + delay) - g_context->cvars().m_sv_maxunlag->get_float());
	}

	__forceinline void player_entry_t::reset() {
		m_player = nullptr;

		m_last_valid_time = -1.f;
		m_server_rate = 0.f;
		m_negative_rate = 0.f;
		m_positive_rate = 0.f;
		m_low_negative_rate = 0.f;
		m_low_positive_rate = 0.f;
		m_zero_rate = 0.f;
		m_render_origin = {};
		m_misses = 0;
		m_prev_side = 0;
		m_trace_side = 0;
		m_prev_type = 0;
		m_try_lby_resolver = true;
		m_try_trace_resolver = true;
		m_try_anim_resolver = true;

		m_lag_records.clear();
	}

	__forceinline float c_lag_comp::calc_time_delta(const float sim_time) const {
		const auto& net_info = g_context->net_info();

		const auto correct = std::clamp(
			net_info.m_lerp + net_info.m_latency.m_in + net_info.m_latency.m_out,
			0.f, g_context->cvars().m_sv_maxunlag->get_float()
		);

		auto tick_base = valve::g_local_player->tick_base();
		if (g_exploits->next_shift_amount() > 0)
			tick_base -= g_exploits->next_shift_amount();

		return fabs(correct - (valve::to_time(tick_base) - sim_time));
	}

	__forceinline player_entry_t& c_lag_comp::entry(const std::size_t i) { return m_entries.at(i); }
}