Title: Asking the compiler to optimize for you.

Asking the compiler to optimize for you.
========================================

There are only two C/C*+ compilers that matter: gcc and Visual Studio.
Both have command-line options to optimize for the size of generated
code.
\
For gcc it’s `-Os` option. It’s an aggregate options which means that it
enables few out of the million gcc optimization flags, those that aim at
reducing the code size.
\
For Visual Studio’s `cl.exe`, it’s `/Os`.
\
The most common practice when building C/C*+ software is to either build
debug version (`-O0` in gcc, `/Od` in Visual C++) or optimized for speed
release version (`-O2` or `-O3` in gcc, `/O2` in `cl.exe`) and
optimization for size is rarely used.

TODO: compile some largish program with various optimizations and report
the differences.

TODO: mention thumb on ARM (saves 30% of code space, makes programs 30%
slower)
