#include <stdlib.h>

struct binding {
    int key;
    void *value;
    struct binding *link;
};

struct hashmap {
    int size;
    int length;
    struct binding **buckets;
};

struct hashmap *map_new(int hinit) {
    int i, size;
    size = hinit;
    struct hashmap *m = malloc(sizeof(*m) + size*sizeof(m->buckets[0]));
    m->size = size;
    m->length = 0;
    m->buckets = (struct binding **)(m + 1);
    for(i=0; i<size; i++)
        m->buckets[i] = 0;
    return m;
}

void *map_get(struct hashmap *map, int key) {
    int i;
    struct binding *p;
    i = key & (map->size - 1);
    for(p=map->buckets[i]; p; p=p->link) {
        if (p->key == key)
            break;
    }
    return p? p->value : NULL;
}

void map_put(struct hashmap *map, int key, void *value) {
    int i;
    struct binding *p;
    i = key & (map->size - 1);
    for(p=map->buckets[i]; p; p=p->link) {
        if (p->key == key)
            break;
    }
    if (p == NULL) {
        p = malloc(sizeof(*p));
        p->key = key;
        p->link = map->buckets[i];
        map->buckets[i] = p;
        map->length++;
    }
    p->value = value;
}

void *map_remove(struct hashmap *map, int key) {
    int i;
    struct binding ** pp;
    i = key & (map->size - 1);
    for (pp = &map->buckets[i]; *pp; pp = &(*pp)->link) {
        if ((*pp)->key == key) {
            struct binding *p = *pp;
            void *value = p->value;
            *pp = p->link;
            free(p);
            map->length--;
            return value;
        }
    }
    return NULL;
}

int map_length(struct hashmap *map) {
    return map->length;
}

void map_free(struct hashmap *map) {
    int i;
    struct binding *p, *q;
    for(i=0; i<map->size; i++) {
        for (p=map->buckets[i]; p; p=q) {
            q = p->link;
            free(p);
        }
    }
    free(map);
}

/* Doubly Linked List */
struct dlnode {
    struct dlnode *prev;
    struct dlnode *next;
    int key;
    int value;
};

struct dllist {
    struct dlnode *first;
    struct dlnode *last;
};

void remove_node(struct dllist *l, struct dlnode *p) {
    if (p->prev == NULL)
        l->first = p->next;
    else
        p->prev->next = p->next;
    if (p->next == NULL)
        l->last = p->prev;
    else
        p->next->prev = p->prev;
}

void insert_before(struct dllist *l, struct dlnode *node, struct dlnode *p) {
    p->prev = node->prev;
    p->next = node;
    if (node->prev == NULL)
        l->first = p;
    else
        node->prev->next = p;
    node->prev = p;
}

void insert_beginning(struct dllist *l, struct dlnode *p) {
    if (l->first == NULL) {
        l->first = p;
        l->last = p;
        p->prev = NULL;
        p->next = NULL;
    }
    else
        insert_before(l, l->first, p);
}

struct dllist *dllist_new() {
    struct dllist *l;
    l = malloc(sizeof(*l));
    l->first = NULL;
    l->last = NULL;
    return l;
}

void dllist_free(struct dllist *l) {
    struct dlnode *p, *q;
    p = l->last;
    while (p) {
        q = p;
        p = p->next;
        free(q);
    }
}

struct lru {
    struct hashmap *map;
    struct dllist *dll;
    int max;
};

static struct lru *cache;

void lruCacheInit(int capacity) {
    cache = malloc(sizeof(*cache));
    cache->map = map_new(capacity);
    cache->dll = dllist_new();
    cache->max = capacity;
}

void lruCacheFree() {
    map_free(cache->map);
    dllist_free(cache->dll);
    free(cache);
}

int lruCacheGet(int key) {
    struct dlnode *p;
    p = (struct dlnode*)map_get(cache->map, key);
    if (p) {
        remove_node(cache->dll, p);
        insert_beginning(cache->dll, p);
        return p->value;
    }
    return -1;
}
    
void lruCacheSet(int key, int value) {
    struct dlnode *p;
    p = (struct dlnode *)map_get(cache->map, key);
    if (p) {
        remove_node(cache->dll, p);
        insert_beginning(cache->dll, p);
        p->value = value;
        p->key = key;
    }
    else {
        p = malloc(sizeof(*p));
        p->key = key;
        p->value = value;
        if (map_length(cache->map) >= cache->max) {
            struct dlnode *last = cache->dll->last;
            int remove_key = last->key;
            remove_node(cache->dll, cache->dll->last);
            map_remove(cache->map, remove_key);
        }
        map_put(cache->map, key, p);
        insert_beginning(cache->dll, p);
    }
}

#include <lauxlib.h>

int luaopen_llru_core(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"init", lruCacheInit},
		{NULL, NULL},
	};
	luaL_newlib(L, l);
	return 1;
}
