#include "def.h"

#define MAX 12345

Transaction *head = NULL;
Seller *sellerHead = NULL;
Buyer *buyerHead = NULL;
BuyerSellerPair *pairHead = NULL;
int transactionCount = 0;

void read_transactions_from_file()
{
    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        printf("No existing file\n");
    }
    else
    {
        char buffer[MAX]; // to store each line of the file
        int count = 0;    // to skip the first line of the file
        while (fgets(buffer, MAX, fp))
        { // read each line of the file
            if (count == 0)
            {
                count++;
                continue;
            }
            // parse the line and store the values in the transact
            Transaction *transact = (Transaction *)malloc(sizeof(Transaction));
            if (transact == NULL)
            {
                printf("Memory allocation failed\n");
            }
            else
            {
                sscanf(buffer, "%d,%d,%d,%lf,%lf,%s",
                       &transact->transactionID, &transact->buyerID, &transact->sellerID,
                       &transact->energyAmount, &transact->pricePerKwh, transact->timestamp);

                // insert the transaction at the beginning of the list
                InsertTransaction(transact);
            }
        }
        fclose(fp);
    }
}

void update_file()
{
    FILE *fp = fopen("transactions.csv", "a+");
    if (fp == NULL)
    {
        printf("Error opening file\n");
    }
    else
    {

        // check whether the file is empty
        fseek(fp, 0, SEEK_END);
        long int size = ftell(fp);
        if (size == 0)
        {
            fprintf(fp, "Transaction ID | Buyer ID | Seller ID | Energy Amount | Price per Kwh | Timestamp\n");
        }

        //only appending the latest transaction to the file
        Transaction *temp = head;
        fprintf(fp, "%d,%d,%d,%.2lf,%.2lf,%s\n", temp->transactionID, temp->buyerID, temp->sellerID, temp->energyAmount, temp->pricePerKwh, temp->timestamp);
        fclose(fp);
    }
}