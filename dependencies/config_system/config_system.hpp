#pragma once

namespace sdk {
	template <typename T>
	struct item_t {
		T* pointer;
		std::string category;
		std::string name;
		T default_value;
	};

	class c_config_system {
	private:
		std::vector<item_t<bool>> booleans;
		std::vector<item_t<int>> ints;
		std::vector<item_t<float>> floats;
		std::vector<item_t<util::c_color>> colors;
		std::vector<item_t<int>> keybinds;
	public:
		void push_item(bool* pointer, std::string category, std::string name, bool def_value);
		void push_item(int* pointer, std::string category, std::string name, int def_value);
		void push_item(float* pointer, std::string category, std::string name, float def_value);
		void push_item(util::c_color* pointer, std::string category, std::string name, util::c_color def_value);

		void setup_values();
		void read(std::string path);
		void save(std::string path);
		void remove(std::string path);
		void refresh();
		void reset();
		void menu();
		std::vector<std::string> config_files;

		// aimbot.
		int weapon_selection;
		bool enabled;
		int enabled_key;
		int enabled_key_style;
		std::unordered_map<int, bool> target_hitbox;
		std::unordered_map<int, bool> multi_point;
		bool static_scale;
		int head_scale;
		int body_scale;
		bool automatic_fire;
		bool silent_aim;
		int minimum_hitchance = 50;
		int minimum_damage = 40;
		bool automatic_penetration;
		int penetration_minimum_damage = 30;
		int override_minimum_damage_key;
		int override_minimum_damage_key_style = 1;
		int override_minimum_damage = 10;
		bool automatic_scope;
		int maximum_fov = 180;
		bool log_misses_due_to_spread;

		// other.
		bool remove_recoil;
		int accuracy_boost;
		bool quick_stop;
		int type;
		bool between_shots;
		int lag_compensation;
		bool anti_aim_correction;
		std::unordered_map<int, bool> body_aim_conditions;
		int body_aim_key;
		int body_aim_key_style = 1;
		int max_misses_body_aim;
		std::unordered_map<int, bool> safe_point_conditions;
		int safe_point_key;
		int safe_point_key_style = 1;
		int safe_point_type;
		int max_misses_safe_point;
		bool double_tap;
		int double_tap_key;
		int double_tap_key_style = 2;
		float recharge_delay = 0.3f;

		// anti-aimbot angles.
		bool enabled2;
		int pitch;
		int yaw_base;
		int yaw;
		int jitter_range;
		int jitter_switch;
		int rotate_range;
		int rotate_speed;
		int body_yaw;
		int body_yaw_key;
		int body_yaw_key_style = 2;
		int yaw_limit = 60;
		int body_lean;
		int inverted_body_lean;
		int on_shot_side;
		int freestanding;	
		int freestanding_key;
		int freestanding_key_style;

		// other.
		bool fake_lag;		
		int amount;
		std::unordered_map<int, bool> conditions;
		int factor_limit;
		bool force_when_peeking;
		bool hide_shots;
		int hide_shots_key;
		int hide_shots_key_style = 2;

		// player esp.		
		bool teammates;
		bool dormant;
		bool bounding_box;
		float bounding_box_color[4] = { 250 / 255.f, 250 / 255.f, 250 / 255.f, 0xb4 / 255.f };
		bool health_bar;
		bool name;
		float name_color[4] = { 1.f, 1.f, 1.f, 0xb4 / 255.f };
		bool flags;
		bool weapon_text;
		bool weapon_icon;
		float weapon_color[4] = { 1.f, 1.f, 1.f, 0xb4 / 255.f };
		bool ammo;
		float ammo_color[4] = { 80 / 255.f, 140 / 255.f, 200 / 255.f, 0xb4 / 255.f };
		bool distance;
		bool glow;
		float glow_color[4] = { 180 / 255.f, 60 / 255.f, 120 / 255.f, 150 / 255.f };
		bool hit_marker;
		bool hit_marker_sound;
		bool visualize_sounds;
		float visualize_sounds_color[4] = { 250 / 255.f, 63 / 255.f, 63 / 255.f, 150 / 255.f };
		bool money;
		bool skeleton;
		float skeleton_color[4] = { 1.f, 1.f, 170 / 255.f, 1.f };
		bool out_of_fow_arrow;
		float out_of_fow_arrow_color[4] = { 255 / 255.f, 30 / 255.f, 30 / 255.f, 190 / 255.f };

		// colored models.		
		bool player;
		float player_color[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 1.f };
		int player_model;
		bool player_behind_wall;
		float player_behind_wall_color[4] = { 60 / 255.f, 120 / 255.f, 180 / 255.f, 1.f };
		int player_behind_wall_model;
		bool teammate;
		float teammate_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int teammate_model;
		bool teammate_behind_wall;
		float teammate_behind_wall_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int teammate_behind_wall_model;
		bool local_player;
		float local_player_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int local_player_model;
		bool local_player_fake;
		float local_player_fake_color[4] = { 0.f, 0.f, 0.f, 0.7f };
		int local_player_fake_model;		
		bool on_shot;
		float on_shot_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int on_shot_model;
		int on_shot_time = 2;
		bool ragdolls;
		bool hands;
		float hands_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int hands_model;
		bool weapon_viewmodel;
		float weapon_viewmodel_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int weapon_viewmodel_model;
		bool weapons;
		float weapons_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int weapons_model;
		bool disable_model_occlusion;
		bool shadow;
		float shadow_color[4] = { 1.f, 1.f, 1.f, 1.f };
		int shadow_model;

		// other esp.
		bool radar;
		int dropped_weapons;
		float dropped_weapons_color[4] = { 1.f, 1.f, 1.f, 0.78f };
		bool grenades;
		float grenades_color[4] = { 1.f, 1.f, 1.f, 1.f };
		bool inaccuracy_overlay;
		float inaccuracy_overlay_color[4] = { 30 / 255.f, 30 / 255.f, 30 / 255.f, 0.47f };
		bool recoil_overlay;
		bool crosshair;
		bool bomb;
		float bomb_color[4] = { 150 / 255.f, 200 / 255.f, 60 / 255.f, 1.f };
		bool grenade_trajectory;
		float grenade_trajectory_color[4] = { 1.f, 50 / 255.f, 50 / 255.f, 0.7f };
		bool grenade_proximity_warning;
		bool spectators;
		bool penetration_reticle;		
		std::unordered_map<int, bool> feature_indicators;

		// effects.
		bool remove_flashbang_effects;
		bool remove_smoke_grenades;
		bool remove_fog;
		bool remove_skybox;		
		int visual_recoil_adjustment;
		float world_color[4] = { 1.f, 1.f, 1.f, 1.f };
		float props_color[4] = { 1.f, 1.f, 1.f, 1.f };
		float skybox_color[4] = { 1.f, 1.f, 1.f, 1.f };
		bool full_bright;
		bool remove_scope_overlay;
		bool disable_post_processing;
		bool force_third_person_alive;
		int third_person_key;
		int third_person_key_style = 2;
		int third_person_distance = 120;
		bool force_third_person_dead;
		bool disable_rendering_of_teammates;
		bool disable_rendering_of_ragdolls;
		bool bullet_tracers;
		float bullet_tracers_color[4] = { 150 / 255.f, 130 / 255.f, 1.f, 1.f };
		bool bullet_impacts;

		// settings.		
		int menu_key = VK_INSERT;
		float menu_color[4] = { 255 / 255.f, 45 / 255.f, 0.f, 179 / 255.f };
		bool menu_watermark = true;

		// movement.		
		bool bunny_hop;
		int air_strafe;
		bool slide_walk;
		int slow_motion_key;
		int slow_motion_key_style = 1;
		int speed_limit;
		int peek_assistence_key;
		int peek_assistence_key_style = 2;
		float peek_assistence_color[4] = { 150 / 255.f, 130 / 255.f, 1.f, 1.f };

		// others.
		bool override_fov;
		int override_fov_amount = 90;
		bool override_fov_when_scoped;
		bool override_viewmodel_fov;
		int override_viewmodel_fov_amount = 90;
		bool aspect_ratio;
		int width = 16;
		int height = 9;
		bool clan_tag_spammer;
		bool chat_spammer;
		bool name_spammer;
		bool share_esp = true;
		bool log_weapon_purchases;
		bool log_damage_dealt;
		int persistent_kill_feed;
		std::unordered_map<int, bool> primary_weapons;
		std::unordered_map<int, bool> secondary_weapons;
		std::unordered_map<int, bool> equipment;
		int auto_buy_minimal_money = 16000;

		// extra.		
		bool infinite_duck;
		int fake_duck_key;
		int fake_duck_key_style = 1;
	
	public:
		struct skin_info {
			int seed = -1;
			int paintkit;
			int rarity;
			std::string tag_name;
			std::string short_name;
			std::string name;
		};
	public:
		std::unordered_map<std::string, std::set<std::string>> weapon_skins;
		std::unordered_map<std::string, skin_info> skin_map;
		std::unordered_map<std::string, std::string> skin_names;

	};

	inline const auto g_config_system = std::make_unique< c_config_system >();
}