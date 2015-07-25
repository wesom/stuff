#include <stdlib.h>
/**
 *  * Note: The returned array must be malloced, assume caller calls free().
 *   */
struct binding{
    int key;
    int value;
    struct binding *link;
};
 
struct hashmap{
    int size;
    struct binding **buckets;
};
 
static struct hashmap *map_new(int hint) {
    int i, size;
    size = hint;
    struct hashmap *m = malloc(sizeof(*m) + size*sizeof(m->buckets[0]));
    m->size = size;
    m->buckets = (struct binding **)(m + 1);
    for (i = 0; i < m->size; i++)
	m->buckets[i] = 0;
    return m;
}
 
static int map_get(struct hashmap *m, int key) {
    int i;
    struct binding *p;
    i = key & (m->size - 1);
    for (p = m->buckets[i]; p; p = p->link) {
	if (p->key == key)
	break;
    }
    return p ? p->value : -1;
}
 
static void map_put(struct hashmap *m, int key, int value) {
    int i;
    struct binding *p;
    i = key & (m->size - 1);
    for (p = m->buckets[i]; p; p = p->link) {
	if (p->key == key)
	    break;
    }
    if (p == NULL) {
	p = malloc(sizeof(*p));
	p->key = key;
	p->link = m->buckets[i];
	m->buckets[i] = p;
    }
    p->value = value;
}

static void map_free(struct hashmap *m) {
    int i;
    struct binding *p, *q;
    for (i = 0; i < m->size; i++) {
	for (p = m->buckets[i]; p; p = q) {
	    q = p->link;
	    free(p);
	}
    }
    free(m);
}

int* twoSum(int* nums, int numsSize, int target) {
    int i;
    struct hashmap *m = map_new(numsSize);
    for (i = 0; i < numsSize; i++) {
        int another = target - nums[i];
        int index = map_get(m, another);
        if (index == -1) {
            map_put(m, nums[i], i);
        } else {
            int *result = malloc(sizeof(int)*2);
            result[0] = index + 1;
            result[1] = i + 1;
            map_free(m);
            return result;
        }
    }
    map_free(m);
    return NULL;
}

//----------------------- for lua ------------------

#include <lauxlib.h>

int ltwosum(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int target = luaL_checkint(L,2);
	int numsSize = lua_rawlen(L, 1);
	int *nums = malloc(sizeof(int)*numsSize);
	int i;
	for (i = 0; i < numsSize; i++) {
		lua_rawgeti(L, 1, i+1);
		nums[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}
	int *result = twoSum(nums, numsSize, target);
	if (result) {
		lua_pushinteger(L, result[0]);
		lua_pushinteger(L, result[1]);
		free(result);
		free(nums);
		return 2;
	}
	free(nums);
	return 0;
}

int luaopen_twosum_core(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"twosum", ltwosum},
		{NULL, NULL},
	};
	luaL_newlib(L, l);
	return 1;
}
