// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "th_modern.h"
#include "kaitai/exceptions.h"

th_modern_t::th_modern_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, th_modern_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_main = nullptr;
    m_userdata = nullptr;
    _read();
}

void th_modern_t::_read() {
    m_main = std::unique_ptr<main_t>(new main_t(m__io, this, m__root));
    m_userdata = std::unique_ptr<userdata_t>(new userdata_t(m__io, this, m__root));
}

th_modern_t::~th_modern_t() {
    _clean_up();
}

void th_modern_t::_clean_up() {
}

th_modern_t::main_t::main_t(kaitai::kstream* p__io, th_modern_t* p__parent, th_modern_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void th_modern_t::main_t::_read() {
    m_magic_ver = m__io->read_u4le();
    m_version = m__io->read_u4le();
    m_unused_1 = m__io->read_bytes(4);
    m_userdata_offset = m__io->read_u4le();
    m_unused_2 = m__io->read_bytes(12);
    m_comp_size = m__io->read_u4le();
    m_size = m__io->read_u4le();
    m_comp_data = m__io->read_bytes(comp_size());
}

th_modern_t::main_t::~main_t() {
    _clean_up();
}

void th_modern_t::main_t::_clean_up() {
}

th_modern_t::userdata_t::userdata_t(kaitai::kstream* p__io, th_modern_t* p__parent, th_modern_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_user_ver = nullptr;
    m_name = nullptr;
    m_date = nullptr;
    m_shot = nullptr;
    m_difficulty = nullptr;
    m_stage = nullptr;
    m_score = nullptr;
    m_slowdown = nullptr;
    _read();
}

void th_modern_t::userdata_t::_read() {
    m_magic_user = m__io->read_bytes(4);
    if (!(magic_user() == std::string("\x55\x53\x45\x52", 4))) {
        throw kaitai::validation_not_equal_error<std::string>(std::string("\x55\x53\x45\x52", 4), magic_user(), _io(), std::string("/types/userdata/seq/0"));
    }
    m_user_length = m__io->read_u4le();
    m_unknown = m__io->read_bytes(4);
    m_user_desc = kaitai::kstream::bytes_to_str(m__io->read_bytes_term(13, false, true, true), std::string("SJIS"));
    m_user_desc_term = kaitai::kstream::bytes_to_str(m__io->read_bytes_term(10, false, true, true), std::string("ASCII"));
    m_user_ver = std::unique_ptr<crlfstring_t>(new crlfstring_t(m__io, this, m__root));
    m_name = std::unique_ptr<userdata_field_t>(new userdata_field_t(m__io, this, m__root));
    m_date = std::unique_ptr<userdata_field_t>(new userdata_field_t(m__io, this, m__root));
    m_shot = std::unique_ptr<userdata_field_t>(new userdata_field_t(m__io, this, m__root));
    m_difficulty = std::unique_ptr<userdata_field_t>(new userdata_field_t(m__io, this, m__root));
    m_stage = std::unique_ptr<crlfstring_t>(new crlfstring_t(m__io, this, m__root));
    m_score = std::unique_ptr<userdata_field_t>(new userdata_field_t(m__io, this, m__root));
    m_slowdown = std::unique_ptr<userdata_field_t>(new userdata_field_t(m__io, this, m__root));
}

th_modern_t::userdata_t::~userdata_t() {
    _clean_up();
}

void th_modern_t::userdata_t::_clean_up() {
}

th_modern_t::crlfstring_t::crlfstring_t(kaitai::kstream* p__io, th_modern_t::userdata_t* p__parent, th_modern_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void th_modern_t::crlfstring_t::_read() {
    m_value = kaitai::kstream::bytes_to_str(m__io->read_bytes_term(13, false, true, true), std::string("ASCII"));
    m_term = m__io->read_u1();
}

th_modern_t::crlfstring_t::~crlfstring_t() {
    _clean_up();
}

void th_modern_t::crlfstring_t::_clean_up() {
}

th_modern_t::userdata_field_t::userdata_field_t(kaitai::kstream* p__io, th_modern_t::userdata_t* p__parent, th_modern_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void th_modern_t::userdata_field_t::_read() {
    m_name = kaitai::kstream::bytes_to_str(m__io->read_bytes_term(32, false, true, true), std::string("ASCII"));
    m_value = kaitai::kstream::bytes_to_str(m__io->read_bytes_term(13, false, true, true), std::string("ASCII"));
    m_term = m__io->read_u1();
}

th_modern_t::userdata_field_t::~userdata_field_t() {
    _clean_up();
}

void th_modern_t::userdata_field_t::_clean_up() {
}
