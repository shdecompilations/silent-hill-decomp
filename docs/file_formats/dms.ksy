meta:
  id: dms
  title: Silent Hill (PSX) Cutscene Format
  file-extension: dms
  endian: le
  bit-endian: le
  encoding: ASCII

doc: |
  DMS is the character cutscene format used in Silent Hill.

seq:
  - id: header
    type: header

instances:
  segments:
    type: segment
    pos: header.segments_offset
    repeat: expr
    repeat-expr: header.segment_count

  character_entries:
    type: entry(entry_type::character)
    pos: header.character_entries_offset
    repeat: expr
    repeat-expr: header.character_entry_count

types:
  header:
    seq:
      - id: is_initialized
        type: u1
  
      - id: character_entry_count
        type: u1
  
      - id: segment_count
        type: u1

      - id: unused_3
        type: u1
        doc: Usually 0, but sometimes filled in. Possibly junk padding data.

      - id: unused_4
        type: u4
        doc: Unknown, correlates with DMS file size.

      - id: segments_offset
        type: u4

      - id: origin
        type: vector_3

      - id: character_entries_offset
        type: u4

      - id: camera_entry
        type: entry(entry_type::camera)

  segment:
    seq:
      - id: start_frame_idx
        type: s2

      - id: frame_count
        type: s2

  entry:
    params:
      - id: entry_type
        type: u4
        enum: entry_type

    seq:
      - id: keyframe_count
        type: s2

      - id: hold_range_count
        type: u1

      - size: 1

      - id: name
        type: strz
        size: 4

      - id: hold_ranges_offset
        type: u4

      - id: keyframes_offset
        type: u4

    instances:
      hold_ranges:
        type: hold_range
        pos: hold_ranges_offset
        repeat: expr
        repeat-expr: hold_range_count

      keyframe_characters:
        type: keyframe_character
        pos: keyframes_offset
        repeat: expr
        repeat-expr: keyframe_count
        if: entry_type == entry_type::character

      keyframe_cameras:
        type: keyframe_camera
        pos: keyframes_offset
        repeat: expr
        repeat-expr: keyframe_count
        if: entry_type == entry_type::camera

  hold_range:
    seq:
      - id: start_frame_idx
        type: s2

      - id: end_frame_idx
        type: s2

      - id: keyframe_idx
        type: s2

  keyframe_character:
    seq:
      - id: position
        type: svector_3(vector_type::position)

      - id: rotation
        type: svector_3(vector_type::rotation)

  keyframe_camera:
    seq:
      - id: position_target
        type: svector_3(vector_type::position)

      - id: look_at_target
        type: svector_3(vector_type::position)

      - id: unused_angle
        type: s2
        doc: Q3.12.

      - id: projection_distance
        type: s2
        doc: Q3.12.

  svector_3:
    doc: Q7.8 position, Q3.12 rotation.

    params:
      - id: vector_type
        type: u4
        enum: vector_type

    seq:
      - id: x
        type: s2

      - id: y
        type: s2

      - id: z
        type: s2

    instances:
      position_x:
        value: x.as<f4> / (1 << 8).as<f4>
        if: vector_type == vector_type::position

      position_y:
        value: y.as<f4> / (1 << 8).as<f4>
        if: vector_type == vector_type::position

      position_z:
        value: z.as<f4> / (1 << 8).as<f4>
        if: vector_type == vector_type::position

      rotation_x:
        value: (x.as<f4> / (1 << 8).as<f4>) * 360
        if: vector_type == vector_type::rotation

      rotation_y:
        value: (y.as<f4> / (1 << 12).as<f4>) * 360
        if: vector_type == vector_type::rotation

      rotation_z:
        value: (z.as<f4> / (1 << 8).as<f4>) * 360
        if: vector_type == vector_type::rotation

  vector_3:
    doc: Q23.8 position.

    seq:
      - id: x
        type: s4

      - id: y
        type: s4

      - id: z
        type: s4

    instances:
      position_x:
        value: x.as<f4> / (1 << 8).as<f4>

      position_y:
        value: y.as<f4> / (1 << 8).as<f4>

      position_z:
        value: z.as<f4> / (1 << 8).as<f4>

enums:
  entry_type:
    0: character
    1: camera

  vector_type:
    0: position
    1: rotation
