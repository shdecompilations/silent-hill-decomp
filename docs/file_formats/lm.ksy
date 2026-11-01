meta:
  id: lm
  title: Silent Hill 1 Model Format
  endian: le
  bit-endian: le
  encoding: ASCII

doc: |
  LM is the generic 3D model format used in Silent Hill (PSX).
  It is used for ILM character skeletons, global PLM world map props, and
  embedded in IPD world map chunks to store local props.

  The header contains a table of meshes, each with a name, ID, and offset to
  its body. Each body contains vertex and primitive data.

  For an ILM, the bone meshes are stored untransformed and the ANM format is
  necessary to render the model correctly.

params:
  - id: base_offset
    type: u4

seq:
  - id: header
    type: header

instances:
  meshes:
    pos: base_offset + header.meshes_offset
    type: mesh
    repeat: expr
    repeat-expr: header.mesh_count

  mesh_ids:
    pos: base_offset + header.mesh_ids_offset
    type: u1
    repeat: expr
    repeat-expr: header.mesh_count

types:
  header:
    seq:
      - id: magic
        contents: [0x30]

      - id: version
        type: u1

      - id: is_initialized
        type: u1

      - type: u1

      - id: name_offset
        type: u4
        valid: 0x14

      - id: mesh_count
        type: u4

      - id: meshes_offset
        type: u4

      - id: mesh_ids_offset
        type: u4

      - id: name
        type: strz

  mesh:
    seq:
      - id: name
        type: strz
        size: 8

      - type: u1

      - id: position_base_idx
        type: u1
        doc: All primitive position indices for the mesh are offset by this value.

      - id: normal_base_idx
        type: u1
        doc: All primitive normal indices for the mesh are offset by this value.

      - type: u1

      - id: body_offset
        type: u4

    instances:
      body:
        pos: _root.base_offset + body_offset
        type: mesh_body

  mesh_body:
    seq:
      - id: primitive_count
        type: u1

      - id: position_count
        type: u1

      - id: normal_count
        type: u1

      - type: u1
      - id: primitives_offset
        type: u4

      - id: positions_xy_offset
        type: u4

      - id: positions_z_offset
        type: u4

      - id: normals_offset
        type: u4

      - id: next_body_offset
        type: u4

    instances:
      primitives:
        pos: _root.base_offset + primitives_offset
        type: primitive
        repeat: expr
        repeat-expr: primitive_count

      vertex_xy:
        pos: _root.base_offset + positions_xy_offset
        type: xy_pair
        repeat: expr
        repeat-expr: position_count
        doc: Q8.7.

      vertex_z:
        pos: _root.base_offset + positions_z_offset
        type: s2
        repeat: expr
        repeat-expr: position_count
        doc: Q8.7.

      normals:
        pos: _root.base_offset + normals_offset
        type: svector
        repeat: expr
        repeat-expr: normal_count
        doc: Q0.7.

  primitive:
    seq:
      - id: uv_0
        type: uv

      - id: clut_position
        type: clut_position

      - id: uv_1
        type: uv

      - id: t_page
        type: s2

      - id: uv_2
        type: uv

      - id: uv_3
        type: uv

      - id: position_idxs
        type: primitive_idxs

      - id: normal_idxs
        type: primitive_idxs

  uv:
    seq:
      - id: u
        type: u1
        doc: Q0.8.

      - id: v
        type: u1
        doc: Q0.8.

  primitive_idxs:
    seq:
      - id: idx_0
        type: u1

      - id: idx_1
        type: u1

      - id: idx_2
        type: u1

      - id: idx_3
        type: u1

  xy_pair:
    seq:
      - id: x
        type: s2
        doc: Q8.7.

      - id: y
        type: s2
        doc: Q8.7.

  svector:
    seq:
      - id: x_int
        type: s1
        doc: Q0.7.

      - id: y_int
        type: s1
        doc: Q0.7.

      - id: z_int
        type: s1
        doc: Q0.7.

      - id: count
        type: u1

    instances:
      x:
        value: x_int / 128.0

      y:
        value: y_int / 128.0

      z:
        value: z_int / 128.0

      length_sqr:
        value: (x * x) + (y * y) + (z * z)

  clut_position:
    seq:
      - id: value
        type: s2

    instances:
      x:
        value: (value & 0x3F) * 0x10

      y:
        value: (value >> 6) & 0x1FF
