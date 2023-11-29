#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdint.h>
#define HM_CAPACITY 65536

typedef struct kvpair_t
{
	const char *key;
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

void hm_set(hashmap_t *hm, const char *key, int value);

int hm_get(hashmap_t *hm, const char *key, int *buffer);

#endif