Title: Extreme (size) optimization in C, C++ and Objective-C.

# Secrets of code optimization gurus

by [Krzysztof Kowalczyk](http://blog.kowalczyk.info)

## What is this about?

SumatraPDF, a project I started, is often praised for its small size and
performance. It implements a lot of functionality at a fraction of size
of competing software.

This is not an accident.

This book will show you the techniques I use to keep Sumatra small despite
having lots of features.

You'll learn extreme techniques for optimizing the size and memory usage of C,
C++ and Objective-C programs.

Sometimes the code will also become faster but speed is not the
main focus. Improving performance is a extensively covered topic.

All examples from optimizing real-life code.

## Table of contents

1.  Why optimize?
2.  [An optimization story](optimization_story.html)
3.  Things you need to know
4.  Measuring size
5.  Asking the compiler to optimize for you
6.  Optimizing size of structures
7.  Optimizing arrays of structures
8.  Passing by value vs. passing by reference
9.  Allocating without freeing
10. Smaller code through better architecture
11. [Case study: optimizing disassembler in v8](optimizing_v8.html)
12. Other misc ideas

