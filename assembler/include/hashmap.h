#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdint.h>
#define HM_CAPACITY 65521

typedef struct kvpair_t
{
	char *key;
	int value;
} kvpair_t;

typedef struct hmentry_t
{
	kvpair_t *data;
	uint16_t size;
} hmentry_t;

typedef struct hashmap_t
{
	hmentry_t entries[HM_CAPACITY];
} hashmap_t;

hashmap_t init_hashmap(kvpair_t *pairs, uint32_t size);

hashmap_t init_blank_hm();

void hm_set(hashmap_t *hm, char *key, int value);

int hm_get(hashmap_t *hm, char *key, int *buffer);

void hm_clear(hashmap_t *hm);

#endif
