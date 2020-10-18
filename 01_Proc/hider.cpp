#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <syscall.h>
#include <string.h>

// https://github.com/lxc/lxc/blob/master/src/lxc/initutils.c

/*
 * Sets the process title to the specified title. Note that this may fail if
 * the kernel doesn't support PR_SET_MM_MAP (kernels <3.18).
 */
int setproctitle(char *title)
{
    FILE *f = NULL;
    int i, fd, len;
    char *buf_ptr, *tmp_proctitle;
    char buf[4096];
    int ret = 0;
    ssize_t bytes_read = 0;
    static char *proctitle = NULL;

    /*
     * We don't really need to know all of this stuff, but unfortunately
     * PR_SET_MM_MAP requires us to set it all at once, so we have to
     * figure it out anyway.
     */    
    unsigned long start_data, end_data, start_brk, start_code, end_code,
    start_stack, arg_start, arg_end, env_start, env_end, brk_val;
    struct prctl_mm_map prctl_map;

    f = fopen("/proc/self/stat", "r");
    if (!f) {
      fprintf(stderr, "fopen(stat): '%m' (%d)\n", errno);
      return -1;
    }

    fd = fileno(f);
    if (fd < 0) {
      fprintf(stderr, "fileno(%p): '%m' (%d)\n", f, errno);
      fclose(f);
      return -1;
    }

    bytes_read = read(fd, buf, sizeof(buf) - 1);
    if (bytes_read <= 0) {
      fprintf(stderr, "read(): '%m' (%d)\n", errno);
      fclose(f);
      return -1;
    }

    buf[bytes_read] = '\0';

    /* Skip the first 25 fields, column 26-28 are start_code, end_code,
     * and start_stack */
    buf_ptr = strchr(buf, ' ');
    for (i = 0; i < 24; i++) {
          if (!buf_ptr) {
            fclose(f);
            return -1;
          }
      buf_ptr = strchr(buf_ptr + 1, ' ');
    }
    if (!buf_ptr) {
      fclose(f);
      return -1;
    }

    i = sscanf(buf_ptr, "%lu %lu %lu", &start_code, &end_code, &start_stack);
    if (i != 3) {
      fclose(f);
      return -1;
    }

    /* Skip the next 19 fields, column 45-51 are start_data to arg_end */
    for (i = 0; i < 19; i++) {
      if (!buf_ptr) {
        fclose(f);
        return -1;
      }
      buf_ptr = strchr(buf_ptr + 1, ' ');
    }

    if (!buf_ptr) {
      fclose(f);
      return -1;
    }

    i = sscanf(buf_ptr, "%lu %lu %lu %*u %*u %lu %lu", &start_data,
           &end_data, &start_brk, &env_start, &env_end);
    if (i != 5) {
      fclose(f);
      return -1;
    }

    /* Include the null byte here, because in the calculations below we
     * want to have room for it. */
    len = strlen(title) + 1;

    tmp_proctitle = (char*)realloc(proctitle, len);
    if (!tmp_proctitle) {
      fclose(f);
      return -1;
    }

    proctitle = tmp_proctitle;

    arg_start = (unsigned long)proctitle;
    arg_end = arg_start + len;

    brk_val = syscall(__NR_brk, 0);

    prctl_map = (prctl_mm_map){
        .start_code = start_code,
        .end_code = end_code,
        .start_data = start_data,
        .end_data = end_data,
        .start_brk = start_brk,
        .brk = brk_val,
        .start_stack = start_stack,
        .arg_start = arg_start,
        .arg_end = arg_end,
        .env_start = env_start,
        .env_end = env_end,
        .auxv = NULL,
        .auxv_size = 0,
        .exe_fd = static_cast<unsigned int>(-1),
    };

    ret = prctl(PR_SET_MM, PR_SET_MM_MAP, &prctl_map,
            sizeof(prctl_map), 0);
    if (ret == 0)
        (void)strncpy((char *)arg_start, title, len);
    else
        fprintf(stderr, "Failed to set cmdline\n");

    fclose(f);

    return ret;
}

int prctl_routine(char* name)
{
	errno = 0;
	int fd = open(name, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return EXIT_FAILURE;
	}
	int ret = prctl(PR_SET_MM, PR_SET_MM_EXE_FILE, fd, 0, 0);
	if(ret < 0)
	{
		perror("prctl");
		std::cout << "errno: " << std::endl;
	}
	close(fd);
	return 0;
}

int main(int argc, char* argv[])
{
	std::cout << "argc = " << argc << " argv:\n";
	for(int i(0); i < argc; ++i)
		std::cout << argv[i] << std::endl;

	pid_t pid = getpid();
	std::cout << "pid = " << pid << std::endl;
	
	setproctitle(argv[1]);
	sleep(1000);

	return 0;
}