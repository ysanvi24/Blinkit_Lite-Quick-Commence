#ifndef BLINKIT_H  //check if blinkit_h is not defined.
#define BLINKIT_H  //if it not defined, it will be defined, cause redefinition will cause errors

#define MAX_SKU 3000
#define MAX_ORD 5000
#define MAX_ITEMS 20

typedef struct {
    int skuId;
    char name[50];
    char category[30];
    float price;
    int stock;
    int soldCount;
} SKU;  //typedef, we have created a alias name of SKU

typedef struct {
    int orderId;
    long long orderTime;    // YYYYMMDDHHMM
    int status;             // 0=Placed, 1=Delivered, 2=Cancelled
    long long deliveryTime; // 0 if NA
    int itemIds[MAX_ITEMS];
    int itemQty[MAX_ITEMS];
    int itemCount;
    float subtotal;
} Order; //typedef, we have created a alias name of Order

// Global arrays and counters
extern SKU SKUList[MAX_SKU];
extern Order OrderList[MAX_ORD];
extern int skuCount;
extern int orderCount;

// Function declarations
void loadData();
void saveData();
void Add_SKU();
void Update_or_Delete_SKU();
void Place_Order();
void Deliver_Order();
void Cancel_Order();
void Search_SKU_ByName_Substring();
void Sort_Orders_ByTime();
void TopK_Bestsellers();
void ABC_Analysis_SoldCount();

#endif //this is end the #ifndef
