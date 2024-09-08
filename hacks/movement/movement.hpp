#pragma once

namespace supremacy::hacks {
	class c_movement {
	private:
		void correct_mouse(valve::user_cmd_t& user_cmd);

		void accelerate(
			const valve::user_cmd_t& user_cmd, const vec3_t& wishdir,
			const float wishspeed, vec3_t& velocity, float acceleration
		) const;

		void walk_move(
			const valve::user_cmd_t& user_cmd, vec3_t& move,
			vec3_t& fwd, vec3_t& right, vec3_t& velocity
		) const;

		void full_walk_move(
			const valve::user_cmd_t& user_cmd, vec3_t& move,
			vec3_t& fwd, vec3_t& right, vec3_t& velocity
		) const;

		void modify_move(valve::user_cmd_t& user_cmd, vec3_t& velocity) const;

		void predict_move(const valve::user_cmd_t& user_cmd, vec3_t& velocity) const;

		void auto_strafe(valve::user_cmd_t& user_cmd);

		vec3_t					m_velocity{};
		int						m_stop_type{};
		bool					m_should_fake_duck{}, m_strafe_switch{};
		float					m_max_player_speed{}, m_max_weapon_speed{}, m_prev_view_yaw{};
	public:
		bool stop(valve::user_cmd_t& user_cmd);

		bool force_lby_update(valve::user_cmd_t& user_cmd);

		void normalize(valve::user_cmd_t& user_cmd) const;

		void rotate(
			valve::user_cmd_t& user_cmd,
			const qangle_t& wish_angles, const valve::e_move_type move_type
		) const;

		void peek_assistence(qangle_t& wish_angles, valve::user_cmd_t& user_cmd);

		void on_create_move(valve::user_cmd_t& user_cmd);

		__forceinline bool& should_fake_duck();

		__forceinline float& prev_view_yaw();

		__forceinline int& stop_type();
	};

	inline const auto g_movement = std::make_unique< c_movement >();
}

#include "impl/movement.inl"