meta:
  id: th_modern
  file-extension: rpy
  endian: le
seq:
  - id: main
    type: main
  - id: userdata
    type: userdata
types:
  main:
    seq:
      - id: magic_ver
        type: u4
      - id: version
        type: u4
      - id: unused_1
        size: 4
      - id: userdata_offset
        type: u4
      - id: unused_2
        size: 12
      - id: comp_size
        type: u4
      - id: size
        type: u4
      - id: comp_data
        size: comp_size
  userdata:
    seq:
      - id: magic_user
        contents: USER
      - id: user_length
        type: u4
      - id: unknown
        size: 4
      - id: user_desc
        type: str
        terminator: 0xd
        encoding: SJIS
      - id: user_desc_term
        type: str
        terminator: 0xa
        encoding: ASCII
      - id: user_ver
        type: crlfstring
      - id: name
        type: userdata_field
      - id: date
        type: userdata_field
      - id: shot
        type: userdata_field
      - id: difficulty
        type: userdata_field
      - id: stage
        type: crlfstring
      - id: score
        type: userdata_field
      - id: slowdown
        type: userdata_field
  crlfstring:
    seq:
      - id: value
        type: str
        terminator: 0xd
        encoding: ASCII
      - id: term
        type: u1
  userdata_field:
    seq:
      - id: name
        type: str
        terminator: 0x20
        encoding: ASCII
      - id: value
        type: str
        terminator: 0xd
        encoding: ASCII
      - id: term
        type: u1