#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/prctl.h>
#include <fcntl.h>

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
	

	prctl_routine(argv[1]);
	sleep(1000);

	return 0;
}