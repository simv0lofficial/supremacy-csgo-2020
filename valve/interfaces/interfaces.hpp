#pragma once

namespace supremacy::valve {
	struct dlight_t {
		int flags;
		vec3_t origin;
		float radius;
		unsigned char r;
		unsigned char g;
		unsigned char b;
		signed char exponent;  
		float die;             
		float decay;             
		float min_light;          
		int key;
		int style;  
		vec3_t direction;
		float inner_angle;
		float outer_angle;
	};

	struct color32;

	class c_effects_t
	{
	public:
		virtual int draw_decal_index_from_name(char* name) = 0;
		virtual void decal_shoot(int texture_index, int entity, const model_t* model, const vec3_t& model_origin, const qangle_t& model_angles, const vec3_t& position, const vec3_t* saxis, int flags) = 0;
		virtual void decal_color_shoot(int texture_index, int entity, const model_t* model, const vec3_t& model_origin, const qangle_t& model_angles, const vec3_t& position, const vec3_t* saxis, int flags, const color32& rgba_color) = 0;
		virtual void player_decal_shoot(material_t* material, void* user_data, int entity, const model_t* model, const vec3_t& model_origin, const qangle_t& model_angles, const vec3_t& position, const vec3_t* saxis, int flags, const color32& rgba_color) = 0;
		virtual dlight_t* cl_alloc_dlight(int key) = 0;
	} inline* g_effects{};

	class c_client {
	public:
		__forceinline client_class_t* all_classes( );
	} inline* g_client{};

	class c_game_rules {
	public:
		__forceinline bool is_valve_ds() {
			static const auto& net_var = g_net_vars->entry(xorstr_("CCSGameRulesProxy->m_bIsValveDS"));

			return *reinterpret_cast<bool*>(
				reinterpret_cast<std::uintptr_t>(this) + std::get< std::uint32_t >(net_var)
				);
		}

		__forceinline int game_phase() {
			static const auto& net_var = g_net_vars->entry(xorstr_("CCSGameRulesProxy->m_gamePhase"));

			return *reinterpret_cast<int*>(
				reinterpret_cast<std::uintptr_t>(this) + std::get< std::uint32_t >(net_var)
				);
		}

	} inline** g_game_rules{};

	struct global_vars_t {
		float			m_real_time{};
		int				m_frame_count{};
		float			m_absolute_frame_time{};
		float			m_absolute_frame_start_time_std_dev{};
		float			m_cur_time{};
		float			m_frame_time{};
		int				m_max_clients{};
		int				m_tick_count{};
		float			m_interval_per_tick{};
		float			m_interpolation_amount{};
		int				m_sim_ticks_this_frame{};
		int				m_network_protocol{};
		std::uintptr_t	m_save_data{};
		bool			m_client{};
		bool			m_remote_client{};
		int				m_timestamp_networking_base{};
		int				m_timestamp_randomize_window{};

		int get_network_base(int tick, int index) {
			int entity_mod = index % m_timestamp_randomize_window;
			int base_tick = m_timestamp_networking_base * (int)((tick - entity_mod) / m_timestamp_networking_base);
			return base_tick;
		}
	} inline* g_global_vars{};

	class c_engine {
	public:
		__forceinline bool in_game( );

		__forceinline int index_for_uid( const int uid );

		__forceinline float last_time_stamp();

		__forceinline std::optional< player_info_t > player_info( const int index );

		__forceinline net_channel_info_t* net_channel_info( );

		__forceinline qangle_t view_angles( );

		__forceinline void set_view_angles( const qangle_t& angles );

		__forceinline void fire_events( );

		__forceinline const mat4x4_t& w2s_matrix( );

		__forceinline void exec_cmd( const char* cmd );

		__forceinline void unrestricted_cmd(const char* cmd, const bool delayed);

		__forceinline int local_player_index( );

		__forceinline std::uintptr_t* bsp_tree_query( );
	} inline* g_engine{};

	class c_entity;

	class c_entity_list {
	public:
		__forceinline c_entity* find_entity( const int index );

		__forceinline c_entity* find_entity( const e_ent_handle handle );

		__forceinline int highest_index( );
	} inline* g_entity_list{};

	class c_handle_entity
	{
	public:
		virtual ~c_handle_entity() {}
		virtual void set_ref_handle(const uint32_t& handle) = 0;
		virtual const uint32_t& get_ref_handle() const = 0;
	};

	class c_collideable
	{
	public:
		virtual c_handle_entity* get_entity_handle() = 0;
		virtual vec3_t& get_mins() const = 0;
		virtual vec3_t& get_maxs() const = 0;
	};

	class c_renderable;
	class c_networkable;
	class c_thinkable;
	class c_unknown;
	class c_client_entity;

	class c_unknown : public c_handle_entity
	{
	public:
		virtual c_collideable* get_collideable() = 0;
		virtual c_networkable* get_networkable_entity() = 0;
		virtual c_renderable* get_renderable_entity() = 0;
		virtual c_client_entity* get_client_entity() = 0;
		virtual c_entity* get_base_entity() = 0;
		virtual c_thinkable* get_thinkable_entity() = 0;
		virtual void* get_client_alpha_property() = 0;
	};

	class c_renderable
	{
	public:
		virtual c_unknown* get_i_unknown_entity() = 0;

		virtual const vec3_t& get_render_origin(void) = 0;
		virtual const vec3_t& get_render_angles(void) = 0;

		virtual bool should_render(void) = 0;
		virtual int get_render_flags(void) = 0;

		virtual void unused(void) const {}

		virtual void* get_shadow_handle() const = 0;
		virtual void*& get_render_handle() = 0;

		virtual model_t* get_model() const = 0;
		virtual int draw_model(int flags, const int& instance) = 0;

		virtual int get_body() = 0;

		virtual void get_color_modulation(float* color) = 0;

		virtual bool lod_test() = 0;

		virtual bool setup_bones(mat3x4_t* matrix, int max_bones, int mask, float time) = 0;
		virtual void setup_weights(const mat3x4_t* matrix, int flex_cnt, float* flex_weights, float* flex_delayed_weights) = 0;

		virtual void useless_func1(void) = 0;

		virtual void* useless_func2() = 0;

		virtual void get_render_bounds(vec3_t& mins, vec3_t& maxs) = 0;
	};

	class c_networkable
	{
	public:
		virtual c_unknown* get_n_unknown_entity() = 0;
		virtual void release() = 0;
		virtual client_class_t* get_client_class() = 0;
		virtual void should_transmit(int state) = 0;
		virtual void pre_data_changed(int update_type) = 0;
		virtual void data_changed(int update_type) = 0;
		virtual void pre_update(int update_type) = 0;
		virtual void pust_update(int update_type) = 0;
		virtual void unk(void) = 0;
		virtual bool dormant(void) = 0;
		virtual int index(void) = 0;
	};

	class c_thinkable
	{
	public:
		virtual c_unknown* get_t_unknown_entity() = 0;
	};

	struct renderable_info_t {
		c_renderable* m_renderable;
		void* m_alpha_property;
		int m_enum_count;
		int m_render_frame;
		unsigned short m_first_shadow;
		unsigned short m_leaf_list;
		short m_area;
		uint16_t m_flags;
		uint16_t m_flags2;
		vec3_t m_vec_bloated_abs_mins;
		vec3_t m_vec_bloated_abs_maxs;
		vec3_t m_vec_abs_mins;
		vec3_t m_vec_abs_maxs;
		int pad;
	};

	struct client_state_t {
		char			pad0[156u]{};
		net_channel_t* m_net_channel{};
		int				m_challenge_number{};
		char			pad2[100u]{};
		int				m_signon_state{};
		char			pad3[8u]{};
		double			m_next_cmd_time{};
		int				m_server_count{};
		int				m_cur_seq{};
		char			pad4[4u]{};
		int				m_clock_drift_mgr{};
		char			pad5[68u]{};
		int				m_server_tick{};
		int				m_client_tick{};
		int				m_delta_tick{};
		bool			m_paused{};
		char			pad6[12u]{};
		char			m_level_name[260u]{};
		char			m_level_name_short[40u]{};
		char			m_group_name[40]{};
		char			m_second_name[32]{};
		char			pad7[140u]{};
		int				m_max_clients{};
		char			pad8[18832u]{};
		int				m_last_cmd_out{};
		int				m_choked_cmds{};
		int				m_last_cmd_ack{};
		int				m_last_server_tick{};
		int				m_cmd_ack{};
		int				m_sound_seq{};
		char			pad9[80u]{};
		qangle_t		m_view_angles{};
		char			pad10[55u]{};
	} inline* g_client_state{};
	
	class c_move_helper {
	public:
		__forceinline void set_host( c_player* const player );
	} inline* g_move_helper{};

	struct prediction_t {
		__forceinline void update(
			const int start_frame, const bool valid_frame,
			const int in_acked, int out_cmd
		);

		__forceinline void setup_move(
			c_player* const player, user_cmd_t* const user_cmd,
			c_move_helper* const move_helper, move_data_t* const move_data
		);

		__forceinline void finish_move(
			c_player* const player,
			user_cmd_t* const user_cmd, move_data_t* const move_data
		);

		char	pad0[8u]{};
		bool	m_in_prediction{};
		char	pad1{};
		bool	m_engine_paused{};
		bool	m_old_cl_predict_value{};
		int		m_prev_start_frame{};
		int		m_incoming_packet_number{};
		float	m_last_server_world_time_stamp{};		
		bool	m_first_time_predicted{};
		char	pad2[3u]{};
		int		m_cmds_predicted{};
		int		m_server_cmds_acked{};
		bool	m_prev_ack_had_errors{};
	} inline* g_prediction{};

	class c_movement {
	public:
		__forceinline void process_movement(
			c_player* const player, move_data_t* const move_data
		);

		__forceinline void start_track_prediction_errors( c_player* const player );

		__forceinline void finish_track_prediction_errors( c_player* const player );
	} inline* g_movement{};

	class c_cvar {
	public:
		__forceinline cvar_t* find_var( const char* name );

		__forceinline void con_print( const std::uint32_t& clr, const char* str );

		__forceinline void con_print_format(const std::uint32_t& clr, const char* format, ...);
	} inline* g_cvar{};
	
	class c_engine_trace {
	public:
		__forceinline e_mask point_contents(
			const vec3_t& point, const e_mask mask = e_mask::all,
			c_entity** const entity = nullptr
		);

		__forceinline void clip_ray_to_entity(
			const ray_t& ray, const e_mask mask,
			c_entity* entity, trace_t* const trace
		);

		__forceinline void trace_ray(
			const ray_t& ray, const e_mask mask,
			trace_filter_t* const filter, trace_t* const trace
		);
	} inline* g_engine_trace{};

	class c_surface_data {
	public:
		__forceinline surface_data_t* find( const int index );
	} inline* g_surface_data{};

	struct input_t {
	public:

		__forceinline user_cmd_t* user_cmd( const int slot, const int seq_number );

		void* pvftable{};
		bool m_track_ir_available{};
		bool m_mouse_initialized{};
		bool m_mouse_active{};
		bool m_joystick_advanced_init{};
		char pad1[ 0xa4u ];
		bool m_camera_intercepting_mouse{};
		bool m_camera_in_third_person{};
		bool m_camera_moving_with_mouse{};
		vec3_t m_camera_offset{};
		bool m_camera_distance_move{};
		int m_camera_old_x{};
		int m_camera_old_y{};
		int m_camera_x{};
		int m_camera_y{};
		bool m_camera_is_orthographic{};
		vec3_t m_previous_view_angles{};
		vec3_t m_previous_view_angles_tilt{};
		float m_last_forward_move{};
		int m_clear_input_state{};
		user_cmd_t* m_cmds{};
		verified_user_cmd_t* m_verified_cmds{};
	} inline* g_input{};

	class c_game_event_mgr {
	public:
		__forceinline void add_listener( base_event_listener_t* listener, const char* event_name, const bool server_side );
	} inline* g_game_event_mgr{};

	struct studio_render_t {
		__forceinline void set_alpha( const float value );

		__forceinline void override_material( material_t* const material, const int type = 0 );

		__forceinline bool forced_material_override( );

		char		pad0[ 592u ]{};
		material_t* m_override_material{};
		char		pad1[ 12u ]{};
		int			m_override_type{};
	} inline* g_studio_render{};

	class c_model_render {
	public:
		__forceinline void draw_model(
			const std::uintptr_t context, const draw_model_state_t& state,
			const model_render_info_t& info, mat3x4_t* const bones
		);
	} inline* g_model_render{};

	class c_mdl_cache {
	public:
		__forceinline std::uintptr_t lookup_hw_data( const std::uint16_t handle );
	} inline* g_mdl_cache{};

	class c_mat_system {
	public:
		__forceinline std::uint16_t first_material( );

		__forceinline std::uint16_t next_material( const std::uint16_t handle );

		__forceinline std::uint16_t invalid_material( );
		
		__forceinline material_t* material( const std::uint16_t handle );

		__forceinline material_t* find_material(
			const char* name, const char* group, const bool complain = true, const char* complain_prefix = nullptr
		);

		__forceinline material_t* create_material( const char* name, const std::uintptr_t key_values );

		__forceinline std::uintptr_t render_context( );
	} inline* g_mat_system{};

	struct view_render_t {
		char			pad0[ 4u ];
		view_setup_t	m_setup{};
	} inline* g_view_render{};

	struct c_surface {
		__forceinline void start_drawing( );

		__forceinline void finish_drawing( );

		__forceinline void set_text_font( const unsigned long font );

		__forceinline void set_text_pos( const vec2_t& pos );

		__forceinline void set_text_clr( const std::uint32_t clr );

		__forceinline void add_text( const wchar_t* const str, const int len );

		__forceinline void set_clr( const std::uint32_t clr );

		__forceinline void set_texture( const int id );

		__forceinline unsigned long create_font( );

		__forceinline void add_poly_line(
			int* const x_points, int* const y_points, const int points_count
		);

		__forceinline void add_clred_text(
			const unsigned long font, const vec2_t& pos,
			const int r, const int g, const int b, const int a, const char* txt
		);

		__forceinline void add_line( const vec2_t& from, const vec2_t& to );

		__forceinline void add_rect( const vec2_t& min, const vec2_t& max );

		__forceinline void add_textured_rect( const vec2_t& min, const vec2_t& max );

		__forceinline void add_rect_filled( const vec2_t& min, const vec2_t& max );

		__forceinline void set_texture_rgba(
			const int id, const std::uint8_t* const clrs, const int width, const int height
		);

		__forceinline int create_new_texture( const bool procedural = false );

		__forceinline void add_textured_polygon(
			const int vertices_count, const vertex_t* const vertices, const bool clip = true
		);

		__forceinline void play_sound(const char* sample);
	} inline* g_surface{};

	struct c_font_mgr {
		__forceinline void calc_text_size( const unsigned long font, const wchar_t* str, int& width, int& height );

		__forceinline void set_font_glyph_set(
			const unsigned long font, const char* name, const int tall, const int weight, const int blur,
			const int scanlines, const int flags, const int range_min = 0, const int range_max = 0
		);
	} inline* g_font_mgr{};

	struct c_localize {
		__forceinline wchar_t* find_safe( const char* token );
	} inline* g_localize{};

	class c_model_info {
	public:
		__forceinline int lookup_model_index( const char* name );

	} inline* g_model_info{};

	struct glow_mgr_t {
		struct object_t {
			c_entity* m_entity{};
			float		m_red{};
			float		m_green{};
			float		m_blue{};
			float		m_alpha{};
			bool		m_alpha_capped_by_render_alpha{};
			char		pad0[3u]{};
			float		m_alpha_fn_of_max_vel{};
			float		m_bloom_amt{};
			float		m_pusle_overdrive{};
			bool		m_render_when_occluded{};
			bool		m_render_when_unoccluded{};
			bool		m_full_bloom_render{};
			char		pad1[1u]{};
			int			m_full_bloom_stencil_test_value{};
			int			m_render_style{};
			int			m_split_screen_slot{};
			int			m_next_free_slot{};
		};

		utl_vec_t< object_t >	m_objects{};
	} inline* g_glow_mgr{};

	class c_beams {
	public:
		__forceinline void draw_beam( const std::uintptr_t beam );

		__forceinline std::uintptr_t create_beam_points( beam_info_t& info );

		__forceinline std::uintptr_t create_beam_ring_point( beam_info_t& info );
	} inline* g_beams{};

	class c_debug_overlay {
	public:
		__forceinline void add_box_overlay(
			const vec3_t& pos, const vec3_t& min, const vec3_t& max, const qangle_t& rotation,
			const int r, const int g, const int b, const int a, const float duration
		);
	} inline* g_debug_overlay{};

	typedef void* file_name_handle_t;
	struct sound_info_t	{
		int			m_guid;
		file_name_handle_t m_file_name_handle;
		int			m_sound_source;
		int			m_channel;
		int			m_speaker_entity;
		float		m_volume;
		float		m_last_spatialized_volume;
		float		m_radius;
		int			m_pitch;
		vec3_t* m_origin;
		vec3_t* m_direction;
		bool		m_update_positions;
		bool		m_is_sentence;
		bool		m_dry_mix;
		bool		m_speaker;
		bool		m_from_server;
	};

	class c_engine_sound {
	public:
		__forceinline void emit_ambient_sound(
			const char* sample, float volume, int pitch, int flags, float time);

		__forceinline void get_active_sounds(utl_vec_t<sound_info_t>& sound_list);

	} inline* g_engine_sound{};

	class c_panel {
	public:
		void set_keyboard_input_enabled(
			unsigned int panel_id, bool state);

		void set_mouse_input_enabled(unsigned int panel_id, bool state);

		const char* get_panel_name(unsigned int panel_id);
	} inline* g_panel{};

	class c_uipanel	{
	public:
		__forceinline int get_child_count();

		__forceinline c_uipanel* get_child(int value);

		__forceinline bool has_class(const char* name);

		__forceinline void set_attribute_float(const char* name, float value);
	} inline* g_uipanel{}; 

	__forceinline int to_ticks( const float time );

	__forceinline float to_time( const int ticks );
}

#include "impl/interfaces.inl"