#include "logger.h"

#define SHLIB       "libc.so.6"
#define LOGGER      "[logger]"
#define NULLPATH    "untouched"
#define BUFMAX      1024
#define PRINT_SIZE  32

static void *handle;

void FILE2name(FILE* stream, char* buf){
    int fd = fileno(stream);
    char path[BUFMAX];
    memset(path, 0, BUFMAX);
    sprintf(path, "/proc/self/fd/%d", fd);
    int result = readlink(path, buf, BUFMAX);
}
void fd2name(int fd, char*buf){
    char path[BUFMAX];
    memset(path, 0, BUFMAX);
    sprintf(path, "/proc/self/fd/%d", fd);
    int result = readlink(path, buf, BUFMAX);
}
int chmod(const char *pathname, mode_t mode){
    handle = dlopen(SHLIB, RTLD_LAZY);
    chmod_t o_chmod = (int(*)(const char*, mode_t))dlsym(handle, __func__);
    int result = o_chmod(pathname, mode);

    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }
    
    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", %o) = %d\n", LOGGER, __func__, path, mode, result);

    free(path);
    return result;
}
int chown(const char *pathname, uid_t owner, gid_t group){
    handle = dlopen(SHLIB, RTLD_LAZY);
    chown_t o_chown = (int(*)(const char*, uid_t, gid_t))dlsym(handle, __func__);
    int result = o_chown(pathname, owner, group);
    
    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", %d, %d) = %d\n", LOGGER, __func__, path, owner, group, result);

    free(path);
    return result;
}
int close(int fd){
    char name[BUFMAX];
    memset(name, 0, BUFMAX);
    fd2name(fd, name);

    int result;
    if(fd!=2){
        handle = dlopen(SHLIB, RTLD_LAZY);
        close_t o_close = (int(*)(int))dlsym(handle, __func__);
        result = o_close(fd);
    }else
        result = 0;

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\") = %d\n", LOGGER, __func__, name, result);

    return result;
}
int creat64(const char *pathname, mode_t mode){
    handle = dlopen(SHLIB, RTLD_LAZY);
    creat64_t o_creat = (int(*)(const char*, mode_t))dlsym(handle, __func__);
    int result = o_creat(pathname, mode);

    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", %o) = %d\n", LOGGER, __func__, path, mode, result);

    free(path);
    return result;
}
int creat(const char *pathname, mode_t mode){
    handle = dlopen(SHLIB, RTLD_LAZY);
    creat_t o_creat = (int(*)(const char*, mode_t))dlsym(handle, __func__);
    int result = o_creat(pathname, mode);

    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", %o) = %d\n", LOGGER, __func__, path, mode, result);

    free(path);
    return result;
}
FILE *fopen(const char *pathname, const char *mode){   
    handle = dlopen(SHLIB, RTLD_LAZY);
    fopen_t o_fopen = (FILE*(*)(const char*, const char*))dlsym(handle, __func__);
    FILE *result = o_fopen(pathname, mode);
    
    char *path = realpath(pathname, NULL);
    if(path==NULL){ 
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", \"%s\") = %p\n", LOGGER, __func__, path, mode, result);

    free(path);
    return result;
}
FILE *fopen64(const char *pathname, const char *mode){   
    handle = dlopen(SHLIB, RTLD_LAZY);
    fopen64_t o_fopen = (FILE*(*)(const char*, const char*))dlsym(handle, __func__);
    FILE *result = o_fopen(pathname, mode);
    
    char *path = realpath(pathname, NULL);
    if(path==NULL){ 
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", \"%s\") = %p\n", LOGGER, __func__, path, mode, result);

    free(path);
    return result;
}
int fclose(FILE *stream){
    char buf[BUFMAX];
    memset(buf, 0, BUFMAX);
    FILE2name(stream, buf);

    handle = dlopen(SHLIB, RTLD_LAZY);
    fclose_t o_fclose = (int(*)(FILE*))dlsym(handle, __func__);
    int result = o_fclose(stream);

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\") = %d\n", LOGGER, __func__, buf, result);

    return result;
}
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
    handle = dlopen(SHLIB, RTLD_LAZY);
    fread_t o_fread = (size_t(*)(void*, size_t, size_t, FILE*))dlsym(handle, __func__);
    size_t result = o_fread(ptr, size, nmemb, stream);

    char buf[BUFMAX];
    memset(buf, 0, BUFMAX);
    FILE2name(stream, buf);

    ptr = (char*)ptr;
    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"", LOGGER, __func__);
    for(int i=0; i<size*nmemb && i<PRINT_SIZE && i<result; i++){
        if(isprint((int)*((char*)ptr+i))==0)    dprintf(out, ".");
        else                                    dprintf(out, "%c", *((char*)ptr+i));
    }
    dprintf(out, "\", %ld, %ld, \"%s\") = %ld\n", size, nmemb, buf, result);
    return result;
}
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
    handle = dlopen(SHLIB, RTLD_LAZY);
    fwrite_t o_fwrite = (size_t(*)(const void*, size_t, size_t, FILE*))dlsym(handle, __func__);
    size_t result = o_fwrite(ptr, size, nmemb, stream);

    char name[BUFMAX];
    memset(name, 0, BUFMAX);
    FILE2name(stream, name);

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"", LOGGER, __func__);
    for(int i=0; i<size*nmemb && i<PRINT_SIZE && i<result; i+=size){
        if(isprint((int)*((char*)ptr+i))==0)    dprintf(out, ".");
        else                                    dprintf(out, "%c", *((char*)ptr+i));
    }
    dprintf(out, "\", %ld, %ld, \"%s\") = %ld\n", size, nmemb, name, result);
    return result;
}
int open(const char *pathname, int flags, ...){
    handle = dlopen(SHLIB, RTLD_LAZY);
    open_t o_open = (int(*)(const char*, int, ...))dlsym(handle, __func__);

    int result;
    mode_t mode = 0;
    va_list args;
    va_start(args, flags);
    /*   
        The  mode  argument  specifies  the  file  mode  bits be applied when a new file is created.  
        This argument must be supplied when O_CREAT or O_TMPFILE is specified in flags; 
        if neither  O_CREAT nor O_TMPFILE is specified, then mode is ignored.   
    */
    if((flags&O_CREAT) || (flags&O_TMPFILE)){
        mode = va_arg(args, mode_t);
        result = o_open(pathname, flags, mode);
    }
    else    result = o_open(pathname, flags);

    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", %o, %o) = %d\n", LOGGER, __func__, path, flags, mode, result);

    free(path);
    return result;
}
int open64(const char *pathname, int flags, ...){
    handle = dlopen(SHLIB, RTLD_LAZY);
    open64_t o_open = (int(*)(const char*, int, ...))dlsym(handle, __func__);

    int result;
    mode_t mode = 0;
    va_list args;
    va_start(args, flags);
    /*   
        The  mode  argument  specifies  the  file  mode  bits be applied when a new file is created.  
        This argument must be supplied when O_CREAT or O_TMPFILE is specified in flags; 
        if neither  O_CREAT nor O_TMPFILE is specified, then mode is ignored.   
    */
    if((flags&O_CREAT) || (flags&O_TMPFILE)){
        mode = va_arg(args, mode_t);
        result = o_open(pathname, flags, mode);
    }
    else    result = o_open(pathname, flags);

    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", %o, %o) = %d\n", LOGGER, __func__, path, flags, mode, result);

    free(path);
    return result;
}
int rename(const char *oldpath, const char *newpath){
    char *path1 = realpath(oldpath, NULL);
    if(path1==NULL){
        path1 = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path1, NULLPATH);
    }

    handle = dlopen(SHLIB, RTLD_LAZY);
    rename_t o_rename = (int(*)(const char*, const char*))dlsym(handle, __func__);
    int result = o_rename(oldpath, newpath);

    char *path2 = realpath(newpath, NULL);
    if(path2==NULL){
        path2 = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path2, NULLPATH);
    }

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\", \"%s\") = %d\n", LOGGER, __func__, path1, path2, result);
    
    free(path1);
    free(path2);
    return result;
}
ssize_t write(int fd, const void *buf, size_t count){
    handle = dlopen(SHLIB, RTLD_LAZY);
    write_t o_write = (ssize_t(*)(int, const void*, size_t))dlsym(handle, __func__);
    ssize_t result = o_write(fd, buf, count);

    char name[BUFMAX];
    memset(name, 0, BUFMAX);
    fd2name(fd, name);

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(%s, \"", LOGGER, __func__, name);
    for(int i=0; i<count && i<PRINT_SIZE && i<result; i++){
        if(isprint((int)*((char*)buf+i))==0)    dprintf(out, ".");
        else                                    dprintf(out, "%c", *((char*)buf+i));
    }
    dprintf(out, "\", %ld) = %ld\n", count, result);

    return result;
}
ssize_t read(int fd, void *buf, size_t count){
    handle = dlopen(SHLIB, RTLD_LAZY);
    read_t o_read = (ssize_t(*)(int, void*, size_t))dlsym(handle, __func__);
    ssize_t result = o_read(fd, buf, count);

    char name[BUFMAX];
    memset(name, 0, BUFMAX);
    fd2name(fd, name);

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(%s, \"", LOGGER, __func__, name);
    for(int i=0; i<count && i<PRINT_SIZE && i<result; i++){
        if(isprint((int)*((char*)buf+i))==0)    dprintf(out, ".");
        else                                    dprintf(out, "%c", *((char*)buf+i));
    }
    dprintf(out, "\", %ld) = %ld\n", count, result);

    return result;
}
FILE* tmpfile(void){
    handle = dlopen(SHLIB, RTLD_LAZY);
    tmpfile_t o_tmpfile = (FILE*(*)(void))dlsym(handle, __func__);
    FILE *result = o_tmpfile();

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s() = \"%p\"\n", LOGGER, __func__, result);

    return result;
}FILE* tmpfile64(void){
    handle = dlopen(SHLIB, RTLD_LAZY);
    tmpfile64_t o_tmpfile = (FILE*(*)(void))dlsym(handle, __func__);
    FILE *result = o_tmpfile();

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s() = \"%p\"\n", LOGGER, __func__, result);

    return result;
}
int remove(const char *pathname){
    char *path = realpath(pathname, NULL);
    if(path==NULL){
        path = (char*)malloc(sizeof(char)*sizeof(NULLPATH));
        strcpy(path, NULLPATH);
    }

    handle = dlopen(SHLIB, RTLD_LAZY);
    remove_t o_remove = (int(*)(const char*))dlsym(handle, __func__);
    int result = o_remove(pathname);

    int out = atoi(getenv("LOGGER_OUT"));
    dprintf(out, "%s %s(\"%s\") = %d\n", LOGGER, __func__, path, result);

    return result;
}