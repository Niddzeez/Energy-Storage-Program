#include "def.h"
#include "fileop.c"

int main()
{
    int choice;
    Transaction *newtransaction;
    status_code sc;
    int exitchoice = 1;
    read_transactions_from_file();
    while (exitchoice)
    {
        printf("1. Insert a new transaction\n");
        printf("2. Update a transaction\n");
        printf("3. Display the buyer list\n");
        printf("4. Display the seller list\n");
        printf("5. Display the transaction list\n");
        printf("6. Display the total revenue by seller\n");
        printf("7. Display the transactions within a period\n");
        printf("8. Display the transaction with the highest energy amount\n");
        printf("9. Sort the buyer list based on energy amount\n");
        printf("10. Sort the buyer-seller pair based on total energy and display\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n\n----------------------------------------\n\n");
        switch (choice)
        {
        case 1:
            sc = InsertTransaction(CreateTransaction());
            update_file();
            break;
        case 2:
            printf("Enter the transaction ID to update: ");
            int Transaction_ID;
            scanf("%d", &Transaction_ID);
            sc = UpdateTransaction(Transaction_ID);
            break;
        case 3:
            DisplayBuyerList();
            break;
        case 4:
            DisplaySellerList();
            break;
        case 5:
            DisplayTransaction();
            break;
        case 6:
            printf("Enter the seller ID: ");
            int SID;
            scanf("%d", &SID);
            printf("Total revenue: %d\n", TotalRevenueBySeller(SID));
            break;
        case 7:
            printf("Enter the start timestamp: ");
            char start[20];
            scanf("%s", start);
            printf("Enter the end timestamp: ");
            char end[20];
            scanf("%s", end);
            listTransactionsInPeriod(start, end);
            break;
        case 8:
            printf("Highest energy amount: %d\n", HighestEnergyAmount(head));
            break;
        case 9:
            SortBuyerEnergyAmount();
            DisplayBuyerEnergyAmount();
            break;
        case 10:
            SortBuyerSellerPair();
            DisplayBuyerSellerPairs();
            break;
        case 11:
            exitchoice = 0;
            
            break;
        }
        
    }
    return 0;
}