Title: Things to know.

Basic things you need to know.
==============================

When optimizing for size, you need to know what the sizes are and what
other factors increase or decrease size.

Sizes of basic C types
----------------------

C has few fundamental types and it’s important to know how much memory
they use:

-   char - 1 byte
-   short - 2 bytes
-   int - 4 bytes on 32-bit machines, 4 or 8 on 64-bit
-   long - like int
-   pointer - 4 bytes on 32-bit machines, 8 bytes on 64-bit machines
-   wchar\_t - 2 bytes on Windows, 4 bytes on posix (unix)

C standard unfortunately doesn’t dictate the size for int and long types
and it might depend on the compiler you’re using or target for which
you’re compiling (i.e. 32 bit vs. 64 bit, ARM vs. Intel).

If you want to operate on exact sizes, define specific types like `i16`,
`i32`, `i64` etc. and use preprocessor tricks to typedef them to the
right type in your compiler.

The exact name doesn’t matter - I like short ones but you’ll find
`int_16_t`. A practical concern is being able to use other peoples code
(or make your code re-usable by others) so a good policy is to only use
those typedefs internally but do not expose them in public interfaces.

Layout of structures
--------------------

What’s the size of this structure:

<code class="cpp">

<pre>
struct foo {\
 char c1;\
 int i1;\
};\

</pre>
</code>

Naive math would give you 5 bytes: 1 byte for char and 4 bytes for i1.
But it’s 8.

C laids out fields in the structures using natural alignment i.e. since
int is 4 bytes in size, its position in the structure will be aligned at
4 byte boundary.

C is a very low-level language and it lays out variables in the exact
order you specified, even if it’s not optimal. It’s therefore up to you
to order them in the most efficient order. In this case it would be:

The rule of thumb here is to put largest fields first, smallest last.

<code class="cpp">

<pre>
struct foo {\
 int i1;\
 char c1;\
};\

</pre>
</code>

Malloc overhead.
----------------

How many bytes do you steal from OS when you call `malloc(1)`? Again,
naive math would indicate 1.

Allocation process, however, is not magic. Allocator has to track
allocations, which takes memory. The exact number for per-allocation
overhead depends on the allocator, but you can expect this to be at
least 8 bytes (which, when you think about it, is only 2 pointers).

Another overhead comes from allocation rounding. The specifics again
depend on the allocator - on Windows allocations are rounded by 16
bytes.

The bottom line is: avoid lots of small allocations. 8 byte overhead for
256 byte allocation isn’t much, 22 byte overhead for 2 byte allocation
is a lot.

Memory hierarchies.
-------------------

Modern processors are super fast. Memory access, on the other hand, is
slow. Accessing data on a hard-drive is in its own class of slowness.

To alleviate the slowness of memory access, modern processors employ
level 1 (L1) and level 2 (L2) caches. L1 and L2 memory is much faster
than main RAM but they’re also much more expensive, so you can only
afford very little L1 and L2 caches.

The important thing to know is relative cost of memory accesses compared
to the cost of an instruction. Here’s the breakdown for modern CPU
(http://www.nwcpp.org/Downloads/2007/Machine\_Architecture\_~~\_NWCPP.pdf):
\
\<table\>\
\<tr\>\
 \<th\>Store\</th\>\
 \<th\>Size\</th\>\
 \<th\>Access cost \</th\>\
\</tr\>\
\<tr\>\
 \<td\>Registers\</td\>\
 \<td\>64B\</td\>\
 \<td\>1\</td\>\
\</tr\>\
\<tr\>\
 \<td\>L1 cache\</td\>\
 \<td\>32KB\</td\>\
 \<td\>3\</td\>\
\</tr\>\
\<tr\>\
 \<td\>L2 cache\</td\>\
 \<td\>4MB\</td\>\
 \<td\>14\</td\>\
\</tr\>\
\<tr\>\
 \<td\>DRAM\</td\>\
 \<td\>4GB\</td\>\
 \<td\>200\</td\>\
\</tr\>\
\<tr\>\
 \<td\>Disk\</td\>\
 \<td\>500GB\</td\>\
 \<td\>15M\</td\>\
\</tr\>\
\</table\>
\
The exact numbers vary per CPU, but this is roughly correct.
\
What this data says is that storing data on disk is a performance
disaster: you can execute millions CPU instructions in the time taken to
read the data. Of course you can’t avoid using disk but you can win big
by choosing compact representation of the data and even
compressing/decompressing data when writing/reading data to/from a file.
\
A corollary: you loose big when you choose a bad, verbose file format
e.g. XML.
\
What it also says is that even accessing data in RAM is slow~~ you can
execute 200 instructions in the time taken to access a byte in RAM. This
means that choosing more compact representation which requires more
instructions to decode than a straightforward representation can
(counterintuitively) speed up the program because cutting down the time
spent accessing memory more than offsets additional CPU instructions
needed to decode the data.
