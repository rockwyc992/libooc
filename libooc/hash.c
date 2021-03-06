#include <stdlib.h>
#include <stdio.h>

#include "hash.struct.h"

#define DEFAULT_SIZE 5

enum ACTION {
    Set,
    Get
};

def_class(Hash, Object)

override
def(ctor, void : va_list * @args_ptr) {
    self->size = DEFAULT_SIZE;
    self->filled = 0;
    self->entries = malloc(DEFAULT_SIZE * (sizeof(struct Hash)));
}

override
def(dtor, void) {
    free(self->entries);
    free(self);
}

def(set, bool : void * @key . void * @data) {
    return search(self, key, data, NULL, Set);
}

def(get, void * : void * @key) {
    void * result;
    if(search(self, key, NULL, &result, Get)) {
        return result;
    }
    return NULL;
}

def(each, void : void (* @iter)(void * key, void * data)) {
    struct HashEntry * entries = self->entries;
    for(size_t i = 0, size = self->size; i < size; i++) {
        struct HashEntry entry = entries[i];
        if(entry.used) iter(entry.key, entry.data);
    }
}

private
def(prime_p, bool : size_t @n) {
    size_t div = 3;
    while(div * div < n && n % div != 0) {
        div += 2;
    }
    return n % div != 0;
}

private
def(search, bool : void * @_key . void * @data . void ** @ret . enum ACTION @action) {
    struct Object * key = _key;
    size_t hval = hash_code(key);
    size_t i = hval % self->size; // First hash function.

    // There are 3 possibilities:
    // 1. The slot is used, same key.
    // 2. The slot is used, different key.
    // 3. The slot is not used.
    struct HashEntry * entries = self->entries;

    // Possibility 1.
    if(entries[i].used == hval &&
            equals(key, entries[i].key)) {
        // Possibility 2.
    } else if(entries[i].used) {
        // The second hash function can't be 0.
        size_t hval2 = 1 + hval % (self->size - 1);
        size_t first = i;
        do {
            i = (i + hval2) % self->size;
            if(i == first) {
                // If all of slots are Possibility 2. The end is here.
                return false;
            }

            // Possibility 1.
            if(entries[i].used == hval &&
                    equals(key, entries[i].key)) {
                break;
            }
            // Possibility 2.
        } while(entries[i].used);
    }
    // Possibility 1, 3.
    struct HashEntry * select = &entries[i];
    switch(action) {
    case Set:
        // Possibility 3.
        if(!select->used) {
            self->filled++;
        }
        select->used = hval;
        select->key  = key;
        select->data = data;
        float ratio = 0.8;
        if(((float) self->filled) / self->size > ratio) {
            rehash(self);
        }
        break;
    case Get:
        // Possibility 3.
        if(!select->used) {
            return false;
        }
        // Possibility 1.
        * ret = select->data;
        break;
    }
    return true;
}

private
def(rehash, void) {
    // Make the new size is double and odd.
    size_t old_size = self->size;
    size_t size = old_size * 2 + 1;
    while(!prime_p(self, size)) size += 2;
    self->size = size;
    self->filled = 0;

    // Create the new entry array.
    struct HashEntry * entries = self->entries;
    self->entries = malloc(size * (sizeof(struct Hash)));
    for(size_t i = 0; i < old_size; i++) {
        struct HashEntry entry = entries[i];
        if(entry.used) set(self, entry.key, entry.data);
    }
    free(entries);
}
