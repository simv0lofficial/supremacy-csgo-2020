#pragma once

namespace supremacy::hacks {
	struct dormant_data_t {
		float m_alpha{};
		bool m_was_unseen{};
		int m_health{};
		vec3_t m_origin{};
		float m_receive_time{};
		int m_weapon_id{};
		int m_weapon_type{ -1 };
		bool m_use_shared{};
		float m_last_shared_time{};
	};

	class c_dormant_esp
	{
	public:
		void start();
		bool adjust_sound(valve::c_player* player);
		void setup_adjust(valve::c_player* player, valve::sound_info_t& sound);
		bool valid_sound(valve::sound_info_t& sound);

		struct sound_player_t {
			float m_receive_time{};
			vec3_t m_origin{};
			valve::e_ent_flags m_flags{};
		} m_sound_players[64];

		valve::utl_vec_t<valve::sound_info_t> m_sound_buffer;
		valve::utl_vec_t<valve::sound_info_t> m_cur_sound_list;
	};

	inline const auto g_dormant_esp = std::make_unique< c_dormant_esp >();

	class c_visuals {
	private:
		struct bbox_t {
			vec2_t m_min{}, m_size{};
		};

		struct grenade_simulation_t {
			__forceinline grenade_simulation_t() = default;

			__forceinline grenade_simulation_t(
				valve::c_player* const owner, const valve::e_item_index index,
				const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset
			) : m_owner{ owner }, m_index{ index } {
				predict(origin, velocity, throw_time, offset);
			}

			void predict(const vec3_t& origin, const vec3_t& velocity, const float throw_time, const int offset);

			bool physics_simulate();

			void physics_trace_entity(
				const vec3_t& src, const vec3_t& dst,
				const valve::e_mask mask, valve::trace_t& trace
			);

			void physics_push_entity(const vec3_t& push, valve::trace_t& trace);

			void perform_fly_collision_resolution(valve::trace_t& trace);

			void think();

			void detonate(const bool bounced);

			void update_path(const bool bounced);

			bool										m_detonated{};
			valve::c_player* m_owner{};
			vec3_t										m_origin{}, m_velocity{};
			valve::c_entity* m_last_hit_entity{}, * m_last_breakable{};
			float										m_detonate_time{}, m_expire_time{};
			valve::e_item_index							m_index{};
			int											m_tick{}, m_next_think_tick{},
				m_last_update_tick{}, m_bounces_count{}, m_collision_group{};
			std::vector< std::pair< vec3_t, bool > >	m_path{};
		};

		using throwed_grenades_t = std::unordered_map< valve::e_ent_handle, grenade_simulation_t >;

		bool add_grenade_simulation(const grenade_simulation_t& sim, const bool warning) const;

		bool calc_bbox(valve::c_player* const player, bbox_t& bbox) const;

		std::optional< valve::bones_t > try_to_lerp_bones(const int index) const;

		valve::material_t* create_material(
			const std::string_view name,
			const std::string_view shader, const std::string_view data
		) const;

		void override_material(
			const int type, const bool ignore_z,
			const float r, const float g, const float b, const float a
		) const;

		const char* char_code_for_item_index(const valve::e_item_index index) const;

		void add_line(
			const vec2_t& from, const vec2_t& to, const std::uint32_t clr
		) const;

		void add_rect(
			const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
		) const;

		void add_rect_filled(
			const vec2_t& pos, const vec2_t& size, const std::uint32_t clr
		) const;
	public:
		void add_text(
			vec2_t pos, const unsigned long font,
			const std::string_view str, const std::uint32_t clr, const int flags
		) const;

		void add_text(
			vec2_t pos, const unsigned long font,
			const std::wstring_view str, const std::uint32_t clr, const int flags
		) const;
	private:
		void add_vertices(
			const valve::vertex_t* const vertices,
			const std::size_t vertices_count, const std::uint32_t clr
		) const;

		void add_circle(
			const vec2_t& pos, const float radius,
			const int segments_count, const std::uint32_t clr
		) const;

		void add_filled_circle(
			const vec2_t& pos, const float radius,
			const int segments_count, const std::uint32_t clr
		) const;

		void add_beam_ring(const vec3_t& pos, const float* const clr) const;

		void add_beam(const vec3_t& from, const vec3_t& to, const float* const clr) const;

		void add_grenade_warning_beam(const vec3_t& from, const vec3_t& to, const float* const clr) const;

		void oof_arrow(const vec3_t& pos) const;

		void send_net_data(valve::c_player* const player);

		void handle_player(valve::c_player* const player);

		void handle_projectile(valve::c_entity* const entity, const valve::e_class_id class_id);

		void handle_weapon(valve::c_weapon* const weapon) const;
		public:
		struct {
			unsigned long	m_tahoma12{}, m_menu{},
				m_verdana26{}, m_verdana26n{},
				m_keybinds{}, m_icons14{},
				m_icons26{};
		}								m_fonts{};
		private:
		struct bullet_impact_t {
			__forceinline bullet_impact_t() = default;

			__forceinline bullet_impact_t(
				const float time, const vec3_t& from, const vec3_t& pos
			) : m_time{ time }, m_from{ from }, m_pos{ pos } {}

			float	m_time{};
			bool	m_final{ true };

			vec3_t	m_from{}, m_pos{};
		};

		struct shot_mdl_t {
			float						m_time{};
			valve::bones_t				m_bones{};
			mat3x4_t					m_world_matrix{};

			valve::model_render_info_t	m_info{};
			valve::draw_model_state_t	m_state{};
		};

		int								m_backup_smoke_count{};

		std::vector< vec3_t >			m_player_sounds{};
		std::vector< bullet_impact_t >	m_bullet_impacts{};

		std::string 					m_surface_font_name{};
		float							m_last_hurt_time{},
			m_next_update{};
		std::optional< pen_data_t >		m_pen_data{};

		grenade_simulation_t			m_grenade_trajectory{};
		throwed_grenades_t				m_throwed_grenades{};

		std::vector< shot_mdl_t >		m_shot_mdls{};
		bool							m_rendering_shot_mdl{};
	public:
		std::array<dormant_data_t, 64> m_dormant_data{};
		std::array < vec3_t, 64 > m_alive_origin{};
		std::array < vec3_t, 64 > m_alive_mins{};
		std::array < vec3_t, 64 > m_alive_maxs{};
	public:		
		void init();

		void watermark();

		void kill_feed();

		void on_paint();

		void on_render_start();

		void on_render_end();

		void on_post_data_update_start();

		void on_pre_create_move(const valve::user_cmd_t& cmd);

		void on_create_move(const valve::user_cmd_t& cmd);

		void on_override_view(valve::view_setup_t* const setup);

		void on_post_screen_effects();

		bool on_draw_model(
			valve::studio_render_t* const ecx, const std::uintptr_t edx,
			const std::uintptr_t a0, const valve::draw_model_info_t& info,
			mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
		) const;

		void on_calc_view(
			valve::c_player* const ecx, const std::uintptr_t edx,
			vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
		);

		void add_shot_mdl(valve::c_player* const player, const std::shared_ptr< lag_record_t >& lag_record);

		__forceinline float& last_hurt_time();

		__forceinline std::string& surface_font_name();

		__forceinline std::vector< vec3_t >& player_sounds();

		__forceinline std::vector< bullet_impact_t >& bullet_impacts();

		__forceinline float& next_update();
	};

	inline const auto g_visuals = std::make_unique< c_visuals >();
}

#include "impl/visuals.inl"