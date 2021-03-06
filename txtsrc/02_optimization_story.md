Title: An optimization story

An optimization story
=====================

![](>http://farm3.static.flickr.com/2295/2446871523_05af7baec8_m.jpg):http://flickr.com/photos/jurvetson/2446871523/

Here’s an example of optimizing a piece of code, starting from simple,
unoptimized code to extremely optimized, yet still simple code.

The task is to write a class that parses a string in the form
`name1=val1&name2=val2&name3=val3&...` and provides a simple interface
for accessing names and values.

It’s not a made up example but a very real (albeit simplified) problem
of parsing arguments of HTTP GET request.

We’ll be trying to minimize size of the code and the amount of memory
used at runtime for storing parsed strings. Here’s the interface:

`includesrc ParsedStrDummy.h 4 11

Before we proceed, we need a way to know if our code is working. All versions of the code provide identical interface to the caller, so we'll write a unit test driver "ParsedStrTest.cpp":src/ParsedStrTest.cpp.html ("txt":src/ParsedStrTest.cpp.txt) and a "makefile":src/makefile.html ("txt":src/makefile.txt) which will compile all versions as separate programs in debug, release and size-optimize configurations. That way we'll be able to compare the code size for each version and ensure all versions work the same way.

Common utility routines are in "ParsedStrUtil.cpp":src/ParsedStrUtil.cpp.html ("txt":src/ParsedStrUtil.cpp.txt).

h2. Naive STL version

A naive STL version will simply use a vector of strings for storing names and a vector of strings for storing values.

!>http://farm1.static.flickr.com/52/181086335_3390d27992_m.jpg!:http://www.flickr.com/photos/jonhanson/181086335/in/set-72157594186301066/

`includesrc ParsedStrStl.h 7 11

`includesrc ParsedStrStl.cpp 8 43

How much memory are we using to store N strings? We really don't know. Not only it depends on a particular STL implementation but whatever that value is, STL does a good job at hiding it from us. You can find out if you're determined enough, but easy it ain't.

h2. Naive non-STL version

Let's get rid of STL and use `char **@ instead of `std::string` and
`char **` instead of `std::vector<std::string>`.
\
STL’s `std::vector` takes care of resizing the array but we would have
to code it manually. We’ll side-step the problem and do it in two steps:
\
** calculate the size of arrays\
\* allocate arrays and fill them out

It’s both simpler and faster.

![](>http://farm1.static.flickr.com/68/181095786_ef9fc49edd_m.jpg):http://www.flickr.com/photos/jonhanson/181095786/in/set-72157594186301066/

`includesrc ParsedStrUnopt.h 4 9

`includesrc ParsedStrUnopt.cpp 13 51

Optimizing allocations of strings
---------------------------------

In the naive version we make N\*2 allocations to store N name/value
pairs. Most of those strings are short, so we’re probably badly hit by
`malloc()`‘s overhead. Furthermore, `malloc()` is relatively slow, so
doing less of it can only be good.
\
Let’s notice two important properties that allows us to optimize this:
\
\* total amount of memory needed to store all strings is exactly the
size of the string given as an argument to `ParsedStr::parse()`\
\* memory for all strings is freed at the same time
\
Our optimized version simply duplicates the original string, replaces
`=` and `&` characters with’\0’ (for compatibility with C) and all the
pointers point inside that copy.

When we’re done we only need to free the string copy.

![](>http://farm1.static.flickr.com/57/181091710_9bb4dd4bcf_m.jpg):http://www.flickr.com/photos/jonhanson/181091710/in/set-72157594186301066/

`includesrc ParsedStrOptAlloc.h 4 10

`includesrc ParsedStrOptAlloc.cpp 15 51

Not only this version uses less memory, is faster but also the code is
simpler and smaller.

One array instead of two
------------------------

Notice that we allocate two arrays of equal length and we do it only
once. We can easily change the code to only use one array of twice the
size.

We adopt a simple indexing convention for accessing elements. Element
`i` of `_name` array is now at index `i*2` of the combined array, and
element `i` of `_value` array is at index `i*2+1`.

This saves one `malloc()` and a little bit of memory due to less
`malloc()` overhead.

![](>http://farm1.static.flickr.com/62/181087250_c3d0fafd24_m.jpg):http://www.flickr.com/photos/jonhanson/181087250/in/set-72157594186301066/

`includesrc ParsedStrOptOneArray.h 4 9

`includesrc ParsedStrOptOneArray.cpp 15 55

This technique can also be applied in languages like C\#, Java and with
STL vectors.

Depending on access patterns of the data, you might try to improve cache
locality (and speed) of access by using a different order e.g. instead
of interleaving names and values, you can first store all values and
then all names.

Offsets instead of pointers
---------------------------

A pointer uses 4 bytes on 32-bit machine and 8 bytes on (increasingly
more common) 64-bit machines.

All pointers point somewhere within a string so they could be
represented as an offset from the beginning of the string.

If we can guarantee that the string size is limited, which is often the
case, we can represent the offset with a smaller number of bytes. For
strings 3 bytes (24 bits) would be a safe value for most cases. In our
case let’s put a hard cap on the string limit at 64 kB and use 2 bytes
for offset.

![](>http://farm1.static.flickr.com/63/181087083_fcf7fe632d_m.jpg):http://www.flickr.com/photos/jonhanson/181087083/in/set-72157594186301066/

`includesrc ParsedStrOptOffsets.h 4 9

`includesrc ParsedStrOptOffsets.cpp 15 55

We save 2 bytes per pointer on 32 bit machines and 6 bytes per pointer
on 64 bit machines.

Getting rid of offsets
----------------------

Do we need the offsets at all? We know that strings are laid out in
memory in a specific order and if we’re willing to sacrifice a little
bit of speed, we don’t have to store the offsets. Let’s just find n-th
string starting from the beginning.

![](>http://farm1.static.flickr.com/58/181086162_1f3ed9359b_m.jpg):http://www.flickr.com/photos/jonhanson/181086162/in/set-72157594186301066/

`includesrc ParsedStrOptNoOffsets.h 4 8

`includesrc ParsedStrOptNoOffsets.cpp 14 33

The code is not only optimized for size but also simpler than most
versions.

Is this approach too slow? It depends.

If there are thousands of strings and we call `name()` and `value()`
functions millions of times, then yes.

If there are only few strings and `name()` and `value()` are called only
a couple of times, then no.

How do you know which case applies in your code? Use a profiler,
[Valgrind](http://valgrind.org) is your friend.

Avoiding copying the string
---------------------------

What if we can modify the string passed in to `ParsedStr::parse()`? What
if we can assume that the lifetime of that string is longer than
lifetime of `ParsedStr` object?

If that’s true, we can avoid allocating a copy of the string and freeing
it.

This optimization can be combined with most other optimization. Here’s
the code after applying it previous version:

`includesrc ParsedStrOptNoOffsetsNoDup.cpp 14 28

This approach is more error prone because clients have to ensure proper lifetime of the string, but hey, this is about extreme optimizations, not safe ones.

h2. Optimizing for common case

What if most of the time the strings are short, say less than 200 bytes in size? If the string to parse is part of HTTP GET request, this is a safe assumption.

We could optimize for common case by using buffer that is part of `ParsedStr@
class. If a string is small enough to fit the buffer (which we presume
is common), we save a `malloc()` and its associated overhead.

If the string doesn’t fit in the buffer, we’ll use more memory (the
buffer is unused).

How do we know if it’s worth it and how do we know what buffer size
should we pick? It has to come from data based on real-life usage of the
application. Instrument the app to gather statistics on the sizes of the
string and pick a value that balances small size of the buffer and high
hit ratio.

![](>http://farm1.static.flickr.com/65/181091396_26f1dc4d00_m.jpg):http://www.flickr.com/photos/jonhanson/181091396/in/set-72157594186301066/

`includesrc ParsedStrOptCommon.h 4 12

`includesrc ParsedStrOptCommon.cpp 14 41

This trick can be used in combination with other optimizations.

Comparing different approaches.
-------------------------------

Here are the file sizes of various versions, compiled with gcc on Linux
and stripped of symbols (to get the most accurate picture of the size of
the code):

`includetxt parsedstr-size-stats-linux.html

Here's the same data when using gcc on Mac OS X 10.5:

`includetxt parsedstr-size-stats-mac.html

The interesting metric is file size delta. It shows how much bigger a
given implementation of `ParsedStr` class is compared to a version that
does nothing.

Why such a difference between Linux and Mac versions? On Mac, a linker
has a `-dead_strip` option that removes code not used anywhere. On
Linux, sadly, this very basic functionality is not yet present in gcc
toolchain. Let’s use results from Mac, because they better represent the
real code generated.

As to the size, STL version is the worst by a wide margin. Not only it’s
more than 31 times bigger than the smallest version but also 24 times
bigger than the biggest non-STL version.

Non-STL versions are very close in size, but if you’re an extremist like
us, saving 56 or 48 bytes is still worth fighting for.

Conclusions.
------------

STL code is arguably the cleanest but you pay a huge price in code size
and inflexibility.

Optimization techniques are all based on understanding what’s happening
under the hood and exploiting particular properties of the data. STL is
bad because it hides how it works internally, which hinders our ability
to understand what’s going on and come up with an optimization.

Some of the above techniques can’t be expressed in STL. Some of them
would cause the code to loose its simplicity, which is the only thing
STL version has going for it.

Important thing to notice is that sometimes optimizing for space also
makes the code simpler and faster so you win at multiple fronts.
