#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <linux/limits.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdio.h>
#include <pwd.h>
#include <iomanip>

using namespace std;

/*  Global Var  */
struct proc_info{
    string cmd;
    string pid;
    string usr;
};


void error(string, int);
void Print(struct porc_info, string, string, string, string);
void PrintHead();

/*  Handle args             */
bool CheckCmd(string, string, string&);

/*  Handle file info        */
void FindCwdRoot(string, struct proc_info, string, string, string);
void FindExe(string, struct proc_info, string, string);
void FindMem(string, struct proc_info, string, string);
void FindFd(string, struct proc_info, string, string);

/*  Handle process Info     */
string FindUsr(string);

void error(string str, int error_no){
    cerr<<"Fail to "<<str<<", "<<strerror(error_no)<<endl;
    exit(EXIT_FAILURE);
}
void Print(struct proc_info proc, string fd, string type, string node, string name){
    cout<<setw(20)<<left<<proc.cmd;
    cout<<setw(10)<<left<<proc.pid;
    cout<<setw(20)<<left<<proc.usr;
    cout<<setw(10)<<left<<fd;
    cout<<setw(10)<<left<<type;
    cout<<setw(10)<<left<<node;
    cout<<setw(15)<<left<<name<<endl;
}
void PrintHead(){
    cout<<setw(20)<<left<<"COMMAND";
    cout<<setw(10)<<left<<"PID";
    cout<<setw(20)<<left<<"USER";
    cout<<setw(10)<<left<<"FD";
    cout<<setw(10)<<left<<"TYPE";
    cout<<setw(10)<<left<<"NODE";
    cout<<setw(15)<<left<<"NAME"<<endl;
}
string FindUsr(string dir){
    struct passwd *pws;
    struct stat statbuf;

    if(stat(dir.c_str(), &statbuf)<0)
        error("stat", errno);

    if((pws=getpwuid(statbuf.st_uid))==NULL)
        error("getpwuid", errno);
    return pws->pw_name;
}
bool CheckCmd(string dir, string regex, string &cmd){
    FILE *fp;
    ssize_t nbytes;
    size_t len = 0;
    char *line = NULL;

    if((fp=fopen(dir.c_str(), "r"))==NULL){
        if(errno==ENOENT || errno==EACCES)   return false;
        error("fopen", errno);
    }
    if((nbytes=getline(&line, &len, fp))<0)
        error("getline", errno);

    if(fclose(fp)==EOF)
        error("fclose", errno);

    size_t pos;
    if((pos=string(line).find(regex))!=string::npos){
        if((pos=string(line).find("\n"))!=string::npos)
            cmd = string(line).replace(pos, pos+1, "");
        return true;
    }
    else    return false;
}
void FindFd(string dir, struct proc_info proc, string regex, string type_regex){
    DIR *dp;
    struct dirent *dirp;
    struct stat statbuf;
    char buf[PATH_MAX];
    ssize_t nbytes;
    size_t pos;
    ino_t node;
    string fd, type, name;

    if((dp=opendir(dir.c_str()))==NULL){
        if(errno==EACCES){
            fd = "NOFD";
            type = "";
            name = dir + " (opendir: Permission denied)";
            Print(proc, fd, type, "", name); 
            return;
        }
        error("opendir", errno);
    }
    while((dirp = readdir(dp))!=NULL){
        if(isdigit(dirp->d_name[0])){
            if((nbytes=readlink((dir+"/"+string(dirp->d_name)).c_str(), buf, PATH_MAX))<0){
                if(errno==ENOENT)  return;
                else    error("readlink", errno);
            }
            if(stat((dir+"/"+string(dirp->d_name)).c_str(), &statbuf)<0)
                error("lstat", errno);

            switch(statbuf.st_mode & S_IFMT){
                case S_IFDIR:   type = "DIR";       break;
                case S_IFREG:   type = "REG";       break;
                case S_IFCHR:   type = "CHR";       break;
                case S_IFIFO:   type = "FIFO";      break;
                case S_IFSOCK:  type = "SOCK";      break;
                default:        type = "unknown";   break;
            }
            node = statbuf.st_ino;
            if(type=="FIFO")
                name = "pipe:[" + to_string(node) + "]";
            else if(type=="SOCK")
                name = "socket:[" + to_string(node) + "]";
            else
                name = string(dirname(buf));

            fd = string(dirp->d_name);
            if(access((dir+"/"+string(dirp->d_name)).c_str(), R_OK&W_OK)==0)
                fd += "u";
            else if(access((dir+"/"+string(dirp->d_name)).c_str(), R_OK)==0)
                fd += "r";
            else if(access((dir+"/"+string(dirp->d_name)).c_str(), W_OK)==0)
                fd += "w";

            if((pos=name.find(regex))!=string::npos && (pos=type.find(type_regex))!=string::npos)
                Print(proc, fd, type, to_string(node), name);
        }
    }
    if(closedir(dp)<0)
        error("closedir", errno);
}
void FindMem(string dir, struct proc_info proc, string regex, string type_regex){
    FILE *fp;
    size_t len = 0, pos;
    char *line = NULL, *token;
    string addr, perm, offset, device, node, name, type, fd = "mem";
    struct stat statbuf;
    
    if((fp=fopen(dir.c_str(), "r"))==NULL){
        if(errno==ENOENT || errno==EACCES)   return;
        error("fopen", errno);
    }
    if(stat(dir.c_str(), &statbuf)<0)   
        error("stat", errno);
    switch(statbuf.st_mode & S_IFMT){
        case S_IFDIR:   type = "DIR";       break;
        case S_IFREG:   type = "REG";       break;
        case S_IFCHR:   type = "CHR";       break;
        case S_IFIFO:   type = "FIFO";      break;
        case S_IFSOCK:  type = "SOCK";      break;
        default:        type = "unknown";   break;
    }

    while(getline(&line, &len, fp)!=EOF){
        if((token=strtok(line, " "))!=NULL)
            addr = string(token);
        if((token=strtok(NULL, " "))!=NULL)
            perm = string(token);
        if((token=strtok(NULL, " "))!=NULL)
            offset = string(token);
        if((token=strtok(NULL, " "))!=NULL)
            device = string(token);
        if((token=strtok(NULL, " "))!=NULL){
            if(node==string(token) || string(token)=="0")
                continue;
            node = string(token);
        }
        if((token=strtok(NULL, " "))!=NULL)
            name = string(token);  
        if((pos=name.find("\n"))!=string::npos)
            name = name.replace(pos, pos+1, "");

        if((pos=name.find("(deleted)"))!=string::npos){
            fd = "del";
            name += " (deleted)";
        }

        if((pos=name.find(regex))!=string::npos && (pos=type.find(type_regex))!=string::npos)
            Print(proc, fd, type, node, name); 
    }

    if(fclose(fp)==EOF)
        error("fclose", errno);

}
void FindExe(string dir, struct proc_info proc, string regex, string type_regex){
    char buf[PATH_MAX];
    struct stat statbuf;
    ssize_t nbytes;
    size_t pos;
    ino_t node;
    string type, name;   

    if((nbytes=readlink(dir.c_str(), buf, PATH_MAX))<0){
        if(errno==EACCES){
            type = "unknown";
            name = dir+" (readlink: Permission denied)";
            Print(proc, "exe", type, "", name);
        }
        else if(errno==ENOENT)  return;
        else error("readlink", errno);
    }
    else{
        if(stat(dir.c_str(), &statbuf)<0)   
            error("stat", errno);
        
        switch(statbuf.st_mode & S_IFMT){
            case S_IFDIR:   type = "DIR";       break;
            case S_IFREG:   type = "REG";       break;
            case S_IFCHR:   type = "CHR";       break;
            case S_IFIFO:   type = "FIFO";      break;
            case S_IFSOCK:  type = "SOCK";      break;
            default:        type = "unknown";   break;
        }
        node = statbuf.st_ino;
        if(type=="FIFO")
            name = "pipe:[" + to_string(node) + "]";
        else if(type=="SOCK")
            name = "socket:[" + to_string(node) + "]";
        else
            name = string(dirname(buf));

        if((pos=name.find(regex))!=string::npos && (pos=type.find(type_regex))!=string::npos)
            Print(proc, "exe", type, to_string(node), name);
    } 
}
void FindCwdRoot(string dir, struct proc_info proc, string fd, string regex, string type_regex){
    char buf[PATH_MAX];
    struct stat statbuf;
    ssize_t nbytes;
    size_t pos;
    ino_t node;
    string type, name;
    if((nbytes=readlink(dir.c_str(), buf, PATH_MAX))<0){
        if(errno==EACCES){
            type = "unknown";
            name = dir+" (readlink: Permission denied)";
            Print(proc, fd, type, "", name);
        }
        else if(errno==ENOENT)  return;
        else
            error("readlink", errno);
    }
    else{
        if(stat(dir.c_str(), &statbuf)<0)
            error("lstat", errno);
        type = "DIR";
        node = statbuf.st_ino;
        name = string(dirname(buf));
        if((pos=name.find(regex))!=string::npos && (pos=type.find(type_regex))!=string::npos)
            Print(proc, fd, type, to_string(node), name);
    } 
}
int main(int argc, const char *argv[]){
    
    DIR *proc_dp;
    struct dirent *dirp;
    string proc_dir = "/proc/", file_regex = "", type_regex = "", cmd_regex = "", cmd;

    for(int i=1; i<argc; i+=2){
        if(strcmp(argv[i], "-c")==0)
            cmd_regex = string(argv[i+1]);
        else if(strcmp(argv[i], "-t")==0){
            type_regex = string(argv[i+1]);
            if(type_regex!="REG" && type_regex!="CHR" && 
                type_regex!="DIR" && type_regex!="FIFO" && 
                type_regex!="SOCK" && type_regex!="unknown"){
                cerr<<"Invalid TYPE option.\n";
                exit(EXIT_FAILURE);
            }
        }
        else if(strcmp(argv[i], "-f")==0)
            file_regex = string(argv[i+1]);
    }

    if((proc_dp = opendir(proc_dir.c_str()))==NULL)
        error("opendir", errno);


    PrintHead();
    while((dirp = readdir(proc_dp))!=NULL){
        if(!isalpha(dirp->d_name[0]) && isdigit(dirp->d_name[0])){
            if(CheckCmd(proc_dir + string(dirp->d_name) + "/comm", cmd_regex, cmd)){
                struct proc_info proc;
                proc.cmd = cmd;
                proc.pid = string(dirp->d_name);
                proc.usr = FindUsr(proc_dir + string(dirp->d_name) + "/comm");

                FindCwdRoot(proc_dir+proc.pid+"/cwd", proc, "cwd", file_regex, type_regex);
                FindCwdRoot(proc_dir+proc.pid+"/root", proc, "root", file_regex, type_regex);
                FindExe(proc_dir+proc.pid+"/exe", proc, file_regex, type_regex);
                FindMem(proc_dir+proc.pid+"/maps", proc, file_regex, type_regex);
                FindFd(proc_dir+proc.pid+"/fd", proc, file_regex, type_regex);
            }
        }
    }

    if(closedir(proc_dp)<0)
        error("closedir", errno);
    return 0;
}