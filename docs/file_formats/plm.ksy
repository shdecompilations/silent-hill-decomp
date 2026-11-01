meta:
  id: plm
  title: Silent Hill (PSX) Global Map Prop Model Format
  file-extension: plm
  endian: le
  bit-endian: le
  encoding: ASCII
  imports:
    - lm

doc: |
  PLM is the 3D model format used for global map props in Silent Hill.
  The base format is LM.

seq:
  - id: lm
    type: lm(0)
