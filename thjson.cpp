/*
    thjson.cpp

    To be implemented in [name pending] Touhou Replay/Scoreboard website
    arguments: [string] file path of valid Touhou .rpy file
    output to stdout: [string] json encoded decrypted stage data


*/

#include "common.h"
#include "thdecode.h"
#include "thjson.h"

#include <cstdio>
#include <iostream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage : %s [filename]", argv[0]);
		return 0;
	}

	char * file = argv[1];
	FILE * fp;
	fp = fopen(file, "rb");
	if (!fp) {
		printf("%s not found.\n", file);
		return 0;
	}

	unsigned int flength;
	fseek(fp, 0L, SEEK_END);
	flength = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	unsigned char *buffer = new unsigned char[flength];
	fread(buffer, flength, 1, fp);
	fclose(fp);

	int magic = *(int*) buffer;

	switch(magic) {
		case 0x50523654:  //"T6RP"
			// th06decode(buffer, flength);
			printf(th06json(buffer, flength));
			break;
		case 0x50523754:  //"T7RP"
			// th07decode(buffer, flength);
			printf(th07json(buffer, flength));
			break;
		case 0x50523854:  //"T8RP"
			// th08decode(buffer, flength);
			printf(th08json(buffer, flength));
			break;
		case 0x50523954: 	//"T9RP"
			// th09decode(buffer, flength);
			break;
		case 0x72303174:  //"t10r"
			// th10decode(buffer, flength);
			break;
		case 0x72313174:  //"t11r"
			// th11decode(buffer, flength);
			break;
		case 0x72323174:  //"t12r"
			// th12decode(buffer, flength);
			break;
		case 0x35323174:  //"t125"
			// th125decode(buffer, flength);
			break;
		case 0x72383231:  //"128r"
			// th128decode(buffer, flength);
			break;
		case 0x72333174:  //"t13r"
			// th13decode(buffer, flength);
			break;
		case 0x72353174: 	//"t15r"
			// th13decode(buffer, flength);
			break;
		case 0x72363174: 	//"t16r"
			// th13decode(buffer, flength);
			break;
		case 0x72373174: 	//"t17r"
			// th13decode(buffer, flength);
			break;
		case 0x72383174: 	//"t18r"
			// th13decode(buffer, flength);
			break;
		default:
			printf("not supported format.\n");
			break;
	}
	
	delete[] buffer;
	return 0;
}


char * th06json(unsigned char *buffer, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(0);

	//	version
	writer.Key("version");
	char ver[7] = "      ";
	snprintf(ver, 7, "%#.2hhx%.2hhx", buffer[0x05], buffer[0x04]);
	writer.String(ver);

	//	player
	writer.Key("shot");
	char shot = buffer[0x06];
	switch((int)shot) {
		case 0:
			writer.String("ReimuA");
			break;
		case 1:
			writer.String("ReimuB");
			break;
		case 2:
			writer.String("MarisaA");
			break;
		case 3:
			writer.String("MarisaB");
			break;
		default:
			writer.String("Unknown");
			break;
	}

	//	difficulty
	writer.Key("difficulty");
	unsigned char diff = buffer[0x07];
	writer.Uint((uint32_t)diff);

	//	now decode the replay
	unsigned char **buf = &buffer;
	flength = th06decode(buf, flength);
	buffer = *buf;

	//	date, null termianted string
	char date[11] = "2000-01-01";
	memcpy(date+2, &buffer[0x16], 2);
	memcpy(date+5, &buffer[0x10], 2);
	memcpy(date+8, &buffer[0x13], 2);
	writer.Key("date");
	writer.String(date);

	//	name, null terminated string
	// if(name[9] != '\0') name[9] = '\0';
	// buffer[0x18] = 0x01;
	writer.Key("name");
	char name[9];
	memcpy(name, &buffer[0x19], 9);
	writer.String(name);

	//	score
	writer.Key("score");
	writer.Uint(*(uint32_t*) &buffer[0x24]);

	//	slowdown
	//	0x2c, float32
	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", * (float *) &buffer[0x2c]);
	writer.String(val);

	writer.Key("stage");
	writer.StartArray();

	uint32_t stage_offset[7];
	int max_stage = 0;
	for(int i = 0; i < 7; i++) {
		stage_offset[i] = * (uint32_t *) &buffer[0x34 + 4 * i];
		if(stage_offset[i] != 0x00) {
			max_stage = i;
		}
	}

	max_stage++;
	for(int i = 0; i < max_stage; i++) {
		if(stage_offset[i] != 0x00) {
			writer.StartObject();
			writer.Key("stage");
			writer.Int(i + 1);

			writer.Key("score");
			writer.Uint(* (uint32_t *) &buffer[stage_offset[i]]);

			writer.Key("power");
			writer.Uint(buffer[stage_offset[i] + 0x8]);

			writer.Key("lives");
			writer.Uint((uint32_t)buffer[stage_offset[i] + 0x9]);

			writer.Key("bombs");
			writer.Uint((uint32_t)buffer[stage_offset[i] + 0xa]);

			writer.Key("rank");
			writer.Uint((uint32_t)buffer[stage_offset[i] + 0xb]);

			writer.EndObject();
		}
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize];
	memcpy(json, s.GetString(), jsonsize);
	return json;
}

char * th07json(unsigned char *buffer, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(1);

	unsigned char **buf = &buffer;
	flength = th07decode1(buf, flength);
	buffer = *buf;

	uint32_t stage_offset[7];
	int max_stage = 0;
	for(int i = 0; i < 7; i++) {
		stage_offset[i] = * (uint32_t *) &buffer[0x1c + 4 * i];
		if(stage_offset[i] != 0x00) {
			stage_offset[i] -= 0x54;
			max_stage = i;
		}
	}

	flength = th07decode2(buf, flength);
	buffer = *buf;

	writer.Key("shot");
	switch(buffer[0x02]) {
		case 0:
			writer.String("ReimuA");
			break;
		case 1:
			writer.String("ReimuB");
			break;
		case 2:
			writer.String("MarisaA");
			break;
		case 3:
			writer.String("MarisaB");
			break;
		case 4:
			writer.String("SakuyaA");
			break;
		case 5:
			writer.String("SakuyaB");
			break;
		default:
			writer.String("Unknown");
			break;
	}

	writer.Key("difficulty");
	writer.Uint(buffer[0x03]);

	char date[11];
	date[0] = '2';
	date[1] = '0';
	date[2] = '0';
	date[3] = '0';
	date[4] = '-';
	memcpy(date+5, &buffer[0x04], 2);
	date[7] = '-';
	memcpy(date+8, &buffer[0x07], 2);
	date[10] = '\0';
	// if(date[9] != '\0') date[9] = '\0';
	writer.Key("date");
	writer.String(date);

	writer.Key("name");
	// buffer[0x09] = 0x01;
	char name[9];
	memcpy(name, &buffer[0x0a], 9);
	name[8] = '\0';
	writer.String(name);

	writer.Key("score");
	uint32_t score = *(uint32_t*)&buffer[0x18];
	uint64_t score_long = score;
	score_long *= 10;
	writer.Uint64(score_long);

	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", * (float *) &buffer[0x78]);
	writer.String(val);

	writer.Key("stage");
	writer.StartArray();

	max_stage++;
	for(int i = 0; i < max_stage; i++) {
		if(stage_offset[i] != 0x00) {
			writer.StartObject();
			writer.Key("stage");
			writer.Uint(i + 1);

			writer.Key("score");
			uint32_t score = *(uint32_t*)&buffer[stage_offset[i]];
			uint64_t score_long = score;
			score_long *= 10;
			writer.Uint64(score_long);

			writer.Key("point_items");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x4]);

			writer.Key("PIV");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x8]);

			writer.Key("cherrymax");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0xc]);

			writer.Key("cherry");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x10]);

			writer.Key("graze");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x14]);

			writer.Key("power");
			writer.Uint(buffer[stage_offset[i] + 0x22]);

			writer.Key("lives");
			writer.Uint(buffer[stage_offset[i] + 0x23]);

			writer.Key("bombs");
			writer.Uint(buffer[stage_offset[i] + 0x24]);

			writer.EndObject();
		}
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize + 1];
	memcpy(json, s.GetString(), jsonsize + 1);
	json[jsonsize] = '\0';
	return json;
}

char * th08json(unsigned char *buffer, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(2);

	uint32_t user_offset = * (uint32_t *) &buffer[0x0c];
	if(user_offset + 8 < flength) {
		if(buffer[user_offset] == 'U' && buffer[user_offset+1] == 'S' && buffer[user_offset+2] == 'E' && buffer[user_offset+3] == 'R') {
			uint32_t user_length = *(uint32_t *) &buffer[user_offset + 4];
			if(user_offset + user_length < flength) {
				writer.Key("user");
				writer.StartObject();

				user_offset += 19;
				int l = 0;

				writer.Key("name");
				for(; buffer[user_offset + l + 1] != 0x0d && buffer[user_offset + l + 2] != 0x0a; l++);
				char *name = new char[l + 1];
				memcpy(name, &buffer[user_offset + 0x19], l);
				name[l] = '\0';
				writer.String(name, l + 1, true);
				delete name;

				user_offset += l;
				user_offset += 0xe;
				l = 0;

				writer.Key("date");
				for(; buffer[user_offset + 0x2e + l + 1] != 0x0d && buffer[user_offset + 0x2e + l + 2] != 0x0a; l++);
				char *date = new char[l + 1];
				memcpy(date, &buffer[user_offset + 0x2e], l);
				date[l] = '\0';
				writer.String(date, l + 1, true);
				delete date;

				user_offset += l;
				user_offset += 0xb;
				l = 0;

				writer.Key("shot");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *shot = new char[l + 1];
				memcpy(shot, &buffer[user_offset + 0x4c], l);
				shot[l] = '\0';
				writer.String(shot, l + 1, true);
				delete shot;

				user_offset += l;
				user_offset += 0xa;
				l = 0;

				writer.Key("score");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *score = new char[l + 1];
				memcpy(score, &buffer[user_offset + 0x4c], l);
				score[l] = '\0';
				writer.String(score, l + 1, true);
				delete score;

				user_offset += l;
				user_offset += 0x9;
				l = 0;

				writer.Key("difficulty");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *difficulty = new char[l + 1];
				memcpy(difficulty, &buffer[user_offset + 0x4c], l);
				difficulty[l] = '\0';
				writer.String(difficulty, l + 1, true);
				delete difficulty;

				user_offset += l;
				l = 0;

				writer.Key("stage");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *stage = new char[l + 1];
				memcpy(stage, &buffer[user_offset + 0x4c], l);
				stage[l] = '\0';
				writer.String(stage, l + 1, true);
				delete stage;

				user_offset += l;
				user_offset += 0x8;
				l = 0;

				writer.Key("mistakes");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *mistakes = new char[l + 1];
				memcpy(mistakes, &buffer[user_offset + 0x4c], l);
				mistakes[l] = '\0';
				writer.String(mistakes, l + 1, true);

				user_offset += l;
				user_offset += 0xb;
				l = 0;

				writer.Key("bombed");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *bombed = new char[l + 1];
				memcpy(bombed, &buffer[user_offset + 0x4c], l);
				bombed[l] = '\0';
				writer.String(bombed, l + 1, true);

				user_offset += l;
				user_offset += 0xb;
				l = 0;

				writer.Key("slowdown");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *slowdown = new char[l + 1];
				memcpy(slowdown, &buffer[user_offset + 0x4c], l);
				slowdown[l] = '\0';
				writer.String(slowdown, l + 1, true);

				user_offset += l;
				user_offset += 0x26;
				l = 0;

				writer.Key("version");
				for(; buffer[user_offset + 0x4c + l + 1] != 0x0d && buffer[user_offset + 0x4c + l + 2] != 0x0a; l++);
				char *version = new char[l + 1];
				memcpy(version, &buffer[user_offset + 0x4c], l);
				version[l] = '\0';
				writer.String(version, l + 1, true);


				writer.EndObject();


			}
		}
	}


	unsigned char **buf = &buffer;
	flength = th08decode1(buf, flength);
	buffer = *buf;

	uint32_t stage_offset[7];
	int max_stage = 0;
	for(int i = 0; i < 9; i++) {
		stage_offset[i] = * (uint32_t *) &buffer[0x20 + 4 * i];
		if(stage_offset[i] != 0x00) {
			stage_offset[i] -= 0x68;
			max_stage = i;
		}
	}

	flength = th08decode2(buf, flength);
	buffer = *buf;

	writer.Key("shot");
	switch(buffer[0x02]) {
		case 0:
			writer.String("Border Team (Reimu & Yukari)");
			break;
		case 1:
			writer.String("Magic Team (Marisa & Alice)");
			break;
		case 2:
			writer.String("Scarlet Devil Team (Sakuya & Remilia)");
			break;
		case 3:
			writer.String("Ghost Team (Youmu & Yuyuko)");
			break;
		case 4:
			writer.String("Reimu");
			break;
		case 5:
			writer.String("Yukari");
			break;
		case 6:
			writer.String("Marisa");
			break;
		case 7:
			writer.String("Alice");
			break;
		case 8:
			writer.String("Sakuya");
			break;
		case 9:
			writer.String("Remilia");
			break;
		case 10:
			writer.String("Youmu");
			break;
		case 11:
			writer.String("Yuyuko");
			break;
		default:
			writer.String("Unknown");
			break;
	}

	writer.Key("difficulty");
	writer.Uint(buffer[0x03]);

	char date[11];
	date[0] = '2';
	date[1] = '0';
	date[2] = '0';
	date[3] = '0';
	date[4] = '-';
	memcpy(date+5, &buffer[0x04], 2);
	date[7] = '-';
	memcpy(date+8, &buffer[0x07], 2);
	date[10] = '\0';
	writer.Key("date");
	writer.String(date);

	writer.Key("name");
	// buffer[0x09] = 0x01;
	char name[9];
	memcpy(name, &buffer[0x0a], 9);
	name[8] = '\0';
	writer.String(name);

	writer.Key("stage");
	writer.StartArray();

	max_stage++;
	for(int i = 0; i < max_stage; i++) {
		if(stage_offset[i] != 0x00) {
			writer.StartObject();
			writer.Key("stage");
			writer.Uint(i + 1);

			writer.Key("score");
			uint32_t score = *(uint32_t*)&buffer[stage_offset[i]];
			uint64_t score_long = score;
			score_long *= 10;
			writer.Uint64(score_long);

			writer.Key("point_items");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x4]);

			writer.Key("time");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0xc]);

			writer.Key("graze");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x8]);

			writer.Key("PIV");
			writer.Uint(*(uint32_t*) &buffer[stage_offset[i] + 0x14]);

			writer.Key("power");
			writer.Uint(buffer[stage_offset[i] + 0x1c]);

			writer.Key("lives");
			writer.Uint(buffer[stage_offset[i] + 0x1d]);

			writer.Key("bombs");
			writer.Uint(buffer[stage_offset[i] + 0x1e]);

			writer.EndObject();

		}
	}


	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize + 1];
	memcpy(json, s.GetString(), jsonsize + 1);
	json[jsonsize] = '\0';
	return json;


}