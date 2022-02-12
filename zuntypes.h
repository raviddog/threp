#ifndef _ZUNTYPES_H
#define _ZUNTYPES_H

#include <stdint.h>

#pragma pack(push,1)

struct th_timer_t {
    int prev_time;
    int time;
    float time_f;
    int game_speed_unused;
    int control;
};

struct th06_replay_header_t {
    char magic[4];
    char version[2];
    uint8_t shot;
    uint8_t difficulty;
    uint32_t checksum;
    uint16_t unknown2;
    uint8_t key;
    //  my decryption function uses a direct buffer offset, so this is unneeded
    //  im replacing it with an unknown char
    // unsigned char crypted_data[1]; // Janky hack
    char unknown;
};

struct th06_replay_t {
    char date[9];
    char name[9];
    uint16_t unknown2;
    uint32_t score;
    uint32_t unknown3;
    float slowdown;
    uint32_t unknown4;
    uint32_t stage_offsets[7];
};

struct th06_replay_stage_t {
	uint32_t score;
    uint16_t random_seed;
    uint16_t unknown1;
    uint8_t power;
    int8_t lives;
    int8_t bombs;
    uint8_t rank;	
};

struct th07_replay_header_t {
    char magic[4];
    char version[2];
    char unknown[7];
    uint8_t key;
    char unknown2[14];
    uint32_t stage_offsets[7];
};

struct th07_replay_t {
    char unknown[2];
    uint8_t shot;
    uint8_t difficulty;
    char date[6];
    char name[9];
    char unknown2[5];
    uint32_t score;
    char unknown3[92];
    float slowdown;
};

struct th07_replay_stage_t {
    uint32_t score; //  end of stage score
    uint32_t point_items;
    uint32_t piv;
    uint32_t cherrymax;
    uint32_t cherry;
    uint32_t graze;
    uint32_t unknown;   //  possibly lives gained
    uint32_t unknown2;  //  possibly next point item extend
    uint16_t unknown4;
    uint8_t power;
    uint8_t lives;
    uint8_t bombs;
    uint8_t unknown5;

};

struct th08_replay_header_t {
	char magic[4];
	char version[2];
	char unknown[6];
	uint32_t comp_size; // doubles as USERDATA offset
	uint32_t unknown2;
	uint8_t key;
	char unknown3[7];
	uint32_t size;
	uint32_t stage_offsets[9];
};

struct th08_replay_t {
	char unknown[2];
	uint8_t shot;
	uint8_t difficulty;
	char date[6];
	char name[9];
};

struct th08_replay_stage_t {
	uint32_t score;
	uint32_t point_items;
	uint32_t graze;
	uint32_t time;
	uint32_t unknown;
	uint32_t piv;
	uint32_t unknown2;
	uint8_t power;
	uint8_t lives;
	uint8_t bombs;
	uint8_t unknown3;
};

struct th09_replay_header_t {
    char magic[4];
    char version[2];
    char unknown[6];
    uint32_t comp_size; //  doubles as USERDATA offset
    uint32_t unknown2;  //  probs checksum or some shit
    uint8_t key;
    char unknown3[7];
    uint32_t size;
    uint32_t stage_offsets[40];
};

struct th09_replay_t {
    char unknown[4];
    char date[10];  //  double null terminated string
    char name[9];   //  null terminated string
    uint8_t difficulty;
};

struct th09_replay_stage_t {
    uint32_t score;
    uint16_t pair;  //  idk what this is but each stage pair has the same value
                    //  maybe its the stage time or something
    uint8_t shot;
    uint8_t ai; //  i think? 0 = human 1 = ai
    uint8_t lives;
    uint8_t unknown3;   //  it's usually 4, but idk
};

struct th10_replay_header_t {
    uint32_t magic;
    uint32_t version;   //  i assume, please doublecheck
    uint32_t unknown;
    uint32_t comp_size;
    char ignore2[12];    //  document later
    uint32_t filelength;
    uint32_t size;
    //  compressed data begins at offset 36
};

struct th10_replay_t {
    char name[12];  //  4 nulls at end
    uint32_t time;  //  its a 32-bit time_t value but there's no standard definition for that
    uint32_t score;
    char unknown2[52];
    float slowdown;
    uint8_t stagecount; //  its probs a full int but i dont care
    char pad[3];
    uint32_t unknown4;
    uint32_t shot;
    uint32_t difficulty;
    uint32_t unknown5;
    uint32_t unknown6;
};

//  first stage offset is at 0x64 of the decoded data
//  number of stage sections is at 0x4c

struct th10_replay_stage_t {
    uint16_t stage;
    uint16_t ignore3;
    uint32_t ignore;
    uint32_t next_stage_offset; // add to current stage offset, + current stage header length which is 0x1c4
    uint32_t score;
    uint32_t power;
    uint32_t piv;   //  faith
    uint32_t ignore2;
    uint32_t lives;
};

//  these are identical
 
    //  stage offset 0x70, stage count 0x58
#define th11_replay_header_t th10_replay_header_t

struct th11_replay_t {
    char name[12];  //  4 nulls at end
    uint64_t time;  //  its a 32-bit time_t value but there's no standard definition for that
    uint32_t score;
    char unknown2[64];
    uint32_t stagecount;
    uint32_t unknown3;
    uint32_t shot;
    uint32_t difficulty;
};

struct th11_replay_stage_t {
    uint16_t stage;
    uint16_t unknown;
    uint32_t ignore;    //  either seed or header length
    uint32_t next_stage_offset; // + 0x90
    uint32_t score;
    uint32_t power;
    uint32_t piv;
    uint16_t lives;
    uint16_t life_pieces;
    char ignore2[0x18];
    uint32_t graze;
};


#define th12_replay_header_t th10_replay_header_t

struct th12_replay_t {
    char name[12];  //  4 nulls at the end
    uint64_t time;  //  64 bit time_t value but theres no standard representation for that so
    uint32_t score;
    char unk[60];
    float slowdown;
    uint32_t stagecount;
    uint32_t shot;
    uint32_t subshot;
    uint32_t difficulty;
    uint32_t cleared;
    uint32_t unk2;
};

struct th12_replay_stage_t {
    uint16_t stage;
    uint16_t rng;
    uint32_t frame_count;    //  either seed or header length
    uint32_t next_stage_offset; // + 0xa0
    uint32_t score;
    uint32_t power;
    uint32_t piv;
    uint16_t lives;
    uint16_t life_pieces;   //  if > 0, subtract 1
    uint16_t bombs;
    uint16_t bomb_pieces;
    uint32_t ufo_1;
    uint32_t ufo_2;
    uint32_t ufo_3;
    char ignore2[24];
    uint32_t graze;
    uint32_t unk;
    uint32_t unk2[20];
};

#define th128_replay_header_t th10_replay_header_t

struct th128_replay_stage_t {
    uint32_t stage; //  commented out in threplay for unknown reason
    uint32_t ignore;    //  either seed or header length
    uint32_t next_stage_offset; //  + 0x90
    uint32_t score;
    uint32_t power;
    char ignore2[0x6c]; //  wtf is this
    uint32_t lives; //  as a %
    uint32_t bombs; //  as a %
    //  freeze area is here, but idk wtf my code is
    //  here's the C#
    //  ((int)System.BitConverter.ToSingle(decodedata, (int)stageoffset + 0x88))
};

struct th13_replay_header_t {
	uint32_t magic;
	uint32_t version;
	char unused[4];
	uint32_t userdata_offset;
	char unused1[12];
	uint32_t comp_size;
	uint32_t size;
};

struct th13_replay_t {
    char name[12];
    uint64_t timestamp;
    uint32_t score;
    uint8_t unk[60];
    float slowdown;
    uint32_t stage_count;
    uint32_t shot;
    uint32_t subshot_unused;
    uint32_t difficulty;
    uint32_t cleared;
    uint32_t field_6C;
    uint32_t spell_practice_id;
};

struct th13_replay_stage_t {
    uint16_t stage_num;
    uint16_t rng;
    uint32_t frame_count;
    uint32_t end_off;
    uint32_t pos_subpixel[2];
    uint32_t shot;
    uint32_t subshot_unused;
    uint32_t score;
    uint32_t field_C;
    uint32_t continues;
    uint32_t field_14;
    uint32_t graze;
    uint32_t field_1C;
    uint32_t field_20;
    uint32_t piv;
    uint32_t piv_min;
    uint32_t piv_max;
    uint32_t power;
    uint32_t power_max;
    uint32_t power_levelup;
    uint32_t lives;
    uint32_t life_pieces;
    uint32_t extends;
    uint32_t bombs;
    uint32_t bomb_pieces;
    uint32_t trance_gauge;
    uint32_t field_54;
    uint32_t player_is_focused;
    uint32_t spellcard_real_times[21];
};

struct th14_replay_stage_t {
    uint16_t stage_num;
    uint16_t rng;
    uint32_t frame_count;
    uint32_t end_off;
    uint32_t pos_subpixel[2];
    uint32_t _stage_num;
    uint32_t __stage_num;
    uint32_t score;
    uint32_t difficulty;
    uint32_t continues;
    uint32_t field_14;
    uint32_t graze;
    uint32_t spell_practice_id;
    uint32_t field_20;
    uint32_t piv;
    uint32_t piv_min;
    uint32_t piv_max;
    uint32_t power;
    uint32_t power_max;
    uint32_t power_levelup;
    uint32_t lives;
    uint32_t life_pieces;
    uint32_t field_44;
    uint32_t bombs;
    uint32_t bomb_pieces;
    uint32_t score_from_poc;
    uint32_t field_54;
    uint32_t field_58;
    uint32_t field_5C;
    float last_item_collected_pos[3];
    uint32_t poc_count;
    uint32_t player_is_focused;
    uint32_t spellcard_real_times[21];
};

struct th14_replay_t {
    char name[12];
    uint32_t timestamp;
    uint32_t score;
    uint8_t unk[96];
    float slowdown;
    uint32_t stage_count;
    uint32_t shot;
    uint32_t subshot;
    uint32_t difficulty;
    uint32_t cleared;
    uint32_t field_8C;
    uint32_t spell_practice_id;
};

// This struct is not complete, and it doesn't need to be
// because nobody ever gets the sizeof of it
struct th143_replay_t {
    char name[20];
    uint64_t timestamp;
    uint32_t score;
    uint32_t field_20[23];
    float slowdown;
    uint32_t field_80;
    uint32_t field_84;
    uint32_t day;
    uint32_t scene;
    uint32_t field_90;
    uint32_t primary_item;
    uint32_t secondary_item;
    uint32_t field_9C;
    uint32_t primary_item_uses;
    uint32_t iframes;
    uint16_t scene_num;
    uint16_t rng;
};

struct th15_replay_t {
    char name[12];
    uint64_t timestamp;
    uint32_t score;
    char unk0[108];
    float slowdown;
    uint32_t stage_count;
    uint32_t shot;
    uint32_t subshot_unused;
    uint32_t difficulty;
    uint32_t cleared;
    uint32_t field_9C;
    uint32_t spell_practice_id;
};

struct th15_replay_stage_t
{
    uint16_t stage;
    uint16_t rng;
    int frame_count;
    int end_off;
    int pos_subpixel[2];
    uint32_t stage_num;
    uint32_t _stage_num;
    uint32_t chapter;
    uint32_t time_in_stage;
    uint32_t time_in_chapter;
    uint32_t shot;
    uint32_t subshot_unused;
    uint32_t score;
    uint32_t difficulty;
    uint32_t continues;
    uint32_t rank;
    uint32_t graze;
    uint32_t graze_chapter;
    int32_t spell_practice_id;
    uint32_t field_38;
    uint32_t miss_count;
    uint32_t point_items_collected;
    uint32_t piv;
    uint32_t piv_min;
    uint32_t piv_max;
    uint32_t power;
    uint32_t power_max;
    uint32_t power_levelup;
    uint32_t field_5C;
    uint32_t lives;
    uint32_t life_pieces;
    uint32_t extends;
    uint32_t bombs;
    uint32_t bomb_pieces;
    uint32_t field_74;
    uint32_t field_78;
    uint32_t field_7C;
    uint32_t field_80;
    float last_item_collected_pos[3];
    uint32_t th14_item_spawn_count;
    uint32_t enemies_in_chapter;
    uint32_t enemies_killed_in_chapter;
    char bgm_fn[256];
    uint32_t field_19C;
    uint32_t field_1A0;
    uint32_t __pd_resets_total;
    uint32_t __pd_resets_stage[8];
    uint32_t __pd_resets_chapter;
    int player_is_focused;
    int spellcard_real_times[21];
};

struct th16_replay_stage_t {
    uint16_t stage;
    uint16_t rng_state;
    uint32_t frame_count;
    uint32_t end_off;
    uint32_t pos_subpixel[2];
    uint32_t stage_num;
    uint32_t _stage_num;
    uint32_t chapter;
    uint32_t stage_time;
    uint32_t __time_in_chapter_possibly_broken;
    uint32_t shot;
    uint32_t subshot_unused;
    uint32_t subseason;
    uint32_t score;
    uint32_t difficulty;
    uint32_t continues;
    uint32_t rank_unused;
    uint32_t graze;
    uint32_t __graze_in_chapter_possibly_broken;
    int32_t spell_practice_id;
    uint32_t miss_count;
    uint32_t point_items_collected;
    uint32_t field_44;
    uint32_t piv;
    uint32_t piv_min;
    uint32_t piv_max;
    uint32_t power;
    uint32_t power_max;
    uint32_t power_levelup;
    uint32_t field_60;
    uint32_t lives;
    uint32_t life_pieces;
    uint32_t next_score_extend_idx;
    uint32_t bombs;
    uint32_t bomb_pieces;
    uint32_t season_power;
    uint32_t season_power_max;
    uint32_t field_80;
    uint32_t field_84;
    uint32_t field_88;
    uint32_t field_8C;
    uint32_t field_90;
    uint32_t field_94;
    uint32_t field_98;
    uint32_t field_9C;
    uint32_t field_A0;
    uint32_t field_A4;
    uint32_t season_power_required[6];
    uint32_t _season_power_max;
    uint32_t field_C4;
    uint32_t field_C8;
    uint32_t field_CC;
    uint32_t field_D0;
    uint32_t field_D4;
    uint32_t field_D8;
    uint32_t field_DC;
    float last_item_collected_pos[3];
    uint32_t th14_item_spawn_count;
    uint8_t field_F0[308];
    uint32_t field_238;
    uint32_t field_23C;
    uint32_t spellcard_real_times[21];
};

struct th16_replay_t {
    char name[12];
    uint64_t timestamp;
    uint32_t score;
    uint32_t unk0[25];
    float slowdown;
    uint32_t stage_count;
    uint32_t shot;
    uint32_t subshot_unused;
    uint32_t difficulty;
    uint32_t cleared;
    uint32_t field_94;
    int32_t spell_practice_id;
    uint32_t subseason;
};

// This struct is not complete, and it doesn't need to be
// because nobody ever gets the sizeof of it
struct th165_replay_t {
    char name[20];
    uint64_t timestamp;
    uint32_t score;
    uint32_t field_20[25];
    float slowdown;
    uint32_t field_88;
    uint32_t day;
    uint32_t scene;
};

struct th17_stage_global_t {
    uint32_t stage_num;
    uint32_t field_4;
    uint32_t chapter;
    uint32_t stage_time[3];
    uint32_t shot;
    uint32_t goast;
    uint32_t score;
    uint32_t diff;
    uint32_t continues;
    uint32_t rank_unused;
    uint32_t graze;
    uint32_t field_34;
    int32_t spell_practice_id;
    uint32_t miss_count;
    uint32_t field_40;
    uint32_t point_items_collected;
    uint32_t piv;
    uint32_t piv_min;
    uint32_t piv_max;
    uint32_t power;
    uint32_t power_max;
    uint32_t power_levelup;
    uint32_t field_60;
    uint32_t lives;
    uint32_t life_pieces;
    uint32_t field_6C;
    uint32_t bombs;
    uint32_t bomb_pieces;
    uint32_t bomb_restock_on_death;
    uint32_t field_7C;
    uint32_t field_80;
    uint32_t hyper_fill;
    uint32_t tokens[5];
    uint32_t field_9C;
    uint32_t field_A0;
    uint32_t field_A4;
    uint32_t field_A8;
    uint32_t field_AC;
    uint32_t field_B0;
    th_timer_t field_B4;
    th_timer_t hyper_time;
    uint32_t field_DC;
    uint32_t field_E0;
    uint32_t field_E4;
    uint32_t hyper_flags;
};

struct th17_replay_t {
    char name[16];
    uint64_t timestamp;
    uint32_t score;
    char unk1[100];
    float slowdown;
    uint32_t stage_count;
    uint32_t shot;
    uint32_t goast;
    uint32_t difficulty;
    uint32_t cleared;
    char unk2[4];
    int32_t spell_practice_id;
};

struct th17_replay_stage_t {
    uint16_t stage;
    uint16_t rng;
    uint32_t frame_count;
    uint32_t end_off; // Relative to the start of this structure!!!
    uint32_t pos_subpixel[2];
    th17_stage_global_t stagedata;
    uint32_t player_is_focused;
    uint32_t spellcard_real_times[21];
};

struct th18_stage_global_t {
  uint32_t stage_num;
  uint32_t _stage_num;
  uint32_t field_8;
  uint32_t stage_time[3];
  uint32_t shot;
  uint32_t subshot;
  uint32_t score;
  uint32_t difficulty;
  uint32_t continues;
  uint32_t field_2C;
  uint32_t graze;
  uint32_t field_34;
  int32_t spell_practice_id;
  uint32_t miss_count;
  uint32_t field_40;
  uint32_t money_collected;
  uint32_t piv;
  uint32_t piv_min;
  uint32_t piv_max;
  uint8_t unk1[8];
  uint32_t power;
  uint32_t power_max;
  uint32_t power_levelup;
  uint32_t field_60;
  uint32_t lives;
  uint32_t life_pieces;
  uint32_t field_6C;
  uint32_t field_70;
  uint32_t bombs;
  uint32_t bomb_pieces;
  uint32_t bomb_restock_on_death;
  uint8_t unk0[72];
  th_timer_t field_C4;
  th_timer_t field_D8;
  uint8_t unk2[4];
};

/* 111 */
struct th18_replay_stage_t {
  uint16_t stage;
  uint16_t rng;
  uint32_t framecount;
  uint32_t end_off; // Relative to the start of this structure!!!
  uint32_t pos_subpixel[2];
  uint32_t player_is_focused;
  uint32_t spellcard_real_times[20];
  th18_stage_global_t stagedata;
  int32_t cards[256];
  uint32_t cards_param[256];
  uint32_t card_active;
  th18_stage_global_t stagedata_end;
  int32_t cards_end[256];
  uint32_t cards_param_end[256];
  uint32_t card_active_end;
  uint32_t player_is_focused_end;
};

struct th18_replay_t
{
  char name[16];
  uint64_t timestamp;
  uint32_t score;
  char unk0[136];
  float slowdown;
  uint32_t stage_count;
  uint32_t shot;
  uint32_t subshot_unused;
  uint32_t difficulty;
  uint32_t cleared;
  uint32_t field_BC;
  int32_t spell_practice_id;
  uint32_t field_C4;
};

struct th_replay_userdata_header_t {
	uint32_t magic; // "USER"
	uint32_t length;
	uint32_t section_type;
};

#pragma pack(pop)

#endif