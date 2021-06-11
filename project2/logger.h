#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

typedef int     (*chmod_t)(const char*, mode_t);
typedef int     (*chown_t)(const char*, uid_t, gid_t);

typedef int     (*creat_t)(const char*, mode_t);
typedef int     (*creat64_t)(const char*, mode_t);
typedef int     (*open_t)(const char*, int, ...);
typedef int     (*open64_t)(const char*, int, ...);
typedef int     (*close_t)(int);

typedef FILE*   (*fopen_t)(const char *, const char *);
typedef FILE*   (*fopen64_t)(const char *, const char *);
typedef int     (*fclose_t)(FILE*);
typedef size_t  (*fread_t)(void*, size_t, size_t, FILE*);
typedef size_t  (*fwrite_t)(const void*, size_t, size_t, FILE*);
typedef FILE*   (*tmpfile_t)(void);
typedef FILE*   (*tmpfile64_t)(void);
typedef int     (*remove_t)(const char*);

typedef int     (*rename_t)(const char*, const char*);

typedef ssize_t (*write_t)(int, const void*, size_t);
typedef ssize_t (*read_t)(int, void*, size_t);