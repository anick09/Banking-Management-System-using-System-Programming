#include <sys/types.h>
#include "sys/stat.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "stdlib.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include "math.h"


typedef struct account
{	
	char name1[20], name2[20];
	int active; 
	long acc_no; 
	char password[20]; 
	int usertype;
	double balance; 
	char operation[10];   
	int result; 
}account;

typedef struct transaction 
{	 
	double amount; 
	double balance;
	char operation[10]; 
	    
}trans;

account sendtoserver(account);

int validatestring(char * ch, char *inp) {
	
	if(strcmp(ch, "INT") == 0) {
		for (int i = 0; i < strlen(inp); i++)
	      	if(!isdigit(inp[i])){
	      		printf("\nInvalid Input(Numbers allowed only)!!!\n");
	      		//getch();
	      		return 0;	
	      	} 
	 	return 1;
	}

}

int main(int argc, char const *argv[])
{
	account acc; 
	char inp[100];
	strcpy(acc.operation,"LOGIN") ; 
	printf("Enter usertype 1.Admin \n 2)Normal(single) account \n 3)Joint Account \n");
	scanf("%d",&acc.usertype); 
	//////////////////////////////////////////////////////// Admin ///////////////////////////////////////////////
 	if(acc.usertype == 1)
	{	
		printf("Enter password : \n");
		scanf("%s",acc.password);
		acc = sendtoserver(acc); 
		if(acc.result==1) printf("Login Success\n");
		else { printf("Login Failure\n"); exit(0); } 

		printf("Enter 1. To Create\n 2.To Search\n 3.To Modify\n 4.To Delete \n"); 
		int choice; 
		scanf("%d",&choice); 
		switch(choice)
		{
			case 1 : 
				{account newacc; 
					int f=0;
				newacc.active = 1; 
				printf("Type of account?\n 2.Normal\n 3.Joint \n");
				scanf("%d",&newacc.usertype); 
				//newacc.usertype++ ; 
				if(newacc.usertype == 2) { printf("Enter Name : \n"); scanf("%s",newacc.name1); } 
				else { printf("Enter Name 1 and Name 2: \n"); scanf("%s",newacc.name1); scanf("%s",newacc.name2 ); }
				
				while(1){
				printf("Enter new 5 digit account number : \n");
				//scanf("%ld",&newacc.acc_no);
				scanf(" %20[^\n]",inp); 
				if(validatestring("INT", inp) == 0) f=1;
				else newacc.acc_no = atol(inp);

				if(f==1 || newacc.acc_no > 99999 ||newacc.acc_no < 10000) {printf("Incorrect account number format...Enter again\n"); f=0; }
				else break;
				} 
				printf("Enter password : \n");
				scanf("%s",newacc.password);
				newacc.balance = 0; 
				strcpy(newacc.operation,"CREATE"); 
				newacc = sendtoserver(newacc); 
				if(newacc.result == 1) printf("Success\n");
				else printf("Failure. Account number already exist\n");} 
			break;

			case 2 :

				 {account newacc;
				 printf("Enter account number want to search :\n");
				 scanf("%ld",&newacc.acc_no); 
				 strcpy(newacc.operation,"SEARCH");  
				 newacc = sendtoserver(newacc); 
				 if(newacc.result == 1){ 
				 printf("Account balance : %0.2lf\n", newacc.balance); 
				 if(newacc.usertype == 2){ printf("User type : Normal\n"); 
				 	printf("User : %s\n", newacc.name1); 
				 } 
				 if(newacc.usertype == 3){ 
				 	printf("User type : Joint\n");  
				 	printf("Users : %s, %s\n", newacc.name1, newacc.name2); 
				 } 
				 } 
				 else printf("No account found\n");}
				 
			break; 
			case 3 : 
				{
					account newacc; 
					printf("Enter account number want to modify :\n");
				 	scanf("%ld",&newacc.acc_no);
				 	if(newacc.acc_no != 11111) {
					strcpy(newacc.operation,"SEARCH"); 
					newacc = sendtoserver(newacc); 
					if(newacc.result == 1)
					{
						int choice; 
						if(newacc.usertype == 2)
							{ 	printf("Change account to joint ? Press 1) YES \n 2)NO \n"); 
								scanf("%d",&choice);
								if(choice == 1) 
								{
									printf("Enter second name :\n"); scanf("%s",newacc.name2);
									newacc.usertype = 3; 
								}
							} 						 
 						
 						printf("Enter new password : \n"); scanf("%s",newacc.password); 
 						strcpy(newacc.operation,"MODIFY"); 
 						newacc = sendtoserver(newacc); 
 						printf("Change Success\n"); 
					}
					else printf("Account not found \n");  }

					else{
						printf("Enter admin password : \n"); 
						scanf("%s",newacc.password); 
						strcpy(newacc.operation,"MODIFY");
						newacc = sendtoserver(newacc); 
						printf("Change Success\n"); } 

				}
			break; 
			case 4 :
				{
					account newacc; 
					printf("Enter account to delete :\n");
				 	scanf("%ld",&newacc.acc_no);
					strcpy(newacc.operation,"SEARCH"); 
					newacc = sendtoserver(newacc); 
					if(newacc.result == 1)
					{
						strcpy(newacc.operation,"DELETE"); 
						newacc.active = 0; 
						newacc = sendtoserver(newacc); 
						if(newacc.result == 1) printf("Delete Success\n");
						else printf("Delete Failed\n");
					}
					else printf("Account not found\n");

				}
			default: printf("Incorrect Choice\n");
			break;
		} 

	}
//////////////////////////////////////////////// Joint Users /////////////////////////////////////////
	else if(acc.usertype == 3)	{	

		
		printf("Enter account number :\n");
		scanf(" %20[^\n]",inp); 
		if(validatestring("INT", inp) == 0) exit(1);
		else acc.acc_no = atol(inp);

		//scanf(" %ld",&acc.acc_no);
		
		printf("Enter password : \n");
		scanf(" %[^\n]",acc.password);
		acc = sendtoserver(acc);

		if(acc.result==1) printf("Login Success\n");
		else if(acc.result==0) { printf("Login Failure\n"); exit(0);} 
		else if(acc.result==-1) { printf("Account not found\n"); exit(0); } 

		//strcpy(acc.operation,"LOCK");
		//acc = sendtoserver(acc); 
		//if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 

		printf("Enter 1)Deposit 2)Withdraw 3)Balance  4)View Details 5)Change Password 6)Exit \n"); 
		int choice; 
		scanf("%d",&choice); 
		switch(choice)
		{
			case 1: 
			{
				strcpy(acc.operation,"LOCK");
				acc = sendtoserver(acc); 
				if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 
				
				strcpy(acc.operation,"DEPOSIT");
				printf("Enter deposit amount : \n");
				scanf(" %20[^\n]",inp); 
				if(validatestring("INT", inp) == 0) break;
				else acc.balance = atof(inp);
				//scanf("%lf",&acc.balance);
				
				if(acc.balance<0){
					strcpy(acc.operation,"UNLOCK");
					sendtoserver(acc); 
					break;
				}
					
				acc = sendtoserver(acc); 
				printf("New Balance : %0.2lf\n",acc.balance);
				
				strcpy(acc.operation,"UNLOCK");
				sendtoserver(acc);   
				
			}
			break; 
			case 2: 
			{
				strcpy(acc.operation,"LOCK");
				acc = sendtoserver(acc); 
				if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 
				
				
				strcpy(acc.operation,"WITHDRAW"); 
				printf("Enter amount to withdraw : \n");
				scanf(" %20[^\n]",inp); 
				if(validatestring("INT", inp) == 0) break;
				else acc.balance = atof(inp);
				//scanf("%lf",&acc.balance);
				
				if(acc.balance<0){
					strcpy(acc.operation,"UNLOCK");
					sendtoserver(acc);
					break;
				}
					 
				acc = sendtoserver(acc); 
				if(acc.result == 1) printf("New balance : %0.2lf\n",acc.balance );
				else printf("Insufficient funds\n");
				
				strcpy(acc.operation,"UNLOCK");
				sendtoserver(acc);   
				 
			}
			break; 
			case 3: 
			{	
				strcpy(acc.operation,"RLOCK");
				acc = sendtoserver(acc); 
				if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 
				
				strcpy(acc.operation,"BALANCE"); 
				acc = sendtoserver(acc); 
				printf("Balance : %0.2lf\n", acc.balance);
				
				strcpy(acc.operation,"UNLOCK");
				sendtoserver(acc); 
			}
			break;

			case 4: 
			{
				strcpy(acc.operation,"RLOCK");
				acc = sendtoserver(acc); 
				if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 
			
				strcpy(acc.operation, "DETAILS"); 
				sendtoserver(acc); 
				
				strcpy(acc.operation,"UNLOCK");
				sendtoserver(acc); 
			}
			break; 
			case 5: 
			{
				strcpy(acc.operation,"LOCK");
				acc = sendtoserver(acc); 
				if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 
				
				
				strcpy(acc.operation,"MODIFY"); 
				printf("Enter new password : \n");
				scanf(" %[^\n]",acc.password); 
				printf("Confirm password : \n");
				char buf[20]; 
				scanf(" %[^\n]",buf); 
				if(strcmp(acc.password,buf)!=0) { printf("Passwords do not match\n"); exit(0); }
				else acc = sendtoserver(acc);
				if(acc.result==1)printf("Password Change Success\n");
				else printf("Password change failed\n"); 
				
				strcpy(acc.operation,"UNLOCK");
				sendtoserver(acc);   
				
			}
			break;
			case 6: break;
			
			default: {printf("Incorrect choice\n");break;}

		}
		//strcpy(acc.operation,"UNLOCK");
		//sendtoserver(acc); 

	}
	else if(acc.usertype == 2){
		printf("Enter account number :\n");
		scanf(" %20[^\n]",inp); 
		if(validatestring("INT", inp) == 0) exit(1);
		else acc.acc_no = atol(inp);

		//scanf(" %ld",&acc.acc_no);
		printf("Enter password : \n");
		scanf(" %[^\n]",acc.password);
		acc = sendtoserver(acc);

		if(acc.result==1) printf("Login Success\n");
		else if(acc.result==0) { printf("Login Failure\n"); exit(0);} 
		else if(acc.result==-1) { printf("Account not found\n"); exit(0); } 

		strcpy(acc.operation,"LOCK");
		acc = sendtoserver(acc); 
		if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 

		printf("Enter 1)Deposit 2)Withdraw 3)Balance  4)View Details 5)Change Password other)Exit \n"); 
		int choice; 
		scanf("%d",&choice); 
		switch(choice)
		{
			case 1: 
			{
				strcpy(acc.operation,"DEPOSIT");
				printf("Enter deposit amount : \n");
				scanf(" %20[^\n]",inp); 
				if(validatestring("INT", inp) == 0) break;
				else acc.balance = atof(inp);

				//scanf("%lf",&acc.balance);
				if(acc.balance<0)
				{printf("Invalid Amount\n");	break;}
				acc = sendtoserver(acc); 
				printf("New Balance : %0.2lf\n",acc.balance);  
				
			}
			break; 
			case 2: 
			{
				strcpy(acc.operation,"WITHDRAW"); 
				printf("Enter amount to withdraw : \n");
				
				scanf(" %20[^\n]",inp); 
				if(validatestring("INT", inp) == 0) break;
				else acc.balance = atof(inp);
				
				//scanf("%lf",&acc.balance);
				if(acc.balance<0)
				{printf("Invalid Amount\n");	break;} 
				acc = sendtoserver(acc); 
				if(acc.result == 1) printf("New balance : %0.2lf\n",acc.balance );
				else printf("Insufficient funds\n"); 
			}
			break; 
			case 3: 
			{
				strcpy(acc.operation,"BALANCE"); 
				acc = sendtoserver(acc); 
				printf("Balance : %0.2lf\n", acc.balance);
			}
			break;

			case 4: 
			{
				strcpy(acc.operation, "DETAILS"); 
				sendtoserver(acc); 
			}
			break; 
			case 5: 
			{
				strcpy(acc.operation,"MODIFY"); 
				printf("Enter new password : \n");
				scanf(" %[^\n]",acc.password); 
				printf("Confirm password : \n");
				char buf[20]; 
				scanf(" %[^\n]",buf); 
				if(strcmp(acc.password,buf)!=0) { printf("Passwords do not match\n"); exit(0); }
				else acc = sendtoserver(acc);
				if(acc.result==1)printf("Password Change Success\n");
				else printf("Password change failed\n"); 
			}
			break; 
			default: break;

		}
		strcpy(acc.operation,"UNLOCK");
		sendtoserver(acc); 
	}
	else{
		printf("Incorrect choice\n");
	}


	exit(0); 
}

account sendtoserver(account a)
{	
	struct sockaddr_in server; 
	int sd; ; 
	sd = socket(AF_UNIX, SOCK_STREAM, 0); 

	server.sin_family = AF_UNIX; 
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(7073); // <---------------------------------------- change port  

	connect(sd, (void *)(&server), sizeof(server));


	write(sd, &a, sizeof(a)); 	
	read(sd, &a, sizeof(a));

	if(strcmp(a.operation,"DETAILS")==0) 
	{
		printf("Account number : %ld\n",a.acc_no );
		printf("Operation.................Amount.................Balance\n");
		printf("--------------------------------------------------------\n");
		trans t; 
		int size = read(sd, &t, sizeof(trans));	
		
		while(size!=0) 
		{
			//printf("%d\n",size );
			printf("%s.................%0.2lf.................%0.2lf \n",t.operation,t.amount,t.balance);
			size = read(sd, &t, sizeof(trans));	
		}
	}
 
	
	return a;  
	
}
