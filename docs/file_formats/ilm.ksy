meta:
  id: ilm
  title: Silent Hill (PSX) Skeletal Character Model Format
  file-extension: ilm
  endian: le
  bit-endian: le
  encoding: ASCII
  imports:
    - lm

doc: |
  ILM is the 3D model format used for skeletal characters in Silent Hill.
  The base format is LM.

  Mesh names store bone indices in the first two `char`s and bone names in the
  remaining `char`s. Bone meshes are stored untransformed and the ANM format is
  necessary to render the model correctly.

seq:
  - id: lm
    type: lm(0)
