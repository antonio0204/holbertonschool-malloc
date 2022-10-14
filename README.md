# 0x0B. C - malloc
## Learning Objectives

    What is a program break
    How to play with a program break in order to allocate memory dynamically
    How the glibc malloc and free functions work
    What is ASLR
    What is memory alignment
    What is a memory page
    How to encapsulate the memory management in order to hide it from the user


## Requirements

    Allowed editors: vi, vim, emacs
    All your files will be compiled on Ubuntu 14.04 LTS
    Your C programs and functions will be compiled with gcc 4.8.4 using the flags -Wall -Werror -Wextra and -pedantic
    All your files should end with a new line
    A README.md file, at the root of the folder of the project, is mandatory
    Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
    You are not allowed to have more than 5 functions per file
    The prototypes of all your functions should be included in your header file called malloc.h
    Don’t forget to push your header files
    All your header files should be include guarded
    You are allowed to use global variables
    You are allowed to use static variables

## Important

It is not required that your _malloc, free, calloc and realloc behave exactly like the glibc malloc, free, calloc and realloc:

    You are free to use any data structure that suits you as long as their purpose is well defined
    You are free to handle the heap as it suits you, as long as the returned pointers (for the functions that return a pointer) are aligned as required and that enough space is available
    You are free to extend the program break as it suits you, as long as it is extended by a multiple of the virtual memory page size, and as long as it is reduced when needed
    You decide of your implementation. During the correction, we will mainly focus on the strength and reliability of your functions, so make sure to handle big allocations :)

