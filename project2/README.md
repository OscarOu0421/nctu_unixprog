# Project2 - Monitor File Activities of Dynamically Linked Programs

## Introduction
In this homework, we are going to practice library injection and API hijacking. Please implement a simple logger program that is able to show file-access-related activities of an arbitrary binary running on a Linux operating system. You have to implement your logger in two parts. One is a logger program that prepares the runtime environment to inject, load, and execute a monitored binary program. The other is a shared object that can be injected into a program by the logger using ```LD_PRELOAD```. You have to dump the library calls as well as the passed parameters and the returned values. Please check the "Requirements" section for more details.

## Program Argument
```
usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]
    -p: set the path to logger.so, default = ./logger.so
    -o: print output to file, print to "stderr" if no file specified
    --: separate the arguments for logger and for the command
```
The message should be displayed if an invalid argument is passed to the logger.

## Monitored file access activities
The list of monitored library calls is shown below. It covers several functions we have introduced in the class.
```
chmod   chown   close   creat   fclose  fopen   fread   fwrite
open    read    remove  rename  tmpfile write
```

## Output
A sample output of the homework is given below. More examples can be found in the "Sample Output" section.
```
$ ./logger ./sample
[logger] creat("/home/ta/hw2/tmp/aaaa", 600) = 3
[logger] chmod("/home/ta/hw2/tmp/aaaa", 666) = 0
[logger] chown("/home/ta/hw2/tmp/aaaa", 65534, 65534) = -1
[logger] rename("/home/ta/hw2/tmp/aaaa", "/home/ta/hw2/tmp/bbbb") = 0
[logger] open("/home/ta/hw2/tmp/bbbb", 1101, 666) = 4
[logger] write("/home/ta/hw2/tmp/bbbb", "cccc.", 5) = 5
[logger] close("/home/ta/hw2/tmp/bbbb") = 0
[logger] open("/home/ta/hw2/tmp/bbbb", 000, 000) = 4
[logger] read("/home/ta/hw2/tmp/bbbb", "cccc.", 100) = 5
[logger] close("/home/ta/hw2/tmp/bbbb") = 0
[logger] tmpfile() = 0x55c418842670
[logger] fwrite("cccc.", 1, 5, "/tmp/#14027911 (deleted)") = 5
[logger] fclose("/tmp/#14027911 (deleted)") = 0
[logger] fopen("/home/ta/hw2/tmp/bbbb", "r") = 0x55c418842670
[logger] fread("cccc.", 1, 100, "/home/ta/hw2/tmp/bbbb") = 5
[logger] fclose("/home/ta/hw2/tmp/bbbb") = 0
[logger] remove("/home/ta/hw2/tmp/bbbb") = 0
sample done.
[logger] write("/dev/pts/19", "sample done...", 14) = 14
```
