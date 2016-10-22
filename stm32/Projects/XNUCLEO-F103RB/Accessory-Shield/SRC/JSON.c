#include "JSON.h"

uint16_t get_json(char *dest, char *str)
{
	char *p = strchr(str,'{');
	char *q = strrchr(str,'}');

	strncpy(dest,p,q-p+1);
	dest[q-p+1] = '\0';

	return (strchr(dest,':')) ? 1:0;
}

uint16_t parse_json(char *str,JsonPair *jsonPair)
{
	char *p;
	char *q;
	uint16_t length;

	p = strchr(str,'\"');
	if (!p) return 0;
	q = strchr(p+1,'\"');
	if (!q) return 0;

	length = q-p-1;
	if(!length) return 0;
	
	jsonPair->keyLength = length;
	strncpy(jsonPair->key, p+1, jsonPair->keyLength);
	jsonPair->key[jsonPair->keyLength] = '\0';

	p = strchr(q+1,'\"');
	if (!p) return 0;
	q = strchr(p+1,'\"');
	if (!q) return 0;

	length = q-p-1;
	if(!length) return 0;

	jsonPair->valueLength = length;
	strncpy(jsonPair->value, p+1, jsonPair->valueLength);
	jsonPair->value[jsonPair->valueLength] = '\0';

	return 1;
}

void printf_json(JsonPair *jsonPair)
{
	USART3_printf("key:%s,value:%s\r\n",jsonPair->key,jsonPair->value);
}

RGB parse_rgb(char *str)
{
	RGB rgb;
	char temp[5];
	int length;
	char *p;
	char *q;

	//R
	p = str;
	q = strchr(str,',');
	length = q-p;
	strncpy(temp, p, length);
	temp[length] = 0x00;
	rgb.R = atoi(temp);

	//G
	p = q+1;
	q = strchr(p,',');
	length = q-p;
	strncpy(temp, p, length);
	temp[length] = 0x00;
	rgb.G = atoi(temp);

	//B
	p = q+1;
	q = strchr(p,0x00);
	length = q-p;
	strncpy(temp, p, length);
	temp[length] = 0x00;
	rgb.B = atoi(temp);

	return rgb;
}


void exec_rgb(JsonPair *jsonPair)
{
	uint32_t value;
	if (!strcmp(jsonPair->key,"Forward"))
	{
		if (HexToInt(jsonPair,&value))
		{
			switch (value)
			{
//				case 0:cout << "keyup" << endl;break;
//				case 1:cout << "key1"  << endl;break;
//				case 2:cout << "key2"  << endl;break;
//				case 3:cout << "key3"  << endl;break;
//				case 4:cout << "key4"  << endl;break;
//				case 5:cout << "key5"  << endl;break;
//				
//				default:cout << "error"  << endl;break;
			}
		}
	}
}



//void exec_bz(JsonPair *jsonPair)
//{
//	if (!strcmp(jsonPair->key,"BZ"))
//	{
//		if (!strcmp(jsonPair->value,"on"))
//		{
////			cout << "BZ on" << endl;
//			return;
//		}
//		else if (!strcmp(jsonPair->value,"ON"))
//		{
////			cout << "BZ ON" << endl;
//			return;
//		}

//		if (!strcmp(jsonPair->value,"off"))
//		{
////			cout << "BZ off" << endl;
//			return;
//		}
//		else if (!strcmp(jsonPair->value,"OFF"))
//		{
////			cout << "BZ OFF" << endl;
//			return;
//		}
//	}
//}

uint32_t HexToInt(JsonPair *jsonPair,uint32_t *value)
{
//	char* str;
	uint16_t strLength = strlen(jsonPair->value); 
	if(strLength > 2)
	{
		if ((jsonPair->value[0] == '0') && (jsonPair->value[1] == 'x' || jsonPair->value[1] == 'X'))
		{
//			*value = strtol(jsonPair->value,&str,16);
			return 1;
		}
		return 0;
	}
	return 0;
}
