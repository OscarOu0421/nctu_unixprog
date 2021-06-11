#include "logger.h"

#define OLDNAME "temp"
#define NEWNAME "temp1"
#define BUFSIZE 1024

int main(){
    int fd;
    FILE *file;
    char buf1[BUFSIZE], buf2[BUFSIZE];

    close(2);
    creat(OLDNAME, 0600);
    chmod(OLDNAME, 0666);
    chown(OLDNAME, 65534, 65534);
    rename(OLDNAME, NEWNAME);
    fd = open(NEWNAME, O_CREAT|O_RDWR, 0666);
    write(fd, "cccc", 5);
    close(fd);

    fd = open(NEWNAME, O_RDONLY);
    read(fd, buf1, 100);
    close(fd);
    
    file = tmpfile();
    fwrite("cccc", 1, 5, file);
    fclose(file);

    file = fopen(NEWNAME, "r");
    fread(buf2, 1, 100, file);
    fclose(file);

    remove(NEWNAME);
    return 0;
}