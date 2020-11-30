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
    NUMBER     = 1,
    NOT_NUMBER = 0,
    BASE       = 10,
    BUF_SIZE   = 2 << 9,
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
int ParseStat(char* buf, Stat * stat_info);
void PrintStat(const Stat* stat_info);

int ReadDir(const char* path)
{
    DIR* d = NULL;
    struct dirent * dir = NULL;

    char* buf = (char*)calloc(BUF_SIZE, sizeof(*buf));
    if(buf == NULL)
    {
        printf("Calloc failed!\n");
        return EXIT_FAILURE;
    }

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

            int gather_ret = GatherInfo(dir->d_name, buf);
            if(gather_ret != 0)
            {
                free(buf);
                return EXIT_FAILURE;
            }
            Stat stat_info = {};
            if(ParseStat(buf, &stat_info) != 0)
            {
                free(buf);
                return EXIT_FAILURE;
            }
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

int ParseStat(char* buf, Stat * stat_info)
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
        return EXIT_FAILURE;
    }
    char* token = strtok(buf, " ");
    
    int counter = 0;
    while(token != NULL)
    {
        strcat(stat_info_str[counter], token);
        if(stat_info_str[counter] == NULL)
        {
            perror("strcat");
            return EXIT_FAILURE;
        }
        token = strtok(NULL, " ");
        counter++;
    }
    char* endptr = NULL;
    pid_t pid = strtol(stat_info_str[0], &endptr, 10);
    if (errno != 0) {
        perror("strtol");
        return EXIT_FAILURE;
    }

    if (endptr == stat_info_str[0]) {
        fprintf(stderr, "No digits were found\n");
        return EXIT_FAILURE;
    }

    stat_info->pid   = pid;
    stat_info->comm  = stat_info_str[1];
    stat_info->state = stat_info_str[2][0];
    stat_info->utime = stat_info_str[13];
    return 0;
}

void PrintStat(const Stat* stat_info)
{
    printf("%d\t%s\t%c\t%s\n", stat_info->pid, stat_info->comm, stat_info->state, stat_info->utime);
}

int CreateFileName(char* path, char* buf, char* full_stat_path)
{
    if((path == NULL) || (buf == NULL))
    {
        printf("Null ptr passed!\n");
        return EXIT_FAILURE;
    }
    const char stat_path[] = "/stat";

    strncat(full_stat_path, "/proc/", 6);
    if(full_stat_path == NULL)
    {
        perror("strncat");
        return EXIT_FAILURE;
    }

    strcat(full_stat_path, path);
    if(full_stat_path == NULL)
    {
        perror("strcat");
        return EXIT_FAILURE;
    }

    strncat(full_stat_path, stat_path, 5);
    if(full_stat_path == NULL)
    {
        perror("strncat");
        return EXIT_FAILURE;
    }
    return 0;
}

int GatherInfo(char* path, char* buf)
{
    if(path == NULL)
    {
        printf("Null ptr passed!\n");
        return EXIT_FAILURE;
    }
    char* full_stat_path = (char*)calloc(BUF_SIZE, sizeof(*full_stat_path));
    if(full_stat_path == NULL)
    {
        perror("calloc");
        return EXIT_FAILURE;
    }

    if(CreateFileName(path, buf, full_stat_path) != 0)
    {
        free(full_stat_path);
        return EXIT_FAILURE;
    }

    int fd = 0;
    if(OpenFile(full_stat_path, &fd) == EXIT_FAILURE)
    {
        free(full_stat_path);
        return EXIT_FAILURE;
    }
    int read_size = read(fd, buf, BUF_SIZE);
    if(read_size < 0)
    {
        perror("read");
        return EXIT_FAILURE;
    }
    free(full_stat_path);
    return 0;
}


static int ps()
{
	const char path[] = "/proc";
    if(ReadDir(path) != 0){
        return EXIT_FAILURE;
    }

	return 0;
}

int main(int argc, char* argv[])
{
	ps();

	return 0;
}