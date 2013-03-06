Title: Extreme (size) optimization in C, C*+ and Objective-C.
\
h1. Extreme optimization in C, C*+ and Objective-C.

by [Krzysztof Kowalczyk](http://blog.kowalczyk.info)

What is this about?
-------------------

This document presents extreme techniques for optimizing the size of C,
C++ and Objective-C programs. It’s about optimizing both the size of the
final executable and the amount of memory used at runtime.

![](>http://farm1.static.flickr.com/206/512024537_3da49317eb_m.jpg):http://www.flickr.com/photos/strict/512024537/

Sometimes the code will also become faster, although speed is not the
main focus.

All examples come from optimizing real-life code.

Table of contents
-----------------

1.  [Why optimize?](why_optimize.html)
2.  [An optimization story](optimization_story.html)
3.  [Things you need to know](things_to_know.html)
4.  [Measuring size](measuring_size.html)
5.  [Asking the compiler to optimize for you](asking_compiler.html)
6.  [Optimizing size of structures](optimize_size_of_structures.html)
7.  [Optimizing arrays of
    structures](optimize_arrays_of_structures.html)
8.  [Passing by value vs. passing by reference](pass_by_value.html)
9.  [Allocating without freeing](alloc_no_free.html)
10. [Smaller code through better architecture](better_architecture.html)
11. [Case study: optimizing disassembler in v8](optimizing_v8.html)
12. [Other misc ideas](misc_ideas.html)

