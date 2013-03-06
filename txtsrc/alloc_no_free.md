Title: Allocating without freeing

Allocating without freeing
==========================

Do you have to `free()` every time you `malloc()`? Conventional wisdom
would have you believed that yes, if you want to avoid memory leaks, you
have to free everything you allocate. Everybody programming in C knows
that memory leaks are common.

But what about garbage collection? In Java, C\# or Python you just
allocate and the runtime takes care of freeing the memory for you when
it’s appropriate, through the magic of garbage collection.

Garbage collection in C?
------------------------

Unfortunately generic garbage collection doesn’t play well with C. While
many experimented with adding garbage collection to C/C++ program, you
don’t see it being used on a regular basis. Garbage collection is
supposed to simplify programmer’s life but trying to combine it with C
creates more problems than it solves.

Temporary allocations only
--------------------------

This technique only applies to temporary allocations. Temporary
allocation is a piece of memory that is used only in a given function or
the functions it calls. It’s quite common pattern in C: allocate piece
of memory, do something with it, free it.

Imagine that you could just allocate temporary memory and never have to
worry about freeing it explicitly. You could save few bytes for a free()
call and your code would be a little bit shorter and a little bit
simpler.

We strongly oppose memory leaks so freeing has to be done. The trick is
to be able to tell, at any given time, whether we can free a given
temporary allocation or not. Garbage collection uses a sophisticated
machinery to determine that but we’ll take advantage of our definition
of temporary: it’s only used in a given function or the functions it
calls.

We’ll tie allocations to a unique value (let’s call it a key) which is
an address of a variable on the stack in the function that allocates the
temporary memory. We’ll store information about each allocation (key and
allocated address). Thanks to the nature of C stack, at any time we can
go through that information and if current stack address is \> key, we
can free that temporary.

What we get from that is a little bit smaller binary (no need to add
free() for every temporary allocation) and slightly simpler code. We pay
with slightly bigger average memory usage and additional code to
implement our scheme.

Implementation details
----------------------

Interface consists of only 3 functions: `temp_alloc()`, `temp_realloc()`
and `temp_freeall()`:

`includesrc temp_alloc.h 6 13

Helper function like `temp\_strdup()@ can be easily build on top of
those functions:

`includesrc temp_alloc.h 15 17

A simple implementation of `temp\_alloc()@ keeps all allocation in a
linked list.

`includesrc temp_alloc.c 41 99

A simple test to make sure it works.

`includesrc temp\_alloc\_test.c 15 42

Another implementation keeps the values in an array. It’s probably
easier to understand than linked-list solution and should be slightly
faster (traversal of array is more cache friendly) but it’s slightly
longer.

@includesrc temp\_alloc2.c 43 116
