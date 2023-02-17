/***********    COPYRIGHT (c)    Rafail Papaioannou      **********************
| Language:       |  C
|-----------------|------------------------------------------------------------
| Project:        | ATM Prototype
|------------------------------------------------------------------------------
|    Date     - Name        -   Description
|19/1/2023      bank.h
|------------------------------------------------------------------------------
| FILE DESCRIPTION:
|Include file for the ATM project
******************************************************************************/


#ifndef BANK_H_INCLUDED
#define BANK_H_INCLUDED

    #include <stdio.h>
    #include <string.h>
    #include <conio.h>
    #include <stdlib.h>
    #include <sys/stat.h> // https://stackoverflow.com/questions/28489691/how-do-i-check-if-a-file-is-empty-in-c

    void Display();
    void DisplayATM();
    void NewCustomer();
    void Menu();
    void Deposit();
    void Withdraw();
    void Transfer();
    int  AssignAccountNumber();
    int  Login();
    void Atm();
    void DisplayData();
    void Balance(float);

    struct customer
    {
        int account_num;
        char first_name[30];
        char last_name[30];
        char iban[28];
        char afm[10];
    };
    struct pin
    {
        int account_num;
        char user_name[20];
        char pin[5];
    };
    struct balance
    {
        int account_num;
        float balance;
    };
    struct deposit
    {
        int account_num;
        float amount;
        char comments[50];
    };
    struct withdraw
    {
        int account_num;
        float amount; 
        char comments[50]; 
    };

    struct customer customer_data;
    struct pin pin_data;
    struct balance balance_data;
    struct deposit deposit_data;
    struct withdraw withdraw_data;    
 
    int g_count_customers=0;            // global variable to keep the number of customer records
    int g_account_number=0;

    FILE *customer_file;
    FILE *pin_file;
    FILE *balance_file;
    FILE *deposit_file;
    FILE *withdraw_file;
#endif // BANK_H_INCLUDED
