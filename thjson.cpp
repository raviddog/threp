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
#include <string>
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

	const char *out = "not supported format";

	switch(magic) {
		case 0x50523654:  //"T6RP"
			out = th06json(buf, flength);
			break;
		case 0x50523754:  //"T7RP"
			out = th07json(buf, flength);
			break;
		case 0x50523854:  //"T8RP"
			out = th08json(buf, flength);
			break;
		case 0x50523954: 	//"T9RP"
			out = th09json(buf, flength);
			break;
		case 0x72303174:  //"t10r"
			out = th10json(buf, flength);
			break;
		case 0x72313174:  //"t11r"
			out = th11json(buf, flength);
			break;
		case 0x72323174:  //"t12r"
			out = th12json(buf, flength);
			break;
		case 0x35323174:  //"t125"
			break;
		case 0x72383231:  //"128r"
			break;
		case 0x72333174:  //"t13r"
			{
				if(flength > sizeof(th13_replay_header_t)) {
					th13_replay_header_t *header = (th13_replay_header_t*)buffer;
					if(flength > header->userdata_offset + 0x10) {
						unsigned char id = buffer[header->userdata_offset + 0x10];
						if(id == 144) {
							out = th13json(buf, flength);
						} else {
							out = th14json(buf, flength);
						}
					} else {
						out = nullptr;
					}
				} else {
					out = nullptr;
				}
			}
			break;
		case 0x72353174: 	//"t15r"
			out = th15json(buf, flength);
			break;
		case 0x72363174: 	//"t16r"
			out = th16json(buf, flength);
			break;
		case 0x72373174: 	//"t17r"
			break;
		case 0x72383174: 	//"t18r"
			break;
		default:
			break;
	}

	if(out) {
		printf("%s\n", out);
		delete[] out;
	} else {
		printf("error reading file\n");
	}

	
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

	if(flength < sizeof(th06_replay_header_t)) return nullptr;
	th06_replay_header_t *header = (th06_replay_header_t *)buffer;

	//	version
	writer.Key("version");
	char ver[5] = "    ";
	snprintf(ver, 5, "%.2hhx%.2hhx", header->version[0], header->version[1]);
	writer.String(ver);

	writer.Key("shot");
	writer.Uint(header->shot);

	writer.Key("difficulty");
	writer.Uint(header->difficulty);

	//	now decode the replay
	flength = th06decode(buf, flength);
	buffer = *buf;

	if(flength < sizeof(th06_replay_t)) return nullptr;
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
		if(rep->stage_offsets[i] != 0x00 && rep->stage_offsets[i] + sizeof(th06_replay_stage_t) < flength) {
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

	if(flength < sizeof(th07_replay_header_t)) return nullptr;
	th07_replay_header_t *header = (th07_replay_header_t*)buffer;

	writer.Key("version");
	char ver[5] = "    ";
	snprintf(ver, 5, "%.2hhx%.2hhx", header->version[0], header->version[1]);
	writer.String(ver);

	flength = th07decode1(buf, flength);
	buffer = *buf;

	//	need to save the stage offsets from header, so make a copy of the header memory
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

	if(flength < sizeof(th07_replay_t)) {
		free(header);
		return nullptr;
	}
	th07_replay_t *rep = (th07_replay_t*)buffer;
	
	writer.Key("shot");
	writer.Uint(rep->shot);

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
		if(header->stage_offsets[i] != 0x00 && header->stage_offsets[i] + sizeof(th07_replay_stage_t) < flength) {
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
	
	if(flength < sizeof(th08_replay_header_t)) return nullptr;
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
	
	if(flength < sizeof(th08_replay_t)) {
		free(header);
		return nullptr;
	}
	th08_replay_t *stage_header = (th08_replay_t*)buffer;

	writer.Key("shot");
	writer.Uint(stage_header->shot);

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
		if(header->stage_offsets[i] != 0x00 && header->stage_offsets[i] + sizeof(th08_replay_stage_t) < flength) {
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

	if(flength < sizeof(th09_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th09_replay_t)) {
		free(header);
		return nullptr;
	}
	th09_replay_t *stage_header = (th09_replay_t*)buffer;

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
			if(header->stage_offsets[i] != 0x00 && header->stage_offsets[i] + sizeof(th09_replay_stage_t) < flength && header->stage_offsets[i + 10] + sizeof(th09_replay_stage_t) < flength) {
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
				writer.Uint(stage->shot);

				writer.Key("lives");
				writer.Uint(stage->lives);
				writer.EndObject();

				writer.Key("cpu");
				writer.StartObject();

				writer.Key("score");
				writer.Uint64((uint64_t)stage_ai->score * 10);

				writer.Key("shot");
				writer.Uint(stage_ai->shot);

				writer.Key("lives");
				writer.Uint(stage_ai->lives);

				writer.EndObject();
				writer.EndObject();
			}
		}
	} else {
		//	vs replay
		if(header->stage_offsets[9] + sizeof(th09_replay_stage_t) < flength && header->stage_offsets[19] + sizeof(th09_replay_stage_t) < flength) {
			writer.Key("stage");
			writer.Int(0);

			th09_replay_stage_t *player1 = (th09_replay_stage_t*)&buffer[header->stage_offsets[9]];
			th09_replay_stage_t *player2 = (th09_replay_stage_t*)&buffer[header->stage_offsets[19]];
		
			writer.Key("player1");
			writer.StartObject();

			writer.Key("cpu");
			writer.Bool(player1->ai);

			writer.Key("shot");
			writer.Uint(player1->shot);

			writer.Key("score");
			writer.Uint64((uint64_t)player1->score * 10);
			writer.EndObject();

			writer.Key("player2");
			writer.StartObject();

			writer.Key("cpu");
			writer.Bool(player2->ai);

			writer.Key("shot");
			writer.Uint(player2->shot);

			writer.Key("score");
			writer.Uint64((uint64_t)player2->score * 10);
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

char * th10json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(5);

	if(flength < sizeof(th10_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th10_replay_t)) return nullptr;
	th10_replay_t *replay = (th10_replay_t*)buffer;

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
	writer.Uint(replay->shot);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);
	
	writer.Key("stage");
	writer.StartArray();
	
	uint32_t next_stage_offset = 0x64;
	for(unsigned int i = 0; i < replay->stagecount; i++) {
		if(next_stage_offset + sizeof(th10_replay_stage_t) < flength) {
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

		} else {
			//	buffer overflow i guess, just stop
			i = replay->stagecount;
		}	    
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

	if(flength < sizeof(th11_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th11_replay_t)) return nullptr;
	th11_replay_t *replay = (th11_replay_t*)buffer;

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
	writer.Uint(replay->shot);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);
	
	writer.Key("stage");
	writer.StartArray();
	
	uint32_t next_stage_offset = 0x70;
	for(unsigned int i = 0; i < replay->stagecount; i++) {
		if(next_stage_offset + sizeof(th11_replay_stage_t) < flength) {
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
		} else {
			i = replay->stagecount;
		}	    
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

	if(flength < sizeof(th12_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th12_replay_t)) return nullptr;
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

	writer.Key("shot");
	writer.Uint(replay->shot);

	writer.Key("subshot");
	writer.Uint(replay->subshot);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);
	
	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0x70;
	for(unsigned int i = 0; i < replay->stagecount; i++) {
		if(next_stage_offset + sizeof(th128_replay_stage_t) < flength) {
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
		} else {
			i = replay->stagecount;
		}
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

	if(flength < sizeof(th13_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th13_replay_t)) return nullptr;
	th13_replay_t *replay = (th13_replay_t*)buffer;

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
	writer.Uint(replay->shot);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);

	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0x74;
	for(unsigned int i = 0; i < replay->stage_count; i++) {
		if(next_stage_offset + sizeof(th13_replay_stage_t) < flength) {
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
		} else {
			i = replay->stage_count;
		}
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

	if(flength < sizeof(th13_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th14_replay_t)) return nullptr;
	th14_replay_t *replay = (th14_replay_t*)buffer;

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
	writer.Uint(replay->shot);
	
	writer.Key("subshot");
	writer.Uint(replay->subshot);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);

	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0x94;
	for(unsigned int i = 0; i < replay->stage_count; i++) {
		if(next_stage_offset + sizeof(th14_replay_stage_t) < flength) {
			th14_replay_stage_t *stage = (th14_replay_stage_t*)&buffer[next_stage_offset];

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
			writer.Uint(stage->bombs);
			
			writer.Key("bomb_pieces");
			writer.Uint(stage->bomb_pieces);

			writer.Key("poc_count");
			writer.Uint(stage->poc_count);

			writer.Key("graze");
			writer.Uint(stage->graze);
			
			next_stage_offset += stage->end_off + 0xdc;
			writer.EndObject();
		} else {
			i = replay->stage_count;
		}
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}

char * th15json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(13);

	if(flength < sizeof(th13_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th15_replay_t)) return nullptr;
	th15_replay_t *replay = (th15_replay_t*)buffer;

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
	writer.Uint(replay->shot);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);

	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0xa4;
	for(unsigned int i = 0; i < replay->stage_count; i++) {
		if(next_stage_offset + sizeof(th15_replay_stage_t) < flength) {
			th15_replay_stage_t *stage = (th15_replay_stage_t*)&buffer[next_stage_offset];

			writer.StartObject();
			writer.Key("stage");
			writer.Uint(stage->stage_num);
			
			writer.Key("score");
			writer.Uint((uint64_t)stage->score * 10);

			writer.Key("misses");
			writer.Uint(stage->miss_count);

			writer.Key("point_items");
			writer.Uint(stage->point_items_collected);
			
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
			
			next_stage_offset += stage->end_off + 0x238;
			writer.EndObject();
		} else {
			i = replay->stage_count;
		}
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}

char * th16json(unsigned char **buf, unsigned int flength) {
	using namespace rapidjson;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	unsigned char *buffer = *buf;

	writer.StartObject();
	writer.Key("gameid");
	writer.Int(11);

	if(flength < sizeof(th13_replay_header_t)) return nullptr;
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

	if(flength < sizeof(th16_replay_t)) return nullptr;
	th16_replay_t *replay = (th16_replay_t*)buffer;

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
	writer.Uint(replay->shot);

	writer.Key("subshot");
	writer.Uint(replay->subseason);

	writer.Key("cleared");
	writer.Uint(replay->cleared);

	writer.Key("difficulty");
	writer.Uint(replay->difficulty);

	writer.Key("stage");
	writer.StartArray();

	uint32_t next_stage_offset = 0xa0;
	for(unsigned int i = 0; i < replay->stage_count; i++) {
		if(next_stage_offset + sizeof(th16_replay_stage_t) < flength) {
			th16_replay_stage_t *stage = (th16_replay_stage_t*)&buffer[next_stage_offset];

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

			writer.Key("season");
			writer.Uint(stage->season_power);

			writer.Key("season_max");
			writer.Uint(stage->season_power_max);

			writer.Key("misses");
			writer.Uint(stage->miss_count);

			writer.Key("point_items");
			writer.Uint(stage->point_items_collected);

			writer.Key("extend");
			writer.Uint64((uint64_t)stage->next_score_extend_idx * 10);
			
			writer.Key("lives");
			writer.Uint(stage->lives);

			writer.Key("bombs");
			writer.Uint(stage->bombs);
			
			writer.Key("bomb_pieces");
			writer.Uint(stage->bomb_pieces);

			writer.Key("graze");
			writer.Uint(stage->graze);
			
			next_stage_offset += stage->end_off + 0x294;
			writer.EndObject();
		} else {
			i = replay->stage_count;
		}
	}

	writer.EndArray();
	writer.EndObject();

	int jsonsize = s.GetSize();
	char *json = new char[jsonsize+1];
	memcpy(json, s.GetString(), jsonsize+1);
	json[jsonsize] = '\0';
	return json;
}