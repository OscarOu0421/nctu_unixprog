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
