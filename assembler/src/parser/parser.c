#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "utils.h"

void consume(token_array *tarr, size_t *index, token_type type)
{
	token_type t = tarr->array[*index + 1].type;
	char *text = (char *) tarr->array[*index + 1].text;
	switch(t)
	{
		case ZERO:
		case RA:
		case SP:
		case GP:
		case TP:
		case T0:
		case T1:
		case T2:
		case S0:
		case FP:
		case S1:
		case A0:
		case A1:
		case A2:
		case A3:
		case A4:
		case A5:
		case A6:
		case A7:
		case S2:
		case S3:
		case S4:
		case S5:
		case S6:
		case S7:
		case S8:
		case S9:
		case S10:
		case S11:
		case T3:
		case T4:
		case T5:
		case T6:
		case FT0:
		case FT1:
		case FT2:
		case FT3:
		case FT4:
		case FT5:
		case FT6:
		case FT7:
		case FS0:
		case FS1:
		case FA0:
		case FA1:
		case FA2:
		case FA3:
		case FA4:
		case FA5:
		case FA6:
		case FA7:
		case FS2:
		case FS3:
		case FS4:
		case FS5:
		case FS6:
		case FS7:
		case FS8:
		case FS9:
		case FS10:
		case FS11:
		case FT8:
		case FT9:
		case FT10:
		case FT11:
			t = REGISTER;
			break;
		case RELLBJMP:
			t = IDENTIFIER;
			break;
		case IDENTIFIER:
			if(is_immediate(text)) tarr->array[*index + 1].type = OFFSET;
			break;
		default:
			break;
	}
	if(*index + 1 >= tarr->size)
	{
		fprintf(stderr, "[ERROR] expected token of type %d, reached EOF instead", type);
		exit(1);
	}
	if(t != type)
	{
		fprintf(stderr, "[ERROR] expected token of type %d, got token of type %d\n", type, t);
		exit(1);
	}
	*index = *index + 1;
}

int is_immediate(char *text)
{
	int ans = 1;
	size_t n = strlen(text);
	for(size_t i = 0; i < n; i++)
	{
		if((text[i] < '0' || text[i] > '9') && !(i == 0 && text[i] == '-'))
		{
			ans = 0;
			break;
		}
	}
	if(text[0] == '0' && n > 1) ans = 0;
	return ans;
}

void assert_immediate(token *t)
{
	if(!is_immediate((char *) t->text))
	{
		fprintf(stderr, "[ERROR] immediate expected, got %s instead", t->text);
		exit(1);
	}
	t->type = IMMEDIATE;
}

void parse(token_array *tarr)
{
	for(size_t i = 0; i < tarr->size; i++)
	{
		token_type type = tarr->array[i].type;
		switch(type)
		{
			case LI:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				assert_immediate(&tarr->array[i + 1]);
				consume(tarr, &i, IMMEDIATE);
				break;
			case ADD:
			case SUB:
			case FADDS:
			case FADDD:
			case FSUBD:
			case FLTS:
			case FGTS:
			case FMULS:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, REGISTER);
				break;
			case LB:
			case LD:
			case SD:
			case SB:
			case LW:
			case FLD:
			case FSW:
			case FLW:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, IDENTIFIER);
				consume(tarr, &i, LPAREN);
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, RPAREN);
				break;
			case BEQZ:
			case BNEZ:
			case LA:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, IDENTIFIER);
				break;
			case ADDI:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				assert_immediate(&tarr->array[i + 1]);
				consume(tarr, &i, IMMEDIATE);
				break;
			case J:
				consume(tarr, &i, IDENTIFIER);
				break;
			case MV:
			case FMVS:
			case FMVSX:
			case FSQRTD:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, REGISTER);
				break;
			case BGE:
			case BLE:
			case BGT:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, IDENTIFIER);
				break;
			case CALL:
				consume(tarr, &i, IDENTIFIER);
				break;
			case SRAI:
			case SLLI:
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				consume(tarr, &i, REGISTER);
				consume(tarr, &i, COMA);
				assert_immediate(&tarr->array[i + 1]);
				consume(tarr, &i, IMMEDIATE);
				break;
			default:
				break;
		}
	}	
}

