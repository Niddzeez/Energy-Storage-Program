#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef enum
{
    FAIL,
    SUCCESS
} status_code;

// to store the transaction details
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

// to store the buyer details
typedef struct Buyer_tag
{
    int buyerID;
    double TotalenergyAmount;
    Transaction *transactionsB;
    struct Buyer_tag *next;

} Buyer;

// to store the seller details
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

// to store the buyer-seller pair details
typedef struct BuyerSellerPair_tag
{
    int buyerID;
    int sellerID;
    double totalEnergy;
    double totalRevenue;
    int BSCount;
    struct BuyerSellerPair_tag *next;
} BuyerSellerPair;

extern Buyer *buyerHead;
extern Seller *sellerHead;
extern Transaction *head;
extern int transactionCount;
extern BuyerSellerPair *pairHead;