#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef enum
{
    FAIL,
    SUCCESS
} status_code;

typedef struct Transaction_tag
{
    int transactionID;
    int buyerID;
    int sellerID;
    double energyAmount; // in kWh
    double pricePerKwh;
    char timestamp[20]; // "YYYY-MM-DD-HH:MM"
    struct Transaction_tag *next;
} Transaction;

Transaction *head = NULL;
int transactionCount = 0;

typedef struct Buyer_tag
{
    int buyerID;
    double TotalenergyAmount;
    Transaction *transactionsB;

    struct Buyer_tag *next;

} Buyer;

Buyer *buyerHead = NULL;
int buyerCount = 0;

typedef struct Seller_tag
{
    int sellerID;
    double rateBelow300;
    double rateAbove300;
    double totalRevenue;
    Buyer *regularBuyers;
    Transaction *transactions; // linked list of transactions
    struct Seller_tag *next;
} Seller;

Seller *sellerHead = NULL;
int sellerCount = 0;

typedef struct BuyerSellerPair_tag
{
    int buyerID;
    int sellerID;
    double totalEnergy;
    double totalRevenue;
    int BSCount;
    struct BuyerSellerPair_tag *next;
} BuyerSellerPair;

BuyerSellerPair *pairHead = NULL;
int pairCount = 0;

Transaction *CreateTransaction()
{

    Transaction *newtransaction = (Transaction *)malloc(sizeof(Transaction));
    if (newtransaction == NULL)
    {
        printf("Memory allocation failed\n");
    }
    else
    {
        newtransaction->transactionID = transactionCount + 1;

        printf("Enter the buyer ID: ");
        scanf("%d", &newtransaction->buyerID);

        printf("Enter the seller ID: ");
        scanf("%d", &newtransaction->sellerID);

        printf("Enter the energy amount: ");
        scanf("%lf", &newtransaction->energyAmount);

        printf("Enter the price per kWh: ");
        scanf("%lf", &newtransaction->pricePerKwh);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(newtransaction->timestamp, "%d-%02d-%02d-%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
        newtransaction->next = NULL;
    }

    return newtransaction;
}

status_code InsertTransaction(Transaction *newtransaction)
{
    status_code sc = SUCCESS;
    if (newtransaction == NULL)
    {
        printf("Transaction is NULL\n");
        sc = FAIL;
    }
    else
    {

        newtransaction->next = head;
        head = newtransaction;
    }

    // for seller structure

    Seller *temp = sellerHead;
    int flag = 1;
    while (temp != NULL && flag)
    {
        if (temp->sellerID == newtransaction->sellerID)
        {
            newtransaction->next = temp->transactions;
            temp->transactions = newtransaction;
            temp->totalRevenue += (newtransaction->energyAmount * newtransaction->pricePerKwh);
            flag = 0;
        }
        temp = temp->next;
    }

    // for buyer structure

    Buyer *tempB = buyerHead;
    int flagB = 1;
    while (tempB != NULL && flagB)
    {
        if (tempB->buyerID == newtransaction->buyerID)
        {
            newtransaction->next = tempB->transactionsB;
            tempB->transactionsB = newtransaction;
            tempB->TotalenergyAmount += (newtransaction->energyAmount);
            flagB = 0;
        }
        tempB = tempB->next;
    }

    return sc;
}

status_code UpdateTransaction(int Transaction_ID)
{
    status_code sc = SUCCESS;
    Transaction *temp = head;
    while (temp != NULL && temp->transactionID != Transaction_ID)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("Transaction not found\n");
        sc = FAIL;
    }
    else
    {

        printf("Enter the new buyer ID: ");
        scanf("%d", &temp->buyerID);

        printf("Enter the new seller ID: ");
        scanf("%d", &temp->sellerID);

        printf("Enter the new energy amount: ");
        scanf("%lf", &temp->energyAmount);

        printf("Enter the new price per kWh: ");
        scanf("%lf", &temp->pricePerKwh);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(temp->timestamp, "%d-%02d-%02d-%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
    }

    return sc;
}

void DisplayTransaction()
{
    Transaction *temp = head;
    if (temp == NULL)
    {

        printf("No transactions available\n");
    }
    else
    {
        while (temp != NULL)
        {
            printf("Transaction ID : %d\n", temp->transactionID);
            printf("Buyer ID : %d\n", temp->buyerID);
            printf("Seller ID : %d\n", temp->sellerID);
            printf("Energy Amount : %lf\n", temp->energyAmount);
            printf("Price per kWh : %lf\n", temp->pricePerKwh);
            printf("Timestamp : %s\n", temp->timestamp);
            temp = temp->next;
        }
    }
}

void DisplaySellerList()
{
    if (head == NULL)
    {
        printf("No transactions available.\n");
    }
    else
    {

        Seller *temp = sellerHead;
        while (temp != NULL)
        {
            printf("Transactions for Seller ID :%d\n", temp->sellerID);
            Transaction *t = temp->transactions;
            if (t == NULL)
            {
                printf("No transactions found for this seller.\n");
            }

            while (t != NULL)
            {
                printf("Transaction ID: %d | Buyer ID: %d | Energy: %.2lf kWh | Price per kWh: %.2lf | Timestamp: %s\n",
                       t->transactionID, t->buyerID, t->energyAmount, t->pricePerKwh, t->timestamp);
                t = t->next;
            }

            temp = temp->next;
        }
    }
}

void DisplayBuyerList()
{

    if (head == NULL)
    {
        printf("No transactions available.\n");
    }
    else
    {

        Buyer *temp = buyerHead;
        while (temp != NULL)
        {
            printf("Transactions for Seller ID :%d\n", temp->buyerID);
            Transaction *t = temp->transactionsB;
            if (t == NULL)
            {
                printf("No transactions found for this seller.\n");
            }

            while (t != NULL)
            {
                printf("Transaction ID: %d | Seller ID: %d | Energy: %.2lf kWh | Price per kWh: %.2lf | Timestamp: %s\n",
                       t->transactionID, t->sellerID, t->energyAmount, t->pricePerKwh, t->timestamp);
                t = t->next;
            }

            temp = temp->next;
        }
    }
}

void TotalRevenueBySeller(int SID)
{

    Seller *temp = sellerHead;
    int flag = 1;
    if (temp == NULL)
    {
        printf("Seller doesn't exist\n");
    }

    while (temp != NULL && flag)
    {

        if (temp->sellerID == SID)
        {
            printf("The total revenue by seller is %.2lf\n", temp->totalRevenue);
            flag = 0;
        }
        temp = temp->next;
    }
}

int isWithinPeriod(const char *timestamp, const char *start, const char *end)
{
    return (strcmp(timestamp, start) >= 0 && strcmp(timestamp, end) <= 0);
}

void listTransactionsInPeriod(const char *start, const char *end)
{
    int found = 0;
    Transaction *temp = head;

    printf("Transactions between %s and %s:\n", start, end);
    while (temp != NULL)
    {
        if (isWithinPeriod(temp->timestamp, start, end))
        {
            found = 1;
            printf("Transaction ID: %d, Buyer ID: %d, Seller ID: %d, Energy: %.2f kWh, Price: $%.2f/kWh, Timestamp: %s\n",
                   temp->transactionID, temp->buyerID, temp->sellerID, temp->energyAmount, temp->pricePerKwh, temp->timestamp);
        }
        temp = temp->next;
    }
    if (!found)
    {
        printf("No transactions found in the specified period.\n");
    }
}
