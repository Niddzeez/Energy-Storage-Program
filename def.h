#ifndef DEF_H
#define DEF_H
#include "functions.c"

Transaction *CreateTransaction();
status_code InsertTransaction();
status_code UpdateTransaction(int Transaction_ID);
void DisplayBuyerList();
void DisplayTransaction();
int TotalRevenueBySeller(int SID);
void DisplaySellerList();
int isWithinPeriod(const char *timestamp, const char *start, const char *end);
void listTransactionsInPeriod(const char *start, const char *end);
Transaction *MergeListsEnergy(Transaction *list1, Transaction *list2);
Transaction *SortEnergy(Transaction *head);
Buyer *MergeListsBuyerEnergy(Buyer *list1, Buyer *list2);
int HighestEnergyAmount(Transaction *head);
Buyer *SortBuyerEnergy(Buyer *buyerHead);
void SortBuyerEnergyAmount();
BuyerSellerPair *MergeListsPair(BuyerSellerPair *list1, BuyerSellerPair *list2);
BuyerSellerPair *SortPair(BuyerSellerPair *pairHead);
void SortBuyerSellerPair();

#endif