#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class th13_t : public kaitai::kstruct {

public:
    class header_t;
    class stage_t;

    th13_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = nullptr, th13_t* p__root = nullptr);

private:
    void _read();
    void _clean_up();

public:
    ~th13_t();

    class header_t : public kaitai::kstruct {

    public:

        header_t(kaitai::kstream* p__io, th13_t* p__parent = nullptr, th13_t* p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~header_t();

    private:
        std::string m_name;
        uint64_t m_timestamp;
        uint32_t m_score;
        std::string m_unknown;
        float m_slowdown;
        uint32_t m_stage_count;
        uint32_t m_shot;
        uint32_t m_subshot_unused;
        uint32_t m_difficulty;
        uint32_t m_cleared;
        std::string m_unused;
        uint32_t m_spell_practice_id;
        th13_t* m__root;
        th13_t* m__parent;

    public:
        std::string name() const { return m_name; }
        uint64_t timestamp() const { return m_timestamp; }
        uint32_t score() const { return m_score; }
        std::string unknown() const { return m_unknown; }
        float slowdown() const { return m_slowdown; }
        uint32_t stage_count() const { return m_stage_count; }
        uint32_t shot() const { return m_shot; }
        uint32_t subshot_unused() const { return m_subshot_unused; }
        uint32_t difficulty() const { return m_difficulty; }
        uint32_t cleared() const { return m_cleared; }
        std::string unused() const { return m_unused; }
        uint32_t spell_practice_id() const { return m_spell_practice_id; }
        th13_t* _root() const { return m__root; }
        th13_t* _parent() const { return m__parent; }
    };

    class stage_t : public kaitai::kstruct {

    public:

        stage_t(kaitai::kstream* p__io, th13_t* p__parent = nullptr, th13_t* p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~stage_t();

    private:
        uint16_t m_stage_num;
        uint16_t m_rng;
        uint32_t m_frame_count;
        uint32_t m_end_off;
        uint32_t m_pos_subpixel_x;
        uint32_t m_pos_subpixel_y;
        uint32_t m_shot;
        uint32_t m_subshot_unused;
        uint32_t m_score;
        uint32_t m_unknown;
        uint32_t m_continues;
        uint32_t m_unknown2;
        uint32_t m_graze;
        uint32_t m_unknown3;
        uint32_t m_unknown4;
        uint32_t m_piv;
        uint32_t m_piv_min;
        uint32_t m_piv_max;
        uint32_t m_power;
        uint32_t m_power_max;
        uint32_t m_power_min;
        uint32_t m_lives;
        uint32_t m_life_pieces;
        uint32_t m_extends;
        uint32_t m_bombs;
        uint32_t m_bomb_pieces;
        uint32_t m_trance;
        uint32_t m_unknown5;
        uint32_t m_focused;
        std::unique_ptr<std::vector<uint32_t>> m_spellcard_real_times;
        std::string m_stage_data;
        th13_t* m__root;
        th13_t* m__parent;

    public:
        uint16_t stage_num() const { return m_stage_num; }
        uint16_t rng() const { return m_rng; }
        uint32_t frame_count() const { return m_frame_count; }
        uint32_t end_off() const { return m_end_off; }
        uint32_t pos_subpixel_x() const { return m_pos_subpixel_x; }
        uint32_t pos_subpixel_y() const { return m_pos_subpixel_y; }
        uint32_t shot() const { return m_shot; }
        uint32_t subshot_unused() const { return m_subshot_unused; }
        uint32_t score() const { return m_score; }
        uint32_t unknown() const { return m_unknown; }
        uint32_t continues() const { return m_continues; }
        uint32_t unknown2() const { return m_unknown2; }
        uint32_t graze() const { return m_graze; }
        uint32_t unknown3() const { return m_unknown3; }
        uint32_t unknown4() const { return m_unknown4; }
        uint32_t piv() const { return m_piv; }
        uint32_t piv_min() const { return m_piv_min; }
        uint32_t piv_max() const { return m_piv_max; }
        uint32_t power() const { return m_power; }
        uint32_t power_max() const { return m_power_max; }
        uint32_t power_min() const { return m_power_min; }
        uint32_t lives() const { return m_lives; }
        uint32_t life_pieces() const { return m_life_pieces; }
        uint32_t extends() const { return m_extends; }
        uint32_t bombs() const { return m_bombs; }
        uint32_t bomb_pieces() const { return m_bomb_pieces; }
        uint32_t trance() const { return m_trance; }
        uint32_t unknown5() const { return m_unknown5; }
        uint32_t focused() const { return m_focused; }
        std::vector<uint32_t>* spellcard_real_times() const { return m_spellcard_real_times.get(); }
        std::string stage_data() const { return m_stage_data; }
        th13_t* _root() const { return m__root; }
        th13_t* _parent() const { return m__parent; }
    };

private:
    std::unique_ptr<header_t> m_header;
    std::unique_ptr<std::vector<std::unique_ptr<stage_t>>> m_stage;
    th13_t* m__root;
    kaitai::kstruct* m__parent;

public:
    header_t* header() const { return m_header.get(); }
    std::vector<std::unique_ptr<stage_t>>* stage() const { return m_stage.get(); }
    th13_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};
