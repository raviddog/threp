// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "th13.h"

th13_t::th13_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, th13_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_header = nullptr;
    m_stage = nullptr;
    _read();
}

void th13_t::_read() {
    m_header = std::unique_ptr<header_t>(new header_t(m__io, this, m__root));
    int l_stage = header()->stage_count();
    m_stage = std::unique_ptr<std::vector<std::unique_ptr<stage_t>>>(new std::vector<std::unique_ptr<stage_t>>());
    m_stage->reserve(l_stage);
    for (int i = 0; i < l_stage; i++) {
        m_stage->push_back(std::move(std::unique_ptr<stage_t>(new stage_t(m__io, this, m__root))));
    }
}

th13_t::~th13_t() {
    _clean_up();
}

void th13_t::_clean_up() {
}

th13_t::header_t::header_t(kaitai::kstream* p__io, th13_t* p__parent, th13_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void th13_t::header_t::_read() {
    m_name = kaitai::kstream::bytes_to_str(kaitai::kstream::bytes_terminate(m__io->read_bytes(12), 0, false), std::string("ASCII"));
    m_timestamp = m__io->read_u8le();
    m_score = m__io->read_u4le();
    m_unknown = m__io->read_bytes(60);
    m_slowdown = m__io->read_f4le();
    m_stage_count = m__io->read_u4le();
    m_shot = m__io->read_u4le();
    m_subshot_unused = m__io->read_u4le();
    m_difficulty = m__io->read_u4le();
    m_cleared = m__io->read_u4le();
    m_unused = m__io->read_bytes(4);
    m_spell_practice_id = m__io->read_u4le();
}

th13_t::header_t::~header_t() {
    _clean_up();
}

void th13_t::header_t::_clean_up() {
}

th13_t::stage_t::stage_t(kaitai::kstream* p__io, th13_t* p__parent, th13_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_spellcard_real_times = nullptr;
    _read();
}

void th13_t::stage_t::_read() {
    m_stage_num = m__io->read_u2le();
    m_rng = m__io->read_u2le();
    m_frame_count = m__io->read_u4le();
    m_end_off = m__io->read_u4le();
    m_pos_subpixel_x = m__io->read_u4le();
    m_pos_subpixel_y = m__io->read_u4le();
    m_shot = m__io->read_u4le();
    m_subshot_unused = m__io->read_u4le();
    m_score = m__io->read_u4le();
    m_unknown = m__io->read_u4le();
    m_continues = m__io->read_u4le();
    m_unknown2 = m__io->read_u4le();
    m_graze = m__io->read_u4le();
    m_unknown3 = m__io->read_u4le();
    m_unknown4 = m__io->read_u4le();
    m_piv = m__io->read_u4le();
    m_piv_min = m__io->read_u4le();
    m_piv_max = m__io->read_u4le();
    m_power = m__io->read_u4le();
    m_power_max = m__io->read_u4le();
    m_power_min = m__io->read_u4le();
    m_lives = m__io->read_u4le();
    m_life_pieces = m__io->read_u4le();
    m_extends = m__io->read_u4le();
    m_bombs = m__io->read_u4le();
    m_bomb_pieces = m__io->read_u4le();
    m_trance = m__io->read_u4le();
    m_unknown5 = m__io->read_u4le();
    m_focused = m__io->read_u4le();
    int l_spellcard_real_times = 21;
    m_spellcard_real_times = std::unique_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>());
    m_spellcard_real_times->reserve(l_spellcard_real_times);
    for (int i = 0; i < l_spellcard_real_times; i++) {
        m_spellcard_real_times->push_back(std::move(m__io->read_u4le()));
    }
    m_stage_data = m__io->read_bytes(end_off());
}

th13_t::stage_t::~stage_t() {
    _clean_up();
}

void th13_t::stage_t::_clean_up() {
}
