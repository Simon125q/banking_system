#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define MAX 200
#define MAX_NAME 30
#define MAX_SURNAME 50
#define MAX_PESEL 15
#define MAX_ADDRESS 100
#define PESEL_LENGTH 11

typedef struct Client
{
    long int ID;
    char name[MAX_NAME];
    char surname[MAX_SURNAME];
    char pesel[MAX_PESEL];
    char address[MAX_ADDRESS];
    long int balance;
    long int savings;
} CLIENT;

CLIENT found_client;

void Starting_Screen();
void Menu();
void Clear_Buff();
int Confirmation();
void My_Scanf(char *text, int size);
void Add_Account();
int Check_String(char to_check[], int full_check);
int Check_Money(long int sum, long int balance, int full_check);
void Print_All();
void Print_Account(CLIENT to_show);
CLIENT *Search_Account();
int Compare_Accounts(CLIENT account1, CLIENT account2);
void Modify_Account(CLIENT to_modify, CLIENT modified);
void Make_Deposit();
void Make_Transfer();
void Transfer_To_Savings();
void Transfer_From_Savings();
void Withdraw();

int main()
{
    Starting_Screen();
    Menu();
    return 0;
}

void Clear_Buff()
{
    char Clear;
    while ((Clear = getchar()) != '\n' && Clear != EOF)
        ;
}

int confirmation()
{
    char decision;
    printf("Do you confirm it? Y/N\n");
    Clear_Buff();
    decision = getchar();

    Clear_Buff();
    if (decision == 'y' || decision == 'Y')
        return TRUE;
    return FALSE;
}

void Starting_Screen()
{
    FILE *pfile;
    pfile = fopen("DATABASE.bin", "rb+");
    if (!pfile)
    {
        printf("Database not found\n");
        printf("Creating new...\n");
        pfile = fopen("DATABASE.bin", "wb+");
        if (!pfile)
        {
            printf("Sorry, error occured\n");
            exit(1);
        }
    }
    printf("\n");
    printf("\n");
    printf("Welcome to my bank!\n\n");
    fclose(pfile);
}

void Menu()
{
    while (TRUE)
    {
        printf("\n\n\n");
        printf("MENU\n");
        printf("1. Add new account\n");
        printf("2. Display all accounts\n");
        printf("3. Search account\n");
        printf("4. Make daposit\n");
        printf("5. Transfer money\n");
        printf("6. Transfer money to savings account\n");
        printf("7. Transfer money from savings account\n");
        printf("8. Withdraw money\n");
        printf("9. EXIT\n");
        printf("\n\n\n");
        int action = 0;
        while (scanf("%d", &action) != 1)
            Clear_Buff();
        Clear_Buff();
        if (action == 1)
            Add_Account();
        else if (action == 2)
            Print_All();
        else if (action == 3)
            Print_Account(*Search_Account());
        else if (action == 4)
            Make_Deposit();
        else if (action == 5)
            Make_Transfer();
        else if (action == 6)
            Transfer_To_Savings();
        else if (action == 7)
            Transfer_From_Savings();
        else if (action == 8)
            Withdraw();
        else if (action == 9)
        {
            printf("Goodbye, hava a nice day!\n");
            break;
        }
        else
            printf("action not found!\n");
    }
}

int Check_String(char to_check[], int full_check)
{
    int index = 0;
    int length = strlen(to_check);
    for (index = 0; index < length; index++)
    {
        if (full_check && isdigit(to_check[index]))
        {
            printf("String should not contain digits!\n");
            return FALSE;
        }
        if (isspace(to_check[index]) && index < length - 1 && isspace(to_check[index - 1]))
        {
            printf("String should not contain 2 spaces next to each other!\n");
            return FALSE;
        }
    }
    if (index < 2)
    {
        printf("String is too short!\n");
        return FALSE;
    }
    return TRUE;
}

int Check_Money(long int sum, long int balance, int full_check)
{
    if (full_check && sum > balance)
    {
        printf("Error, not enough money\n");
        return FALSE;
    }
    else if (sum < 0)
    {
        printf("Transfer cannot be negative\n");
        return FALSE;
    }
    return TRUE;
}

void My_Scanf(char text[], int size)
{
    int go = TRUE;
    int end_line = FALSE;
    while (go)
    {
        fgets(text, size, stdin);
        int index = 0;
        int length = strlen(text);
        for (index = 0; index < length; index++)
        {
            if (text[index] == '\n')
            {
                text[index] = '\0';
                end_line = TRUE;
                go = FALSE;
            }
        }
        if (!end_line)
        {
            Clear_Buff();
            printf("String is too long\n");
            printf("Provide it again: ");
        }
    }
}

void Add_Account()
{
    int correct = FALSE;
    CLIENT new_client;
    CLIENT current_client;
    memset(&new_client, 0, sizeof(CLIENT));
    memset(&current_client, 0, sizeof(CLIENT));
    int ID = 0;
    while (!correct)
    {
        printf("Provide your name: ");
        My_Scanf(new_client.name, MAX_NAME);
        if (Check_String(new_client.name, TRUE))
            correct = TRUE;
    }
    correct = FALSE;
    while (!correct)
    {
        printf("Provide your surname: ");
        My_Scanf(new_client.surname, MAX_SURNAME);
        if (Check_String(new_client.surname, TRUE))
            correct = TRUE;
    }
    correct = FALSE;
    while (!correct)
    {
        printf("Provide your PESEL: ");
        My_Scanf(new_client.pesel, MAX_PESEL);
        if (strlen(new_client.pesel) == PESEL_LENGTH)
        {
            correct = TRUE;
            for (int index = 0; index < PESEL_LENGTH; index++)
            {
                if (!isdigit(new_client.pesel[index]))
                {
                    printf("PESEL should contain only digits!\n");
                    correct = FALSE;
                }
            }
        }
        else
            printf("Too short/long PESEL!\n");
    }
    correct = FALSE;
    while (!correct)
    {
        printf("Provide your address: ");
        My_Scanf(new_client.address, MAX_ADDRESS);
        if (Check_String(new_client.address, FALSE))
            correct = TRUE;
    }
    printf("\n\n");
    printf("provided data:\nNAME: %s\nSURNAME: %s\nPESEL: %s\nADDRESS: %s\n", new_client.name, new_client.surname, new_client.pesel, new_client.address);
    if (confirmation())
    {
        FILE *pFile;
        pFile = fopen("DATABASE.bin", "rb+");
        if (!pFile)
        {
            printf("Error with file occured\n");
            return;
        }
        fseek(pFile, -1 * sizeof(CLIENT), 2);
        fread(&current_client, sizeof(CLIENT), 1, pFile);
        ID = current_client.ID;
        new_client.ID = ID + 1;
        fseek(pFile, (ID) * sizeof(CLIENT), 0);
        if (fwrite(&new_client, sizeof(CLIENT), 1, pFile) == 1)
        {
            printf("Account succesfully created\n");
        }
        else
        {
            printf("Error occured while creating, please try again\n");
        }
        fclose(pFile);
    }
}

void Print_All()
{
    FILE *pFile;
    CLIENT current_account;
    pFile = fopen("DATABASE.bin", "rb+");
    printf("LIST OF ALL ACCOUNTS:\n");
    while (TRUE)
    {
        fread(&current_account, sizeof(CLIENT), 1, pFile);
        if (feof(pFile))
            break;
        Print_Account(current_account);
    }
    fclose(pFile);
}

void Print_Account(CLIENT to_show)
{
    float savings_show = to_show.savings / 100.0;
    float balance_show = to_show.balance / 100.0;
    printf("\n");
    printf("ID: %ld\n", to_show.ID);
    printf("NAME: %s\n", to_show.name);
    printf("SURNAME: %s\n", to_show.surname);
    printf("PESEL: %s\n", to_show.pesel);
    printf("ADDRESS: %s\n", to_show.address);
    printf("BALANCE: %.2f\n", balance_show);
    printf("SAVINGS: %.2f\n", savings_show);
    printf("\n");
}

CLIENT *Search_Account()
{
    FILE *pFile;
    char key_string[MAX];
    long int key_ID;
    int decision;
    pFile = fopen("DATABASE.bin", "rb+");
    printf("Which criteria do you want to use to search?\n");
    printf("1. Account ID\n");
    printf("2. Name\n");
    printf("3. Surname\n");
    printf("4. Pesel\n");
    printf("5. Address\n");
    while(scanf("%d", &decision)!=1)
        Clear_Buff();
    if (decision == 1)
    {
        printf("\nProvide ID you are looking for: ");
        while(scanf("%ld", &key_ID) != 1)
            Clear_Buff();
    }
    else if (decision > 1 && decision < 6)
    {
        printf("\nProvide what you are looking for: ");
        My_Scanf(key_string, MAX);
    }
    else
    {
        printf("Invalid input!\n");
        fclose(pFile);
        return NULL;
    }
    while (TRUE)
    {
        fread(&found_client, sizeof(CLIENT), 1, pFile);
        if (feof(pFile))
        {
            printf("404 Account not found\n");
            break;
        }
        if (decision == 1 && key_ID == found_client.ID)
        {
            fclose(pFile);
            return &found_client;
        }
        else if (decision == 2 && strcmp(key_string, found_client.name) == 0)
        {
            fclose(pFile);
            return &found_client;
        }
        else if (decision == 3 && strcmp(key_string, found_client.surname) == 0)
        {
            fclose(pFile);
            return &found_client;
        }
        else if (decision == 4 && strcmp(key_string, found_client.pesel) == 0)
        {
            fclose(pFile);
            return &found_client;
        }
        else if (decision == 5 && strcmp(key_string, found_client.address) == 0)
        {
            fclose(pFile);
            return &found_client;
        }
    }
    fclose(pFile);
    return NULL;
}

int Compare_Accounts(CLIENT account1, CLIENT account2)
{
    if (account1.ID != account2.ID)
        return FALSE;
    else if (strcmp(account1.name, account2.name) != 0)
        return FALSE;
    else if (strcmp(account1.surname, account2.surname) != 0)
        return FALSE;
    else if (strcmp(account1.pesel, account2.pesel) != 0)
        return FALSE;
    else if (strcmp(account1.address, account2.address) != 0)
        return FALSE;
    else if (account1.balance != account2.balance)
        return FALSE;
    else if (account1.savings != account2.savings)
        return FALSE;
    return TRUE;
}

void Modify_Account(CLIENT to_modify, CLIENT modified)
{
    FILE *pFile;
    CLIENT found_client;
    pFile = fopen("DATABASE.bin", "rb+");
    while (TRUE)
    {
        fread(&found_client, sizeof(CLIENT), 1, pFile);
        if (feof(pFile))
        {
            printf("404 Account not found\n");
            break;
        }
        if (Compare_Accounts(to_modify, found_client))
        {
            fseek(pFile, -sizeof(CLIENT), 1);
            if (fwrite(&modified, sizeof(CLIENT), 1, pFile) == 1)
                printf("Account succesfully updated\n");
            else
                printf("Error occured while updating, please try again\n");
            fclose(pFile);
            return;
        }
    }
    fclose(pFile);
}

void Make_Deposit()
{
    CLIENT *ptr_curr_client = Search_Account();
    if (!ptr_curr_client)
        return;
    CLIENT curr_client = *ptr_curr_client;
    CLIENT new_client = curr_client;
    float get_sum;
    long int sum;
    printf("How much do you want to deposit?\n");
    printf("WARNING! Every digit after 2 decimal places will be cut down\n");
    while(scanf("%f", &get_sum) != 1)
        Clear_Buff();
    sum = get_sum * 100;
    if (!Check_Money(sum, curr_client.balance, FALSE))
        return;
    new_client.balance += sum;
    if (confirmation())
        Modify_Account(curr_client, new_client);
}

void Make_Transfer()
{
    CLIENT transfer_from, from_orginal;
    CLIENT transfer_to, to_orginal;
    CLIENT *ptr_client;
    long int sum;
    float get_sum;
    printf("\nFrom what account do you want to make transfer?\n");
    ptr_client = Search_Account();
    if (!ptr_client)
        return;
    transfer_from = from_orginal = *ptr_client;
    printf("To what account do you want to make transfer?\n");
    ptr_client = Search_Account();
    if (!ptr_client)
        return;
    transfer_to = to_orginal = *ptr_client;
    printf("How much do you want to transfer?\n");
    printf("WARNING! Every digit after 2 decimal places will be cut down\n");
    while(scanf("%f", &get_sum) != 1)
        Clear_Buff();
    sum = get_sum * 100;
    if (!Check_Money(sum, transfer_from.balance, TRUE))
        return;
    transfer_from.balance -= sum;
    transfer_to.balance += sum;
    if (confirmation())
    {
        Modify_Account(from_orginal, transfer_from);
        Modify_Account(to_orginal, transfer_to);
    }
}

void Transfer_To_Savings()
{
    CLIENT *ptr_client = Search_Account();
    if (!ptr_client)
        return;
    CLIENT curr_client = *ptr_client;
    CLIENT new_client = curr_client;
    long int sum;
    float get_sum;
    printf("How much do you want to transfer to savings?\n");
    printf("WARNING! Every digit after 2 decimal places will be cut down\n");
    while(scanf("%f", &get_sum) != 1)
        Clear_Buff();
    sum = get_sum * 100;
    if (!Check_Money(sum, curr_client.balance, TRUE))
        return;
    new_client.balance -= sum;
    new_client.savings += sum;
    if (confirmation())
        Modify_Account(curr_client, new_client);
}

void Transfer_From_Savings()
{
    CLIENT *ptr_client = Search_Account();
    if (!ptr_client)
        return;
    CLIENT curr_client = *ptr_client;
    CLIENT new_client = curr_client;
    long int sum;
    float get_sum;
    printf("How much do you want to transfer from savings?\n");
    printf("WARNING! Every digit after 2 decimal places will be cut down\n");
    while(scanf("%f", &get_sum) != 1)
        Clear_Buff();
    sum = get_sum * 100;
    if (!Check_Money(sum, curr_client.savings, TRUE))
        return;
    new_client.balance += sum;
    new_client.savings -= sum;
    if (confirmation())
        Modify_Account(curr_client, new_client);
}

void Withdraw()
{
    CLIENT *ptr_client = Search_Account();
    if (!ptr_client)
        return;
    CLIENT curr_client = *ptr_client;
    CLIENT new_client = curr_client;
    long int sum;
    float get_sum;
    printf("How much do you want to withdraw?\n");
    printf("WARNING! Every digit after 2 decimal places will be cut down\n");
    while(scanf("%f", &get_sum) != 1)
        Clear_Buff();
    sum = get_sum * 100;
    if (!Check_Money(sum, curr_client.balance, TRUE))
        return;
    new_client.balance -= sum;
    if (confirmation())
        Modify_Account(curr_client, new_client);
}
