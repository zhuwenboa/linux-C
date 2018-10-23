#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc, char * argv[])
{
	struct stat buf;
	int mode, mode_u, mode_g, mode_o;
	//检查参数个数
	if(argc != 2)
	{
		printf("Usage: my_stat <filename>\n");
		exit(0);
	}
	//获取文件权限属性
	if(stat(argv[1], &buf) == -1)
	{
		perror("stat:");
		exit(1);
	}

	/*转换权限*/
	printf("mode is:");
	mode = buf.st_mode;
	if(S_ISREG(mode))
		printf("-");
	else if(S_ISLNK(mode))
		printf("l");
	else if(S_ISDIR(mode))
		printf("d");
	else if(S_ISCHR(mode))
		printf("c");
	else if(S_ISBLK(mode))
		printf("b");
	else if(S_ISFIFO(mode))
		printf("f");
	else if(S_ISSOCK(mode))
		printf("s");
	if(mode & S_IRUSR)
		printf("r");
	else
		printf("-");
	if(mode & S_IWUSR)
		printf("w");
	else
		printf("-");
	if(mode & S_IXUSR)
		printf("x");
	else
		printf("-");
	if(mode & S_IRGRP)
		printf("r");
	else
		printf("-");
	if(mode & S_IWGRP)
		printf("w");
	else
		printf("-");
	if(mode & S_IXGRP)
		printf("x");
	else
		printf("-");
	if(mode & S_IROTH)
		printf("r");
	else
		printf("-");
	if(mode & S_IWOTH)
		printf("w");
	else
		printf("-");
	if(mode & S_IXOTH)
		printf("x");
	else
		printf("-");
	printf("\n");

	//打印文件属性
	printf("device is: %ld\n", buf.st_dev);
	printf("inode is: %ld\n", buf.st_ino);
	printf("mode is:%d\n", mode);
	printf("number of hand links is: %ld\n", buf.st_nlink);
	printf("user ID of owner is: %d\n", buf.st_uid);
	printf("group ID of owner is: %d\n", buf.st_gid);
	printf("device tyoe (if inode device) is: %ld\n", buf.st_rdev);

	printf("total size, in bytes is: %ld\n", buf.st_size);
	printf("blocksize for filesystem I/O is: %ld\n", buf.st_blksize);
	printf("number of blocks allocated is: %ld\n", buf.st_blocks);

	printf("time of last aceess is: %s\n", ctime(&buf.st_atime));
	printf("time of last modification is: %s\n", ctime(&buf.st_mtime));
	printf("time of last change is: %s\n", ctime(&buf.st_ctime));

	return 0;
}


