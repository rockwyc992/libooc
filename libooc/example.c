#include <stdio.h>

#include "value.h"
#include "string.conflict.h"
#include "hash.h"
#include "array.h"
#include "file.h"

static void
array_print(void * value, size_t index) {
    printf("%zd: %s\n", index, value_get_str(value));
}

int main(void) {
    // static string
    value_init();
    Value a;
    value_type(&a, StaticString);
    value_set_str(&a, "a");
    value_get_str(&a);

    // string
    string_init();
    void * b = new(String, "b");

    // hash
    hash_init();
    void * hash = new(Hash);
    hash_set(hash, &a, b);
    void * c = hash_get(hash, &a);
    string_puts(c);
    delete(hash);

    // array
    array_init();
    void * ary = new(Array);
    array_push(ary, &a);
    array_unshift(ary, b);
    void * d = array_shift(ary);
    string_puts(d);
    delete(ary);

    delete(b);

    // file
    file_init();
    void * file = new(File, "libooc/example");
    void * content = file_read(file);
    string_puts(content);
    delete(file);
}
