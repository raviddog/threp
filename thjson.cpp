/*
    thjson.cpp

    To be implemented in [name pending] Touhou Replay/Scoreboard website
    arguments: [string] file path of valid Touhou .rpy file
    output to stdout: [string] json encoded decrypted stage data


*/

#include "common.h"

#include <cstdio>
#include <iostream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


char * th06json(unsigned char *buffer, unsigned int flength);
char * th07json(unsigned char *buffer, unsigned int flength);

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
	char ver[7];
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
	char date[11];
	date[0] = '2';
	date[1] = '0';
	memcpy(date+2, &buffer[0x16], 2);
	date[4] = '-';
	memcpy(date+5, &buffer[0x10], 2);
	date[7] = '-';
	memcpy(date+8, &buffer[0x13], 2);
	date[10] = '\0';
	// if(date[9] != '\0') date[9] = '\0';
	writer.Key("date");
	writer.String(date);

	//	name, null terminated string
	// if(name[9] != '\0') name[9] = '\0';
	buffer[0x18] = 0x01;
	writer.Key("name");
	writer.String((char*)&buffer[0x19]);

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

	if(max_stage == 6) {
		//	extra
		writer.StartObject();
		writer.Key("stage");
		writer.Uint(7u);

		writer.Key("score");
		writer.Uint(* (uint32_t *) &buffer[stage_offset[6]]);

		writer.Key("power");
		writer.Uint(buffer[stage_offset[6] + 0x8]);

		writer.Key("lives");
		writer.Uint((uint32_t)buffer[stage_offset[6] + 0x9]);

		writer.Key("bombs");
		writer.Uint((uint32_t)buffer[stage_offset[6] + 0xa]);

		writer.Key("rank");
		writer.Uint((uint32_t)buffer[stage_offset[6] + 0xb]);

		writer.EndObject();

	} else {
		max_stage++;
		for(int i = 0; i < max_stage; i++) {
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
	char *json = new char[jsonsize + 1];
	memcpy(json, s.GetString(), jsonsize + 1);
	json[jsonsize] = '\0';
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
	buffer[0x09] = 0x01;
	writer.String((char*)&buffer[0x0a]);

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

	if(max_stage == 6) {
		//	extra or phantasm
		writer.StartObject();
		writer.Key("stage");
		writer.Uint(7u);

		writer.Key("score");
		uint32_t score = *(uint32_t*)&buffer[stage_offset[6]];
		uint64_t score_long = score;
		score_long *= 10;
		writer.Uint64(score_long);

		writer.Key("PIV");
		writer.Uint(*(uint32_t*) &buffer[stage_offset[6] + 0x8]);

		writer.Key("point_items");
		writer.Uint(*(uint32_t*) &buffer[stage_offset[6] + 0x4]);

		writer.Key("graze");
		writer.Uint(*(uint32_t*) &buffer[stage_offset[6] + 0x14]);

		writer.Key("power");
		writer.Uint(buffer[stage_offset[6] + 0x22]);

		writer.Key("lives");
		writer.Uint(buffer[stage_offset[6] + 0x23]);

		writer.Key("bombs");
		writer.Uint(buffer[stage_offset[6] + 0x24]);

		writer.EndObject();

	} else {

	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize + 1];
	memcpy(json, s.GetString(), jsonsize + 1);
	json[jsonsize] = '\0';
	return json;
}