meta:
  id: dat
  title: Silent Hill (PSX) Demo State Format
  file-extension: dat
  endian: le
  bit-endian: le
  encoding: ASCII

doc: |
  DAT is the demo state format used in Silent Hill. This spec covers all
  `MISC/DEMO****.DAT` files.

seq:
  - id: options_config
    type: options_config

  - size: 200

  - id: savegame
    type: savegame
  
  - size: 1148

  - id: frame_count
    type: u4

  - id: rand_seed
    type: u2

types:
  options_config:
    seq:
      - id: controller_config
        type: controller_config

      - id: screen_position_x
        type: s1

      - id: screen_position_y
        type: s1

      - id: sound_type
        type: u1

      - id: volume_bgm
        type: u1

      - id: volume_se
        type: u1

      - id: vibration_enabled
        type: u1

      - id: brightness
        type: u1

      - id: extra_weapon_ctrl
        type: u1

      - id: extra_blood_color
        type: u1

      - id: auto_load
        type: s1

      - id: unused_26
        type: s1

      - id: extra_options_enabled
        type: u1

      - id: extra_view_ctrl
        type: s1

      - id: extra_view_mode
        type: s1

      - id: extra_retreat_turn
        type: s1

      - id: walk_run_ctrl
        type: s1

      - id: extra_auto_aiming
        type: s1

      - id: extra_bullet_adjust
        type: s1

      - id: seen_game_over_tips
        type: u2

      - id: unk_30
        size: 4

      - id: pal_language_id
        type: u4

  controller_config:
    seq:
      - id: enter
        type: u2
        enum: controller_flags

      - id: cancel
        type: u2
        enum: controller_flags

      - id: skip
        type: u2
        enum: controller_flags

      - id: action
        type: u2
        enum: controller_flags

      - id: aim
        type: u2
        enum: controller_flags

      - id: light
        type: u2
        enum: controller_flags

      - id: run
        type: u2
        enum: controller_flags

      - id: view
        type: u2
        enum: controller_flags

      - id: step_left
        type: u2
        enum: controller_flags

      - id: step_right
        type: u2
        enum: controller_flags

      - id: pause
        type: u2
        enum: controller_flags

      - id: item
        type: u2
        enum: controller_flags

      - id: map
        type: u2
        enum: controller_flags

      - id: option
        type: u2
        enum: controller_flags

  savegame:
    seq:
      - id: items
        type: inventory_item
        repeat: expr
        repeat-expr: inv_item_count_max

      - id: field_a0
        type: s1

      - id: unused_a1
        size: 3

      - id: map_idx
        type: s1
        enum: map_idx

      - id: map_room_idx
        type: s1

      - id: savegame_count
        type: s2

      - id: location_id
        type: s1

      - id: paper_map_idx
        type: u1

      - id: equipped_weapon
        type: u1

      - id: inventory_slot_count
        type: u1

      - id: item_toggle_flags
        type: u4

      - id: map_enemy_states
        type: s4
        repeat: expr
        repeat-expr: 45

      - id: paper_map_flags
        type: s4

      - id: event_flags
        type: u4
        repeat: expr
        repeat-expr: 52

      - id: health_saturation
        type: s4
        doc: Q19.12.

      - id: picked_up_item_count
        type: s2

      - id: unused_23e
        type: s1

      - id: inventory_item_flags
        type: u1

      - id: player_health
        type: s4
        doc: Q19.12.

      - id: player_position_x
        type: s4
        doc: Q19.12.

      - id: player_rotation_y
        type: s2
        doc: Q19.12.

      - id: clear_game_count
        type: u1

      - id: clear_game_endings
        type: u1

      - id: player_position_z
        type: s4
        doc: Q19.12.

      - id: gameplay_timer
        type: s4
        doc: Q20.12.

      - id: run_distance
        type: s4
        doc: Q20.12.

      - id: walk_distance
        type: s4
        doc: Q20.12.

      - id: is_next_fear_mode
        type: b1

      - id: add_290_hours
        type: b2

      - id: picked_up_special_item_count
        type: b5

      - id: melee_kill_count
        type: u1

      - id: melee_kill_count_b
        type: u1

      - id: ranged_kill_count
        type: u1

      - id: field_260
        type: b28

      - id: game_difficulty
        type: b4
        enum: game_difficulty

      - id: fired_shot_count
        type: u2

      - id: close_range_shot_count
        type: u2

      - id: mid_range_shot_count
        type: u2

      - id: long_range_shot_count
        type: u2

      - id: field_26c
        type: u2

      - id: field_26e
        type: u2

      - id: field_270
        type: u2

      - id: field_272
        type: u2

      - id: field_274
        type: u2

      - id: field_276
        type: u2

      - id: field_278
        type: u2

      - id: current_ending_flags
        type: s1

      - id: continue_count
        type: u1

    instances:
      inv_item_count_max:
        value: 40

  inventory_item:
      seq:
        - id: id_0
          type: u1

        - id: count_1
          type: u1

        - id: command_2
          type: u1

        - id: field_3
          type: u1

enums:
  map_idx:
    0: map0_s00
    1: map0_s01
    2: map0_s02
    3: map1_s00
    4: map1_s01
    5: map1_s02
    6: map1_s03
    7: map1_s04
    8: map1_s05
    9: map1_s06
    10: map2_s00
    11: map2_s01
    12: map2_s02
    13: map2_s03
    14: map2_s04
    15: map3_s00
    16: map3_s01
    17: map3_s02
    18: map3_s03
    19: map3_s04
    20: map3_s05
    21: map3_s06
    22: map4_s00
    23: map4_s01
    24: map4_s02
    25: map4_s03
    26: map4_s04
    27: map4_s05
    28: map4_s06
    29: map5_s00
    30: map5_s01
    31: map5_s02
    32: map5_s03
    33: map6_s00
    34: map6_s01
    35: map6_s02
    36: map6_s03
    37: map6_s04
    38: map6_s05
    39: map7_s00
    40: map7_s01
    41: map7_s02
    42: map7_s03
    43: mapt_s00
    44: mapx_s00

  game_difficulty:
    0: easy
    1: normal
    2: hard

  blood_color:
    0: normal
    2: green
    5: violet
    11: black

  controller_flags:
    0: none
    1: select
    2: l_3
    4: r_3
    8: start
    16: dpad_up
    32: dpad_right
    64: dpad_down
    128: dpad_left
    256: l_2
    512: r_2
    1024: l_1
    2048: r_1
    4096: triangle
    8192: circle
    16384: cross
    32768: square
    65536: l_stick_up_2
    131072: l_stick_right_2
    262144: l_stick_down_2
    524288: l_stick_left_2
    1048576: r_stick_up
    2097152: r_stick_right
    4194304: r_stick_down
    8388608: r_stick_left
    16777216: l_stick_up
    33554432: l_stick_right
    67108864: l_stick_down
    134217728: l_stick_left
