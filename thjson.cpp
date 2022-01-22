/*
    thjson.cpp

    To be implemented in [name pending] Touhou Replay/Scoreboard website
    arguments: [string] file path of valid Touhou .rpy file
    output to stdout: [string] json encoded decrypted stage data


*/

#include "common.h"
#include "thdecode.h"
#include "thjson.h"
#include "zuntypes.h"

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
			printf("%s", th06json(buffer, flength));
			break;
		case 0x50523754:  //"T7RP"
			// th07decode(buffer, flength);
			printf("%s", th07json(buffer, flength));
			break;
		case 0x50523854:  //"T8RP"
			// th08decode(buffer, flength);
			printf("%s", th08json(buffer, flength));
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

	th06_replay_header_t *header = (th06_replay_header_t *)buffer;

	//	version
	writer.Key("version");
	char ver[5] = "    ";
	snprintf(ver, 5, "%.2hhx%.2hhx", header->version[0], header->version[1]);
	writer.String(ver);

	//	player
	const char *shots[] = {
		"ReimuA",
		"ReimuB",
		"MarisaA",
		"MarisaB"
	};

	writer.Key("shot");
	if(header->shot < 4) {
		writer.String(shots[header->shot]);
	} else {
		writer.String("Unknown");
	}

	//	difficulty
	writer.Key("difficulty");
	writer.Uint(header->difficulty);

	//	now decode the replay
	unsigned char **buf = &buffer;
	flength = th06decode(buf, flength);
	buffer = *buf;

	th06_replay_t *rep = (th06_replay_t*)&buffer[0x10];

	//	date, null terminated string
	char date[11] = "2000-01-01";
	memcpy(date+5, rep->date, 2);
	memcpy(date+8, &rep->date[3], 2);
	writer.Key("date");
	writer.String(date);

	//	name, null terminated string
	//	ensure that its null terminated
	if(rep->name[8] != '\0') rep->name[8] = '\0';
	writer.Key("name");
	writer.String(rep->name);

	//	score
	writer.Key("score");
	writer.Uint(rep->score);

	//	slowdown, format to appear as ingame
	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", rep->slowdown);
	writer.String(val);

	writer.Key("stage");
	writer.StartArray();

	for(int i = 0; i < 7; i++) {
		if(rep->stage_offsets[i] != 0x00) {
			th06_replay_stage_t *stage = (th06_replay_stage_t*)&buffer[rep->stage_offsets[i]];
			writer.StartObject();
			writer.Key("stage");
			writer.Int(i + 1);

			writer.Key("score");
			writer.Uint(stage->score);

			writer.Key("power");
			writer.Uint(stage->power);

			writer.Key("lives");
			writer.Int(stage->lives);

			writer.Key("bombs");
			writer.Int(stage->bombs);

			writer.Key("rank");
			writer.Uint(stage->rank);

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

	th07_replay_header_t *header = (th07_replay_header_t*)buffer;

	writer.Key("version");
	char ver[5] = "    ";
	snprintf(ver, 5, "%.2hhx%.2hhx", header->version[0], header->version[1]);
	writer.String(ver);

	unsigned char **buf = &buffer;
	flength = th07decode1(buf, flength);
	buffer = *buf;

	//	need to save the stage offsets from header, so make a copy of the memory
	//	also decrement each stage offset by 84 to account for full header size
	header = (th07_replay_header_t*)malloc(sizeof(th07_replay_header_t));
	memcpy(header, buffer, sizeof(th07_replay_header_t));
	for(int i = 0; i < 7; i++) {
		if(header->stage_offsets[i] != 0x00) {
			header->stage_offsets[i] -= 84;
		}
	}

	flength = th07decode2(buf, flength);
	buffer = *buf;

	const char *shots[] = {
		"ReimuA",
		"ReimuB",
		"MarisaA",
		"MarisaB",
		"SakuyaA",
		"SakuyaB"
	};

	th07_replay_t *rep = (th07_replay_t*)buffer;
	
	writer.Key("shot");
	if(rep->shot < 6) {
		writer.String(shots[rep->shot]);
	} else {
		writer.String("Unknown");
	}

	writer.Key("difficulty");
	writer.Uint(rep->difficulty);

	char date[11] = "2000-01-01";
	memcpy(date+5, rep->date, 2);
	memcpy(date+8, &rep->date[3], 2);
	writer.Key("date");
	writer.String(date);

	if(rep->name[8] != '\0') rep->name[8] = '\0';
	writer.Key("name");
	writer.String(rep->name);

	writer.Key("score");
	writer.Uint64((uint64_t)rep->score * 10);

	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", rep->slowdown);
	writer.String(val);

	writer.Key("stage");
	writer.StartArray();

	// max_stage++;
	for(int i = 0; i < 7; i++) {
		if(header->stage_offsets[i] != 0x00) {
			th07_replay_stage_t *stage = (th07_replay_stage_t*)&buffer[header->stage_offsets[i]];

			writer.StartObject();
			writer.Key("stage");
			writer.Uint(i + 1);

			writer.Key("score");
			writer.Uint64((uint64_t)stage->score * 10);

			writer.Key("point_items");
			writer.Uint(stage->point_items);

			writer.Key("PIV");
			writer.Uint(stage->piv);

			writer.Key("cherrymax");
			writer.Uint(stage->cherrymax);

			writer.Key("cherry");
			writer.Uint(stage->cherry);

			writer.Key("graze");
			writer.Uint(stage->graze);

			writer.Key("power");
			writer.Uint(stage->power);

			writer.Key("lives");
			writer.Uint(stage->lives);

			writer.Key("bombs");
			writer.Uint(stage->bombs);

			writer.EndObject();
		}
	}

	writer.EndArray();
	writer.EndObject();

	free(header);

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
	
	th08_replay_header_t *header = (th08_replay_header_t*)buffer;
	uint32_t user_offset = header->comp_size;
	if(user_offset + 8 < flength) {
		uint32_t magic = *(uint32_t*)&buffer[user_offset];
		if(magic == 0x52455355) {
			uint32_t user_length = *(uint32_t*)&buffer[user_offset + 4];
			if(user_offset + user_length <= flength) {
				writer.Key("user");
				writer.StartObject();
				
				user_offset += 25;
				int l = 0;
				
				writer.Key("name");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);
				
				user_offset += 13 + l;
				l = 0;
				
				writer.Key("date");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);
				
				user_offset += 11 + l;
				l = 0;
				
				writer.Key("shot");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);
				
				user_offset += 10 + l;
				l = 0;
				
				writer.Key("score");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);
				
				user_offset += 10 + l;
				l = 0;
				
				writer.Key("difficulty");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);
				
				user_offset += 2 + l;
				l = 0;
				
				writer.Key("stage");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);

				user_offset += 11 + l;
				l = 0;

				writer.Key("misses");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);

				user_offset += 11 + l;
				l = 0;

				writer.Key("bombs");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);

				user_offset += 13 + l;
				l = 0;

				writer.Key("slowdown");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);

				user_offset += 10 + l;
				l = 0;

				//	it's labelled as lives in the replay, but i think its the rating or something
				//	also its not written properly
				// writer.Key("skill");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				// writer.String((const char*)&buffer[user_offset], l);

				user_offset += 21 + l;
				l = 0;

				writer.Key("version");
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				buffer[user_offset + l] = '\0';
				writer.String((const char*)&buffer[user_offset], l);
				
				writer.EndObject();
			}
		}
	}


	unsigned char **buf = &buffer;
	flength = th08decode1(buf, flength);
	buffer = *buf;
	
	header = (th08_replay_header_t*)malloc(sizeof(th08_replay_header_t));
	memcpy(header, buffer, sizeof(th08_replay_header_t));
	for(int i = 0; i < 9; i++) {
		if(header->stage_offsets[i] != 0) {
			header->stage_offsets[i] -= 104;
		}
	}
	
	flength = th08decode2(buf, flength);
	buffer = *buf;
	
	th08_replay_t *stage_header = (th08_replay_t*)buffer;
	
	const char *shots[12] = {
		"Border Team (Reimu & Yukari)",
		"Magic Team (Marisa & Alice)",
		"Scarlet Devil Team (Sakuya & Remilia)",
		"Ghost Team (Youmu & Yuyuko)",
		"Reimu",
		"Yukari",
		"Marisa",
		"Alice",
		"Sakuya",
		"Remilia",
		"Youmu",
		"Yuyuko",
	};

	writer.Key("shot");
	if(stage_header->shot < 12) {
		writer.String(shots[stage_header->shot]);
	} else {
		writer.String("Unknown");
	}

	writer.Key("difficulty");
	writer.Uint(stage_header->difficulty);

	char date[11] = "2000-01-01";
	memcpy(date+5, stage_header->date, 2);
	memcpy(date+8, &stage_header->date[3], 2);
	writer.Key("date");
	writer.String(date);

	writer.Key("name");
	if(stage_header->name[8] != '\0') stage_header->name[8] = '\0';
	writer.String(stage_header->name);

	writer.Key("stage");
	writer.StartArray();

	for(int i = 0; i < 9; i++) {
		if(header->stage_offsets[i] != 0x00) {
			writer.StartObject();
			writer.Key("stage");
			writer.Uint(i + 1);
			
			th08_replay_stage_t *stage = (th08_replay_stage_t*)&buffer[header->stage_offsets[i]];

			writer.Key("score");
			writer.Uint64((uint64_t)stage->score * 10);

			writer.Key("point_items");
			writer.Uint(stage->point_items);

			writer.Key("time");
			writer.Uint(stage->time);

			writer.Key("graze");
			writer.Uint(stage->graze);

			writer.Key("PIV");
			writer.Uint(stage->piv);

			writer.Key("power");
			writer.Uint(stage->power);

			writer.Key("lives");
			writer.Uint(stage->lives);

			writer.Key("bombs");
			writer.Uint(stage->bombs);

			writer.EndObject();

		}
	}


	writer.EndArray();
	writer.EndObject();
	
	free(header);

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;


}