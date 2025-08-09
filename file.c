#include "file.h"

/*
 * Read entire file into dynamically sized buffer.
 */
FileReadResult f_readall(const char *file_name, char **data, size_t *out_size) {

    size_t len = 0;
    size_t cap = INTIAL_CAPACITY;
 
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        perror("fopen() failed");
        return FILE_READ_ERROR;
    }

    // One byte extra for null terminator
    char *buf = (char *)malloc(cap + 1);
    if (buf == NULL) {
        perror("malloc() failed");
        fclose(fp);
        return MEMORY_ALLOCATION_ERROR;
    }

    size_t space;
    size_t n_read;

    for (;;) {
        space = cap - len;
        n_read = fread(buf + len, 1, space, fp);
        //printf("cap: %d, len: %d, n_read: %d, space: %d\n", cap, len, n_read, space);
        len += n_read;

        if (n_read < space) {
            if (ferror(fp)) {
                free(buf);
                fclose(fp);
                perror("ferror() caught file error");
                return FILE_READ_ERROR;
            }

            // EOF reached
            break;
        }
        
        // Double the buffer capacity.
        cap *= 2;

        char *tmp = (char *)realloc(buf, cap + 1); // +1 for null terminator here too.
        if (!tmp) {
            
            free(buf);
            fclose(fp);
            perror("realloc() failed");
            return MEMORY_ALLOCATION_ERROR;
        }

        buf = tmp;
    }

    buf[len] = '\0'; // NULL-terminate
    
    *out_size = len;
    *data = buf;
    
    fclose(fp);

    return READ_SUCCESSFUL;
}