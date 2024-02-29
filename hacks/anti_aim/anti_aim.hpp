#pragma once

namespace supremacy::hacks {
	class c_anti_aim {
	private:
		void at_target(float& yaw) const;

		bool auto_direction(float& yaw);

		bool					m_choke_cycle_switch{};
		int						m_prev_tick_count{}, m_auto_dir_side{},
			m_side_counter{}, m_on_shot_side_counter{}, m_choke_start_cmd_number{}, m_manual_side{};
	public:
		int select_side();

		void select_yaw(float& yaw, const int side);

		void choke(valve::user_cmd_t* const user_cmd);

		void set_pitch(valve::user_cmd_t& user_cmd);

		void process(valve::user_cmd_t& user_cmd, const float yaw, const int side, const int choked_cmds) const;

		__forceinline bool enabled(const valve::user_cmd_t* const user_cmd) const;

		__forceinline int& manual_side();
	};

	inline const auto g_anti_aim = std::make_unique< c_anti_aim >();
}

#include "impl/anti_aim.inl"