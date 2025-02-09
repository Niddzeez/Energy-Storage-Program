#include "struct.c"

// function to create a new transaction
// returns the pointer to the new transaction
// returns NULL if memory allocation fails
// the transaction ID is automatically assigned
// the timestamp is automatically assigned

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

        printf("Transaction created successfully\n");
    }

    return newtransaction;
}

// function to update a transaction
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

        printf("Enter the new energy amount: ");
        scanf("%lf", &temp->energyAmount);

        printf("Enter the new price per kWh: ");
        scanf("%lf", &temp->pricePerKwh);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(temp->timestamp, "%d-%02d-%02d-%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
    }
    printf("\n\n----------------------------------------\n\n");

    return sc;
}

// Function to create a copy of a transaction
Transaction *copyTransaction(Transaction *original)
{
    Transaction *copy = (Transaction *)malloc(sizeof(Transaction));
    if (copy)
    {
        *copy = *original; // Copy all fields
        copy->next = NULL; // Reset next pointer
    }
    return copy;
}

status_code InsertTransaction(Transaction *newtransaction)
{
    status_code sc = SUCCESS;
    if (newtransaction == NULL)
    {
        printf("Transaction creation failed\n");
        return FAIL;
    }

    // Insert into the main transaction list
    newtransaction->next = head;
    head = newtransaction;
    transactionCount++;

    // -------- Seller Structure -------- //
    Seller *temp = sellerHead;
    Seller *thisseller = NULL;
    while (temp != NULL && thisseller == NULL)
    {
        if (temp->sellerID == newtransaction->sellerID)
        {
            printf("Seller found\n");
            Transaction *copy = copyTransaction(newtransaction);
            copy->next = temp->transactions;
            temp->transactions = copy;
            temp->totalRevenue += newtransaction->energyAmount * newtransaction->pricePerKwh;
            thisseller = temp;
        }
        temp = temp->next;
    }

    if (thisseller == NULL)
    {
        printf("Creating new seller\n");
        Seller *newSeller = (Seller *)malloc(sizeof(Seller));
        if (newSeller == NULL)
        {
            printf("Memory allocation failed for Seller\n");
            return FAIL;
        }
        newSeller->sellerID = newtransaction->sellerID;
        newSeller->totalRevenue = newtransaction->energyAmount * newtransaction->pricePerKwh;
        newSeller->transactions = copyTransaction(newtransaction);
        newSeller->regularBuyers = NULL;
        newSeller->next = sellerHead;
        sellerHead = newSeller;
        thisseller = newSeller;
    }

    // -------- Buyer Structure -------- //
    Buyer *tempB = buyerHead;
    Buyer *thisbuyer = NULL;
    while (tempB != NULL && thisbuyer == NULL)
    {
        if (tempB->buyerID == newtransaction->buyerID)
        {
            printf("Buyer found\n");
            Transaction *copy = copyTransaction(newtransaction);
            copy->next = tempB->transactionsB;
            tempB->transactionsB = copy;
            tempB->TotalenergyAmount += newtransaction->energyAmount;
            thisbuyer = tempB;
        }
        tempB = tempB->next;
    }

    if (thisbuyer == NULL)
    {
        printf("Creating new buyer\n");
        Buyer *newBuyer = (Buyer *)malloc(sizeof(Buyer));
        if (newBuyer == NULL)
        {
            printf("Memory allocation failed for Buyer\n");
            return FAIL;
        }
        newBuyer->buyerID = newtransaction->buyerID;
        newBuyer->TotalenergyAmount = newtransaction->energyAmount;
        newBuyer->transactionsB = copyTransaction(newtransaction);
        newBuyer->next = buyerHead;
        buyerHead = newBuyer;
        thisbuyer = newBuyer;
    }

    // -------- Buyer-Seller Pair -------- //
    BuyerSellerPair *tempPair = pairHead;
    BuyerSellerPair *foundPair = NULL;
    while (tempPair != NULL && foundPair == NULL)
    {
        if (tempPair->buyerID == newtransaction->buyerID && tempPair->sellerID == newtransaction->sellerID)
        {
            printf("Pair found\n");
            tempPair->totalEnergy += newtransaction->energyAmount;
            tempPair->totalRevenue += newtransaction->energyAmount * newtransaction->pricePerKwh;
            tempPair->BSCount++;
            foundPair = tempPair;
        }
        tempPair = tempPair->next;
    }

    if (foundPair == NULL)
    {
        printf("Creating new Buyer-Seller pair\n");
        BuyerSellerPair *newPair = (BuyerSellerPair *)malloc(sizeof(BuyerSellerPair));
        if (newPair == NULL)
        {
            printf("Memory allocation failed for BuyerSellerPair\n");
            return FAIL;
        }
        newPair->buyerID = newtransaction->buyerID;
        newPair->sellerID = newtransaction->sellerID;
        newPair->totalEnergy = newtransaction->energyAmount;
        newPair->totalRevenue = newtransaction->energyAmount * newtransaction->pricePerKwh;
        newPair->BSCount = 1;
        newPair->next = pairHead;
        pairHead = newPair;
        foundPair = newPair;
    }

    // -------- Add Buyer to RegularBuyers if BSCount > 4 -------- //
    if (foundPair->BSCount > 4 && thisseller != NULL)
    {
        printf("Adding buyer to regular buyers list\n");
        Buyer *tempbuyer = thisseller->regularBuyers;
        int alreadyExists = 0;
        while (tempbuyer != NULL && alreadyExists == 0)
        {
            if (tempbuyer->buyerID == thisbuyer->buyerID)
            {
                alreadyExists = 1;
            }
            tempbuyer = tempbuyer->next;
        }
        if (!alreadyExists)
        {
            thisbuyer->next = thisseller->regularBuyers;
            thisseller->regularBuyers = thisbuyer;
            printf("Buyer added to regular buyers list\n");
        }
    }

    printf("Transaction successfully inserted.\n");
    return sc;
}

void DisplayBuyerSellerPairs()
{
    BuyerSellerPair *temp = pairHead;
    if (temp == NULL)
    {
        printf("No buyer-seller pairs available\n");
    }
    else
    {
        while (temp != NULL)
        {
            printf("Buyer ID: %d\n", temp->buyerID);
            printf("Seller ID: %d\n", temp->sellerID);
            printf("Total Energy: %lf\n", temp->totalEnergy);
            printf("Total Revenue: %lf\n", temp->totalRevenue);
            printf("BS Count: %d\n", temp->BSCount);
            printf("\n\n");
            temp = temp->next;
        }
    }
    printf("\n\n----------------------------------------\n\n");
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
            printf("\n\n");

            temp = temp->next;
        }
    }
    printf("\n\n----------------------------------------\n\n");
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

            while (t != NULL && temp->sellerID == t->sellerID)
            {
                printf("Transaction ID: %d | Buyer ID: %d | Energy: %.2lf kWh | Price per kWh: %.2lf | Timestamp: %s\n",
                       t->transactionID, t->buyerID, t->energyAmount, t->pricePerKwh, t->timestamp);
                t = t->next;
            }

            temp = temp->next;
        }
    }
    printf("\n\n----------------------------------------\n\n");
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
            printf("Transactions for Buyer ID :%d\n", temp->buyerID);
            Transaction *t = temp->transactionsB;
            if (t == NULL)
            {
                printf("No transactions found for this seller.\n");
            }

            while (t != NULL && temp->buyerID == t->buyerID)
            {
                printf("Transaction ID: %d | Seller ID: %d | Energy: %.2lf kWh | Price per kWh: %.2lf | Timestamp: %s\n",
                       t->transactionID, t->sellerID, t->energyAmount, t->pricePerKwh, t->timestamp);
                t = t->next;
            }
            temp = temp->next;
        }
    }
    printf("\n\n----------------------------------------\n\n");
}

void DisplayBuyerEnergyAmount()
{
    Buyer *temp = buyerHead;
    if (temp == NULL)
    {
        printf("No buyers available\n");
    }
    else
    {
        while (temp != NULL)
        {
            printf("Buyer ID: %d | Total Energy Amount: %.2lf kWh\n", temp->buyerID, temp->TotalenergyAmount);
            temp = temp->next;
        }
    }
    printf("\n\n----------------------------------------\n\n");
}

int TotalRevenueBySeller(int SID)
{
    int revenue = 0;
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
            revenue = temp->totalRevenue;
            printf("The total revenue by seller is %.2lf\n", temp->totalRevenue);
            flag = 0;
        }
        temp = temp->next;
    }

    printf("\n\n----------------------------------------\n\n");

    return revenue;
}

// function to check if a timestamp is within a specified period

int isWithinPeriod(const char *timestamp, const char *start, const char *end)
{
    return (strcmp(timestamp, start) >= 0 && strcmp(timestamp, end) <= 0);
}

void listTransactionsInPeriod(const char *start, const char *end)
{
    int found = 0;
    Transaction *temp = head;

    printf("Transactions between %s and %s (in form YYYY-MM-DD-HH-MinMin):\n", start, end);
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

    printf("\n\n----------------------------------------\n\n");
}

// function for sorting based on amount of energy of each transaction
// sorted in descending order

Transaction *MergeListsEnergy(Transaction *list1, Transaction *list2)
{
    printf("Entered merge lists energy\n");
    Transaction dummy;
    Transaction *temp = &dummy;
    dummy.next = NULL;

    while (list1 != NULL && list2 != NULL)
    {
        if (list1->energyAmount > list2->energyAmount)
        {
            temp->next = list1;
            temp = list1;
            list1 = list1->next;
        }
        else
        {
            temp->next = list2;
            temp = list2;
            list2 = list2->next;
        }
    }
    if (list1)
        temp->next = list1;
    else
        temp->next = list2;
    printf("Sorted\n");
    return dummy.next;
}

Transaction *SortEnergy(Transaction *head)
{
    printf("Entered sort energy\n");
    if (head != NULL && head->next != NULL)
    {
        Transaction *slow = head;
        Transaction *fast = head->next;

        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        Transaction *list1 = head;
        Transaction *list2 = slow->next;
        slow->next = NULL;

        printf("found list1 and list2\n");
        list1 = SortEnergy(list1);
        list2 = SortEnergy(list2);

        head = MergeListsEnergy(list1, list2);
        printf("Sorted\n");
    }

    return head;
}

int HighestEnergyAmount(Transaction *head)
{
    int highest_energy = 0;
    if (head == NULL)
        highest_energy = 0;
    else if (head->next == NULL)
        highest_energy = head->energyAmount;
    else
    {
        printf("Sorting based on energy amount\n");
        SortEnergy(head);
        printf("Sorted in function highestenergy amount\n");
        highest_energy = head->energyAmount;
    }
    printf("Highest energy amount: %d\n", highest_energy);
    return highest_energy;
}

// sorting list of buyers based on total energy amount
// sorted in ascending
Buyer *MergeListsBuyerEnergy(Buyer *list1, Buyer *list2)
{
    Buyer dummy;
    Buyer *temp = &dummy;
    dummy.next = NULL;

    while (list1 != NULL && list2 != NULL)
    {
        if (list1->TotalenergyAmount < list2->TotalenergyAmount)
        {
            temp->next = list1;
            temp = list1;
            list1 = list1->next;
        }
        else
        {
            temp->next = list2;
            temp = list2;
            list2 = list2->next;
        }
    }
    if (list1)
        temp->next = list1;
    else
        temp->next = list2;
    printf("Sorted\n");
    return dummy.next;
}

Buyer *SortBuyerEnergy(Buyer *buyerHead)
{
    if (buyerHead != NULL && buyerHead->next != NULL)
    {
        Buyer *slow = buyerHead;
        Buyer *fast = buyerHead->next;

        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        Buyer *list1 = buyerHead;
        Buyer *list2 = slow->next;
        slow->next = NULL;

        list1 = SortBuyerEnergy(list1);
        list2 = SortBuyerEnergy(list2);
        printf("Sorted\n");
        buyerHead = MergeListsBuyerEnergy(list1, list2);
    }
    printf("Sorted\n");
    return buyerHead;
}

// sorting buyer energy amount

void SortBuyerEnergyAmount()
{
    if (buyerHead == NULL)
        printf("No buyers available\n");
    else
    {
        SortBuyerEnergy(buyerHead);
        printf("Sorted buyer energy amount successfully\n");
    }

    printf("\n\n----------------------------------------\n\n");
}

// sorting Buyer-Seller Pair
// sorted in desending order based on no. of transactions between them

BuyerSellerPair *MergeListsPair(BuyerSellerPair *list1, BuyerSellerPair *list2)
{
    printf("Entered the merging");
    BuyerSellerPair dummy;
    BuyerSellerPair *temp = &dummy;
    dummy.next = NULL;

    while (list1 != NULL && list2 != NULL)
    {
        if (list1->BSCount > list2->BSCount)
        {
            temp->next = list1;
            temp = list1;
            list1 = list1->next;
        }
        else
        {
            temp->next = list2;
            temp = list2;
            list2 = list2->next;
        }
    }
    if (list1)
        temp->next = list1;
    else
        temp->next = list2;
    printf("Sorted\n");

    return dummy.next;
}

BuyerSellerPair *SortPair(BuyerSellerPair *pairHead)
{
    if (pairHead != NULL && pairHead->next != NULL)
    {
        BuyerSellerPair *slow = pairHead;
        BuyerSellerPair *fast = pairHead->next;

        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        BuyerSellerPair *list1 = pairHead;
        BuyerSellerPair *list2 = slow->next;
        slow->next = NULL;

        list1 = SortPair(list1);
        list2 = SortPair(list2);
        printf("Sorted\n");
        pairHead = MergeListsPair(list1, list2);
        printf("Merged\n");
    }

    return pairHead;
}

void SortBuyerSellerPair()
{
    if (pairHead == NULL)
        printf("No Buyer-Seller pairs available\n");
    else
    {
        SortPair(pairHead);
        printf("Sorted Buyer-Seller pairs successfully\n");
    }

    printf("\n\n----------------------------------------\n\n");
}
