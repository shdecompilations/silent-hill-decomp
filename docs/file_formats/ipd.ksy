meta:
  id: ipd
  title: Silent Hill 1 Map Model Format
  file-extension: ipd
  endian: le
  encoding: ASCII
  imports:
    - lm

doc: |
  IPD is the 3D model format containing instanced map models and collision data
  used in Silent Hill (PSX).
  
  Local map prop models are stored in an embedded PLM (LM) block, while global
  props reference models in a separate `***_GLB.PLM` file.

  The first few meshes of the embedded PLM contain texture names and are
  skipped.

seq:
  - id: header
    type: header

instances:
  lm:
    pos: header.lm_header_offset
    type: lm(header.lm_header_offset)

  model_infos:
    pos: header.model_infos_offset
    type: model_info
    repeat: expr
    repeat-expr: header.model_info_count

  model_buffers:
    pos: header.model_buffers_offset
    type: model_buffer
    repeat: expr
    repeat-expr: header.model_buffer_count

  model_order:
    pos: header.model_order_offset
    type: u1
    repeat: expr
    repeat-expr: header.model_order_size

  collision:
    pos: header.collision_offset
    type: collision

types:
  header:
    seq:
      - id: magic
        contents: [0x14]

      - id: is_initialized
        type: u1

      - id: chunk_x
        type: s1

      - id: chunk_z
        type: s1

      - id: lm_header_offset
        type: u4

      - id: model_info_count
        type: u1

      - id: model_buffer_count
        type: u1

      - id: model_order_size
        type: u1

      - size: 9

      - id: model_infos_offset
        type: u4

      - id: model_buffers_offset
        type: u4

      - id: texture_count
        type: u1

      - id: unk_1d
        size: 51
        doc: TODO Actual texture data array?

      - id: model_order_offset
        type: u4

    instances:
      collision_offset:
        value: 0x54

  model_info:
    seq:
      - id: is_global_plm
        type: u1

      - size: 3

      - id: name
        type: strz
        size: 8

      - id: model_header
        type: u4
        doc: "Always 0 in files. The engine loads it by name from
              global PLM headers if `is_global_plm` is `true`,
              otherwise from the IPD's LM header."

  model_buffer:
    seq:
      - id: model_instance_count
        type: u1

      - id: billboard_instance_count
        type: u1

      - id: subchunk_range_count
        type: u1

      - size: 1

      - id: min_x
        type: s2
        doc: Q7.8.


      - id: max_x
        type: s2
        doc: Q7.8.


      - id: min_z
        type: s2
        doc: Q7.8.


      - id: max_z
        type: s2
        doc: Q7.8.


      - id: model_instances_offset
        type: u4

      - id: billboard_instances_offset
        type: u4
        doc: Q7.8.

      - id: subchunk_ranges_offset
        type: u4
        doc: Q7.8.

    instances:
      model_instances:
        pos: model_instances_offset
        type: model_instance
        repeat: expr
        repeat-expr: model_instance_count

      billboard_instances:
        pos: billboard_instances_offset
        type: billboard_instance
        repeat: expr
        repeat-expr: billboard_instance_count

      subchunk_ranges:
        pos: subchunk_ranges_offset
        type: subchunk_range
        repeat: expr
        repeat-expr: subchunk_range_count

    types:
      model_instance:
        seq:
          - id: model_info_idx
            type: u4
            doc: Becomes `model_header` from `model_info` when loaded.

          - id: transform
            type: matrix

        instances:
          model_info:
            value: _root.model_infos[model_info_idx]

      billboard_instance:
        seq:
          - id: offset
            type: svector_3

          - id: idx
            type: s2

          - size: 1

      subchunk_range:
        seq:
          - id: min_x
            type: s2
            doc: Q7.8.
    
          - id: max_x
            type: s2
            doc: Q7.8.
    
          - id: min_z
            type: s2
            doc: Q7.8.
    
          - id: max_z
            type: s2
            doc: Q7.8.

  model_header:
    params:
      - id: base_offset
        type: u4

    seq:
      - id: name
        type: strz
        size: 8

      - id: mesh_count
        type: u1

      - id: vertex_offset
        type: u1

      - id: normal_offset
        type: u1

      - id: field_b_0
        type: b1

      - id: field_b_1
        type: b3

      - id: field_b_4
        type: b2

      - id: field_b_6
        type: b2

      - id: mesh_headers_offset
        type: u4

    instances:
      mesh_headers:
        pos: base_offset + mesh_headers_offset
        type: mesh_header(base_offset)
        repeat: expr
        repeat-expr: mesh_count

  mesh_header:
    params:
      - id: base_offset
        type: u4

    seq:
      - id: primitive_count
        type: u1

      - id: vertex_count
        type: u1

      - id: normal_count
        type: u1

      - id: unk_count
        type: u1

      - id: primitives_offset
        type: u4

      - id: vertices_xz_offset
        type: u4

      - id: verticez_z_offset
        type: u4

      - id: normals_offset
        type: u4

      - id: unk_ptrs_14_offset
        type: u4

    instances:
      primitives:
        pos: base_offset + primitives_offset
        type: primitive
        repeat: expr
        repeat-expr: primitive_count

      vertices_xy:
        pos: base_offset + vertices_xz_offset
        type: svector_2
        repeat: expr
        repeat-expr: vertex_count

      vertices_z:
        pos: base_offset + verticez_z_offset
        type: s2
        repeat: expr
        repeat-expr: vertex_count

      normals:
        pos: base_offset + normals_offset
        type: normal
        repeat: expr
        repeat-expr: normal_count

  primitive:
    seq:
      - id: u0
        type: u1

      - id: v0
        type: u1

      - id: bits0
        type: u2

      - id: u1
        type: u1

      - id: v1
        type: u1

      - id: bits1
        type: u2

      - id: u2
        type: u1

      - id: v2
        type: u1

      - id: u3
        type: u1

      - id: v3
        type: u1

      - id: face0
        type: u1

      - id: face1
        type: u1

      - id: face2
        type: u1

      - id: face3
        type: u1

      - id: unk_10
        size: 4

  collision:
    seq:
      - id: position_x
        type: s4
        doc: Q23.8

      - id: position_z
        type: s4
        doc: Q23.8

      - id: polygon_split_vertex_count
        type: b8

      - id: polygon_surface_count
        type: b8

      - id: polygon_count
        type: b8

      - id: cylinder_count
        type: b8

      - id: polygon_split_vertices_offset
        type: u4

      - id: polygon_surfaces_offset
        type: u4

      - id: polygons_offset
        type: u4

      - id: cylinders_offset
        type: u4

      - id: cell_size
        type: s2

      - id: cell_count_x
        type: u1

      - id: cell_count_z
        type: u1

      - id: shape_allocs_offset
        type: u4

      - id: shape_lut_size
        type: u2

      - id: surface_lut_size
        type: u2

      - id: shape_lut_offset
        type: u4

      - id: surface_lut_offset
        type: u4

      - id: shape_pass_counter
        type: u1
        doc: Used by engine.

      - size: 3

      - id: shape_pass_stamp_buffer
        size: 256
        doc: Used by engine.

    instances:
      polygon_split_vertices:
        pos: _root.header.collision_offset + polygon_split_vertices_offset
        type: svector_3
        repeat: expr
        repeat-expr: polygon_split_vertex_count
        doc: Q7.8.

      polygon_surfaces:
        pos: _root.header.collision_offset + polygon_surfaces_offset
        type: polygon_surface
        repeat: expr
        repeat-expr: polygon_surface_count

      polygons:
        pos: _root.header.collision_offset + polygons_offset
        type: polygon
        repeat: expr
        repeat-expr: polygon_count

      cylinders:
        pos: _root.header.collision_offset + cylinders_offset
        type: cylinder
        repeat: expr
        repeat-expr: cylinder_count

      shape_ranges:
        pos: _root.header.collision_offset + shape_allocs_offset
        type: shape_range
        repeat: expr
        repeat-expr: (cell_count_x * cell_count_z) + 1
        doc: TODO Unsure if `+ 1` is necessary, looks like one extra entry?

      polygon_lut:
        pos: _root.header.collision_offset + shape_lut_offset
        type: u1
        repeat: expr
        repeat-expr: shape_lut_size

      surface_lut:
        pos: _root.header.collision_offset + surface_lut_offset
        type: u1
        repeat: expr
        repeat-expr: surface_lut_size

    types:
      polygon_surface:
        seq:
          - id: offset
            type: svector_3
    
          - id: floor_type
            type: b5
    
          - id: disable_height
            type: b3
    
          - id: field_6_8
            type: b3
    
          - id: trigger_flag_idx
            type: b4
    
          - id: slope_gradient_x
            type: s2
            doc: Q7.8.
    
          - id: slope_gradient_z
            type: s2
            doc: Q7.8.
    
        instances:
          slope_gradient_x_flt:
            value: slope_gradient_x / (1 << 8).as<f4>
    
          slope_gradient_z_flt:
            value: slope_gradient_z / (1 << 8).as<f4>
    
      polygon:
        seq:
          - id: offset_x
            type: b14
            doc: Q7.8.
    
          - id: trigger_flag_idx_low
            type: b2
    
          - id: offset_y
            type: b14
            doc: Q7.8.
    
          - id: trigger_flag_idx_high
            type: b2
    
          - id: offset_z
            type: s2
            doc: Q7.8.
    
          - id: split_vertex_idx_0
            type: u1
    
          - id: split_vertex_idx_1
            type: u1
    
          - id: surface_idx_0
            type: u1
    
          - id: surface_idx_1
            type: u1
    
          - size: 2
    
        instances:
          offset_x_flt:
            value: offset_x / (1 << 8).as<f4>
    
          offset_y_flt:
            value: offset_y / (1 << 8).as<f4>
    
          offset_z_flt:
            value: offset_z / (1 << 8).as<f4>
    
          trigger_flags:
            value: (trigger_flag_idx_low << 2) | trigger_flag_idx_high
    
      cylinder:
        seq:
          - id: floor_type
            type: b5
    
          - id: disable_height
            type: b3
    
          - id: trigger_flag_idx
            type: b4
    
          - id: offset
            type: svector_3
            doc: Q7.8.
    
          - id: radius
            type: u2
            doc: Q7.8.

      shape_range:
        seq:
          - id: polygon_lut_idx
            type: s2
    
          - id: surface_lut_idx
            type: s2

  svector_2:
    seq:
      - id: x
        type: s2

      - id: y
        type: s2

  svector_3:
    seq:
      - id: x
        type: s2

      - id: y
        type: s2

      - id: z
        type: s2

    instances:
      x_flt:
        value: x / (1 << 8).as<f4>

      y_flt:
        value: y / (1 << 8).as<f4>

      z_flt:
        value: z / (1 << 8).as<f4>

  normal:
    seq:
      - id: x
        type: s1

      - id: y
        type: s1

      - id: z
        type: s1

      - id: count
        type: u1

  matrix:
    seq:
      - id: rotation
        type: s2
        repeat: expr
        repeat-expr: 9

      - size: 2

      - id: translation
        type: s4
        repeat: expr
        repeat-expr: 3
