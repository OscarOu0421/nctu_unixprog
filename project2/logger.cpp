#include "logger.h"

#define DEFAULT_SO  "./logger.so"
#define DEFAULT_OUT "2"
#define SHLIB       "libc.so.6"

int main(const int argc, char *argv[]){
    int i, j;
    char *file, *sopath, *cmd, buf[10];
    char ** cmd_arg;

    if(argc==1){
        printf("no command given.\n");
        exit(1);
    }
    setenv("LD_PRELOAD", DEFAULT_SO, 1);
    setenv("LOGGER_OUT", DEFAULT_OUT, 1);
    for(i=1; i<argc; i++){
        if(strcmp("-o", argv[i])==0){
            int size = strlen(argv[++i]);
            file = (char*)malloc(sizeof(char)*size);
            strcpy(file, argv[i]);
            void *handle = dlopen(SHLIB, RTLD_LAZY);
            open_t o_open = (int(*)(const char*, int, ...))dlsym(handle, "open");
            int fd = o_open(file, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
            sprintf(buf, "%d", fd);
            setenv("LOGGER_OUT", buf, 1);
        }
        else if(strcmp("-p", argv[i])==0){
            int size = strlen(argv[++i]);
            sopath = (char*)malloc(sizeof(char)*size);
            strcpy(sopath, argv[i]);
            setenv("LD_PRELOAD", sopath, 1);
        }
        else if(strcmp("--", argv[i])==0){
            int size = strlen(argv[++i]);
            cmd = (char*)malloc(sizeof(char)*size);
            strcpy(cmd, argv[i]);

            size = 2 + argc - (++i);
            if(size>=0){
                cmd_arg = (char**)malloc(sizeof(char*)*size);
                cmd_arg[0] = (char*)malloc(sizeof(char)*strlen(cmd));
                strcpy(cmd_arg[0], cmd);
                for(j=1; i<argc; i++, j++){
                    size = strlen(argv[i]);
                    cmd_arg[j] = (char*)malloc(sizeof(char)*size);
                    strcpy(cmd_arg[j], argv[i]);
                }
                cmd_arg[j] = NULL;
            }
            execvp(cmd, cmd_arg);
        }
        else if(argv[i][0]!='-'){
            int size = strlen(argv[i]);
            cmd = (char*)malloc(sizeof(char)*size);
            strcpy(cmd, argv[i]);

            size = 2 + argc - (++i);
            if(size>=0){
                cmd_arg = (char**)malloc(sizeof(char*)*size);
                cmd_arg[0] = (char*)malloc(sizeof(char)*strlen(cmd));
                strcpy(cmd_arg[0], cmd);
                for(j=1; i<argc; i++, j++){
                    size = strlen(argv[i]);
                    cmd_arg[j] = (char*)malloc(sizeof(char)*size);
                    strcpy(cmd_arg[j], argv[i]);
                }
                cmd_arg[j] = NULL;
            }
            execvp(cmd, cmd_arg);
        }
        else{
            char *pch = strtok(argv[i], "-");
            printf("./logger: invalid option -- '%s'\n", pch);
            printf("usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n");
            printf("        -p: set the path to logger.so, default = ./logger.so\n");
            printf("        -o: print output to file, print to \"stderr\" if no file specified\n");
            printf("        --: separate the arguments for logger and for the command\n");
            exit(1);
        }
    }
    
    return 0;
}