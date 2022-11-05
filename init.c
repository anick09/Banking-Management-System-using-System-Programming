#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "string.h"

struct account
{
	 
	char name1[20], name2[20]; 
	int active; 
	long acc_no; 
	char password[20]; 
	int usertype;
	double balance;  
	char action[10]; 
	int result;  
}a;

int main(int argc, char const *argv[])
{

	a.acc_no = 11111;
	a.active=1;
	strcpy(a.name1,"admin");
	strcpy(a.password,"1234");
	a.balance=1000;
	a.usertype=1;
	

	
	int fd = open("account", O_CREAT |O_TRUNC|O_RDWR, 0644) ;
	write(fd, &a, sizeof(a)); 


	close(fd);
	
	return 0;
}
