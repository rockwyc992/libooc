#   What's libooc

libooc is library with object oriented written in c.

#   Define your class

##  Example: Circle

*   Implementation: `circle.c`

        #include <stdlib.h>

        // struct definition of Circle
        #include "circle.struct.h"

        // inherit form Object
        def_class(Circle, Object)

        // constructor
        override
        def(ctor, void : va_list * @args) {
            self->radius = va_arg(* args, int);
        }

        // destructor
        override
        def(dtor, void) {
            free(self);
        }

        // public method
        def(area, int) {
            // call private method
            return my_pow(self, self->radius, 2);
        }

        // private method
        private
        def(my_pow, int : int @base, int @exponent) {
            int result = 1;
            while(exponent > 0) {
                result *= base;
                exponent -= 1;
            }
            return result;
        }

*   Struct definition: `circle.struct.h`

        #ifndef CIRCLE_STRCUT_H
        #define CIRCLE_STRCUT_H

        // get the parent's struct definition
        #include <libooc/object.struct.h>

        // declare the methods
        #include "circle.h"

        struct Circle {
            struct Object super;
            int radius;
        };

        // declare virtual table(like C++) of Circle
        def_class_struct()

        #endif

*   Public methods declaration: `circle.h`

    (I create a script for vim, the script can create/update this file according to `circle.c`)

        #ifndef CIRCLE_H
        #define CIRCLE_H

        // declare parent's public methods
        #include <libooc/object.h>

        // you can puts some struct or enum declarations here
        // ...

        #undef CLASS
        #define CLASS Circle
        #define _Circle_PUBLIC_OVERRIDE_METHODS \
            (_Circle_ctor), \
            (_Circle_dtor)
        #define _Circle_PUBLIC_METHODS \
            (_Circle_area)
        #define _Circle_PRIVATE_METHODS \
            (_Circle_my_pow)
        #define _Circle_ctor ctor, void, (va_list * args, args)
        #define _Circle_dtor dtor, void
        #define _Circle_area area, int
        #define _Circle_my_pow my_pow, int, (int base, base), (int exponent, exponent)

        // declare the public methods(macro expansion)
        def_public_methods()

        #endif

*   Usage: `main.c`

        #include <stdio.h>
        #include "circle.h"

        int
        main(void) {
            Circle_init();
            void * circle = new(Circle, 2);
            int area = Circle_area(circle);
            printf("circle area: %d\n", area);
            delete(circle);
            return 0;
        }

Compile:
(make sure your have installed libooc)

    $ cc -c -o circle.o circle.c -I/usr/local/include/libooc-0.0.1
    $ cc -o main main.c circle.o -I/usr/local/include/libooc-0.0.1 -L/usr/local/lib -looc-0.0.1

Run:

    $ ./main
    circle area: 4

#   Available Classes in libooc

*   String

        String_init();
        void * a = new(String, "a");
        delete(a);

*   Static String

        Value_init();
        Value a;
        Value_type(&a, StaticString);
        Value_set_str(&a, "a");
        Value_get_str(&a);

*   Hash

        String_init();
        void * a = new(String, "a");
        void * b = new(String, "b");

        Hash_init();
        void * hash = new(Hash);
        Hash_set(hash, a, b);
        Hash_get(hash, a);
        delete(hash);

*   Array

        Array_init();
        void * ary = new(Array);
        Array_push(ary, a);
        Array_push(ary, b);
        Array_each(ary, Array_print);
        delete(ary);

#   How to install

1.  Download this repository.

        $ git clone git://github.com/xsoameix/libooc.git

2.  Change directory to libooc.

        $ cd libooc

3.  If `./configure` does not exist or is older than configure.in,
    run autoconf to (re)generate configure.

4.  Run `./configure`, which will generate config.h and Makefile.

5.  Run `make`.

6.  Run `make install`.

#   Copying

See the file `COPYING`.
