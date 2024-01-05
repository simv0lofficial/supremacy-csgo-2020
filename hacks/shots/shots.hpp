#pragma once

namespace supremacy::hacks {
	struct shot_t {
		__forceinline constexpr shot_t() = default;

		__forceinline shot_t(
			const vec3_t& src, const aim_last_target_t* const target, const int next_shift_amount
		) : m_src{ src }, m_target{ target ? *target : aim_last_target_t{} }, m_time{ valve::g_global_vars->m_real_time },
			m_target_index{ target && target->m_entry->m_player ? target->m_entry->m_player->index() : -1 } {}

		vec3_t							m_src{};

		aim_last_target_t				m_target{};

		float							m_time{};
		bool							m_processed{};
		int								m_dmg{},
			m_target_index{},
			m_next_shift_amount{},
			m_cmd_number{ -1 }, m_process_tick{};

		struct {
			vec3_t	m_impact_pos{};
			int		m_fire_tick{}, m_hurt_tick{}, m_hitgroup{};
		}								m_server_info{};
	};

	class c_shots {
	private:
		std::deque< shot_t > m_elements{};
	public:
		void on_net_update();

		void on_new_event(valve::game_event_t* const event);

		__forceinline std::deque< shot_t >& elements();

		__forceinline shot_t* last_unprocessed();

		__forceinline void add(
			const vec3_t& src, const aim_last_target_t* const target, const int next_shift_amount
		);
	};

	inline const auto g_shots = std::make_unique< c_shots >();
}

#include "impl/shots.inl"