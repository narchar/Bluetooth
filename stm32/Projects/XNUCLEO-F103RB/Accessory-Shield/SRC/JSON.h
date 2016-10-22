#ifndef _JSON_H
#define _JSON_H

#include "LIB_Config.h"
#include <string.h>
#include <stdlib.h>

typedef struct _JsonPair
{
	char key[100];
	char value[100];
	int keyLength;
	int valueLength;

}JsonPair;

typedef struct _RGB
{
	int R;
	int G;
	int B;
}RGB;


uint16_t get_json(char *dest, char *str);
uint16_t parse_json(char *str,JsonPair *jsonPair);
void printf_json(JsonPair *jsonPair);
RGB parse_rgb(char *str);


void exec_joy(JsonPair *jsonPair);
uint32_t HexToInt(JsonPair *jsonPair,uint32_t *value);
void exec_bz(JsonPair *jsonPair);

#endif /*_JSON_H*/

