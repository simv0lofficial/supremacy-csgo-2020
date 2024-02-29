#pragma once

#include "../anti_aim.hpp"

namespace supremacy::hacks {
	__forceinline bool c_anti_aim::enabled(const valve::user_cmd_t* const user_cmd) const {
		const auto cur_move_type = valve::g_local_player->move_type();

		valve::e_move_type cmd_move_type{}, cmd_pred_move_type{};
		if (user_cmd) {
			const auto& local_data = g_eng_pred->local_data().at(user_cmd->m_number % 150);

			cmd_move_type = local_data.m_net_vars.m_move_type;
			cmd_pred_move_type = local_data.m_pred_net_vars.m_move_type;
		}

		return sdk::g_config_system->enabled2
			&& cur_move_type != valve::e_move_type::noclip
			&& cur_move_type != valve::e_move_type::ladder
			&& (!user_cmd ||
				(cmd_move_type != valve::e_move_type::ladder
					&& cmd_pred_move_type != valve::e_move_type::ladder
					&& !(user_cmd->m_buttons & valve::e_buttons::in_use)))
			&& !(valve::g_local_player->flags() & valve::e_ent_flags::frozen)
			&& !(g_context->flags() & e_context_flags::aim_fire)
			&& !g_context->freeze_time();
	}

	__forceinline int& c_anti_aim::manual_side() {
		return m_manual_side;
	}
}