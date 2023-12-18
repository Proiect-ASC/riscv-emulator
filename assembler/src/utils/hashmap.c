#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint16_t hash(char *str)
{
	uint32_t hash_value = 0;
	for(uint32_t i = 0; i < strlen(str); i++)
	{
		hash_value += 47 * str[i];
		hash_value %= HM_CAPACITY;
	}
	return (uint16_t) hash_value % HM_CAPACITY;

}

hashmap_t init_hashmap(kvpair_t *pairs, uint32_t size)
{
	hashmap_t hm;
	memset(&hm, 0, sizeof(hm));
	for(uint32_t i = 0; i < size; i++)
		hm_set(&hm, pairs[i].key, pairs[i].value);
	return hm;
}

hashmap_t init_blank_hm()
{
	hashmap_t hm;
	memset(&hm, 0, sizeof(hm));
	return hm;
}

void hm_set(hashmap_t *hm, char *key, int value)
{
	uint16_t hash_value = hash(key);
	hmentry_t *entry = &hm->entries[hash_value];
	for(uint16_t i = 0; i < entry->size; i++)
	{
		if(strcmp(entry->data[i].key, key) == 0)
		{
			entry->data[i].value = value;
			return;
		}
	}
	entry->data = realloc(entry->data, ++entry->size * sizeof(kvpair_t));
	entry->data[entry->size - 1].key = malloc(strlen(key) + 1);
	strcpy(entry->data[entry->size - 1].key, key);
	entry->data[entry->size - 1].value = value;
}

int hm_get(hashmap_t *hm, char *key, int *buffer)
{
	uint16_t hash_value = hash(key);
	hmentry_t *entry = &hm->entries[hash_value];
	for(uint16_t i = 0; i < entry->size; i++)
	{
		if(strcmp(entry->data[i].key, key) == 0)
		{
			if(buffer != NULL) *buffer = entry->data[i].value;
			return 0;
		}
	}
	if(buffer != NULL) *buffer = 0;
	return -1;
}

void hm_clear(hashmap_t *hm)
{
	for(int i = 0; i < HM_CAPACITY; i++)
	{
		hmentry_t *entry = &hm->entries[i];
		for(int j = 0; j < entry->size; j++)
		{
			free(entry->data[j].key);
			entry->data[j].key = NULL;
		}
		if(entry->size > 0) free(entry->data);
		entry->data = NULL;
	}
}
