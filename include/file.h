#ifndef FILE_H
#define FILE_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#define INTIAL_CAPACITY 240 // bytes

typedef enum {
    READ_SUCCESSFUL,
    INVALID_ARGUMENT,
    MEMORY_ALLOCATION_ERROR,
    FILE_READ_ERROR
} FileReadResult;

/*
 * Read entire file into dynamically sized buffer.
 */
FileReadResult f_readall(const char *file_name, char **data, size_t *out_size);


#endif