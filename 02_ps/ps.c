#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>


enum COMPONENTS
{
    NUMBER = 1,
    NOT_NUMBER = 0,
    BUF_SIZE = 2 << 9,
    FIELDS_NUM = 52
};

typedef struct
{
    pid_t pid;
    char* comm;
    char  state;
    char* utime;
} Stat;

int IsNumber(const char* str);
int GatherInfo(char* path, char* buf);
Stat ParseStat(char* buf);
void PrintStat(const Stat* stat_info);

int ReadDir(const char* path)
{
    DIR* d = NULL;
    struct dirent * dir = NULL;

    char* buf = (char*)calloc(BUF_SIZE, sizeof(*buf));

    d = opendir(path);
    if(d)
    {
        printf("Pid\tcomm\t\tstate\tstarttime\n");
        while((dir = readdir(d)) != NULL)
        {
            if(dir->d_type != DT_DIR)
                continue;
            if(!IsNumber(dir->d_name))
                continue;
            // printf("%s\n", dir->d_name);

            int gather_ret = GatherInfo(dir->d_name, buf);
            if(gather_ret != 0)
            {
                free(buf);
                return EXIT_FAILURE;
            }
            Stat stat_info = ParseStat(buf);
            PrintStat(&stat_info);
        }
    }
    else
    {
        perror("opendir");
        return EXIT_FAILURE;
    }
    free(buf);
    return 0;
}

int IsNumber(const char* str)
{
    char* ptr = NULL;
    strtol(str, &ptr, 10);
    if(*ptr)
    {
        return NOT_NUMBER;
    }
    else
    {
        return NUMBER;
    }
}

int OpenFile(const char* filename, int* fd)
{
    assert(fd != NULL);
    errno = 0;
    mode_t mode = S_IRUSR | S_IRGRP;
    *fd = open(filename, O_RDONLY | O_CREAT, mode);
    if(*fd < 0)
    {
        // std::cout << "Open failed!" << std::endl;
        perror("open");
        return EXIT_FAILURE;
    }
    else
    {
        // std::cout << "open succeed, fd = " << *fd << std::endl;
        return *fd;
    }
}

Stat ParseStat(char* buf)
{
    // Stat stat_info = {};
    char** stat_info_str = (char**)calloc(FIELDS_NUM, sizeof(*stat_info_str));
    for(int i = 0; i < FIELDS_NUM; ++i)
    {
        stat_info_str[i] = (char*)calloc(_POSIX_PATH_MAX, sizeof(stat_info_str[0]));
    }

    if(stat_info_str == NULL)
    {
        printf("Calloc failed!\n");
        /*return Stat;*/
    }
    char* token = strtok(buf, " ");
    // check
    
    int counter = 0;
    while(token != NULL)
    {
        strcat(stat_info_str[counter], token);
        token= strtok(NULL, " ");
        // check
        
        counter++;
    }
    char* endptr = NULL;
    pid_t pid = strtol(stat_info_str[0], &endptr, 10);
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (endptr == stat_info_str[0]) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    Stat stat_info = {
        .pid = pid,
        .comm = stat_info_str[1],
        .state = stat_info_str[2][0],
        .utime = stat_info_str[13]
    };
    return stat_info;
}

void PrintStat(const Stat* stat_info)
{
    printf("%d\t%s\t%c\t%s\n", stat_info->pid, stat_info->comm, stat_info->state, stat_info->utime);
}

const char stat_path[] = "/stat";
int GatherInfo(char* path, char* buf)
{
    if(path == NULL)
    {
        printf("Null ptr passed!\n");
        return EXIT_FAILURE;
    }


    char* full_stat_path = (char*)calloc(BUF_SIZE, sizeof(*full_stat_path));
    strncat(full_stat_path, "/proc/", 6);
    strcat(full_stat_path, path);
    strncat(full_stat_path, stat_path, 5);
    // char* buf = (char*)calloc(1024);
    if(full_stat_path == NULL)
    {
        perror("strncat");
        return EXIT_FAILURE;
    }
    // printf("full_stat_path: %s\n", full_stat_path);
    int fd = 0;
    if(OpenFile(full_stat_path, &fd) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    int read_size = read(fd, buf, BUF_SIZE);
    if(read_size < 0)
    {
        perror("read");
        return EXIT_FAILURE;
    }
    //printf("read_size = %d\n", read_size);
    // printf("buf: %s\n", buf);
    return 0;
}


static int ps()
{
	const char path[] = "/proc";
	
    ReadDir(path);
	// read dir +
	// determine if dir is process (pid) + 
	// read pid/stat +
    // parse buf +
	return 0;
}

int main()
{
	ps();

	return 0;
}