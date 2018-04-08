#pragma once

namespace HaloScriptCommon {
	enum class hs_opcode : unsigned int {
		begin,
		begin_random,
		hs_if,
		cond,
		set,
		and,
		or ,
		add,
		subtract,
		multiple,
		divide,
		min,
		max,
		equal,
		not_equal,
		greater_then,
		lesser_than,
		greater_than_or_equal,
		lesser_than_or_equal,
		sleep,
		sleep_forever,
		sleep_until,
		wake,
		inspect,
		unit,
		not,
		pin,
		print,
		players,
		kill_volume_enable,
		kill_volume_disable,
		volume_teleport_players_not_inside,
		volume_test_object,
		volume_test_objects,
		volume_test_objects_all,
		volume_return_objects,
		volume_return_objects_by_type,
		list_get,
		list_count,
		list_count_not_dead,
		effect_new,
		effect_new_on_object_marker,
		damage_new,
		damage_object,
		damage_objects,
		damage_players,
		object_create,
		object_create_clone,
		object_create_anew,
		object_create_containing,
		object_create_clone_containing,
		object_create_anew_containing,
		object_destroy,
		object_destroy_containing,
		object_destroy_all,
		object_destroy_type_mask,
		objects_delete_by_definition,
		object_hide,
		object_set_shadowless,
		object_function_set,
		object_set_function_variable,
		object_clear_function_variable,
		object_clear_all_function_variables,
		object_dynamic_simulation_disable,
		object_set_phantom_power,
		object_wake_physics,
		object_set_ranged_attack_inhibited,
		object_set_melee_attack_inhibited,
		objects_dump_memory,
		object_get_health,
		object_get_shield,
		object_set_shield_effect,
		object_set_physics,
		object_get_parent,
		objects_attach,
		object_at_marker,
		objects_detach,
		object_set_scale,
		object_set_velocity_scalar,
		object_set_velocity_vector,
		object_set_deleted_when_deactivated,
		object_copy_player_appearance,
		object_model_target_destroyed,
		object_model_targets_destroyed,
		object_damage_damage_section,
		object_cannot_die,
		object_vitality_pinned,
		garbage_collect_now,
		garbage_collect_unsafe,
		garbage_collect_multiplayer,
		object_cannot_take_damage,
		object_can_take_damage,
		object_cinematic_lod,
		object_cinematic_collision,
		object_cinematic_visibility,
		object_uses_cinematic_lighting,
		cinematic_lighting_set_primary_light,
		cinematic_lighting_set_secondary_light,
		cinematic_lighting_set_ambient_light,
		objects_predict,
		objects_predict_high,
		objects_predict_low,
		object_type_predict_high,
		object_type_predict_low,
		object_type_predict,
		object_teleport,
		object_set_facing,
		object_set_shield,
		object_set_shield_stun,
		object_set_shield_stun_infinite,
		object_set_permutation,
		object_set_region_state,
		objects_can_see_object,
		objects_can_see_flag,
		objects_distance_to_object,
		objects_distance_to_flag,
		map_info,
		position_predict,
		shader_predict,
		bitmap_predict,
		script_recompile,
		script_doc,
		help,
		game_engine_objects,
		random_range,
		real_random_range,
		physics_constants_reset,
		physics_set_gravity,
		physics_set_velocity_frame,
		physics_disable_character_ground_adhesion_forces,
		havok_debug_start,
		havok_dump_world,
		havok_dump_world_close_movie,
		havok_profile_start,
		havok_profile_end,
		havok_reset_allocated_state,
		breakable_surfaces_enable,
		breakable_surfaces_reset,
		recording_play,
		recording_play_and_delete,
		recording_play_and_hover,
		recording_kill,
		recording_time,
		render_lights,
		texture_camera_set_object_marker,
		texture_camera_off,
		render_lights_enable_cinematic_shadow,
		fog_report,
		fog_report_summary,
		fog_report_verbose,
		fog_mode_control,
		fog_override_off,
		fog_override_mode,
		fog_override_mode_off,
		fog_override_atmospheric_fog,
		fog_override_atmospheric_fog_off,
		fog_override_secondary_fog,
		fog_override_secondary_fog_off,
		fog_override_sky_fog,
		fog_override_sky_fog_off,
		fog_override_planar_fog,
		fog_override_planar_fog_off,
		fog_override_planar_fog_plane,
		fog_override_planar_fog_plane_off,
		fog_override_planar_max_atmospheric_depth,
		fog_override_planar_max_atmospheric_depth_off,
		fog_override_planar_planar_eye_density,
		fog_override_planar_planar_eye_density_off,
		fog_override_atmospheric_secondary_blend,
		fog_override_atmospheric_secondary_blend_off,
		fog_override_atmospheric_planar_blend,
		fog_override_atmospheric_planar_blend_off,
		render_debug_structure_cluster_visibility,
		render_debug_structure_cluster_fog,
		render_debug_structure_fog_plane,
		render_debug_structure_fog_plane_infinite_extent,
		render_debug_structure_fog_zone,
		render_debug_structure_fog_zone_floodfill,
		render_debug_structure_all_fog_planes,
		render_debug_structure_all_cluster_errors,
		render_debug_structure_line_opacity,
		render_debug_structure_text_opacity,
		render_debug_structure_opacity,
		render_debug_structure_non_occluded_fog_planes,
		scenery_animation_start,
		scenery_animation_start_loop,
		scenery_animation_start_relative,
		scenery_animation_start_relative_loop,
		scenery_animation_start_at_frame,
		scenery_animation_idle,
		scenery_get_animation_time,
		render_effects,
		unit_can_blink,
		unit_set_active_camo,
		unit_open,
		unit_close,
		unit_kill,
		unit_kill_silent,
		unit_is_emitting,
		unit_get_custom_animation_time,
		unit_stop_custom_animation,
		custom_animation,
		custom_animation_loop,
		custom_animation_relative,
		custom_animation_relative_loop,
		custom_animation_list,
		unit_custom_animation_at_frame,
		unit_is_playing_custom_animation,
		object_set_custom_animations_hold_on_last_frame,
		object_set_custom_animations_prevent_lipsync_head_movement,
		unit_set_actively_controlled,
		unit_get_team_index,
		unit_aim_without_turning,
		unit_set_enterable_by_player,
		unit_get_enterable_by_player,
		unit_only_takes_damage_from_players_team,
		unit_enter_vehicle,
		unit_board_vehicle,
		unit_set_emotion,
		unit_set_emotion_animation,
		unit_set_emotional_state,
		unit_enable_eye_tracking,
		unit_in_vehicle,
		vehicle_test_seat_list,
		vehicle_test_seat,
		unit_set_prefer_tight_camera_track,
		unit_exit_vehicle,
		unit_exit_vehicle_exit_type,
		unit_set_maximum_vitality,
		units_set_maximum_vitality,
		unit_set_current_vitality,
		units_set_current_vitality,
		vehicle_load_magic,
		vehicle_unload,
		unit_set_animation_mode,
		magic_melee_attack,
		vehicle_riders,
		vehicle_driver,
		vehicle_gunner,
		unit_get_health,
		unit_get_shield,
		unit_get_total_grenade_count,
		unit_has_weapon,
		unit_has_weapon_readied,
		unit_lower_weapon,
		unit_raise_weapon,
		unit_animation_forced_seat,
		unit_doesnt_drop_items,
		unit_impervious,
		unit_suspended,
		unit_add_equipment,
		weapon_hold_trigger,
		weapon_enable_warthog_chaingun_light,
		device_set_never_appears_locked,
		device_set_power,
		device_get_power,
		device_set_position,
		device_get_position,
		device_set_position_immediate,
		device_group_get,
		device_group_set,
		device_group_set_immediate,
		device_one_sided_set,
		device_operates_automatically_set,
		device_closes_automatically_set,
		device_group_change_only_once_more_set,
		device_set_position_track,
		device_set_overlay_track,
		device_animate_position,
		device_animate_overlay,
		cheat_all_powerups,
		cheat_all_weapons,
		cheat_all_vehicles,
		cheat_teleport_to_camera,
		cheat_active_camouflage,
		cheat_active_camouflage_by_player,
		cheats_load,
		drop,
		ai_enable,
		ai_enabled,
		ai_grenades,
		ai_dialogue_enable,
		ai_dialogue_log_reset,
		ai_dialogue_log_dump,
		ai_get_object,
		ai_get_unit,
		ai_attach,
		ai_attach_units,
		ai_detach,
		ai_detach_units,
		ai_place,
		ai_place_sqaud_with_member_count,
		ai_place_in_vehicle,
		ai_cannot_die,
		ai_vitality_pinned,
		ai_overcomes_oversteer,
		ai_kill,
		ai_kill_silent,
		ai_erase,
		ai_erase_all,
		ai_disposable,
		ai_select,
		ai_deselect,
		ai_set_deaf,
		ai_set_blind,
		ai_magically_see,
		ai_magically_see_object,
		ai_set_active_camo,
		ai_suppress_combat,
		ai_migrate,
		ai_allegiance,
		ai_allegiance_remove,
		ai_braindead,
		ai_braindead_by_unit,
		ai_disregard,
		ai_prefer_target,
		ai_teleport_to_starting_location_if_outside_bsp,
		ai_renew,
		ai_force_active,
		ai_force_active_by_unit,
		ai_playfight,
		ai_reconnect,
		ai_berserk,
		ai_set_team,
		ai_allow_dormant,
		ai_is_attacking,
		ai_fighting_count,
		ai_living_count,
		ai_living_fraction,
		ai_strength,
		ai_swarm_count,
		ai_nonswarm_count,
		ai_actors,
		ai_allegiance_broken,
		ai_set_orders,
		ai_spawn_count,
		object_get_ai,
		ai_trigger_test,
		generate_pathfinding,
		ai_render_paths_all,
		ai_vehicle_get,
		ai_vehicle_get_from_starting_location,
		ai_vehicle_reserve_seat,
		ai_vehicle_reserve,
		ai_vehicle_enter_seat_mapping,
		ai_vehicle_enter,
		ai_vehicle_enter_immediate_seat_mapping,
		ai_vehicle_enter_immediate,
		ai_enter_squad_vehicles,
		ai_vehicle_exit_seat_mapping,
		ai_vehicle_exit,
		vehicle_overturned,
		vehicle_flip,
		ai_combat_status,
		flock_start,
		flock_stop,
		flock_create,
		flock_delete,
		ai_verify_tags,
		ai_wall_lean,
		ai_play_line,
		ai_play_line_at_player,
		ai_play_line_on_object,
		ai_scene_sqaud,
		ai_scene_sqaud_set,
		ai_scene_sqaud_set_2,
		cs_run_command_script,
		cs_queue_command_script,
		cs_stack_command_script,
		cs_run_joint_command_script,
		cs_run_joint_command_script_2,
		cs_command_script_running,
		cs_command_script_queued,
		cs_number_queued,
		cs_switch,
		cs_switch_index,
		cs_transfer,
		cs_transfer_stack,
		cs_transfer_queue,
		cs_fly_to,
		cs_fly_to_tolerance,
		cs_fly_to_and_face,
		cs_fly_to_and_face_tolerance,
		cs_fly_by,
		cs_fly_by_tolerance,
		cs_go_to,
		cs_go_to_tolerance,
		cs_go_by,
		cs_go_by_tolerance,
		cs_go_to_and_face,
		cs_start_to,
		cs_go_to_nearest,
		cs_moving,
		cs_look,
		cs_look_player,
		cs_look_object,
		cs_aim,
		cs_aim_player,
		cs_aim_object,
		cs_face,
		cs_face_player,
		cs_face_object,
		cs_move_in_direction,
		cs_pause,
		cs_shoot,
		cs_shoot_object,
		cs_shoot_point,
		cs_vehicle_speed,
		cs_grenade,
		cs_jump,
		cs_jump_to_point,
		cs_vocalize,
		cs_play_sound,
		cs_play_sound_len,
		cs_play_sound_len_volume,
		cs_stop_sound,
		cs_custom_animation,
		cs_stop_custom_animation,
		cs_play_line,
		cs_die,
		cs_teleport,
		cs_animate,
		cs_movement_mode,
		cs_crouch,
		cs_crouch_time,
		cs_set_pathfinding_radius,
		cs_go_to_vehicle,
		cs_set_behavior,
		cs_formation,
		cs_deploy_turret,
		cs_approach,
		cs_start_approach,
		cs_approach_player,
		cs_start_approach_player,
		cs_approach_stop,
		cs_ignore_obstacles,
		cs_turn_sharpness,
		cs_vehicle_speed_instantaneous,
		cs_vehicle_boost,
		cs_abort_on_alert,
		cs_abort_on_damage,
		cs_abort_on_combat_status,
		cs_enable_targeting,
		cs_enable_looking,
		cs_enable_moving,
		cs_enable_dialogue,
		cs_suppress_dialogue_global,
		cs_set_style,
		cs_force_combat_status,
		cs_enable_pathfinding_failsafe,
		camera_control,
		camera_set,
		camera_set_relative,
		camera_set_animation,
		camera_set_animation_relative,
		camera_predict_resources_at_frame,
		camera_predict_resources_at_point,
		camera_set_first_person,
		camera_time,
		camera_set_field_of_view,
		camera_set_pan,
		camera_pan,
		debug_camera_save,
		debug_camera_load,
		debug_camera_save_name,
		debug_camera_load_name,
		director_debug_camera,
		game_difficulty_get,
		game_difficulty_get_real,
		pvs_set_object,
		pvs_set_camera,
		pvs_clear,
		players_unzoom_all,
		player_enable_input,
		player_disable_movement,
		player_flashlight_on,
		player_active_camouflage_on,
		player_camera_control,
		player_action_test_reset,
		player_action_test_jump,
		player_action_test_primary_trigger,
		player_action_test_grenade_trigger,
		player_action_test_vision_trigger,
		player_action_test_zoom,
		player_action_test_rotate_weapons,
		player_action_test_rotate_grenades,
		player_action_test_melee,
		player_action_test_action,
		player_action_test_accept,
		player_action_test_cancel,
		player_action_test_look_relative_up,
		player_action_test_look_relative_down,
		player_action_test_look_relative_left,
		player_action_test_look_relative_right,
		player_action_test_look_relative_all_directions,
		player_action_test_move_relative_all_directions,
		player_action_test_start,
		player_action_test_back,
		player0_looking_up,
		player0_looking_down,
		player_action_test_look_up_begin,
		player_action_test_look_down_begin,
		player_action_test_look_pitch_end,
		player_action_test_lookstick_forward,
		player_action_test_lookstick_backward,
		debug_teleport_player,
		map_reset,
		switch_bsp,
		switch_bsp_by_name,
		structure_bsp_index,
		crash,
		version,
		status,
		record_movie,
		record_movie_distributed,
		screenshot,
		screenshot_big,
		screenshot_big_jittered,
		screenshot_cubemap,
		main_menu,
		main_halt,
		map_name,
		game_multiplayer,
		game_splitscreen,
		game_difficulty,
		game_coop_players,
		game_initial_bsp,
		game_tick_rate,
		game_start,
		game_rate,
		texture_cache_flush,
		geometry_cache_flush,
		sound_cache_flush,
		animation_cache_flush,
		font_cache_flush,
		texture_cache_test_malloc,
		debug_memory,
		debug_memory_by_file,
		debug_memory_for_file,
		debug_tags,
		tags_verify_all,
		profile_reset,
		profile_activate,
		profile_deactivate,
		profile_mode,
		collision_log_enable,
		damage_control_get,
		damage_control_set,
		ai_lines,
		ai_debug_sound_point_set,
		ai_dialogue_break_on_vocalization,
		fade_in,
		fade_out,
		cinematic_start,
		cinematic_stop,
		cinematic_skip_start_internal,
		cinematic_skip_stop_internal,
		cinematic_show_letterbox,
		cinematic_show_letterbox_immediate,
		cinematic_set_title,
		cinematic_set_title_delayed,
		cinematic_suppress_bsp_object_creation,
		cinematic_subtitle,
		attract_mode_start,
		attract_mode_set_seconds,
		game_won,
		game_lost,
		game_revert,
		game_is_cooperative,
		game_is_playtest,
		game_can_use_flashlights,
		game_save_and_quit,
		game_save_unsafe,
		debug_spawning,
		king_set_hill,
		core_load,
		core_load_name,
		core_save,
		core_save_name,
		core_load_game,
		core_load_game_name,
		core_regular_upload_to_debug_server,
		core_set_upload_option,
		film_play_hack,
		game_safe_to_save,
		game_safe_to_speak,
		game_all_quiet,
		game_save,
		game_save_cancel,
		game_save_no_timeout,
		game_save_immediate,
		game_saving,
		game_reverted,
		sound_set_tag_parameter_unsafe,
		sound_impulse_predict,
		sound_impulse_trigger,
		sound_impulse_start,
		sound_impulse_start_cinematic,
		sound_impulse_start_effect,
		sound_impulse_time,
		sound_impulse_time_2,
		sound_impulse_language_time,
		sound_impulse_stop,
		sound_impulse_start_3d,
		sound_looping_predict,
		sound_looping_start,
		sound_looping_stop,
		sound_looping_stop_immediately,
		sound_looping_set_scale,
		sound_looping_set_alternate,
		sound_loop_spam,
		sound_class_show_channel,
		sound_class_debug_sound_start,
		debug_sounds_enable,
		sound_class_set_gain,
		sound_class_set_gain_db,
		sound_class_enable_ducker,
		debug_sound_environment_parameter,
		sound_set_global_effect,
		sound_set_global_effect_scale,
		sound_set_trace_callback_level,
		sound_dump_miles_debug,
		vehicle_hover,
		vehicle_count_bipeds_killed,
		biped_ragdoll,
		hud_cinematic_fade,
		show_hud,
		show_hud_help_text,
		show_hud_messages,
		enable_hud_help_flash,
		hud_help_flash_restart,
		hud_show_training_text,
		hud_set_training_text,
		hud_enable_training,
		player_training_activate_flashlight,
		player_training_activate_crouch,
		player_training_activate_stealth,
		activate_nav_point_flag,
		activate_nav_point_object,
		activate_team_nav_point_flag,
		activate_team_nav_point_object,
		deactivate_nav_point_flag,
		deactivate_nav_point_object,
		deactivate_team_nav_point_flag,
		deactivate_team_nav_point_object,
		cls,
		error_overflow_suppression,
		error_geometry_show,
		error_geometry_hide,
		error_geometry_show_all,
		error_geometry_hide_all,
		error_geometry_list,
		player_effect_set_max_translation,
		player_effect_set_max_rotation,
		player_effect_set_max_vibration,
		player_effect_start,
		player_effect_stop,
		hud_show_health,
		hud_blink_health,
		hud_show_shield,
		hud_blink_shield,
		hud_show_motion_sensor,
		hud_blink_motion_sensor,
		hud_show_crosshair,
		hud_show_ammo,
		hud_clear_messages,
		hud_set_help_text,
		hud_set_objective_text,
		hud_set_timer_time,
		hud_set_timer_warning_time,
		hud_set_timer_position,
		show_hud_timer,
		pause_hud_timer,
		hud_get_timer_ticks,
		time_code_show,
		time_code_start,
		time_code_reset,
		rasterizer_profile_include,
		rasterizer_profile_include_all,
		rasterizer_profile_include_all_except,
		rasterizer_profile_exclude,
		rasterizer_profile_exclude_all,
		rasterizer_profile_exclude_all_except,
		rasterizer_debug_display_bitmap,
		rasterizer_decals_flush,
		rasterizer_lens_flares_clear_for_frame,
		rasterizer_test_screen,
		rasterizer_test_screen_supablur,
		rasterizer_test_screen_off,
		rasterizer_f2,
		rasterizer_f3,
		rasterizer_f4,
		rasterizer_f5,
		rasterizer_f6,
		rasterizer_decal_depth_bias,
		rasterizer_decal_depth_bias_slope_scale,
		rasterizer_hud_ten_foot,
		rasterizer_blur,
		rasterizer_solid_model_color,
		rasterizer_solid_structure_color,
		rasterizer_solid_structure_instanced_geometry_color,
		rasterizer_solid_sky_color,
		rasterizer_solid_decorator_color,
		rasterizer_solid_decorator_model_color,
		rasterizer_solid_decorator_decal_color,
		rasterizer_solid_decorator_sprite_color,
		decorator_rebuild_all,
		script_screen_effect_set_value,
		cinematic_screen_effect_start,
		cinematic_screen_effect_set_depth_of_field,
		cinematic_screen_effect_set_crossfade,
		cinematic_screen_effect_set_crossfade2,
		cinematic_screen_effect_stop,
		cinematic_set_near_clip_distance,
		cinematic_set_far_clip_distance,
		cinematic_set_environment_map_attenuation,
		cinematic_set_environment_map_bitmap,
		cinematic_reset_environment_map_bitmap,
		cinematic_set_environment_map_tint,
		cinematic_reset_environment_map_tint,
		cinematic_layer,
		cinematic_dynamic_reflections,
		lightmaps_expose,
		player_profile_create,
		player_profile_create_corrupt,
		player_has_skills,
		player_has_mad_secret_skills,
		controller_invert_look,
		controller_look_speed,
		controller_set_look_invert,
		controller_get_look_invert,
		controller_unlock_solo_levels,
		user_interface_controller_get_last_level_played,
		ui_load_main_menu,
		ui_text_bounds,
		ui_show_title_safe_bounds,
		ui_element_bounds,
		ui_screen_tag,
		ui_transition_out_console_window,
		ui_show_screen_tag_path,
		ui_show_current_screen_tag,
		ui_set_beta,
		ui_memory_dump,
		ui_set_automation_mode,
		ui_set_automation_hopper_type,
		ui_set_automation_variant_type_and_map,
		ui_set_automation_desired_local_user_count,
		ui_set_automation_desired_desired_network_game_player_count,
		ui_set_automation_desired_network_game_length_seconds,
		ui_set_automation_desired_network_session_name,
		ui_set_automation_desired_controller_team,
		ui_set_automation_desired_controller_player_profile,
		objectives_clear,
		objectives_show_up_to,
		objectives_finish_up_to,
		input_suppress_vibration,
		update_remote_camera,
		net_status_filter,
		net_sim_reset,
		net_sim_spike_now,
		net_sim_dropspike_now,
		net_ping,
		net_channel_loopback,
		net_channel_delete,
		net_leave_squad,
		net_delegate_host,
		net_delegate_leader,
		net_map_name,
		net_campaign_difficulty,
		net_player_color,
		net_reset_objects,
		net_fatal_error,
		net_matchmaking_hopper_list,
		net_matchmaking_hopper_print,
		net_matchmaking_hopper_set_game,
		net_set_machine_name,
		net_event_display_category,
		net_event_log_category,
		net_event_list_categories,
		online_task_list,
		play_bink_movie,
		play_credits,
		bink_done,
		set_global_doppler_factor,
		set_global_mixbin_headroom,
		debug_sound_environment_source_parameter,
		data_mine_set_mission_segment,
		data_mine_display_mission_segment,
		data_mine_insert,
		data_mine_upload,
		data_mine_playback,
		data_mine_playback_last,
		data_mine_playback_exit,
		data_mine_enable,
		data_mine_track_event,
		data_mine_display_event,
		data_mine_show_all_events,
		data_mine_show_tracked_events,
		data_mine_show_displayed_events,
		data_mine_display_session_data,
		data_mine_display_disk_writes,
		data_mine_debug_menu_setting,
		data_mine_open_debug_menu,
		error_enable,
		render_layer_enable,
		render_layer_enable_all,
		rasterizer_overdraw_z,
		rasterizer_overdraw,
		rasterizer_layers_overdraw_z,
		rasterizer_layers_overdraw,
		test_memory_allocators,
		test_memory_allocators_save,
		display_video_standard,
		test_xcr_monkey_enable,
		test_web_map_snapshot,
		test_telnet_status_enable,
		test_telnet_status_interval,
		webstats_disable,
		webstats_submit,
		netdebug_prefer_internet,
		script_temporary_disable_lightmap_shadows,
		flag_new,
		flag_new_at_look,
		flags_clear,
		flags_save,
		flags_save_filtered,
		flags_save_named,
		flags_save_named_filtered,
		flags_default_name,
		flags_default_comment,
		flags_set_filter,
		flags_export,
		flags_export_filtered,
		flags_export_named,
		flags_export_named_filtered,
		bug_now,
		bug_now_lite,
		bug_now_auto,
		object_list_children,
		voice_set_outgoing_channel_count,
		voice_set_voice_repeater_peer_index,
		net_leaderboard_clear,
		net_leaderboard_set,
		dump_loaded_tags,
		interpolator_start,
		interpolator_start_smooth,
		interpolator_stop,
		interpolator_restart,
		interpolator_is_active,
		interpolator_is_finished,
		interpolator_set_current_value,
		interpolator_get_current_value,
		interpolator_get_start_value,
		interpolator_get_final_value,
		interpolator_get_current_phase,
		interpolator_get_current_time_fraction,
		interpolator_get_start_time,
		interpolator_get_final_time,
		interpolator_evaluate_at,
		interpolator_evaluate_at_time_fraction,
		interpolator_evaluate_at_time,
		interpolator_evaluate_at_time_delta,
		interpolator_stop_all,
		interpolator_restart_all,
		interpolator_flip,
		weather_start,
		weather_stop,
		weather_change_intensity,
		animation_cache_stats_reset,
		cinematic_clone_players_weapon,
		cinematic_move_attached_objects,
		vehicle_enable_ghost_effects,
		ice_cream_flavor_stock,
		ice_cream_flavor_available,
		set_global_sound_environment,
		reset_dsp_image,
		game_save_cinematic_skip,
		cinematic_outro_start,
		cinematic_enable_ambience_details,
		set_rasterizer_gamma,
		predict_model_section,
		predict_structure_section,
		predict_lightmap_bucket,
		predict_bitmap,
		get_prediction_offset,
		rasterizer_bloom_override,
		rasterizer_bloom_override_reset,
		rasterizer_bloom_override_blur_amount,
		rasterizer_bloom_override_threshold,
		rasterizer_bloom_override_brightness,
		rasterizer_bloom_override_box_factor,
		rasterizer_bloom_override_max_factor,
		rasterizer_bloom_override_silver_bullet,
		rasterizer_bloom_override_only,
		rasterizer_bloom_override_high_res,
		rasterizer_bloom_override_brightness_alpha,
		rasterizer_bloom_override_max_factor_alpha,
		cache_block_for_one_frame,
		sound_suppress_ambience_update_on_revert,
		ui_error_ok,
		ui_error_ok_cancel,
		ui_confirmation,
		cinematic_lightmap_shadow_disable,
		cinematic_lightmap_shadow_enable,
		predict_animation,
		clan_data_cache_flush,
		online_message_send,
		test_web_event_local_cheater,
		test_web_event_remote_cheater,
		test_cheating_detect,
		test_network_storage_simulate,
		scenario_dump_scenario_signature_enable,
		scenario_force_signature_failure_enable,
		scenario_force_name_failure_enable,
		scenario_ignore_failures_enable,
		scenario_dump_scenario_signature_disable,
		scenario_force_signature_failure_disable,
		scenario_force_name_failure_disable,
		scenario_ignore_failures_disable,
		loading_screen_fade_to_white,
		rasterizer_debug_frame_usage,
		rasterizer_debug_crap_pixel_shader,
		enable_render_light_suppressor,
		disable_render_light_suppressor,

		// existing halo 2 functions the toolkit couldn't use
		mess_up_rendering,
		hs_unk_1,
		hs_unk_2,

		// API extension functions
		// TODO


		enum_count,
		invalid = 0xFFFFFFFF
	};
}