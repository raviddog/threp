#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class th_modern_t : public kaitai::kstruct {

public:
    class main_t;
    class userdata_t;
    class crlfstring_t;
    class userdata_field_t;

    th_modern_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = nullptr, th_modern_t* p__root = nullptr);

private:
    void _read();
    void _clean_up();

public:
    ~th_modern_t();

    class main_t : public kaitai::kstruct {

    public:

        main_t(kaitai::kstream* p__io, th_modern_t* p__parent = nullptr, th_modern_t* p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~main_t();

    private:
        uint32_t m_magic_ver;
        uint32_t m_version;
        std::string m_unused_1;
        uint32_t m_userdata_offset;
        std::string m_unused_2;
        uint32_t m_comp_size;
        uint32_t m_size;
        std::string m_comp_data;
        th_modern_t* m__root;
        th_modern_t* m__parent;

    public:
        uint32_t magic_ver() const { return m_magic_ver; }
        uint32_t version() const { return m_version; }
        std::string unused_1() const { return m_unused_1; }
        uint32_t userdata_offset() const { return m_userdata_offset; }
        std::string unused_2() const { return m_unused_2; }
        uint32_t comp_size() const { return m_comp_size; }
        uint32_t size() const { return m_size; }
        std::string comp_data() const { return m_comp_data; }
        th_modern_t* _root() const { return m__root; }
        th_modern_t* _parent() const { return m__parent; }
    };

    class userdata_t : public kaitai::kstruct {

    public:

        userdata_t(kaitai::kstream* p__io, th_modern_t* p__parent = nullptr, th_modern_t* p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~userdata_t();

    private:
        std::string m_magic_user;
        uint32_t m_user_length;
        std::string m_unknown;
        std::string m_user_desc;
        std::string m_user_desc_term;
        std::unique_ptr<crlfstring_t> m_user_ver;
        std::unique_ptr<userdata_field_t> m_name;
        std::unique_ptr<userdata_field_t> m_date;
        std::unique_ptr<userdata_field_t> m_shot;
        std::unique_ptr<userdata_field_t> m_difficulty;
        std::unique_ptr<crlfstring_t> m_stage;
        std::unique_ptr<userdata_field_t> m_score;
        std::unique_ptr<userdata_field_t> m_slowdown;
        th_modern_t* m__root;
        th_modern_t* m__parent;

    public:
        std::string magic_user() const { return m_magic_user; }
        uint32_t user_length() const { return m_user_length; }
        std::string unknown() const { return m_unknown; }
        std::string user_desc() const { return m_user_desc; }
        std::string user_desc_term() const { return m_user_desc_term; }
        crlfstring_t* user_ver() const { return m_user_ver.get(); }
        userdata_field_t* name() const { return m_name.get(); }
        userdata_field_t* date() const { return m_date.get(); }
        userdata_field_t* shot() const { return m_shot.get(); }
        userdata_field_t* difficulty() const { return m_difficulty.get(); }
        crlfstring_t* stage() const { return m_stage.get(); }
        userdata_field_t* score() const { return m_score.get(); }
        userdata_field_t* slowdown() const { return m_slowdown.get(); }
        th_modern_t* _root() const { return m__root; }
        th_modern_t* _parent() const { return m__parent; }
    };

    class crlfstring_t : public kaitai::kstruct {

    public:

        crlfstring_t(kaitai::kstream* p__io, th_modern_t::userdata_t* p__parent = nullptr, th_modern_t* p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~crlfstring_t();

    private:
        std::string m_value;
        uint8_t m_term;
        th_modern_t* m__root;
        th_modern_t::userdata_t* m__parent;

    public:
        std::string value() const { return m_value; }
        uint8_t term() const { return m_term; }
        th_modern_t* _root() const { return m__root; }
        th_modern_t::userdata_t* _parent() const { return m__parent; }
    };

    class userdata_field_t : public kaitai::kstruct {

    public:

        userdata_field_t(kaitai::kstream* p__io, th_modern_t::userdata_t* p__parent = nullptr, th_modern_t* p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~userdata_field_t();

    private:
        std::string m_name;
        std::string m_value;
        uint8_t m_term;
        th_modern_t* m__root;
        th_modern_t::userdata_t* m__parent;

    public:
        std::string name() const { return m_name; }
        std::string value() const { return m_value; }
        uint8_t term() const { return m_term; }
        th_modern_t* _root() const { return m__root; }
        th_modern_t::userdata_t* _parent() const { return m__parent; }
    };

private:
    std::unique_ptr<main_t> m_main;
    std::unique_ptr<userdata_t> m_userdata;
    th_modern_t* m__root;
    kaitai::kstruct* m__parent;

public:
    main_t* main() const { return m_main.get(); }
    userdata_t* userdata() const { return m_userdata.get(); }
    th_modern_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};
