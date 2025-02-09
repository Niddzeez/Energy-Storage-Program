#include "def.h"

#define MAX 12345

// to read exisiting transactions from csv file
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
                transact->next = head;
                head = transact;

                // Handle seller structure
                Seller *tempSeller = sellerHead;
                int sellerFound = 0;
                while (tempSeller != NULL && !sellerFound)
                {
                    if (tempSeller->sellerID == transact->sellerID)
                    {
                        transact->next = tempSeller->transactions;
                        tempSeller->transactions = transact;
                        tempSeller->totalRevenue += (transact->energyAmount * transact->pricePerKwh);
                        sellerFound = 1;
                    }
                    tempSeller = tempSeller->next;
                }

                if (!sellerFound)
                {
                    // Create a new seller if not found
                    Seller *newSeller = (Seller *)malloc(sizeof(Seller));
                    if (newSeller == NULL)
                    {
                        printf("Memory allocation failed for new seller\n");
                        fclose(fp);
                    }
                    else{
                    newSeller->sellerID = transact->sellerID;
                    newSeller->transactions = transact;
                    newSeller->totalRevenue = transact->energyAmount * transact->pricePerKwh;
                    newSeller->next = sellerHead;
                    sellerHead = newSeller;
                    }
                }

                // Handle buyer structure
                Buyer *tempBuyer = buyerHead;
                int buyerFound = 0;
                while (tempBuyer != NULL && !buyerFound)
                {
                    if (tempBuyer->buyerID == transact->buyerID)
                    {
                        transact->next = tempBuyer->transactionsB;
                        tempBuyer->transactionsB = transact;
                        tempBuyer->TotalenergyAmount += transact->energyAmount;
                        buyerFound = 1;
                    }
                    tempBuyer = tempBuyer->next;
                }

                if (!buyerFound)
                {
                    // Create a new buyer if not found
                    Buyer *newBuyer = (Buyer *)malloc(sizeof(Buyer));
                    if (newBuyer == NULL)
                    {
                        printf("Memory allocation failed for new buyer\n");
                        fclose(fp);
                    }
                    else{
                    newBuyer->buyerID = transact->buyerID;
                    newBuyer->transactionsB = transact;
                    newBuyer->TotalenergyAmount = transact->energyAmount;
                    newBuyer->next = buyerHead;
                    buyerHead = newBuyer;
                    }
                }
            }

            fclose(fp);
        }
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

            // writing the transactions to the file
            Transaction *temp = head;
            while (temp != NULL)
            {
                fprintf(fp, "%d,%d,%d,%.2lf,%.2lf,%s\n",
                        temp->transactionID, temp->buyerID, temp->sellerID,
                        temp->energyAmount, temp->pricePerKwh, temp->timestamp);
                temp = temp->next;
            }
            fclose(fp);
        }
    }