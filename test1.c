#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 3 //openfile_num

int main ()
{
	int fd1 = 0;
	int fd2 = 0;
	int fd3 = 0;
	unsigned int word_maxcount = 20;
	unsigned char buff_name[BUFFER_SIZE][word_maxcount];
	unsigned long buff_value[BUFFER_SIZE];
	struct stat buf;
	int retu = 0;
	
	printf("<<USER>>\n");

	if((fd1 = open("testfile1.txt", O_RDONLY)) < 0) {
		perror("ファイルオープン失敗\n");
		return 1;
	}
	else{
		printf("open'testfile1.txt':successed\n");
		retu = stat("testfile1.txt", &buf);
		if(retu < 0){
			return 1;
		}
		printf("inode_num:%d\n", buf.st_ino);
	}
	if((fd2 = open("testfile2.txt", O_RDONLY)) < 0) {
		perror("ファイルオープン失敗\n");
		return 1;
	}
	else{
		printf("open'testfile2.txt':successed\n");
		retu = stat("testfile2.txt", &buf);
		if(retu < 0){
			return 1;
		}
		printf("inode_num:%d\n", buf.st_ino);
	}
	if((fd3 = open("testfile3.txt", O_RDONLY)) < 0) {
		perror("ファイルオープン失敗\n");
		return 1;
	}
	else{
		printf("open'testfile3.txt':successed\n");
		retu = stat("testfile3.txt", &buf);
		if(retu < 0){
			return 1;
		}
		printf("inode_num:%d\n", buf.st_ino);
	}
	unsigned int op_file_num = 3;
	long i, ret;
	unsigned long long from, to, diff;

	for(i = 0; i < 2; i++){
		//get overhead
		__asm__ volatile("rdtsc" : "=A" (from));
		__asm__ volatile("rdtsc" : "=A" (to));
		diff = to - from;

		//get counter
		__asm__ volatile("rdtsc" : "=A" (from));

		//ret = testsyscall(NULL); "351" is the systemcall number of testsyscall()
		__asm__ volatile("int $0x80" : "=a" (ret) : "0" (351),"b" (buff_value),"c" (buff_name),"d" (op_file_num) );

		//get counter
		__asm__ volatile("rdtsc" : "=A" (to));
		printf("\n<<KERNEL>>\n");
		printf("Retval: %d, Clock: %u\n", ret, (to - from - diff));

		int j = 0;
		for(j=0 ;j<3 ;j++){
			printf("openfile[%d]::%s:%lu\n", j+1, buff_name[j], buff_value[j]);
		}
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return(0);
}
