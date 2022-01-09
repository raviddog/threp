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
	snprintf(ver, 7, "%#.2hhx%.2hhx", buffer[0x05], buffer[0x06]);
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
	unsigned char *date = &buffer[0x10];
	if(date[9] != '\0') date[9] = '\0';
	writer.Key("date");
	writer.String((char*)date);

	//	name, null terminated string
	unsigned char *name = &buffer[0x1a];
	if(name[9] != '\0') name[9] = '\0';
	writer.Key("name");
	writer.String((char*)name);

	//	score
	writer.Key("score");
	writer.Uint(*(uint32_t*) &buffer[0x24]);

	//	slowdown
	//	0x2c, float32
	writer.Key("slowdown");
	float val = * (float *) &buffer[0x2c];
	writer.Double((double)val);

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
		writer.Int(7);

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

	}

	writer.EndArray();
	writer.EndObject();

	char *json = new char[s.GetSize()];
	memcpy(json, s.GetString(), s.GetSize());
	return json;
}