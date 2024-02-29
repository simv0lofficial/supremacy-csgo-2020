#include "../../../supremacy.hpp"

namespace supremacy::hacks {
	void c_anim_sync::update(
		const player_entry_t& entry, lag_record_t* const current,
		lag_record_t* const previous, lag_record_t* const penultimate, const int side
	) const {
		struct backup_t {
			__forceinline constexpr backup_t() = default;

			__forceinline backup_t(valve::c_player* const player)
				: m_real_time{ valve::g_global_vars->m_real_time },
				m_cur_time{ valve::g_global_vars->m_cur_time },
				m_frame_time{ valve::g_global_vars->m_frame_time },
				m_absolute_frame_time{ valve::g_global_vars->m_absolute_frame_time },
				m_frame_count{ valve::g_global_vars->m_frame_count },
				m_tick_count{ valve::g_global_vars->m_tick_count },
				m_interpolation_amount{ valve::g_global_vars->m_interpolation_amount },
				m_duck_amount{ player->duck_amount() },
				m_lby{ player->lby() },
				m_eflags{ player->eflags() },
				m_flags{ player->flags() },
				m_velocity{ player->velocity() },
				m_abs_velocity{ player->abs_velocity() } {}

			__forceinline void restore(valve::c_player* const player) const {
				valve::g_global_vars->m_real_time = m_real_time;
				valve::g_global_vars->m_cur_time = m_cur_time;
				valve::g_global_vars->m_frame_time = m_frame_time;
				valve::g_global_vars->m_absolute_frame_time = m_absolute_frame_time;
				valve::g_global_vars->m_frame_count = m_frame_count;
				valve::g_global_vars->m_tick_count = m_tick_count;
				valve::g_global_vars->m_interpolation_amount = m_interpolation_amount;
				player->duck_amount() = m_duck_amount;
				player->lby() = m_lby;
				player->eflags() = m_eflags;
				player->flags() = m_flags;
				player->velocity() = m_velocity;
				player->abs_velocity() = m_abs_velocity;
			}

			float m_real_time{}, m_cur_time{}, m_frame_time{},
				m_absolute_frame_time{}, m_interpolation_amount{},
				m_duck_amount{}, m_lby{};
			int m_frame_count{}, m_tick_count{};
			std::size_t			m_eflags{};
			valve::e_ent_flags	m_flags{};
			vec3_t				m_velocity{}, m_abs_velocity{};
		} backup{ entry.m_player };

		const auto anim_state = entry.m_player->anim_state();

		if (previous) {
			entry.m_player->anim_layers() = previous->m_anim_layers;

			const auto& prev_movement_move_layer = previous->m_anim_layers.at(6u);
			const auto& prev_movement_strafe_change_layer = previous->m_anim_layers.at(7u);

			anim_state->m_feet_cycle = prev_movement_move_layer.m_cycle;
			anim_state->m_feet_weight = prev_movement_move_layer.m_weight;
			anim_state->m_strafe_weight = prev_movement_strafe_change_layer.m_weight;
			anim_state->m_strafe_sequence = prev_movement_strafe_change_layer.m_sequence;
			anim_state->m_strafe_cycle = prev_movement_strafe_change_layer.m_cycle;
			anim_state->m_acceleration_weight = previous->m_anim_layers.at(12u).m_weight;
		}
		else {			
			if (current->m_flags & valve::e_ent_flags::on_ground)
				anim_state->m_on_ground = true;

			anim_state->m_prev_update_time = current->m_sim_time - valve::g_global_vars->m_interval_per_tick;

			const auto& cur_movement_move_layer = current->m_anim_layers.at(6u);
			const auto& cur_movement_strafe_change_layer = current->m_anim_layers.at(7u);

			anim_state->m_feet_cycle = cur_movement_move_layer.m_cycle;
			anim_state->m_feet_weight = cur_movement_move_layer.m_weight;
			anim_state->m_strafe_weight = cur_movement_strafe_change_layer.m_weight;
			anim_state->m_strafe_sequence = cur_movement_strafe_change_layer.m_sequence;
			anim_state->m_strafe_cycle = cur_movement_strafe_change_layer.m_cycle;
			anim_state->m_acceleration_weight = current->m_anim_layers.at(12u).m_weight;
		}

		if (previous
			&& current->m_sim_ticks > 1) {	
			auto land_time = 0.f;
			auto land_in_cycle = false;
			auto is_landed = false;
			auto on_ground = false;

			if (current->m_anim_layers.at(4u).m_cycle < 0.5f
				&& (!(current->m_flags & valve::e_ent_flags::on_ground) || !(previous->m_flags & valve::e_ent_flags::on_ground))) {
				land_time = current->m_sim_time - current->m_anim_layers.at(4u).m_playback_rate * current->m_anim_layers.at(4u).m_cycle;
				land_in_cycle = land_time >= previous->m_sim_time;
			}

			for (auto sim_tick = 1; sim_tick <= current->m_sim_ticks; ++sim_tick) {
				const auto cur_sim_time = previous->m_sim_time + valve::to_time(sim_tick);				
				const auto cur_sim_tick = valve::to_ticks(cur_sim_time);

				valve::g_global_vars->m_real_time = valve::g_global_vars->m_cur_time = cur_sim_time;
				valve::g_global_vars->m_frame_time = valve::g_global_vars->m_absolute_frame_time = valve::g_global_vars->m_interval_per_tick;
				valve::g_global_vars->m_frame_count = valve::g_global_vars->m_tick_count = cur_sim_tick;
				valve::g_global_vars->m_interpolation_amount = 0.f;

				on_ground = current->m_flags & valve::e_ent_flags::on_ground;
				if (land_in_cycle && !is_landed) {
					if (land_time <= cur_sim_time) {
						is_landed = true;
						on_ground = true;
					}
					else
						on_ground = previous->m_flags & valve::e_ent_flags::on_ground;
				}

				if (sim_tick < current->m_sim_ticks) {
					if (penultimate) {
						const auto frac = float(sim_tick) / float(current->m_sim_ticks);
						entry.m_player->velocity() = entry.m_player->abs_velocity() = math::hermite_spline(penultimate->m_velocity, previous->m_velocity, current->m_velocity, frac);
						entry.m_player->duck_amount() = math::hermite_spline(penultimate->m_duck_amount, previous->m_duck_amount, current->m_duck_amount, frac);
					}
					else {
						entry.m_player->velocity() = entry.m_player->abs_velocity() = math::anim_lerp(previous->m_velocity, current->m_velocity, sim_tick, current->m_sim_ticks);
						entry.m_player->duck_amount() = math::anim_lerp(previous->m_duck_amount, current->m_duck_amount, sim_tick, current->m_sim_ticks);
					}		

					if (on_ground)
						entry.m_player->flags() |= valve::e_ent_flags::on_ground;
					else
						entry.m_player->flags() &= ~valve::e_ent_flags::on_ground;
				}
				else {
					entry.m_player->velocity() = entry.m_player->abs_velocity() = current->m_velocity;
					entry.m_player->duck_amount() = current->m_duck_amount;
					entry.m_player->flags() = current->m_flags;					
				}

				switch (side) {
				case 0:
					anim_state->m_foot_yaw = current->m_eye_angles.y;
					break;
				case 1:
					anim_state->m_foot_yaw = current->m_eye_angles.y - 120.f;
					break;
				case 2:
					anim_state->m_foot_yaw = current->m_eye_angles.y + 120.f;
					break;
				case 3:
					anim_state->m_foot_yaw = current->m_eye_angles.y - 25.f;
					break;
				case 4:
					anim_state->m_foot_yaw = current->m_eye_angles.y + 25.f;
					break;
				}

				entry.m_player->eflags() &= ~0x1000u;
								
				anim_state->m_prev_update_frame = 0;
				if (anim_state->m_prev_update_time == valve::g_global_vars->m_cur_time)
					anim_state->m_prev_update_time += valve::g_global_vars->m_interval_per_tick;

				entry.m_player->client_side_anim() = g_context->allow_anim_update() = true;
				entry.m_player->update_client_side_anim();
				entry.m_player->client_side_anim() = g_context->allow_anim_update() = false;

				backup.restore(entry.m_player);
			}
		}
		else {
			valve::g_global_vars->m_real_time = valve::g_global_vars->m_cur_time = current->m_sim_time;
			valve::g_global_vars->m_frame_time = valve::g_global_vars->m_absolute_frame_time = valve::g_global_vars->m_interval_per_tick;
			valve::g_global_vars->m_frame_count = valve::g_global_vars->m_tick_count = valve::to_ticks(current->m_sim_time);
			valve::g_global_vars->m_interpolation_amount = 0.f;

			entry.m_player->velocity() = entry.m_player->abs_velocity() = current->m_velocity;

			switch (side) {
			case 0:
				anim_state->m_foot_yaw = current->m_eye_angles.y;
				break;
			case 1:
				anim_state->m_foot_yaw = current->m_eye_angles.y - 120.f;
				break;
			case 2:
				anim_state->m_foot_yaw = current->m_eye_angles.y + 120.f;
				break;
			case 3:
				anim_state->m_foot_yaw = current->m_eye_angles.y - 25.f;
				break;
			case 4:
				anim_state->m_foot_yaw = current->m_eye_angles.y + 25.f;
				break;
			}

			entry.m_player->eflags() &= ~0x1000u;

			anim_state->m_prev_update_frame = 0;
			if (anim_state->m_prev_update_time == valve::g_global_vars->m_cur_time)
				anim_state->m_prev_update_time += valve::g_global_vars->m_interval_per_tick;

			entry.m_player->client_side_anim() = g_context->allow_anim_update() = true;
			entry.m_player->update_client_side_anim();
			entry.m_player->client_side_anim() = g_context->allow_anim_update() = false;

			backup.restore(entry.m_player);
		}

		auto& cur_anim_side = side < 3 ? current->m_sides.at(side) : current->m_low_sides.at(side - 3);
		cur_anim_side.m_abs_angles = entry.m_player->abs_angles();
		cur_anim_side.m_foot_yaw = anim_state->m_foot_yaw;
		cur_anim_side.m_anim_layers = entry.m_player->anim_layers();		

		entry.m_player->anim_layers() = current->m_anim_layers;

		setup_bones(entry.m_player, cur_anim_side.m_bones, current->m_sim_time, 15);

		const auto mdl_data = entry.m_player->mdl_data();
		if (!mdl_data
			|| !mdl_data->m_studio_hdr)
			return;

		cur_anim_side.m_bones_count = mdl_data->m_studio_hdr->m_bones_count;
	}

	float c_anim_sync::get_max_body_rotation(valve::c_player* player) const {
		auto anim_state = player->anim_state();

		auto aim_matrix_width_range = ((anim_state->m_walk_to_run_transition * -0.3f) - 0.2f) * std::max(0.f, std::min(1.f, anim_state->m_speed_as_portion_of_walk_speed)) + 1.f;
		if (anim_state->m_duck_amount)
			aim_matrix_width_range += ((anim_state->m_duck_amount * std::max(0.f, std::min(1.f, anim_state->m_speed_as_portion_of_crouch_speed))) * (0.5f - aim_matrix_width_range));

		return anim_state->m_max_body_yaw * aim_matrix_width_range;
	}

	void c_anim_sync::find_server_foot_yaw(
		player_entry_t& entry, lag_record_t* const current,
		lag_record_t* const previous, lag_record_t* const penultimate
	) const {
		current->m_priority = 5;
		current->m_side = entry.m_prev_side;
		current->m_type = 1;

		switch (entry.m_prev_type) {
		case 1:
			current->m_type = 2;
			break;
		case 2:
			current->m_type = 3;
			break;
		case 3:
			current->m_type = 4;
			break;
		}

		if (!previous) {
			current->m_type = 5;
			return;
		}

		if (current->m_flags & valve::e_ent_flags::fake_client
			|| (current->m_sim_ticks < 2 && previous->m_sim_ticks < 2 && penultimate && penultimate->m_sim_ticks < 2)) {
			current->m_side = 0;
			current->m_type = 6;

			goto end;
		}

		if ((current->m_flags & valve::e_ent_flags::frozen)
			|| (entry.m_player->move_type() == valve::e_move_type::ladder && current->m_eye_angles.x < 70.f)) {
			current->m_side = 0;
			current->m_type = 7;

			goto end;
		}

		const auto& shoot_pos = g_context->shoot_pos();
		const auto negative_pos = current->m_sides.at(1u).m_bones[8].get_origin();
		const auto positive_pos = current->m_sides.at(2u).m_bones[8].get_origin();
		const auto negative_info = g_auto_wall->fire_emulated(valve::g_local_player, entry.m_player, shoot_pos, negative_pos);
		const auto positive_info = g_auto_wall->fire_emulated(valve::g_local_player, entry.m_player, shoot_pos, positive_pos);

		if (negative_info.m_dmg != positive_info.m_dmg
			&& (negative_info.m_dmg > 10 || positive_info.m_dmg > 10)) {
			auto best_dmg = std::max(negative_info.m_dmg, positive_info.m_dmg) - 5;

			if (negative_info.m_dmg < best_dmg) {
				best_dmg = negative_info.m_dmg;
				entry.m_trace_side = 1;
			}

			if (positive_info.m_dmg < best_dmg)
				entry.m_trace_side = 2;
		}
	
		if (current->m_velocity.length_2d() <= 0.1f) {
			if (!entry.m_try_lby_resolver) {
				current->m_type = 8;
				goto end;
			}
						
			if (current->m_extending) {
				const auto angle_diff = math::angle_diff(current->m_eye_angles.y, current->m_sides.at(0u).m_foot_yaw);

				if (abs(angle_diff) > 35.f) {
					current->m_priority = 1;
					current->m_side = angle_diff < 0.f ? 2 : 1;
					current->m_type = 9;
					entry.m_prev_type = 1;
				}
			}			
			else {
			trace_part:
				if (!entry.m_try_trace_resolver) {
					current->m_type = 10;
					goto end;
				}

				if (entry.m_trace_side) {
					current->m_priority = 2;
					current->m_side = entry.m_trace_side;
					current->m_type = 11;
					entry.m_prev_type = 2;
				}
			}
		}
		else {
		anim_part:
			if (!entry.m_try_anim_resolver) {
				current->m_type = 12;
				goto end;
			}

			entry.m_server_rate = current->m_anim_layers.at(6u).m_playback_rate;
			entry.m_negative_rate = current->m_sides.at(1u).m_anim_layers.at(6u).m_playback_rate;
			entry.m_positive_rate = current->m_sides.at(2u).m_anim_layers.at(6u).m_playback_rate;
			entry.m_zero_rate = current->m_sides.at(0u).m_anim_layers.at(6u).m_playback_rate;
			entry.m_low_negative_rate = current->m_low_sides.at(0u).m_anim_layers.at(6u).m_playback_rate;
			entry.m_low_positive_rate = current->m_low_sides.at(1u).m_anim_layers.at(6u).m_playback_rate;

			// note: simv0l - make it yourself)
			current->m_side = 2;
			current->m_type = 13;
			entry.m_prev_type = 3;
		}
	end:
		if (current->m_shot) {
			current->m_priority = 1;
			current->m_side = 0;
			current->m_type = 17;
		}
		else if (current->m_throw) {
			current->m_priority = 1;
			current->m_side = 0;
			current->m_type = 18;
		}
		else if ((!(current->m_flags & valve::e_ent_flags::on_ground)
			&& !(previous->m_flags & valve::e_ent_flags::on_ground))) {
			current->m_priority = 1;
			current->m_side = current->m_type == 6 ? 0 : 3;
			current->m_type = 19;
		}
	}

	bool c_anim_sync::setup_bones(
		valve::c_player* const player,
		valve::bones_t& bones, const float time, const int flags
	) const {
		struct backup_t {
			__forceinline constexpr backup_t() = default;

			__forceinline backup_t(valve::c_player* const player)
				: m_real_time{ valve::g_global_vars->m_real_time },
				m_cur_time{ valve::g_global_vars->m_cur_time },
				m_frame_time{ valve::g_global_vars->m_frame_time },
				m_absolute_frame_time{ valve::g_global_vars->m_absolute_frame_time },
				m_interpolation_amount{ valve::g_global_vars->m_interpolation_amount },
				m_frame_count{ valve::g_global_vars->m_frame_count },
				m_tick_count{ valve::g_global_vars->m_tick_count },
				m_occlusion_frame{ player->occlusion_frame() },
				last_setup_bones_frame{ player->last_setup_bones_frame() },
				m_inverse_kinematics{ player->inverse_kinematics() },
				m_ent_client_effects{ player->ent_client_effects() },
				m_effects{ player->effects() },
				m_occlusion_flags{ player->occlusion_flags() } {}

			__forceinline void restore_globals() const {
				valve::g_global_vars->m_real_time = m_real_time;
				valve::g_global_vars->m_cur_time = m_cur_time;
				valve::g_global_vars->m_frame_time = m_frame_time;
				valve::g_global_vars->m_absolute_frame_time = m_absolute_frame_time;
				valve::g_global_vars->m_frame_count = m_frame_count;
				valve::g_global_vars->m_tick_count = m_tick_count;
				valve::g_global_vars->m_interpolation_amount = m_interpolation_amount;
			}

			float m_real_time{}, m_cur_time{}, m_frame_time{},
				m_absolute_frame_time{}, m_interpolation_amount{};

			int m_frame_count{}, m_tick_count{},
				m_occlusion_frame{}, last_setup_bones_frame{};

			valve::ik_context_t* m_inverse_kinematics{};

			uint8_t m_ent_client_effects{};

			size_t m_effects{}, m_occlusion_flags{};
		} backup{ player };

		g_context->force_bone_mask() = flags & 4;

		valve::g_global_vars->m_real_time = valve::g_global_vars->m_cur_time = time;
		valve::g_global_vars->m_frame_time = valve::g_global_vars->m_absolute_frame_time = valve::g_global_vars->m_interval_per_tick;
		valve::g_global_vars->m_frame_count = valve::g_global_vars->m_tick_count = valve::to_ticks(time);
		valve::g_global_vars->m_interpolation_amount = 0.f;

		if (flags & 8) {
			player->effects() |= 8u;
			player->occlusion_flags() &= ~0xau;
			player->occlusion_frame() = 0;
		}

		if (flags & 4) {
			player->inverse_kinematics() = nullptr;
			player->ent_client_effects() |= 2u;
		}

		if (flags & 2)
			player->last_setup_bones_frame() = 0;

		if (flags & 1) {
			player->mdl_bone_counter() = 0;
			player->last_setup_bones_time() = std::numeric_limits< float >::max() * -1.f;

			auto& bone_accessor = player->bone_accessor();

			bone_accessor.m_writable_bones = bone_accessor.m_readable_bones = 0;
		}

		auto& jiggle_bones = g_context->cvars().m_r_jiggle_bones;

		const auto backup_jiggle_bones = jiggle_bones->get_bool();

		jiggle_bones->set_int(false);

		g_context->allow_setup_bones() = true;
		const auto ret = player->setup_bones(bones.data(), 256, (((flags >> 4) & 1) << 9) + 0xffd00, time);
		g_context->allow_setup_bones() = false;

		jiggle_bones->set_int(backup_jiggle_bones);

		if (flags & 8) {
			player->effects() = backup.m_effects;
			player->occlusion_flags() = backup.m_occlusion_flags;
			player->occlusion_frame() = backup.m_occlusion_frame;
		}

		if (flags & 4) {
			player->inverse_kinematics() = backup.m_inverse_kinematics;
			player->ent_client_effects() = backup.m_ent_client_effects;
		}

		backup.restore_globals();

		if (!(flags & 4))
			return ret;

		const auto mdl_data = player->mdl_data();
		if (!mdl_data
			|| !mdl_data->m_studio_hdr)
			return ret;

		const auto hitbox_set = mdl_data->m_studio_hdr->hitbox_set(player->hitbox_set_index());
		if (!hitbox_set)
			return ret;

		for (int i{}; i < hitbox_set->m_hitboxes_count; ++i) {
			const auto hitbox = hitbox_set->hitbox(i);
			if (!hitbox
				|| hitbox->m_radius >= 0.f)
				continue;

			mat3x4_t rot_mat{};
			g_context->addresses().m_angle_matrix(hitbox->m_rotation, rot_mat);

			math::concat_transforms(
				bones[hitbox->m_bone], rot_mat, bones.at(hitbox->m_bone)
			);
		}

		return ret;
	}

	void c_anim_sync::on_net_update(player_entry_t& entry, lag_record_t* const current, lag_record_t* const previous, lag_record_t* const penultimate) const {
		struct anim_backup_t {
			__forceinline constexpr anim_backup_t() = default;

			__forceinline anim_backup_t(valve::c_player* const player)
				: m_anim_state{ *player->anim_state() },
				m_anim_layers{ player->anim_layers() },
				m_pose_params{ player->pose_params() } {}

			__forceinline void restore(valve::c_player* const player) const {
				*player->anim_state() = m_anim_state;
				player->anim_layers() = m_anim_layers;
				player->pose_params() = m_pose_params;
			}

			valve::anim_state_t		m_anim_state{};
			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup(entry.m_player);

		if (g_context->cvars().m_cl_lagcompensation->get_bool()) {
			if (previous) {
				if (previous->m_sim_time > current->m_sim_time) {
					entry.m_last_valid_time = current->m_sim_time + std::abs(previous->m_sim_time - current->m_sim_time) + valve::to_time(1);
					current->m_shifting = true;
				}
				else if (entry.m_last_valid_time > current->m_sim_time)
					current->m_shifting = true;	
				else if ((current->m_origin - previous->m_origin).length_sqr() > 4096.f)
					current->m_broke_lc = true;
			}
		}
		else
			current->m_shifting = current->m_broke_lc = false;
	
		if (current->m_sim_ticks < 1
			|| current->m_sim_ticks > 17)
			current->m_sim_ticks = 1;

		const auto& cur_alive_loop_layer = current->m_anim_layers.at(11u);
		if (current->m_flags & valve::e_ent_flags::fake_client)
			current->m_sim_ticks = 1;
		else if (previous) {
			const auto& prev_alive_loop_layer = previous->m_anim_layers.at(11u);
			auto ticks_animated = current->m_sim_ticks;

			if (cur_alive_loop_layer.m_playback_rate == prev_alive_loop_layer.m_playback_rate)
				ticks_animated = (cur_alive_loop_layer.m_cycle - prev_alive_loop_layer.m_cycle) / (cur_alive_loop_layer.m_playback_rate * valve::g_global_vars->m_interval_per_tick);
			else
				ticks_animated = ((((cur_alive_loop_layer.m_cycle / cur_alive_loop_layer.m_playback_rate) + ((1.f - prev_alive_loop_layer.m_cycle) / prev_alive_loop_layer.m_playback_rate)) / valve::g_global_vars->m_interval_per_tick));

			current->m_sim_ticks = std::min(std::max(ticks_animated, current->m_sim_ticks), 17);
		}

		const auto sim_time_diff = valve::to_time(current->m_sim_ticks);

		if (current->m_weapon
			&& current->m_last_shot_time > (current->m_sim_time - sim_time_diff)
			&& current->m_sim_time >= current->m_last_shot_time)
			if (const auto wpn_data = current->m_weapon->wpn_data(); wpn_data
				&& (static_cast<size_t>(wpn_data->m_unk_type - 2) <= 5 || current->m_weapon->item_index() == valve::e_item_index::taser))
				current->m_shot = true;

		if (current->m_weapon)
			if (const auto wpn_data = current->m_weapon->wpn_data(); wpn_data
				&& wpn_data->m_type == 9
				&& current->m_weapon->throw_time() != 0.f)
				current->m_throw = true;

		const auto max_speed = current->m_weapon ? fmaxf(current->m_weapon->max_speed(), 0.001f) : 260.f;

		if (previous) {
			const auto& cur_adjust_layer = current->m_anim_layers.at(3u);
			const auto& prev_adjust_layer = previous->m_anim_layers.at(3u);
			const auto& cur_land_or_climb_layer = current->m_anim_layers.at(5u);
			const auto& prev_land_or_climb_layer = previous->m_anim_layers.at(5u);
			const auto& cur_movement_move_layer = current->m_anim_layers.at(6u);
			const auto& prev_movement_move_layer = previous->m_anim_layers.at(6u);
			const auto& cur_movement_strafe_change_layer = current->m_anim_layers.at(7u);
			const auto& prev_movement_strafe_change_layer = previous->m_anim_layers.at(7u);
			const auto& prev_alive_loop_layer = previous->m_anim_layers.at(11u);

			float animation_velocity;

			current->m_velocity = (current->m_origin - previous->m_origin) / sim_time_diff;

			if (cur_movement_move_layer.m_playback_rate < 0.00001f)
				current->m_velocity = {};
			else {
				auto weight = cur_alive_loop_layer.m_weight;
				if (weight < 1.f
					&& cur_alive_loop_layer.m_playback_rate == prev_alive_loop_layer.m_playback_rate
					&& cur_alive_loop_layer.m_sequence == prev_alive_loop_layer.m_sequence
					&& cur_alive_loop_layer.m_cycle > prev_alive_loop_layer.m_cycle
					&& current->m_weapon == previous->m_weapon) {
					const auto speed_as_portion_of_run_top_speed = ((1.f - weight) * 0.35f) + 0.55f;
					if (speed_as_portion_of_run_top_speed > 0.55f 
						&& speed_as_portion_of_run_top_speed < 0.9f)
						animation_velocity = speed_as_portion_of_run_top_speed * max_speed;
					else if (speed_as_portion_of_run_top_speed > 0.9f)
						animation_velocity = current->m_velocity.length_2d();
				}

				if (animation_velocity <= 0.f) {
					auto weight = cur_movement_move_layer.m_weight;
					if (weight > 0.1f && weight < 0.9f
						&& cur_land_or_climb_layer.m_weight <= 0.f
						&& weight > prev_movement_move_layer.m_weight
						&& cur_adjust_layer.m_sequence == prev_adjust_layer.m_sequence
						&& cur_land_or_climb_layer.m_sequence == prev_land_or_climb_layer.m_sequence
						&& cur_movement_move_layer.m_sequence == prev_movement_move_layer.m_sequence
						&& cur_movement_strafe_change_layer.m_sequence == prev_movement_strafe_change_layer.m_sequence
						&& (current->m_flags & valve::e_ent_flags::on_ground)) {
						auto speed_modifier = 1.f;

						if (current->m_flags & valve::e_ent_flags::ducking)
							speed_modifier = 0.34f;
						else if (current->m_walking)
							speed_modifier = 0.52f;

						if (speed_modifier < 1.f)
							animation_velocity = (weight * (max_speed * speed_modifier));
					}
				}
			}

			if (animation_velocity > 0.f) {
				const auto speed = current->m_velocity.length_2d();
				if (speed > 0.f) {
					const auto modifier = animation_velocity / current->m_velocity.length_2d();
					current->m_velocity.x *= modifier;
					current->m_velocity.y *= modifier;
				}
			}
		}

		const auto velocity_length = current->m_velocity.length();
		if (velocity_length > max_speed)
			current->m_velocity *= max_speed / velocity_length;

		const auto at_target = math::angle_normalize(math::calculate_angle(valve::g_local_player->origin(), entry.m_player->abs_origin()).y);
		const auto eye_yaw = math::angle_normalize(current->m_eye_angles.y);

		current->m_sideways = abs(math::angle_normalize(eye_yaw - math::angle_normalize(at_target - 90.f))) < 45.f
			|| abs(math::angle_normalize(eye_yaw - math::angle_normalize(at_target + 90.f))) < 45.f;
		current->m_forward = abs(math::angle_normalize(eye_yaw - math::angle_normalize(at_target + 180.f))) < 45.f;
		current->m_max_body_rotation = get_max_body_rotation(entry.m_player);
		if (current->m_max_body_rotation < 35.f
			|| (previous && std::abs(math::angle_diff(previous->m_eye_angles.y, current->m_eye_angles.y)) > 55.f))
			current->m_too_much_diff = true;

		entry.m_player->set_abs_origin(current->m_origin);

		if (!entry.m_player->friendly()) {
			for (int i{ 1 }; i < 5; i++) {
				update(entry, current, previous, penultimate, i);

				anim_backup.restore(entry.m_player);
			}

			if (sdk::g_config_system->anti_aim_correction)
				current->m_trying_to_resolve = true;
		}

		update(entry, current, previous, penultimate, 0);

		if (current->m_trying_to_resolve)
			find_server_foot_yaw(entry, current, previous, penultimate);

		entry.m_player->anim_layers() = current->m_anim_layers;

		g_context->allow_setup_bones() = true;
		entry.m_player->setup_bones(entry.m_bones.data(), 256, 0xfff00, valve::g_global_vars->m_cur_time);
		g_context->allow_setup_bones() = false;
	}

	void c_anim_sync::update_local(const qangle_t& view_angles, const bool no_view_model) {
		const auto anim_state = valve::g_local_player->anim_state();
		if (!anim_state)
			return;

		const auto backup_cur_time = valve::g_global_vars->m_cur_time;
		const auto backup_frame_time = valve::g_global_vars->m_frame_time;

		valve::g_global_vars->m_cur_time = valve::to_time(valve::g_local_player->tick_base());
		valve::g_global_vars->m_frame_time = valve::g_global_vars->m_interval_per_tick;

		valve::g_local_player->set_local_view_angles(view_angles);

		anim_state->m_prev_update_frame = 0;
		if (anim_state->m_prev_update_time == valve::g_global_vars->m_cur_time)
			anim_state->m_prev_update_time += valve::g_global_vars->m_interval_per_tick;

		const auto backup_abs_velocity = valve::g_local_player->abs_velocity();

		valve::g_local_player->abs_velocity() = valve::g_local_player->velocity();

		const auto backup_eflags = valve::g_local_player->eflags();

		valve::g_local_player->eflags() &= ~0x1000u;

		const auto backup_client_side_anim = valve::g_local_player->client_side_anim();

		valve::g_local_player->client_side_anim() = g_context->allow_anim_update() = true;

		if (no_view_model)
			anim_state->update(view_angles.x, view_angles.y);
		else
			valve::g_local_player->update_client_side_anim();

		valve::g_local_player->client_side_anim() = backup_client_side_anim;

		g_context->allow_anim_update() = false;

		valve::g_local_player->eflags() = backup_eflags;

		valve::g_local_player->abs_velocity() = backup_abs_velocity;

		valve::g_global_vars->m_cur_time = backup_cur_time;
		valve::g_global_vars->m_frame_time = backup_frame_time;

		const auto walk_speed = std::clamp(anim_state->m_speed_as_portion_of_walk_speed, 0.f, 1.f);
		const auto run_speed = ((0.8f - (anim_state->m_walk_to_run_transition * 0.3f)) - 1.f) * walk_speed;

		auto body_yaw_modifier = run_speed + 1.f;

		if (anim_state->m_duck_amount > 0.f)
		{
			const auto crouch_walk_speed = std::clamp(anim_state->m_speed_as_portion_of_crouch_speed, 0.f, 1.f);

			body_yaw_modifier += (anim_state->m_duck_amount * crouch_walk_speed) * (0.5f - body_yaw_modifier);
		}

		m_local_data.m_max_body_yaw = anim_state->m_max_body_yaw * body_yaw_modifier;
		m_local_data.m_min_body_yaw = anim_state->m_min_body_yaw * body_yaw_modifier;
	}

	void c_anim_sync::update_local_real(valve::user_cmd_t& user_cmd) {
		const auto anim_state = valve::g_local_player->anim_state();
		if (!anim_state)
			return;

		const auto backup_anim_layers = valve::g_local_player->anim_layers();

		if (!g_anti_aim->enabled(&user_cmd)) {
			g_movement->normalize(user_cmd);

			const auto& prev_local_data = g_eng_pred->local_data().at((user_cmd.m_number - 1) % 150);
			if (prev_local_data.m_spawn_time == valve::g_local_player->spawn_time())
				prev_local_data.restore_anim(false);

			update_local(user_cmd.m_view_angles, false);

			g_eng_pred->local_data().at(user_cmd.m_number % 150).store_anim();

			valve::g_local_player->anim_layers() = backup_anim_layers;

			return;
		}

		const auto side = g_anti_aim->select_side();

		auto yaw = user_cmd.m_view_angles.y;
		g_anti_aim->select_yaw(yaw, side);

		const auto& first_local_data = g_eng_pred->local_data().at(valve::g_client_state->m_last_cmd_out % 150);
		if (first_local_data.m_spawn_time == valve::g_local_player->spawn_time())
			first_local_data.restore_anim(false);

		qangle_t shot_cmd_view_angles{};

		auto i = 1;
		auto choked_cmds = valve::g_client_state->m_choked_cmds;

		const auto total_cmds = choked_cmds + 1;
		if (total_cmds < 1) {
			valve::g_local_player->anim_layers() = backup_anim_layers;

			return;
		}

		for (; i <= total_cmds; ++i, --choked_cmds) {
			const auto& cur_local_data = g_eng_pred->local_data().at((valve::g_client_state->m_last_cmd_out + i) % 150);
			if (cur_local_data.m_spawn_time == valve::g_local_player->spawn_time())
				break;
		}

		const auto shot_in_this_cycle =
			m_local_data.m_shot_cmd_number > valve::g_client_state->m_last_cmd_out
			&& m_local_data.m_shot_cmd_number <= (valve::g_client_state->m_last_cmd_out + total_cmds);

		for (; i <= total_cmds; ++i, --choked_cmds) {
			const auto j = (valve::g_client_state->m_last_cmd_out + i) % 150;

			auto& cur_user_cmd = valve::g_input->m_cmds[j];
			auto& cur_local_data = g_eng_pred->local_data().at(j);

			if (cur_local_data.m_net_vars.m_move_type != valve::e_move_type::ladder
				&& cur_local_data.m_pred_net_vars.m_move_type != valve::e_move_type::ladder)
			{
				const auto old_view_angles = cur_user_cmd.m_view_angles;

				g_anti_aim->process(cur_user_cmd, yaw, side, choked_cmds);

				if (cur_user_cmd.m_view_angles.x != old_view_angles.x
					|| cur_user_cmd.m_view_angles.y != old_view_angles.y
					|| cur_user_cmd.m_view_angles.z != old_view_angles.z)
					g_movement->rotate(
						cur_user_cmd, old_view_angles,
						cur_local_data.m_pred_net_vars.m_flags,
						cur_local_data.m_pred_net_vars.m_move_type
					);
			}

			g_movement->normalize(cur_user_cmd);

			valve::g_local_player->origin() = cur_local_data.m_pred_net_vars.m_origin;
			valve::g_local_player->move_state() = cur_local_data.m_pred_net_vars.m_move_state;
			valve::g_local_player->strafing() = cur_local_data.m_pred_net_vars.m_strafing;
			valve::g_local_player->move_type() = cur_local_data.m_pred_net_vars.m_move_type;
			valve::g_local_player->scoped() = cur_local_data.m_pred_net_vars.m_scoped;
			valve::g_local_player->walking() = cur_local_data.m_pred_net_vars.m_walking;
			valve::g_local_player->lby() = cur_local_data.m_pred_net_vars.m_lby;

			valve::g_local_player->set_collision_bounds(
				cur_local_data.m_pred_net_vars.m_obb_min,
				cur_local_data.m_pred_net_vars.m_obb_max
			);

			valve::g_local_player->velocity() = cur_local_data.m_pred_net_vars.m_velocity;
			valve::g_local_player->abs_velocity() = cur_local_data.m_pred_net_vars.m_abs_velocity;
			valve::g_local_player->third_person_recoil() = cur_local_data.m_pred_net_vars.m_third_person_recoil;
			valve::g_local_player->duck_amount() = cur_local_data.m_pred_net_vars.m_duck_amount;
			valve::g_local_player->flags() = cur_local_data.m_pred_net_vars.m_flags;

			valve::g_local_player->tick_base() = cur_local_data.m_pred_net_vars.m_tick_base;

			auto cur_view_angles = cur_user_cmd.m_view_angles;

			if (shot_in_this_cycle) {
				if (cur_user_cmd.m_number == m_local_data.m_shot_cmd_number)
					shot_cmd_view_angles = cur_view_angles;

				if (cur_user_cmd.m_number > m_local_data.m_shot_cmd_number)
					cur_view_angles = shot_cmd_view_angles;
			}

			const auto last_user_cmd = cur_user_cmd.m_number == user_cmd.m_number;

			update_local(cur_view_angles, !last_user_cmd);

			cur_local_data.m_user_cmd = cur_user_cmd;

			cur_local_data.store_anim();

			if (last_user_cmd)
				continue;

			valve::g_input->m_verified_cmds[j] = { cur_user_cmd, cur_user_cmd.calc_checksum() };
		}

		valve::g_local_player->anim_layers() = backup_anim_layers;
	}

	void c_anim_sync::setup_local_bones() {
		const auto anim_state = valve::g_local_player->anim_state();
		if (!anim_state)
			return;

		const auto& local_data = g_eng_pred->local_data().at(g_context->last_sent_cmd_number() % 150);
		if (local_data.m_spawn_time != valve::g_local_player->spawn_time())
			return;

		struct anim_backup_t {
			__forceinline anim_backup_t()
				: m_anim_state{ *valve::g_local_player->anim_state() },
				m_abs_yaw{ m_anim_state.m_foot_yaw },
				m_anim_layers{ valve::g_local_player->anim_layers() },
				m_pose_params{ valve::g_local_player->pose_params() } {}

			__forceinline void restore() const {
				valve::g_local_player->set_abs_angles({ 0.f, m_abs_yaw, 0.f });

				*valve::g_local_player->anim_state() = m_anim_state;

				valve::g_local_player->anim_layers() = m_anim_layers;
				valve::g_local_player->pose_params() = m_pose_params;
			}

			valve::anim_state_t		m_anim_state{};

			float					m_abs_yaw{};

			valve::anim_layers_t	m_anim_layers{};
			valve::pose_params_t	m_pose_params{};
		} anim_backup{};

		local_data.restore_anim(true);

		if (m_local_data.m_fake.m_spawn_time == 0.f
			|| m_local_data.m_fake.m_spawn_time != valve::g_local_player->spawn_time()) {
			m_local_data.m_fake.m_anim_state = *valve::g_local_player->anim_state();
			m_local_data.m_fake.m_spawn_time = valve::g_local_player->spawn_time();
		}

		if (!valve::g_client_state->m_choked_cmds
			&& valve::g_global_vars->m_cur_time != m_local_data.m_fake.m_anim_state.m_prev_update_time) {
			*valve::g_local_player->anim_state() = m_local_data.m_fake.m_anim_state;

			anim_state->m_foot_yaw = local_data.m_user_cmd.m_view_angles.y;

			anim_state->update(
				local_data.m_user_cmd.m_view_angles.x,
				local_data.m_user_cmd.m_view_angles.y
			);

			m_local_data.m_fake.m_abs_yaw = anim_state->m_foot_yaw;
			m_local_data.m_fake.m_anim_state = *valve::g_local_player->anim_state();
			m_local_data.m_fake.m_anim_layers = valve::g_local_player->anim_layers();
			m_local_data.m_fake.m_pose_params = valve::g_local_player->pose_params();
		}

		local_data.restore_anim(true);

		valve::g_local_player->anim_layers() = m_local_data.m_fake.m_anim_layers;
		valve::g_local_player->pose_params() = m_local_data.m_fake.m_pose_params;

		valve::g_local_player->set_abs_angles({ 0.f, m_local_data.m_fake.m_abs_yaw, 0.f });

		setup_bones(valve::g_local_player, m_local_data.m_fake.m_bones, valve::g_global_vars->m_cur_time, 11);

		local_data.restore_anim(true);

		setup_bones(valve::g_local_player, m_local_data.m_real.m_bones, valve::g_global_vars->m_cur_time, 27);

		anim_backup.restore();
	}
}