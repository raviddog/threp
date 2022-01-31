meta:
  id: th13
  file-extension: raw
  endian: le
seq:
  - id: header
    type: header
  - id: stage
    type: stage
    repeat: expr
    repeat-expr: header.stage_count
types:
  header:
    seq:
    - id: name
      type: str
      size: 12
      terminator: 0
      encoding: ASCII
    - id: timestamp
      type: u8
    - id: score
      type: u4
    - id: unknown
      size: 60
    - id: slowdown
      type: f4
    - id: stage_count
      type: u4
    - id: shot
      type: u4
    - id: subshot_unused
      type: u4
    - id: difficulty
      type: u4
    - id: cleared
      type: u4
    - id: unused
      size: 4
    - id: spell_practice_id
      type: u4
  stage:
    seq:
    - id: stage_num
      type: u2
    - id: rng
      type: u2
    - id: frame_count
      type: u4
    - id: end_off
      type: u4
    - id: pos_subpixel_x
      type: u4
    - id: pos_subpixel_y
      type: u4
    - id: shot
      type: u4
    - id: subshot_unused
      type: u4
    - id: score
      type: u4
    - id: unknown
      type: u4
    - id: continues
      type: u4
    - id: unknown2
      type: u4
    - id: graze
      type: u4
    - id: unknown3
      type: u4
    - id: unknown4
      type: u4
    - id: piv
      type: u4
    - id: piv_min
      type: u4
    - id: piv_max
      type: u4
    - id: power
      type: u4
    - id: power_max
      type: u4
    - id: power_min
      type: u4
    - id: lives
      type: u4
    - id: life_pieces
      type: u4
    - id: extends
      type: u4
    - id: bombs
      type: u4
    - id: bomb_pieces
      type: u4
    - id: trance
      type: u4
    - id: unknown5
      type: u4
    - id: focused
      type: u4
    - id: spellcard_real_times
      type: u4
      repeat: expr
      repeat-expr: 21
    - id: stage_data
      size: end_off