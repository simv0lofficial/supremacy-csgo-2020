#pragma once

namespace supremacy::valve {
	class c_entity {
	public:
		__forceinline std::uintptr_t renderable( );

		__forceinline std::uintptr_t networkable( );

		__forceinline int index( );

		__forceinline e_ent_handle handle( );

		__forceinline bool dormant( );

		__forceinline client_class_t* client_class( );

		__forceinline bool setup_bones(
			mat3x4_t* const bones, const int max_bones,
			const int mask, const float time
		);

		__forceinline float& sim_time( );

		__forceinline bool compute_hitbox(vec3_t* obb_min, vec3_t* obb_max);

		__forceinline float& old_sim_time( );

		__forceinline vec3_t& origin( );

		__forceinline qangle_t& abs_angles( );

		__forceinline vec3_t& abs_origin( );

		__forceinline vec3_t& abs_velocity( );

		__forceinline void set_abs_angles( 
			const qangle_t& abs_angles 
		);

		__forceinline void set_abs_origin(
			const vec3_t& abs_origin
		);

		__forceinline e_ent_flags& flags( );

		__forceinline std::uint32_t& eflags( );

		__forceinline model_t* model( );

		__forceinline model_data_t*& mdl_data( );

		__forceinline studio_hdr_t* studio_hdr();

		__forceinline studio_hdr_t* mdl_ptr();

		__forceinline std::uint32_t& effects( );

		__forceinline std::uint32_t& occlusion_flags( );

		__forceinline int& occlusion_frame( );

		__forceinline ik_context_t*& inverse_kinematics();

		__forceinline int& last_setup_bones_frame( );

		__forceinline float& last_setup_bones_time( );

		__forceinline unsigned long& mdl_bone_counter( );

		__forceinline bone_accessor_t& bone_accessor( );

		__forceinline utl_vec_t< mat3x4_t >& bone_cache( );

		__forceinline std::uint8_t& ent_client_effects( );

		__forceinline bool breakable( );

		__forceinline int& team( );

		__forceinline bool friendly( );

		__forceinline int& sim_tick( );

		__forceinline vec3_t& obb_min( );

		__forceinline vec3_t& obb_max( );

		__forceinline const void update_collision_bounds();

		__forceinline void set_collision_bounds(
			const vec3_t& obb_min, const vec3_t& obb_max
		);

		__forceinline const vec3_t& world_space_center( );

		__forceinline bool is_player( );

		__forceinline bool is_weapon( );

		__forceinline var_mapping_t& var_mapping( );

		__forceinline float& cycle( );

		__forceinline float& anim_time( );

		__forceinline e_ent_handle& move_parent( );

		__forceinline bool& spotted( );

		__forceinline bool& spotted_by(int index);

		__forceinline e_ent_handle& thrower( );

		__forceinline int& explode_effect_tick_begin( );

		__forceinline float& grenade_spawn_time( );

		__forceinline bool& use_fast_path( );

		__forceinline int& bomb_site( );

		__forceinline bool& is_bomb_ticking( );

		__forceinline float& c4_blow( );

		__forceinline std::uint16_t model_instance( );

		__forceinline int& model_index( );

		__forceinline e_ent_handle& world_model_handle( );

		__forceinline int& sequence( );

		__forceinline e_ent_handle& view_model_weapon_handle( );

		__forceinline e_ent_handle& view_model_owner( );

		__forceinline void set_model_index( const int index );
	};

	class c_weapon : public c_entity {
	public:
		__forceinline float max_speed( );

		__forceinline e_item_index& item_index( );

		__forceinline float& postpone_fire_ready_time( );

		__forceinline float& accuracy_penalty( );

		__forceinline float& last_shot_time( );

		__forceinline float& recoil_index( );

		__forceinline int& ammo( );

		__forceinline float& next_primary_attack( );

		__forceinline float& next_secondary_attack( );

		__forceinline float& throw_time( );

		__forceinline float& throw_strength( );

		__forceinline bool& pin_pulled( );

		__forceinline float spread( );

		__forceinline float inaccuracy( );

		__forceinline void update_inaccuracy( );

		__forceinline weapon_data_t* wpn_data( );

		__forceinline bool& in_reload( );

		__forceinline int& zoom_lvl( );

		__forceinline int& item_id_high( );

		__forceinline int& account_id( );

		__forceinline int& fallback_paint_kit( );

		__forceinline int& item_quality( );

		__forceinline int& fallback_stat_trak( );

		__forceinline float& fallback_wear( );

		__forceinline int& owner_xuid_low( );

		__forceinline int& owner_xuid_high( );

		__forceinline bool& custom_material_initialized( );

		__forceinline utl_vec_t< ret_counted_t* >& custom_materials( );

		__forceinline utl_vec_t< ret_counted_t* >& custom_materials2( );

		__forceinline utl_vec_t< ret_counted_t* >& visuals_data_processors( );
	};

	class c_player : public c_entity {
	public:
		__forceinline void set_local_view_angles( const qangle_t& angle );

		__forceinline int& tick_base( );

		__forceinline e_life_state& life_state( );

		__forceinline int& health( );

		__forceinline bool alive( );

		__forceinline float& spawn_time( );

		__forceinline pose_params_t& pose_params( );

		__forceinline anim_layers_t& anim_layers( );

		__forceinline anim_state_t*& anim_state( );

		__forceinline int& final_predicted_tick();

		__forceinline bool& client_side_anim( );

		__forceinline float get_sequence_cycle_rate(studio_hdr_t* studio_hdr, int sequence);

		__forceinline void get_sequence_linear_motion(studio_hdr_t* studio_hdr, int sequence, pose_params_t pose_params, vec3_t* out);

		__forceinline float get_sequence_move_dist(studio_hdr_t* studio_hdr, int sequence);

		__forceinline void setup_movement();

		__forceinline void update_client_side_anim( );

		__forceinline qangle_t& eye_angles( );

		__forceinline vec3_t& velocity( );

		__forceinline float& duck_amount( );

		__forceinline e_ent_handle& wpn_handle( );

		__forceinline e_ent_handle& ground_entity( );

		__forceinline c_weapon* weapon( );

		__forceinline int& hitbox_set_index( );

		__forceinline int& skin( );

		__forceinline int& body( );

		__forceinline float& lby( );

		__forceinline int lookup_seq_act( const int sequence );

		__forceinline vec3_t& view_offset( );

		__forceinline qangle_t& aim_punch( );

		__forceinline vec3_t& aim_punch_vel( );

		__forceinline float& next_attack( );

		__forceinline bool& ducking( );

		__forceinline bool& ducked( );

		__forceinline bool& walking( );

		__forceinline bool& strafing( );

		__forceinline e_move_type& move_type( );

		__forceinline float& velocity_modifier( );

		__forceinline float& fall_velocity();

		__forceinline user_cmd_t*& cur_user_cmd( );

		__forceinline user_cmd_t& last_user_cmd( );

		__forceinline cmd_context_t& cmd_context( );

		__forceinline bool& scoped( );

		__forceinline qangle_t& view_punch( );

		__forceinline float& duck_speed( );

		__forceinline int& move_state( );

		__forceinline float& third_person_recoil( );

		__forceinline int lookup_bone( const char* name );

		__forceinline int& armor_value( );

		__forceinline bool& has_helmet( );

		__forceinline bool& has_heavy_armor( );

		__forceinline float& max_speed( );

		__forceinline float& surface_friction( );

		__forceinline bool& immune( );

		__forceinline e_ent_handle view_model_handle( );

		__forceinline c_entity* view_model( );

		__forceinline int& money( );

		__forceinline float fov( );

		__forceinline bool has_c4( );

		__forceinline int& observer_mode( );

		__forceinline e_ent_handle& observer_target( );

		__forceinline bool& use_new_anim_state( );

		__forceinline void invalidate_physics_recursive(int bits);

		__forceinline e_ent_handle* weapons( );

		__forceinline float& flash_duration( );

		__forceinline int& shots_fired( );
	};

	class c_local_player {
	private:
		c_player** m_local{};
	public:
		__forceinline operator bool( ) const;

		__forceinline operator c_player*( ) const;

		__forceinline c_player* operator ->( ) const;
	} inline g_local_player{};
}

#include "impl/entities.inl"