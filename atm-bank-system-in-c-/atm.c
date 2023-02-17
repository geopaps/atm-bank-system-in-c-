/***********    COPYRIGHT (c)    Rafail Papaioannou      **********************
| Language:       |  C
|-----------------|------------------------------------------------------------
| Project:        | ATM Prototype
|------------------------------------------------------------------------------
|    Date     - Name        -   Description
|19/1/2023
|------------------------------------------------------------------------------
| FILE DESCRIPTION:
|
******************************************************************************/
#include "atm.h"

const char first_leg_iban[21]="GR234820000010011001";

void main(void)
{
    char choice;

    printf("********* Rafail Papaioannou ATM ***************\n") ;


    while(1)
    {
        Display();
        fflush(stdin);
        choice=getchar();
        switch (choice)
        {
            case '1':
                NewCustomer();
                break;
            case '2':
                if(Login())             // If login successfull then enter ATM
                {
                    Atm();
                    Display();          // when exits ATM return to main menu and wait
                    scanf("%c", &choice) ;
                }
                break;
            case '3':
                DisplayData();
                break;
            case '6':                   
                return;
            default :
                system("cls");              /*to clean the screen after any opreation*/
                printf("TO CONTINUE USE THE NUMBERS ON KEYBOARD PLEASE! (1/2/3/4/5/6) \n") ;
                break;
        }
    }
}


/*!
 * Display the main menu
 *
 * @param[]
 *
 * @returns
 */
void Display()
{
    printf("\n**********************************************\n") ;
    printf("TO CONTINUE \nEnter your choice :\n") ;
    printf("\n1. Create a New Customer Account\n") ;
    printf("2. ATM \n") ;
    printf("3. Display data\n");
    printf("6. EXIT (END) \n") ;
}


/*!
 * Login to ATM
 *
 * @param[]
 *
 * @returns     1-Successfull login     0-Unsuccessful login
 */
int Login()
{
    int user, account_temp, account_flag;
    char pin_temp[5], pin_num[5];

    printf("\n**********************************************\n");
    printf("\n**************** Login to ATM ****************\n");
    printf("\n**********************************************\n");
    printf("\n Enter account number: ");
    fflush(stdin);
    scanf("%d", &user);

    account_flag=0;
    g_account_number=0;
    pin_file=fopen("pin_file.dat", "r");
                                /* Check if the account number entered exists in the pin_file.dat */
    while(fread(&pin_data, sizeof(struct pin), 1, pin_file))  
    {
        if(user==pin_data.account_num)
        {
            strcpy(pin_temp, pin_data.pin);
            account_temp=pin_data.account_num;
            account_flag=1;
        }            
    }
    if(account_flag==0)         // If the account is not found return a 0 and exit the login process
    {
        printf("\nThere is no such account. Try again\n");
        return(0);
    }
                                // Get the pin and compare it to the stored one
    printf("\n Enter pin: ");               
    fflush(stdin);
    scanf("%s", pin_num);
    if(!strcmp(pin_num, pin_temp))  // If pin found then exit function returning a 1
    {
        g_account_number=account_temp;
        return(1);
    }
    else                                // If wrong pin exit function with 0
    {
        printf("\nWrong pin. Try again\n");
        return(0);
    }    
}


/*!
 * ATM functionality
 *
 * @param[]
 *
 * @returns
 */
void Atm()
{
    char select;
    
    while(1)
    {
//        system("cls");      // to clean the screen after any opreation
        DisplayATM();

        fflush(stdin);
        select=getchar();
        switch (select)
        {
            case '1':
                Deposit();
                break;
            case '2':
                Withdraw();
                break;
            case '3':
                Transfer();
                break;
            case '6':
                return;
            default :
                system("cls");              
                Display();
                printf("TO CONTINUE MAKE A CHOICE! (1/2/3/4/5/6) \n") ;
                break;
        }
    }
} 


/*!
 * Display the ATM menu
 *
 * @param[]
 *
 * @returns
 */
void DisplayATM()
{
    printf("\n**********************************************\n") ;
    printf("TO CONTINUE \nEnter your choice :\n") ;
    printf("**********************************************\n") ;
    printf("1. Deposit Money to Account \n") ;
    printf("2. Withdraw Money from Account \n") ;
    printf("3. Transfer Money from Account \n") ;
    printf("6. EXIT to Main Menu \n") ;
}


/*!
 * Create a new customer.
 *
 * @param[]
 *
 * @returns
 */
void NewCustomer()
{
    char temp_account[8];
//    const char first_leg_iban[28]="GR234820000010011001"; 
    FILE *w_customer;

    fflush(stdin);

    customer_data.account_num=AssignAccountNumber();    // Create account number for the customer
    printf("\nEnter the first name: ");
    scanf("%s", customer_data.first_name);
    printf("\nEnter the last name: ");
    scanf("%s", customer_data.last_name);
    printf("\nEnter the AFM: ");
    scanf("%s", customer_data.afm);
    printf("\nEnter the user name: ");
    scanf("%s", pin_data.user_name);
    printf("\nEnter the pin: ");
    scanf("%s",pin_data.pin);

    /* Make IBAN from "GR234820000010011001" + account_num */
    sprintf(temp_account, "%d", customer_data.account_num);
    strcpy(customer_data.iban, first_leg_iban);
    strcat(customer_data.iban, temp_account);
 

    w_customer = fopen("customer_file.dat","a");   // Make a record of the customer_data in "customer_file.dat"
    fwrite(&customer_data, sizeof(struct customer), 1, w_customer);
    fclose(w_customer);

    pin_data.account_num=customer_data.account_num; // Make a record of the pin_data in "pin_file.dat"
    pin_file = fopen("pin_file.dat", "a");
    fwrite(&pin_data, sizeof(struct pin), 1, pin_file); 
    fclose(pin_file);

    balance_data.account_num= customer_data.account_num;    // Make a record of the customer with 0 balance in
    balance_data.balance=0;                                 // "balance_file.dat"
    balance_file = fopen("balance_file.dat", "ab");
    fwrite(&balance_data, sizeof(struct balance), 1, balance_file);
    fclose(balance_file);
}


/*!
 * Create an account number for a new customer
 *
 * @param[]    
 *
 * @returns   Account number for the customer
 */
int AssignAccountNumber()
{
    const int first_account_num=5000001;
    int account_number, size, num_of_records=0;
    FILE *r_customer;
    int count_records=0;

    r_customer=fopen("customer_file.dat", "rb");        
    fseek(r_customer,0,SEEK_END);
    size=ftell(r_customer);
    fseek(r_customer,0,SEEK_SET);

    if (size == 0)             
    {
        account_number=first_account_num;           // if there are no records in the file, 
        g_count_customers = first_account_num;      // then give the initial value to the account number
    }                            
    else
    {                                                       // Find out the number of accounts  
        num_of_records = size / sizeof(customer_data);      // in the file by dividing the size of the
        account_number=first_account_num+num_of_records;    // file by the size of each record
    }
    fclose(r_customer);
    g_count_customers=account_number;
    return account_number;
}


/*!
 * Deposit money to the account
 *
 * @param[]    
 *
 * @returns   
 */
void Deposit()
{
    printf("Enter the amount to deposit:  ");
    scanf("%f", &deposit_data.amount);
    printf("\nEnter comments:  ");
    fflush(stdin);
    fgets(deposit_data.comments, 50, stdin);
    deposit_data.account_num=g_account_number;
    deposit_file = fopen("deposit_file.dat", "a");
    fwrite(&deposit_data, sizeof(struct deposit), 1, deposit_file);
    fclose(deposit_file);
    Balance(deposit_data.amount);
}


/*!
 * Withdraw money from the account
 *
 * @param[]    
 *
 * @returns   
 */
void Withdraw()
{
    printf("Enter the amount to withdraw:  ");
    scanf("%f", &withdraw_data.amount);
    printf("\nEnter comments:  ");
    fflush(stdin);
    fgets(withdraw_data.comments, 50, stdin);    
    withdraw_data.account_num=g_account_number;
    withdraw_file = fopen("withdraw_file.dat", "a");
    fwrite(&withdraw_data, sizeof(struct withdraw), 1, withdraw_file);
    fclose(withdraw_file);
    Balance(withdraw_data.amount*(-1));
}


/*!
 * Transfer an amount to another customer
 *
 * @param[]    
 *
 * @returns   
 */
void Transfer()
{
    char iban[28], target_account[8];
    int i, j, sender_account_num=0;

    printf("Enter the IBAN of tha account to send:  ");   //Withdraw from account
    scanf("%s", iban);
    printf("Enter the amount to transfer:  ");
    scanf("%f", &withdraw_data.amount);
    printf("\nEnter comments:  ");
    fflush(stdin);
    fgets(withdraw_data.comments, 50, stdin); 
    withdraw_data.account_num=g_account_number;
    withdraw_file = fopen("withdraw_file.dat", "a");
    fwrite(&withdraw_data, sizeof(struct withdraw), 1, withdraw_file);  
    fclose(withdraw_file);
    Balance(withdraw_data.amount*(-1));             // update the balance

    for(i=27, j=7; j>=0; --i, --j)          //Copy the last 7 digits from IBAN which are the account
        target_account[j]=iban[i]; 
    deposit_data.account_num=atoi(target_account);

    deposit_data.amount=withdraw_data.amount;
    strcpy(deposit_data.comments, withdraw_data.comments);
    deposit_file = fopen("deposit_file.dat", "a");
    fwrite(&deposit_data, sizeof(struct deposit), 1, deposit_file);
    fclose(deposit_file);
    sender_account_num=g_account_number;
    g_account_number=deposit_data.account_num;
    Balance(deposit_data.amount);
    g_account_number=sender_account_num;
}


/*!
 * Update the balance of the customer
 *
 * @param[float]    amount   (Deposit or Withdraw)
 *
 * @returns   
 */
void Balance(float amount)
{
    balance_file=fopen("balance_file.dat", "r+");

    while(fread(&balance_data, sizeof(struct balance), 1, balance_file))
    {
        if(g_account_number==balance_data.account_num)
        {
            balance_data.balance += amount;
            fseek(balance_file, -(long)sizeof(struct balance), SEEK_CUR);
            fwrite(&balance_data, sizeof(struct balance), 1, balance_file);
            break;
        }            
    }
    fclose(balance_file);
}


/*!
 * Display all data in the database
 *
 * @param[]    
 *
 * @returns   
 */
void DisplayData()
{
    printf("\n*******************************************\n");
    printf("************** Customer data **************\n");
    printf("*******************************************\n");
    customer_file=fopen("customer_file.dat", "r");
    while(fread(&customer_data, sizeof(struct customer), 1, customer_file))
        printf("%d\t%15s\t%15s\t%28s\t%10s\n",
            customer_data.account_num, customer_data.first_name, customer_data.last_name,
             customer_data.iban, customer_data.afm);
    fclose(customer_file);

    printf("\n*******************************************\n");
    printf("**************** Pin data *****************\n");
    printf("*******************************************\n");
    pin_file=fopen("pin_file.dat", "r");
    while(fread(&pin_data, sizeof(struct pin), 1, pin_file))
        printf("%8d\t%s\t%5s\n", pin_data.account_num, pin_data.user_name, pin_data.pin);
    fclose(pin_file);

    printf("\n*******************************************\n");
    printf("************** Balance data ***************\n");
    printf("*******************************************\n");
    balance_file=fopen("balance_file.dat", "r");
    while(fread(&balance_data, sizeof(struct balance), 1, balance_file))
        printf("%8d\t%.2f\n", balance_data.account_num, balance_data.balance);
    fclose(balance_file);

    printf("\n*******************************************\n");
    printf("************** Deposit data ***************\n");
    printf("*******************************************\n");
    deposit_file=fopen("deposit_file.dat", "r");
    while(fread(&deposit_data, sizeof(struct deposit), 1, deposit_file))
        printf("%8d\t%.2f\t\t%s\n", deposit_data.account_num, deposit_data.amount, deposit_data.comments);
    fclose(deposit_file);

    printf("\n*******************************************\n");
    printf("************** Withdraw data **************\n");
    printf("*******************************************\n");
    withdraw_file=fopen("withdraw_file.dat", "r");
    while(fread(&withdraw_data, sizeof(struct withdraw), 1, withdraw_file))
        printf("%8d\t%.2f\t\t%s\n", withdraw_data.account_num, withdraw_data.amount, withdraw_data.comments);
    fclose(withdraw_file);
}



    

