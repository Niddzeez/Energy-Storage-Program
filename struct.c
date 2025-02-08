
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef enum
{
    FAIL,
    SUCCESS
} status_code;

//to store the transaction details
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

//to store the transaction details
int transactionCount = 0;

//to store the buyer details
typedef struct Buyer_tag
{
    int buyerID;
    double TotalenergyAmount;
    Transaction *transactionsB;
    struct Buyer_tag *next;

} Buyer;

Buyer *buyerHead = NULL;
int buyerCount = 0;

//to store the seller details
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

//to store the buyer-seller pair details
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