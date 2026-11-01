meta:
  id: anm
  title: Silent Hill (PSX) Animation Format
  file-extension: anm
  endian: le
  bit-endian: le
  encoding: ASCII

doc: |
  ANM is the animation format used for 3D skeletal characters (ILM) in Silent
  Hill.

  The body is a list of keyframes. Each keyframe contains section of
  translations for the first `translation_count` bones, followed by a section of
  rotations for the next `rotation_count` bones. The header contains base
  translations for all bones as well as a magic value that is the offset to the
  first keyframe.

  Some ANMs for the player character contain only a list of frames which are
  appended to a base ANM.

seq:
  - id: header
    type: header

instances:
  bones:
    type: bone
    repeat: expr
    repeat-expr: header.bone_count

  keyframes:
    type: keyframe(_index)
    pos: header.keyframes_offset
    repeat: expr
    repeat-expr: header.keyframe_count

  transforms_per_keyframe:
    value: header.rotation_count + header.translation_count

types:
  header:
    seq:
      - id: keyframes_offset
        type: s2
    
      - id: rotation_count
        type: u1
    
      - id: translation_count
        type: u1
    
      - id: keyframe_size
        type: s2
        valid:
          eq: (rotation_count * 9) + (translation_count * 3)
    
      - id: bone_count
        type: s2
    
      - id: flags
        type: s4
    
      - id: end_offset
        type: s4
    
      - id: keyframe_count
        type: u2
    
      - id: translation_shift
        type: u1
        doc: Translations are scaled with `<< translation_shift`.
    
      - size: 1

  keyframe:
    params:
      - id: keyframe_idx
        type: u2

    seq:
      - id: translations
        type: translation
        repeat: expr
        repeat-expr: _root.header.translation_count

      - id: rotations
        type: matrix
        repeat: expr
        repeat-expr: _root.header.rotation_count

  bone:
    seq:
      - id: parent
        type: s1

      - id: rotation_index
        type: s1

      - id: translation_index
        type: s1

      - id: bind_translation
        type: translation

  translation:
    seq:
      - id: x_int
        type: s1

      - id: y_int
        type: s1

      - id: z_int
        type: s1

    instances:
      x:
        value: (x_int << _root.header.translation_shift) / 128.0

      y:
        value: (y_int << _root.header.translation_shift) / 128.0

      z:
        value: (x_int << _root.header.translation_shift) / 128.0

  matrix:
    doc: Q0.7 3x3 matrix.

    seq:
      - id: value
        type: s1
        repeat: expr
        repeat-expr: 9
