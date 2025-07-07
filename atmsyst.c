//link section
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
//defined constants 
#define USD_C 81.35
#define GBP_C 100.41
#define CAD_C 60.37
#define AED_C 22.15
#define MYR_C 18.88
//global variables
char uid[100]="\0", pwd[100]="\0", p[5]="\0", np[5]="\0", curr[4]="\0", bals[100]="\0", c[1000]="\0", temp1[1000]="\0", temp2[1000]="\0", temp3[1000]="\0", temp4[1000]="\0", req[500]="\0", real[500]="\0", check[1000]="\0", dtemp2[1000]="\0", uvalidity[1000]="\0", dbtemp2[1000]="\0", newbals[100]="\0", pins[5]="\0", phrase[500];
char* val;
char* nval;
int bal, amt, conva, denomination=500;
int  ch, x, logincheck=0, i=1, j=1, row=0, col=0, fl=0, count=0, co=0, sum=0, lc=0, nc=0, flag=0, l=0, usercheck=0, newn=0, an=0, balcheck=0, balco=0, bORp; 
FILE* fp;
//function declarations
void fxXbal(float conv_const, char curr[], int conv_amt, int am_to_dORw);
void choice(int a);
int pretransac(void);
int calcAmt(int code);
void newuser(void);
int deposit(void);
int withdrawal(void);
int coinDispenser(void);
void pin(void);
int fexchange(void);
void write(char what);
void login(void);
void pfound(void);
void nfound(void);
void remain(void);
void pfinal(void);
void talk(char p[]);
void bfound(void);
void bfinal(void);
int balance(char user[], char c[]);
int checkbal(int bal, int a, char user[]);

int main(void)
{
	//reading and storing contents of file in a char array
    char w[10000]="\0";
    fp=fopen("newfile.txt", "r");
	while(fgets(w, 1000, fp))
	{
		strcat(c, w);   //concatenating each iteration of the read loop
	}
	fclose(fp);
    //counting no. of lines in the file
    for (int i=0; i<strlen(c); i++)
	{
		if (c[i]=='\n')
			lc++;
	}
    //storing contents of 1st line of the file in 3 different arrays
	for (int i=0; i<strlen(c); i++)
		{
	    	if (c[i]!='\n')
        	{
            	strncat(temp2, &c[i], 1);
				strncat(temp1, &c[i], 1);
				strncat(dtemp2, &c[i], 1); 
				strncat(dbtemp2, &c[i], 1);  
				//counting the number of characters that were concatenated to the arrays
                count++;
				newn++; 
				balco++ ;
        	}
			else
				break;   //break once the first newline character is encountered
		}
	printf("\nUSER ID: ");  
	scanf("%s", uid);
	login();   //login function
	if (logincheck!=0)
	{
		printf("\n\nLOGIN AGAIN TO ACCESS ATM SERVICES. \nTHANK YOU!");   //exit after storing new user's details
		return 0;
	}
	if (logincheck==0)
	{
		bal=balance(uid, c);   
        balcheck=checkbal(bal, 500, uid);   //check balance to see if it is greater than the min. available denomination, 500
	}
	//displaying available operations
	if (balcheck!=0)
	{	
		printf("\nWELCOME TO ATM");
		printf("\nAVAILABLE OPERATIONS\n1. DEPOSIT\n2. BALANCE ENQUIRY\n3. RESET PIN\n4. CURRENCY EXCHANGE\n5. COIN DISPENSER\n6. PRE-STAGED TRANSACTION");
		printf("\nEnter your choice: ");
		scanf("%d", &ch);    //no withdrawal option if balance is less than 500
	}
	else
	{
		printf("\nWELCOME TO ATM");
		printf("\nAVAILABLE OPERATIONS\n1. DEPOSIT\n2. WITHDRAWAL\n3. BALANCE ENQUIRY\n4. RESET PIN\n5. CURRENCY EXCHANGE\n6. COIN DISPENSER\n7. PRE-STAGED TRANSACTION");
		printf("\nEnter your choice: ");
		scanf("%d", &ch);
	}
	//PIN verification	
	printf("\nPIN: ");   
	scanf("%s", p);
	fp=fopen("newfile.txt", "r");
	//iterate through string containing user's details
	for (int i=0; i<strlen(dtemp2); i++)
	{
		if (dtemp2[i]!='$')   
		{
			if (dtemp2[i]!='#')
			{
				strncat(check, &dtemp2[i], 1);   //concatenate PIN 
			}
			if (strcmp(check, p)==0)
			{
				an=1;
				break;
			}
			else if (dtemp2[i]==',')
			{
				strcpy(check, "\0");   //removing contents of string if PIN was not concatenated
			}
		}	
	}
	if (an!=1)
	{
		printf("\nPIN NOT FOUND!");
		printf("\nYOU HAVE ENTERED THE WRONG PIN. PLEASE LOGIN AGAIN IN ORDER TO ACCESS ATM SERVICES. \nTHANK YOU!");
			return 0;    //exit after wrong attempt
	}
	choice(balcheck);   //displaying options according to user balance (whether withdrawal will be available or not)
	i=0;
	j=0;
	balcheck=0;
return 0;
}

void newuser(void)   //function to enter new user's details
{
	printf("\nPASSWORD: ");
	scanf("%s", pwd);
	printf("\nSET PIN: _ _ _ _");
	scanf("%s", p);
	printf("\nINITIAL DEPOSIT AMOUNT: ");
	scanf("%d", &bal);
	sprintf(bals, "%d", bal);
	fp=fopen("newfile.txt", "a");
	fprintf(fp, "[!%s,@%s,#%s,$%s]", uid, pwd, p, bals);  
    fprintf(fp, "\n");     //writing info of new user into file
	fclose(fp);
	return;
}

int deposit(void)   //function for depositing cash
{
	int depo;
	printf("\nDEPOSIT AMOUNT: ");
	scanf("%d", &depo);
	bal+=depo;
	sprintf(bals, "%d", bal);
	write('b');    //writing updated balance to file
	return bal;
}
int withdrawal(void)   //function for withdrawing cash
{
	int withd;
	char ans;
	printf("\nWITHDRAWAL AMOUNT: ");
	scanf("%d", &withd);
	if (bal>=withd)      //withdrawal allowed only if user balance is greater than amount to be withdrawn
	{
		if (bal==withd)
		{
			printf("\nWARNING! WITHDRAWING RS. %d WILL DRAIN YOUR ACCOUNT BALANCE TO 0. DO YOU WISH TO PROCEED?\ny/n: ", withd);
			scanf(" %c", &ans);
			if (ans=='y' || ans=='Y')
			{
				bal=bal-withd;
       			sprintf(bals, "%d", bal);
				write('b');     //writing updated balance to file
			}
			else
				return 0;
		}
		else
		{
				bal=bal-withd;
       			sprintf(bals, "%d", bal);
				write('b');     //writing updated balance to file
		}
	}
	return bal;
}

void pin(void)   //function to reset PIN
{
	printf("\nNEW PIN: ");
	scanf("%s", np);
	//old PIN and new PIN must not be same
	while (strcmp(p,np)==0)
	{
		printf("\nNEW PIN CANNOT BE SAME AS OLD PIN!");
		printf("\n\nEnter new pin: ");
		scanf("%s", np);   //re-enter PIN as long as the old and new pins are same
	}
	write('p');    //writing updated PIN to file
	return;
}
int fexchange(void)   //function for foreign currency exchange
{
	char c;
	printf("\nDEPOSIT OR WITHDRAWAL? (d/w): ");
	scanf(" %c", &c);
	//displaying available currency exchanges
	printf("\nAVAILABLE CURRENCIES: \nUNITED STATES DOLLARS (USD)\nPOUND STERLING (GBP)\nUNITED ARAB EMIRATES DIRHAMS (AED)\nMALAYSIAN RINGGIT (MYR)");
	//for deposits
	if ((c=='d') || (c=='D'))
	{
		printf("\nDEPOSIT CURRENCY: ");
		scanf("%s", curr);
		//checking currencies and calculating amount accordingly
		if (strcmp(curr, "USD")==0 || strcmp(curr, "usd")==0)
		{
			printf("\nDEPOSIT AMOUNT (USD): ");
			scanf("%d", &amt);
			conva=amt*USD_C;     //using defined constants for conversion rates
			printf("\nAMOUNT (INR) TO BE DEPOSITED: %d", conva);
			bal+=conva;
			write('b');
		}
		else if (strcmp(curr, "GBP")==0 || strcmp(curr, "gbp")==0)
		{
			printf("\nDEPOSIT AMOUNT (GBP): ");
			scanf("%d", &amt);
			conva=amt*GBP_C;
			printf("\nAMOUNT (INR) TO BE DEPOSITED: %d", conva);
			bal+=conva;
			write('b');
		}
		else if (strcmp(curr, "AED")==0 || strcmp(curr, "aed")==0)
		{
			printf("\nDEPOSIT AMOUNT (AED): ");
			scanf("%d", &amt);
			conva=amt*AED_C;
			printf("\nAMOUNT (INR) TO BE DEPOSITED: %d", conva);
			bal+=conva;
			write('b');
		}
		else if (strcmp(curr, "CAD")==0 || strcmp(curr, "cad")==0)
		{
			printf("\nDEPOSIT AMOUNT (CAD): ");
			scanf("%d", &amt);
			conva=amt*CAD_C;
			printf("\nAMOUNT (INR) TO BE DEPOSITED: %d", conva);
			bal+=conva;
			write('b');
		}
		else if (strcmp(curr, "MYR")==0 || strcmp(curr, "myr")==0)
		{
			printf("\nDEPOSIT AMOUNT (MYR): ");
			scanf("%d", &amt);
			conva=amt*MYR_C;
			printf("\nAMOUNT (INR) TO BE DEPOSITED: %d", conva);
			bal+=conva;
			write('b');
		}
		printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
	}
	else
	//for withdrawals
	{
		printf("\nWITHDRAWAL CURRENCY: ");
		scanf("%s", curr);
		if (strcmp(curr, "USD")==0 || strcmp(curr, "usd")==0)
		{
			printf("\nWITHDRAWAL AMOUNT (USD): ");
			scanf("%d", &amt);
			conva=amt*USD_C;
			//display appropriate message for less balance
			if ((USD_C*bal)<amt)
			{
				fxXbal(USD_C, curr, conva, amt);
				return 0;
			}
			else
			{
				bal-=amt*USD_C;
				write('b');
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
			}
		}
		else if (strcmp(curr, "GBP")==0 || strcmp(curr, "gbp")==0)
		{
			printf("\nWITHDRAWAL AMOUNT (GBP): ");
			scanf("%d", &amt);
			conva=amt*GBP_C;
			checkbal(bal, conva, uid);
			if ((GBP_C*bal)<amt)
			{
				fxXbal(GBP_C, curr, conva, amt);
				return 0;
			}
			else
			{
				bal-=amt*GBP_C;
				write('b');
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
			}
		}
		else if (strcmp(curr, "AED")==0 || strcmp(curr, "aed")==0)
		{
			printf("\nWITHDRAWAL AMOUNT (AED): ");
			scanf("%d", &amt);
			conva=amt*AED_C;
			checkbal(bal, conva, uid);
			if ((AED_C*bal)<amt)
			{
				fxXbal(AED_C, curr, conva, amt);
				return 0;
			}
			else
			{
				bal-=amt*AED_C;
				write('b');
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
			}
		}
		else if (strcmp(curr, "CAD")==0 || strcmp(curr, "cad")==0)
		{
			printf("\nWITHDRAWAL AMOUNT (CAD): ");
			scanf("%d", &amt);
			conva=amt*CAD_C;
			checkbal(bal, conva, uid);
			if ((CAD_C*bal)<amt)
			{
				fxXbal(CAD_C, curr, conva, amt);
				return 0;
			}
			else
			{
				bal-=amt*CAD_C;
				write('b');
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
			}
		}
		else if (strcmp(curr, "MYR")==0 || strcmp(curr, "myr")==0)
		{
			printf("\nWITHDRAWAL AMOUNT (MYR): ");
			scanf("%d", &amt);
			conva=amt*MYR_C;
			checkbal(bal, conva, uid);
			if ((MYR_C*bal)<amt)
			{
				fxXbal(MYR_C, curr, conva, amt);
				return 0;
			}
			else
			{
				bal-=amt*MYR_C;
				write('b');
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
			}
		}
	}
	return 0;
}
void write(char what)  //function to write to file
{
	char w[1000]="\n[!";
    sprintf(bals, "%d", bal);
	strcat(w, uid);     //using temporary variable to compare user ID with the one saved in file (in a specific format)
	for (l=0; l<lc; l++)
	{
       	strcpy(temp4, temp2);
		val=strtok(temp4, ",");
		co=strlen(temp4);
		if (strcmp(temp4, w)==0)   //if user is found
		{
			if (what=='b' || what=='B')
			{
				bfound();   //for updating balance
				bORp=1;
			}
			else
			{
                pfound();   //for updating pin
				bORp=2;
			}
		}
		else
		{
			nfound();    //if user did not match in the current iteration
		}
		if (fl==1)
		{
			remain();    //concatenate rest of the file's contents if user is found
		}
		if (flag==1)
		{
			break;   //break loop once user is found
		}
	}
	if (bORp==1)
		bfinal();
	else
		pfinal();
	fp=fopen("newfile.txt", "w");
	//writing details into file
	if (strcmp(temp1, temp2)!=0)
	{
		fprintf(fp, "%s", real);   //contents before the user's details
		fprintf(fp, "%s", req);	   //user's details
		fprintf(fp, "%s", temp3);  //content's after user's details
		
	}
	else
	//if user is 1st user in the file	
	{
		fprintf(fp, "%s", req);
		fprintf(fp, "%s", temp3);
	}
	fclose(fp);
	return;
}
void login(void)
{
	char tArr[1000]="\n[!";
    strcat(tArr, uid);
	for (int l=0; l<lc; l++)   //iterate the file contents for as many times as there are no. of lines in it
	{
		strcpy(uvalidity, dtemp2);  //storing contents of indidvidual lines of file
      //array to compare with the file contents
		nval=strtok(uvalidity, ",");  //extracting user's info 
		if (strcmp(uvalidity, tArr)==0)
		{
			usercheck=1;
			break;   //break if user is found
		}
		else
		{
			//change contents of the two arrays to null
            strcpy(dtemp2, "\0");
			strcpy(uvalidity, "\0");    
			for (int i=newn; i<strlen(c); i++)
			{
				if (c[i]!=']')
				{
        			strncat(dtemp2, &c[i], 1);   //adding contents of the rest of the lines
					newn++;
        		}
				else
					break;
			}
			strcat(dtemp2, "]");
			newn++;
		}
	}
	if (usercheck==1)
		logincheck=0;   //user found
	else
	{
		logincheck=1;  //user not found
		newuser();     //function for storing new user's details
	}
	fclose(fp);
	return;
}
void fxXbal(float conv_const, char curr[], int conv, int am_to_dORw)   //function to print minimum deposit required to access withdrawal service in foreign currency exchange 
{
	int diff;
	diff=am_to_dORw-(bal*conv_const);
	printf("\nREQUIRED MINIMUM DEPOSIT IN %s: %d", strupr(curr), diff);   //printing min. deposit in the respective foreign currency
	return;
}
void choice(int x)   //function to read user's choice and perform the desired operation
{
	if (x!=0)  //if balance is less than Rs. 500
	{
		switch(ch)
		{
			case 1:
				bal=deposit();
				printf("\nAMOUNT SUCCESSFULLY DEPOSITED");
				break;
			case 2:
				bal=balance(uid, c);
				printf("\nBALANCE AVAILABLE: %d", bal);
				break;
			case 3:
				pin();
				printf("\nPIN SUCCESSFULLY CHANGED");	
				break;
			case 4:
				fexchange();
				break;
			case 5:
				coinDispenser();
				break;
			case 6:
				pretransac();
				break;
			default:
				printf("\nINVALID CHOICE!");
		}
	}
	else    //if balance is not less than Rs. 500
	{
		switch(ch)
		{
			case 1:
				bal=deposit();
				printf("\nAMOUNT SUCCESSFULLY DEPOSITED");
				break;
			case 2:
				bal=withdrawal();
				printf("\nAMOUNT SUCCESSFULLY WITHDRAWN");
				break;
			case 3:
				bal=balance(uid, c);
				printf("\nBALANCE AVAILABLE: %d", bal);
				break;
			case 4:
				pin();
				printf("\nPIN SUCCESSFULLY CHANGED");	
				break;
			case 5:
				fexchange();
				break;
			case 6:
				coinDispenser();
				break;
			case 7:
				pretransac();
				break;
			default:
				printf("INVALID CHOICE!");
		}
	}
	return;
}
int coinDispenser(void)   //function to withdraw coins
{
	int coin, q, tot;
	printf("\nAVAILABLE DENOMINATIONS:\n1\n2\n5");	
	printf("\n\nENTER YOUR CHOICE: ");
	scanf("%d", &coin);
	printf("\nENTER QUANTITY: ");
	scanf("%d", &q);
	tot=coin*q;
	if ((coin==1) || (coin==2) || (coin==5))
	{
		bal-=tot;
		sprintf(bals, "%d", bal);
		write('b');
		printf("\nCOINS DISPENSED SUCCESSFULLY");
	}
	else
	{
		printf("\n\nINVALID DENOMINATION!");
	}
	return ((int)bal);
}
void pfound(void)   //for writing pin if correct user is found
{
	strcpy(req, temp4);
	fl=1;	//flag to call function "remain"		
	for (int j=co; j<strlen(temp2); j++)
	{
        if (temp2[j]!='#')
		{
		    strncat(req, &temp2[j], 1);   //concatenate contents of file before PIN
		}
		else
			break;
	}
	flag=1;    //flag to break for loop
	return;
}	
void nfound(void)    //if user is not found
{
	strcpy(temp2, "\0");
	strcpy(temp4, "\0");   //modifying to null strings 
	for (int i=count; i<strlen(c); i++ )   //start loop from 2nd line
	{
		if (c[i]!=']')
		{
        	strncat(temp2, &c[i], 1);    //temp2 has only 2nd line
			strncat(temp1, &c[i], 1);   //temp1 has both 1st and 2nd lines
			count++;
        }
		else
			break;  //break when reaching "]" to prevent from going to next line 
	}
	strcat(temp1, "]");
	strcat(temp2, "]");   //concatenate "]" to the end as per format
	count++;   //count no. of characters that were encountered
	return;	
}	
void remain(void)    //funtion to concatenate conents after the required set of details corresponding to user
{
	sum=strlen(temp1);
	for (int i=sum; i<strlen(c); i++)
	{
		strncat(temp3, &c[i], 1);    //remaining contents of file concatenated
	}
	return;
}
void pfinal(void)    //function to concatenate new pin
{
	char pf[3]="#";
	strcpy(pins, np);   //copy new pin to another array "pins"
	strcat(pf, pins);   //creating  a string matching storing format
	strcat(pf, ",");    //adding "," to it
	strcat(req, pf);   //storing the new pin in a separate array
	for (int i=0; i<strlen(temp2); i++)
	{
		if (temp2[i]!='$')
			nc++;    //counting characters before slot of cash in file list set
		else
			break;
	}
	for (int i=nc; i<strlen(temp2); i++)
	{
		strncat(req, &temp2[i], 1);    //concatenating balance amount to array with new pin
	}
	for (int i=0; i<(strlen(temp1)-strlen(temp2)); i++)
	{
		strncat(real, &temp1[i], 1);      //concatenating contents before the current user's details set
	}
	return;	
}					
void talk(char p[])     //function for speech synthesis using espeak
{
	char com[100]="\0";
	strcpy(com, "espeak \"");
	strcat(com, p);
	strcat(com, "\"");
	system(com);
	return;
}				
void bfound(void)    //for writing balance if correct user is found
{
	strcpy(req, temp4);
	fl=1;	//flag to call function "remain"			
	for (int j=co; j<strlen(temp2); j++)
	{
        if (temp2[j]!='$')
		{
		    strncat(req, &temp2[j], 1);   //concatenate everything in user's list before '$' ie balance
		}
		else
			break; 
	}
	flag=1;   //flag to break for loop
	return;
}					
void bfinal(void)    //function to write new balance
{
	char fb[3]="$";
    strcpy(newbals, bals);
	strcat(fb, newbals);
	strcat(fb, "]");
	strcat(req, fb);   //req holds the new balance in the required storing format
	for (int i=0; i<(strlen(temp1)-strlen(temp2)); i++)
	{
		strncat(real, &temp1[i], 1);     //concatenate contents before current user's list
	}
	return;	
}										
int balance(char user[], char c[])     //function to read user account balance from file
{
	int cb;
    int chco=0, termif=0;
    char nf[10]="\n[!", store[1000]="\0", bchecker[1000]="\0", bcopy[1000]="\0";
	strcat(nf, uid);
	for (int l=0; l<lc; l++)
	{
		strcpy(bchecker, dbtemp2);    //first set of details
		strcpy(bcopy, bchecker);
		nval=strtok(bchecker, ",");
		if (strcmp(bchecker, nf)==0)   //checking if it's the same username
		{
			for (int i=0; i<strlen(bcopy); i++)
			{
				if (bcopy[i]!='$')   
					chco++;  //count of characters before balance in the list having the info about the particular user
				else
					break;
			}
			for (int j=chco; j<strlen(bcopy); j++)
			{
				if (bcopy[j]!='$')
				{
					if (bcopy[j]!=']')
					{
						strncat(store, &bcopy[j], 1);   //store balance amount
					}
					else
						termif=1;  //flag to terminate
				}
				if (termif==1)
					break;
			}
			usercheck=1;
			break;
		}
		else
		{
			strcpy(dbtemp2, "\0");
			strcpy(bchecker, "\0");
			for (int i=balco; i<strlen(c); i++ )
			{
				//read subsequent lines of file
				if (c[i]!=']')
				{
        			strncat(dbtemp2, &c[i], 1);
					balco++;
        		}
				else
					break;
			}
			strcat(dbtemp2, "]");
			balco++;
		}
	}
	cb=atoi(store);
    return cb;
}
int checkbal(int bal, int a, char user[])    //function to check minimum required balance for ATM operations
{
	int balf=0;
	if (bal<a)
	{
		printf("\n\nINSUFFICIENT BALANCE FOR WITHDRAWAL! \nPLEASE DEPOSIT MINIMUM RS. %d/- IN ORDER TO ENABLE WITHDRAWAL SERVICE. THANK YOU!", a-bal);
		balf=1;  //balf!=0 => No option for cash withdrawal
	}
	return balf;
}
int pretransac(void)  //function to process pre-staged transaction
{
	int xc, rc, codeCount=0;
	char read[100]="\0", crc[100]="\0", rcs[100]="\0", xcs[10]="\0", rcode[10]="\0";
	printf("\nENTER R-CODE: ");
	scanf("%d", &rc);
	sprintf(rcs, "%d", rc);
	FILE* f;
	f=fopen("code.txt", "r");
	while(fgets(read, 100, f))   //reading generated R-code and X-code
	{
		strcat(crc, read);
	}
	for (int i=0; i<strlen(crc); i++)
	{
		if (crc[i]!=',')
		{
			strncat(rcode, &crc[i], 1);  //concatenating R-code
			codeCount++;
		}
		else
			break;
	}
	for (int i=codeCount+1; i<strlen(crc); i++)
	{
		if (crc[i]!=',')
		{
			strncat(xcs, &crc[i], 1);   //concatenating X-code
		}
		else
			break;
	}
	if (strcmp(rcs, rcode)!=0)    //comparing user's input with generated code
	{
		printf("\nINVALID CODE! \nPLEASE TRY AGAIN LATER. THANK YOU!");
		return 0;
	}
	xc=atoi(xcs);
	bal=calcAmt(xc);
	sprintf(bals, "%d", bal);
	write('b');
	return 0;
}
int calcAmt(int code)     //function to calculate balance corresponding to a pre-staged transaction
{
	FILE* f;
	char wb[1000]="\0", ams[100]="\0", user[100]="\0", amo[100]="\0";
	int blns=0, conv, am, uco=0;
	f=fopen("bal.txt", "r");
	while(fgets(wb, 1000, f))   //reading amount entered by user to deposit/withdraw from file
	{
		strcat(ams, wb);
	}
	for (int i=0; i<strlen(ams); i++)
	{
		if (ams[i]!=',')
		{
			strncat(user, &ams[i], 1);  //concatenating R-code
			uco++;
		}
		else
			break;
	}
	for (int i=uco+1; i<strlen(ams); i++)
	{
		if (ams[i]!=',')
		{
			strncat(amo, &ams[i], 1);   //concatenating X-code
		}
		else
			break;
	}
	am=atoi(amo);
	if (strcmp(user, uid)!=0)
	{
		printf("\nINVALID USER!");
		return 0;
	}
	else
	{
		switch(code)    //checking cases as per codes
		{
			case 4+9:
				blns+=am;
				printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
				break;
			case 4+21:
				conv=am*USD_C;
				blns+=conv;
				printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
				break;
			case 4+7:
				conv=am*GBP_C;
				blns+=conv;
				printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
				break;
			case 4+1:
				conv=am*AED_C;
				blns+=conv;
				printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
				break;
			case 4+13:
				conv=am*MYR_C;
				blns+=conv;
				printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
				break;
			case 4+3:
				conv=am*CAD_C;
				blns+=conv;
				printf("\nAMOUNT DEPOSITED SUCCESSFULLY");
				break;
			case 23+9:
				blns-=am;
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
				break;
			case 23+21:
				conv=am*USD_C;
				blns-=conv;
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
				break;
			case 23+7:
				conv=am*GBP_C;
				blns-=conv;
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
				break;
			case 23+1:
				conv=am*AED_C;
				blns-=conv;
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
				break;
			case 23+13:
				conv=am*MYR_C;
				blns-=conv;
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
				break;
			case 23+3:
				conv=am*CAD_C;
				blns-=conv;
				printf("\nAMOUNT WITHDRAWN SUCCESSFULLY");
				break;
			default:
				printf("INVALID CURRENCY! \nPLEASE TRY AGAIN LATER. THANK YOU!");
		}
		return blns;
	}	
}	
