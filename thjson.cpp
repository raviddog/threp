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
	unsigned char **buf = &buffer;

	const char *out = "not supported format\n";
	bool e = false;

	switch(magic) {
		case 0x50523654:  //"T6RP"
			// th06decode(buffer, flength);
			out = th06json(buf, flength);
			e = true;
			break;
		case 0x50523754:  //"T7RP"
			// th07decode(buffer, flength);
			out = th07json(buf, flength);
			e = true;
			break;
		case 0x50523854:  //"T8RP"
			// th08decode(buffer, flength);
			out = th08json(buf, flength);
			e = true;
			break;
		case 0x50523954: 	//"T9RP"
			// th09decode(buffer, flength);
			out = th09json(buf, flength);
			e = true;
			break;
		case 0x72303174:  //"t10r"
			// th10decode(buffer, flength);
			out = th10json(buf, flength);
			e = true;
			break;
		case 0x72313174:  //"t11r"
			// th11decode(buffer, flength);
			out = th11json(buf, flength);
			e = true;
			break;
		case 0x72323174:  //"t12r"
			// th12decode(buffer, flength);
			out = th12json(buf, flength);
			e = true;
			break;
		case 0x35323174:  //"t125"
			// th125decode(buffer, flength);
			break;
		case 0x72383231:  //"128r"
			// th128decode(buffer, flength);
			break;
		case 0x72333174:  //"t13r"
			// th13decode(buffer, flength);
			{
				th13_replay_header_t *header = (th13_replay_header_t*)buffer;
				unsigned char id = buffer[header->userdata_offset + 0x10];
				if(id == 144) {
					out = th13json(buf, flength);
					e = true;
				} else {
					out = th14json(buf, flength);
					e = true;
				}
			}
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
			break;
	}

	printf("%s\n", out);
	if(e) delete[] out;
	
	delete[] buffer;
	return 0;
}

char * th06json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

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

char * th07json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(1);

	th07_replay_header_t *header = (th07_replay_header_t*)buffer;

	writer.Key("version");
	char ver[5] = "    ";
	snprintf(ver, 5, "%.2hhx%.2hhx", header->version[0], header->version[1]);
	writer.String(ver);

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

char * th08json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

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
				
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}
				
				user_offset += 13 + l;
				l = 0;
				
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}
				
				user_offset += 11 + l;
				l = 0;
				
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("shot");
					writer.String((const char*)&buffer[user_offset], l);
				}
				
				user_offset += 10 + l;
				l = 0;
				
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("score");
					writer.String((const char*)&buffer[user_offset], l);
				}
				
				user_offset += 10 + l;
				l = 0;
				
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}
				
				user_offset += 2 + l;
				l = 0;
				
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 11 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("misses");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 11 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("bombs");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 13 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("slowdown");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 10 + l;
				l = 0;

				//	it's labelled as lives in the replay, but i think its the rating or something
				//	also its not written properly
				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					// writer.Key("skill");
					// writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 21 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}
				
				writer.EndObject();
			}
		}
	}


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

char * th09json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(3);

	th09_replay_header_t *header = (th09_replay_header_t*)buffer;
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

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 13 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 10 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 10 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 30 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("handicap");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 21 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}

				writer.EndObject();
			}
		}
	}

	
	flength = th09decode1(buf, flength);
	buffer = *buf;

	header = (th09_replay_header_t*)malloc(sizeof(th09_replay_header_t));
	memcpy(header, buffer, sizeof(th09_replay_header_t));
	for(int i = 0; i < 40; i++) {
		if(header->stage_offsets[i] != 0) {
			header->stage_offsets[i] -= 192;
		}
	}

	flength = th09decode2(buf, flength);
	buffer = *buf;

	th09_replay_t *stage_header = (th09_replay_t*)buffer;
	
	const char *shots[16] = {
		"Reimu",
		"Marisa",
		"Sakuya",
		"Youmu",
		"Reisen",
		"Cirno",
		"Lyrica",
		"Mystia",
		"Tewi",
		"Aya",
		"Medicine",
		"Yuuka",
		"Komachi",
		"Eiki",
		"Merlin",
		"Lunasa"
	};

	writer.Key("difficulty");
	writer.Uint(stage_header->difficulty);

	char date[11] = "2000-01-01";
	memcpy(date+2, stage_header->date, 2);
	memcpy(date+5, &stage_header->date[3], 2);
	memcpy(date+8, &stage_header->date[6], 2);
	writer.Key("date");
	writer.String(date);

	writer.Key("name");
	if(stage_header->name[8] != '\0') stage_header->name[8] = '\0';
	writer.String(stage_header->name);

	writer.Key("stage");
	writer.StartArray();

	if(header->stage_offsets[9] == 0) {
		//	story mode
		for(int i = 0; i < 9; i++) {
			if(header->stage_offsets[i] != 0x00) {
				writer.StartObject();
				writer.Key("stage");
				writer.Uint(i + 1);

				th09_replay_stage_t *stage = (th09_replay_stage_t*)&buffer[header->stage_offsets[i]];
				th09_replay_stage_t *stage_ai = (th09_replay_stage_t*)&buffer[header->stage_offsets[i + 10]];

				writer.Key("player");
				writer.StartObject();

				writer.Key("score");
				writer.Uint64((uint64_t)stage->score * 10);

				writer.Key("shot");
				if(stage->shot < 16) {
					writer.String(shots[stage->shot]);
				} else {
					writer.String("Unknown");
				}

				writer.Key("lives");
				writer.Uint(stage->lives);
				writer.EndObject();

				writer.Key("cpu");
				writer.StartObject();

				writer.Key("score");
				writer.Uint64((uint64_t)stage_ai->score * 10);

				writer.Key("shot");
				if(stage_ai->shot < 16) {
					writer.String(shots[stage_ai->shot]);
				} else {
					writer.String("Unknown");
				}

				writer.Key("lives");
				writer.Uint(stage_ai->lives);

				writer.EndObject();
				writer.EndObject();
			}
		}
	} else {
		//	vs replay
		writer.Key("stage");
		writer.Int(1);

		th09_replay_stage_t *player1 = (th09_replay_stage_t*)&buffer[header->stage_offsets[9]];
		th09_replay_stage_t *player2 = (th09_replay_stage_t*)&buffer[header->stage_offsets[19]];
	
		writer.Key("player1");
		writer.StartObject();

		writer.Key("cpu");
		writer.Bool(player1->ai);

		writer.Key("shot");
		if(player1->shot < 16) {
			writer.String(shots[player1->shot]);
		} else {
			writer.String("Unknown");
		}

		writer.Key("score");
		writer.Uint64((uint64_t)player1->score * 10);
		writer.EndObject();

		writer.Key("player2");
		writer.StartObject();

		writer.Key("cpu");
		writer.Bool(player2->ai);

		writer.Key("shot");
		if(player2->shot < 16) {
			writer.String(shots[player2->shot]);
		} else {
			writer.String("Unknown");
		}

		writer.Key("score");
		writer.Uint64((uint64_t)player2->score * 10);
		writer.EndObject();
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

char * th10json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(5);

	th10_replay_header_t *header = (th10_replay_header_t*)buffer;
	uint32_t user_offset = header->comp_size;
	if(user_offset + 8 < flength) {
		uint32_t magic = *(uint32_t*)&buffer[user_offset];
		if(magic == 0x52455355) {
			uint32_t user_length = *(uint32_t*)&buffer[user_offset + 4];
			if(user_offset + user_length <= flength) {
				writer.Key("user");
				writer.StartObject();

				user_offset += 4;
				int l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				//SJIS, 東方XYZ リプレイファイル情報, Touhou XYZ replay file info
				// if(user_offset + l <= flength) {
				// 	buffer[user_offset + l] = '\0';
				// 	writer.Key("name");
				// 	writer.String((const char*)&buffer[user_offset], l);
				// }

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("shot");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					char *score = new char[l + 2];
					memcpy(score, &buffer[user_offset], l);
					score[l] = '0';
					score[l + 1] = '\0';
					writer.Key("score");
					writer.String(score);
					delete[] score;
				}

				writer.EndObject();
			}
		}
	}
	
	flength = th10decode(buf, flength);
	buffer = *buf;

	th10_replay_t *replay = (th10_replay_t*)buffer;

	const char *shots[] = {
		"ReimuA",
		"ReimuB",
		"ReimuC",
		"MarisaA",
		"MarisaB",
		"MarisaC"
	};

	writer.Key("name")	;
	writer.String(replay->name);

	writer.Key("timestamp");
	writer.Uint(replay->time);

	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", replay->slowdown);
	writer.String(val);

	writer.Key("score");
	writer.Uint64((uint64_t)replay->score * 10);

	writer.Key("shot");
	if(replay->shot < 6) {
		writer.String(shots[replay->shot]);
	} else {
		writer.String("Unknown %d", replay->shot);
	}

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);
	
	writer.Key("stage");
	writer.StartArray();
	
	uint32_t next_stage_offset = 0x64;
	for(unsigned int i = 0; i < replay->stagecount; i++) {
	    th10_replay_stage_t *stage = (th10_replay_stage_t*)&buffer[next_stage_offset];
	    
	    writer.StartObject();
	    writer.Key("stage");
	    writer.Uint(stage->stage);
	    
	    writer.Key("score");
	    writer.Uint((uint64_t)stage->score * 10);
	    
	    writer.Key("power");
		char power[5];
		snprintf(power, 5, "%4f", stage->power * 0.05);
		writer.String(power);
	    // writer.Uint(stage->power);
	    
	    writer.Key("piv");
	    writer.Uint(stage->piv);
	    
	    writer.Key("lives");
	    writer.Uint(stage->lives);
	    
	    next_stage_offset += stage->next_stage_offset + 0x1c4;
	    writer.EndObject();
	    
	}
	
	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;

}

char * th11json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(6);

	th11_replay_header_t *header = (th11_replay_header_t*)buffer;
	uint32_t user_offset = header->comp_size;
	if(user_offset + 8 < flength) {
		uint32_t magic = *(uint32_t*)&buffer[user_offset];
		if(magic == 0x52455355) {
			uint32_t user_length = *(uint32_t*)&buffer[user_offset + 4];
			if(user_offset + user_length <= flength) {
				writer.Key("user");
				writer.StartObject();

				user_offset += 4;
				int l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				//SJIS, 東方XYZ リプレイファイル情報, Touhou XYZ replay file info
				// if(user_offset + l <= flength) {
				// 	buffer[user_offset + l] = '\0';
				// 	writer.Key("name");
				// 	writer.String((const char*)&buffer[user_offset], l);
				// }

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("shot");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					char *score = new char[l + 2];
					memcpy(score, &buffer[user_offset], l);
					score[l] = '0';
					score[l + 1] = '\0';
					writer.Key("score");
					writer.String(score);
					delete[] score;
				}

				writer.EndObject();
			}
		}
	}

	flength = th11decode(buf, flength);
	buffer = *buf;

	th11_replay_t *replay = (th11_replay_t*)buffer;

	const char *shots[] = {
		"ReimuA",
		"ReimuB",
		"ReimuC",
		"MarisaA",
		"MarisaB",
		"MarisaC"
	};

	writer.Key("name")	;
	writer.String(replay->name);

	writer.Key("timestamp");
	writer.Uint(replay->time);

	// writer.Key("slowdown");
	// char val[6];
	// snprintf(val, 6, "%5f", replay->slowdown);
	// writer.String(val);

	writer.Key("score");
	writer.Uint64((uint64_t)replay->score * 10);

	writer.Key("shot");
	if(replay->shot < 6) {
		writer.String(shots[replay->shot]);
	} else {
		writer.String("Unknown %d", replay->shot);
	}

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);
	
	writer.Key("stage");
	writer.StartArray();
	
	uint32_t next_stage_offset = 0x70;
	for(unsigned int i = 0; i < replay->stagecount; i++) {
	    th11_replay_stage_t *stage = (th11_replay_stage_t*)&buffer[next_stage_offset];
	    
	    writer.StartObject();
	    writer.Key("stage");
	    writer.Uint(stage->stage);
	    
	    writer.Key("score");
	    writer.Uint((uint64_t)stage->score * 10);
	    
	    writer.Key("power");
		char power[5];
		snprintf(power, 5, "%4f", stage->power * 0.05);
		writer.String(power);
	    // writer.Uint(stage->power);
	    
	    writer.Key("piv");
	    writer.Uint(stage->piv);
	    
	    writer.Key("lives");
	    writer.Uint(stage->lives);

		writer.Key("life_pieces");
		writer.Uint(stage->life_pieces);

		writer.Key("graze");
		writer.Uint(stage->graze);
	    
	    next_stage_offset += stage->next_stage_offset + 0x90;
	    writer.EndObject();
	    
	}
	
	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}

char * th12json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(7);

	th12_replay_header_t *header = (th12_replay_header_t*)buffer;
	uint32_t user_offset = header->comp_size;
	if(user_offset + 8 < flength) {
		uint32_t magic = *(uint32_t*)&buffer[user_offset];
		if(magic == 0x52455355) {
			uint32_t user_length = *(uint32_t*)&buffer[user_offset + 4];
			if(user_offset + user_length <= flength) {
				writer.Key("user");
				writer.StartObject();

				user_offset += 4;
				int l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				//SJIS, 東方XYZ リプレイファイル情報, Touhou XYZ replay file info
				// if(user_offset + l <= flength) {
				// 	buffer[user_offset + l] = '\0';
				// 	writer.Key("name");
				// 	writer.String((const char*)&buffer[user_offset], l);
				// }

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("shot");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					char *score = new char[l + 2];
					memcpy(score, &buffer[user_offset], l);
					score[l] = '0';
					score[l + 1] = '\0';
					writer.Key("score");
					writer.String(score);
					delete[] score;
				}

				writer.EndObject();
			}
		}
	}

	flength = th12decode(buf, flength);
	buffer = *buf;

	th12_replay_t *replay = (th12_replay_t*)buffer;

	writer.Key("name")	;
	writer.String(replay->name);

	writer.Key("timestamp");
	writer.Uint(replay->time);

	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", replay->slowdown);
	writer.String(val);

	writer.Key("score");
	writer.Uint64((uint64_t)replay->score * 10);

	const char *shots[] = {
		"ReimuA",
		"ReimuB",
		"MarisaA",
		"MarisaB",
		"SanaeA",
		"SanaeB"
	};

	writer.Key("shot");
	if(replay->shot < 3 && replay->subshot < 2) {
		writer.String(shots[replay->shot * 2 + replay->subshot]);
	} else {
		writer.String("Unknown %d %d", replay->shot, replay->subshot);
	}

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);
	
	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0x70;
	for(unsigned int i = 0; i < replay->stagecount; i++) {
		th12_replay_stage_t *stage = (th12_replay_stage_t*)&buffer[next_stage_offset];

		writer.StartObject();
	    writer.Key("stage");
	    writer.Uint(stage->stage);
	    
	    writer.Key("score");
	    writer.Uint((uint64_t)stage->score * 10);
	    
	    writer.Key("power");
	    writer.Uint(stage->power);
	    
	    writer.Key("piv");
	    writer.Uint(stage->piv);
	    
	    writer.Key("lives");
	    writer.Uint(stage->lives);

		writer.Key("life_pieces");
		if(stage->life_pieces > 0) stage->life_pieces -= 1;
		writer.Uint(stage->life_pieces);

		writer.Key("bombs");
		writer.Uint(stage->bombs);

		writer.Key("bomb_pieces");
		writer.Uint(stage->bomb_pieces);

		writer.Key("ufos");
		writer.StartArray();
		writer.Uint(stage->ufo_1);
		writer.Uint(stage->ufo_2);
		writer.Uint(stage->ufo_3);
		writer.EndArray();

		writer.Key("graze");
		writer.Uint(stage->graze);
	    
	    next_stage_offset += stage->next_stage_offset + 0xa0;
	    writer.EndObject();
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}

char * th13json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(10);

	th13_replay_header_t *header = (th13_replay_header_t*)buffer;
	uint32_t user_offset = header->userdata_offset;
	if(user_offset + 8 < flength) {
		uint32_t magic = *(uint32_t*)&buffer[user_offset];
		if(magic == 0x52455355) {
			uint32_t user_length = *(uint32_t*)&buffer[user_offset + 4];
			if(user_offset + user_length <= flength) {
				writer.Key("user");
				writer.StartObject();

				user_offset += 4;
				int l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				//SJIS, 東方XYZ リプレイファイル情報, Touhou XYZ replay file info
				// if(user_offset + l <= flength) {
				// 	buffer[user_offset + l] = '\0';
				// 	writer.Key("name");
				// 	writer.String((const char*)&buffer[user_offset], l);
				// }

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("shot");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					char *score = new char[l + 2];
					memcpy(score, &buffer[user_offset], l);
					score[l] = '0';
					score[l + 1] = '\0';
					writer.Key("score");
					writer.String(score);
					delete[] score;
				}

				writer.EndObject();
			}
		}
	}

	flength = th13decode(buf, flength);
	buffer = *buf;

	th13_replay_t *replay = (th13_replay_t*)buffer;

	const char *shots[] = {
		"Reimu",
		"Marisa",
		"Sanae",
		"Youmu"
	};

	writer.Key("name");
	writer.String(replay->name);

	writer.Key("timestamp");
	writer.Uint64(replay->timestamp);

	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", replay->slowdown);
	writer.String(val);

	writer.Key("score");
	writer.Uint64((uint64_t)replay->score * 10);

	writer.Key("shot");
	if(replay->shot < 4) {
		writer.String(shots[replay->shot]);
	} else {
		writer.String("Unknown %d", replay->shot);
	}

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);

	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0x74;
	for(unsigned int i = 0; i < replay->stage_count; i++) {
		th13_replay_stage_t *stage = (th13_replay_stage_t*)&buffer[next_stage_offset];

		writer.StartObject();
	    writer.Key("stage");
	    writer.Uint(stage->stage_num);
	    
	    writer.Key("score");
	    writer.Uint((uint64_t)stage->score * 10);
	    
	    writer.Key("power");
		// char power[5];
		// snprintf(power, 5, "%4f", stage->power * 0.05);
		// writer.String(power);
	    writer.Uint(stage->power);
	    
	    writer.Key("piv");
	    writer.Uint(stage->piv);
	    
	    writer.Key("lives");
	    writer.Uint(stage->lives);

		writer.Key("life_pieces");
		writer.Uint(stage->life_pieces);

		writer.Key("bombs");
		writer.Uint(stage->bomb_pieces);

		writer.Key("graze");
		writer.Uint(stage->graze);

		writer.Key("trance");
		writer.Uint(stage->trance_gauge);
	    
	    next_stage_offset += stage->end_off + 0xc4;
	    writer.EndObject();
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}

char * th14json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(11);

	th13_replay_header_t *header = (th13_replay_header_t*)buffer;
	uint32_t user_offset = header->userdata_offset;
	if(user_offset + 8 < flength) {
		uint32_t magic = *(uint32_t*)&buffer[user_offset];
		if(magic == 0x52455355) {
			uint32_t user_length = *(uint32_t*)&buffer[user_offset + 4];
			if(user_offset + user_length <= flength) {
				writer.Key("user");
				writer.StartObject();

				user_offset += 4;
				int l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				//SJIS, 東方XYZ リプレイファイル情報, Touhou XYZ replay file info
				// if(user_offset + l <= flength) {
				// 	buffer[user_offset + l] = '\0';
				// 	writer.Key("name");
				// 	writer.String((const char*)&buffer[user_offset], l);
				// }

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("version");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("name");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("date");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("shot");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 7 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("difficulty");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 2 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					buffer[user_offset + l] = '\0';
					writer.Key("stage");
					writer.String((const char*)&buffer[user_offset], l);
				}

				user_offset += 8 + l;
				l = 0;

				for(uint16_t crlf = *(uint16_t*)&buffer[user_offset + l]; crlf!=0x0a0d && user_offset + l <= flength;crlf = *(uint16_t*)&buffer[user_offset + ++l]);
				if(user_offset + l <= flength) {
					char *score = new char[l + 2];
					memcpy(score, &buffer[user_offset], l);
					score[l] = '0';
					score[l + 1] = '\0';
					writer.Key("score");
					writer.String(score);
					delete[] score;
				}

				writer.EndObject();
			}
		}
	}

	flength = th13decode(buf, flength);
	buffer = *buf;

	th14_replay_t *replay = (th14_replay_t*)buffer;

	const char *shots[] = {
		"ReimuA",
		"ReimuB",
		"MarisaA",
		"MarisaB",
		"SakuyaA",
		"SakuyaB"
	};

	writer.Key("name");
	writer.String(replay->name);

	writer.Key("timestamp");
	writer.Uint64(replay->timestamp);

	writer.Key("slowdown");
	char val[6];
	snprintf(val, 6, "%5f", replay->slowdown);
	writer.String(val);

	writer.Key("score");
	writer.Uint64((uint64_t)replay->score * 10);

	writer.Key("shot");
	if(replay->shot < 4) {
		writer.String(shots[replay->shot]);
	} else {
		writer.String("Unknown %d", replay->shot);
	}

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);

	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0x94;
	for(unsigned int i = 0; i < replay->stage_count; i++) {
		th13_replay_stage_t *stage = (th13_replay_stage_t*)&buffer[next_stage_offset];

		writer.StartObject();
	    writer.Key("stage");
	    writer.Uint(stage->stage_num);
	    
	    writer.Key("score");
	    writer.Uint((uint64_t)stage->score * 10);
	    
	    writer.Key("power");
		// char power[5];
		// snprintf(power, 5, "%4f", stage->power * 0.05);
		// writer.String(power);
	    writer.Uint(stage->power);
	    
	    writer.Key("piv");
	    writer.Uint(stage->piv);
	    
	    writer.Key("lives");
	    writer.Uint(stage->lives);

		writer.Key("life_pieces");
		writer.Uint(stage->life_pieces);

		writer.Key("bombs");
		writer.Uint(stage->bomb_pieces);

		writer.Key("graze");
		writer.Uint(stage->graze);
	    
	    next_stage_offset += stage->end_off + 0xdc;
	    writer.EndObject();
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}