meta:
  id: harry_map_keyframes
  title: Silent Hill (PSX) Harry Animation Keyframe Block
  endian: le
  bit-endian: le
  encoding: ASCII

doc: |
  Map-specific block of Harry keyframes appended to the end of `HB_BASE.ANM`.
  This spec covers all `ANM/HB_M*S**.ANM` files.

seq:
  - id: keyframes
    type: keyframe(_index)
    repeat: expr
    repeat-expr: keyframe_count

instances:
  keyframe_count:
    value: _root._io.size / keyframe_size

  keyframe_size:
    value: 156

  translations_count:
    value: 1

  rotations_count:
    value: 17

  translation_shift:
    value: 2

types:
  keyframe:
    params:
      - id: keyframe_idx
        type: u2

    seq:
      - id: translations
        type: translation
        repeat: expr
        repeat-expr: _root.translations_count

      - id: rotations
        type: matrix
        repeat: expr
        repeat-expr: _root.rotations_count

  translation:
    doc: Q0.7.

    seq:
      - id: x_int
        type: s1

      - id: y_int
        type: s1

      - id: z_int
        type: s1

    instances:
      x:
        value: (x_int << _root.translation_shift).as<f4> / ((1 << 7) << _root.translation_shift).as<f4>

      y:
        value: (y_int << _root.translation_shift).as<f4> / ((1 << 7) << _root.translation_shift).as<f4>

      z:
        value: (x_int << _root.translation_shift).as<f4> / ((1 << 7) << _root.translation_shift).as<f4>

  matrix:
    doc: Q0.7 3x3 matrix.

    seq:
      - id: value
        type: s1
        repeat: expr
        repeat-expr: 9
