#include "struct.c"

//function to create a new transaction
//returns the pointer to the new transaction
//returns NULL if memory allocation fails
//the transaction ID is automatically assigned
//the timestamp is automatically assigned

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


//function to insert a new transaction
//returns SUCCESS if the transaction is successfully inserted
//returns FAIL if the transaction is NULL
//the transaction is inserted at the beginning of the linked list
//the transaction is also inserted in the seller and buyer linked lists
//if the seller or buyer does not exist, the transaction is not inserted
//if the buyer-seller pair does not exist, a new pair is created
//if the buyer-seller pair exists, the total energy and total revenue are updated
//if the buyer-seller pair has more than 4 transactions, the buyer is added to the seller's regular buyers list
//if the transaction is successfully inserted, the function returns SUCCESS
//if the transaction is NULL, the function returns FAIL
status_code InsertTransaction()
{
    status_code sc = SUCCESS;
    Transaction *newtransaction = CreateTransaction();
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
    Seller *thisseller = NULL;
    while (temp != NULL && flag)
    {
        if (temp->sellerID == newtransaction->sellerID)
        {
            newtransaction->next = temp->transactions;
            temp->transactions = newtransaction;
            temp->totalRevenue += (newtransaction->energyAmount * newtransaction->pricePerKwh);
            flag = 0;
            thisseller = temp;
        }
        temp = temp->next;
    }

    // for buyer structure

    Buyer *tempB = buyerHead;
    int flagB = 1;
    Buyer *thisbuyer= NULL;
    while (tempB != NULL && flagB)
    {
        if (tempB->buyerID == newtransaction->buyerID)
        {
            newtransaction->next = tempB->transactionsB;
            tempB->transactionsB = newtransaction;
            tempB->TotalenergyAmount += (newtransaction->energyAmount);
            flagB = 0;
            Buyer *thisbuyer = tempB;
        }
        tempB = tempB->next;
    }

    BuyerSellerPair *tempPair = pairHead;
    int flagPair = 1;
    while (tempPair != NULL && flagPair)
    {
        if (tempPair->buyerID == newtransaction->buyerID && tempPair->sellerID == newtransaction->sellerID)
        {
            tempPair->totalEnergy += newtransaction->energyAmount;
            tempPair->totalRevenue += (newtransaction->energyAmount * newtransaction->pricePerKwh);
            tempPair->BSCount += 1;
            flagPair = 0;
            if( tempPair->BSCount > 4 ){
                //adding buyer to regular buyers list
                Buyer *tempbuyer = thisseller->regularBuyers;
                if(tempbuyer == NULL){
                    thisseller->regularBuyers = thisbuyer;
                }
                else{
                    while(tempbuyer->next != NULL){
                        tempbuyer = tempbuyer->next;
                    }
                    tempbuyer->next = thisbuyer;
            }
        }
        //if the pair does not exist
        else{
            //creating a new pair
            BuyerSellerPair *newPair = (BuyerSellerPair *)malloc(sizeof(BuyerSellerPair));
            if (newPair == NULL)
            {
                printf("Memory allocation failed\n");
            }
            else
            {
                newPair->buyerID = newtransaction->buyerID;
                newPair->sellerID = newtransaction->sellerID;
                newPair->totalEnergy = newtransaction->energyAmount;
                newPair->totalRevenue = (newtransaction->energyAmount * newtransaction->pricePerKwh);
                newPair->BSCount = 1;
                newPair->next = pairHead;
                pairHead = newPair;
            }
        }
        tempPair = tempPair->next;
    }

    printf("\n\n----------------------------------------\n\n");

    return sc;
}
}

//function to update a transaction
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

            while (t != NULL)
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

//function to check if a timestamp is within a specified period

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

    printf("\n\n----------------------------------------\n\n");
}

//function for sorting based on amount of energy of each transaction
//sorted in descending order

Transaction *MergeListsEnergy(Transaction *list1, Transaction *list2)
{
    Transaction *dummy;
    Transaction *temp = dummy;

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
        if(list1) temp->next = list1;
        else temp->next = list2;

        return dummy->next;
    }
}

Transaction *SortEnergy(Transaction *head){
    if(head!= NULL && head->next!=NULL){
        Transaction *slow = head;
        Transaction *fast = head->next;

        while(fast != NULL && fast->next != NULL){
            slow = slow->next;
            fast = fast->next->next;
        }

        Transaction *list1 = head;
        Transaction *list2 = slow->next;
        slow->next = NULL;

        list1 = SortEnergy(list1);
        list2 = SortEnergy(list2);

        head =  MergeListsEnergy(list1, list2);
    }

    return head;
}

int HighestEnergyAmount(Transaction *head){
    int highest_energy = 0;
    if(head == NULL) highest_energy = 0;
    else if (head -> next == NULL) highest_energy = head -> energyAmount;
    else{
        SortEnergy(head);
        highest_energy = head -> energyAmount;
    }
    return highest_energy;
}

//sorting list of buyers based on total energy amount
//sorted in ascending
Buyer *MergeListsBuyerEnergy(Buyer *list1, Buyer *list2)
{
    Buyer *dummy;
    Buyer *temp = dummy;

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
        if(list1) temp->next = list1;
        else temp->next = list2;

        return dummy->next;
    }
}

Buyer *SortBuyerEnergy(Buyer *buyerHead){
    if(buyerHead!= NULL && buyerHead->next!=NULL){
        Buyer *slow = buyerHead;
        Buyer *fast = buyerHead->next;

        while(fast != NULL && fast->next != NULL){
            slow = slow->next;
            fast = fast->next->next;
        }

        Buyer *list1 = buyerHead;
        Buyer *list2 = slow->next;
        slow->next = NULL;

        list1 = SortBuyerEnergy(list1);
        list2 = SortBuyerEnergy(list2);

        buyerHead =  MergeListsBuyerEnergy(list1, list2);
    }

    return buyerHead;
}

//sorting buyer energy amount


void SortBuyerEnergyAmount(){
    if(buyerHead == NULL) printf("No buyers available\n");
    else{
        SortBuyerEnergy(buyerHead);
    }

    printf("\n\n----------------------------------------\n\n");
}

//sorting Buyer-Seller Pair
//sorted in desending order based on no. of transactions between them

BuyerSellerPair *MergeListsPair(BuyerSellerPair *list1, BuyerSellerPair *list2)
{
    BuyerSellerPair *dummy;
    BuyerSellerPair *temp = dummy;
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
        if(list1) temp->next = list1;
        else temp->next = list2;

        return dummy->next;
    }
}

BuyerSellerPair *SortPair(BuyerSellerPair *pairHead){
    if(pairHead!= NULL && pairHead->next!=NULL){
        BuyerSellerPair *slow = pairHead;
        BuyerSellerPair *fast = pairHead->next;

        while(fast != NULL && fast->next != NULL){
            slow = slow->next;
            fast = fast->next->next;
        }

        BuyerSellerPair *list1 = pairHead;
        BuyerSellerPair *list2 = slow->next;
        slow->next = NULL;

        list1 = SortPair(list1);
        list2 = SortPair(list2);

        pairHead =  MergeListsPair(list1, list2);
    }

    return pairHead;
}

void SortBuyerSellerPair(){
    if(pairHead == NULL) printf("No Buyer-Seller pairs available\n");
    else{
        SortPair(pairHead);
    }

    printf("\n\n----------------------------------------\n\n");
}




