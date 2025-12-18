#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "blinkit.h" //Include this header file where all the functions are declared

SKU SKUList[MAX_SKU];
Order OrderList[MAX_ORD];
int skuCount = 0;
int orderCount = 0;

// File Handling
// to save the data manually for sku and order
void saveData() {

    FILE *fs = fopen("sku_data.txt", "w"); //opens txt file in write mode
    for (int i = 0; i < skuCount; i++) {    //runs a loop of skucount to update all the SKUs
        fprintf(fs, "%d %s %s %.2f %d %d\n",
                SKUList[i].skuId, SKUList[i].name, SKUList[i].category,
                SKUList[i].price, SKUList[i].stock, SKUList[i].soldCount);
    }
    fclose(fs);      // closes the pointer

    FILE *fo = fopen("order_data.txt", "w"); 
    for (int i = 0; i < orderCount; i++) { //runs a smae loop as skucount for ordercount and updates all of it
        fprintf(fo, "%d %lld %d %lld %d %.2f\n",
                OrderList[i].orderId, OrderList[i].orderTime,
                OrderList[i].status, OrderList[i].deliveryTime,
                OrderList[i].itemCount, OrderList[i].subtotal);
        for (int j = 0; j < OrderList[i].itemCount; j++)
            fprintf(fo, "%d %d ", OrderList[i].itemIds[j], OrderList[i].itemQty[j]);
        fprintf(fo, "\n");
    }
    fclose(fo);
}

//this loads in the data from the text file into the program
void loadData() { 
    FILE *fs = fopen("sku_data.txt", "r");
    skuCount = 0;
    if (fs) {
        while (fscanf(fs, "%d %s %s %f %d %d\n",
                      &SKUList[skuCount].skuId, SKUList[skuCount].name,
                      SKUList[skuCount].category, &SKUList[skuCount].price,
                      &SKUList[skuCount].stock, &SKUList[skuCount].soldCount) == 6)
            skuCount++;
        fclose(fs);
    }

    FILE *fo = fopen("order_data.txt", "r");
    orderCount = 0;
    if (fo) {
        while (fscanf(fo, "%d %lld %d %lld %d %f\n",
                      &OrderList[orderCount].orderId, &OrderList[orderCount].orderTime,
                      &OrderList[orderCount].status, &OrderList[orderCount].deliveryTime,
                      &OrderList[orderCount].itemCount, &OrderList[orderCount].subtotal) == 6) {
            for (int j = 0; j < OrderList[orderCount].itemCount; j++)
                fscanf(fo, "%d %d ", &OrderList[orderCount].itemIds[j], &OrderList[orderCount].itemQty[j]);
            fscanf(fo, "\n");
            orderCount++;
        }
        fclose(fo);
    }
}

//two basic functions findSKU and findOrder as they are used frequently
int findSKU(int id) {
    for (int i = 0; i < skuCount; i++)
        if (SKUList[i].skuId == id) return i;
    return -1;
}

int findOrder(int id) {
    for (int i = 0; i < orderCount; i++)
        if (OrderList[i].orderId == id) return i;
    return -1;
}

//Main functions
//Function 1 - 
void Add_SKU() {
    SKU s;
    printf("Enter SKU ID: "); 
    scanf("%d", &s.skuId);
    if (findSKU(s.skuId) != -1)  //checks if SKU exists or not using the findSKU
    { 
        printf("SKU ID already exists!\n"); 
        return; 
    }
    printf("Enter Name: ");  //if id doesnt exist take other all inputs
    scanf(" %s", s.name);
    printf("Enter Category: "); 
    scanf(" %s", s.category); 
    printf("Enter Price: "); 
    scanf("%f", &s.price);
    printf("Enter Stock: "); 
    scanf("%d", &s.stock);
    if (s.price < 0 || s.stock < 0) //if price and stock is less than zero it is invalid
    { 
        printf(" Invalid price/stock!\n"); 
        return; 
    }
    s.soldCount = 0;
    SKUList[skuCount++] = s;
    printf(" SKU added.\n");
}

//Function 2-

void Update_or_Delete_SKU() { 
    int id; 
    printf("Enter SKU ID: ");  //first check whether the id exists or not
    scanf("%d", &id);
    int idx = findSKU(id);
    if (idx == -1)
    { 
        printf(" SKU not found.\n"); 
        return; 
    }
    printf("1. Update  2. Delete: "); 
    int ch; 
    scanf("%d", &ch);
    if (ch == 1) {  //for update, take the new input from the user
        printf("New Name: "); scanf(" %s", SKUList[idx].name);
        printf("New Category: "); scanf(" %s", SKUList[idx].category);          
        printf("New Price: "); scanf("%f", &SKUList[idx].price);
        printf("New Stock: "); scanf("%d", &SKUList[idx].stock);
        printf(" Updated.\n");
    } 
    else if (ch == 2) {     
        for (int i = 0; i < orderCount; i++)
            if (OrderList[i].status == 1) //check the entire orderlist if any order is delivered or not
                for (int j = 0; j < OrderList[i].itemCount; j++)    //if it has status of 1 then find the itemCount
                    if (OrderList[i].itemIds[j] == id) {    //check if the id matches?
                        printf(" Exists in delivered order.\n");
                        return;
                    }
        for (int i = idx; i < skuCount - 1; i++)    //just update the value of i+1 to i and reduce the size of skuCount
            SKUList[i] = SKUList[i + 1];
        skuCount--;
        printf(" Deleted.\n");
    }
}
//Function 3

void Place_Order() {
    Order o;    
    printf("Enter Order ID: "); 
    scanf("%d", &o.orderId);
    if (findOrder(o.orderId) != -1) //check if the order if already exists or not?
    { 
        printf(" Order ID exists!\n"); 
        return; 
    }
    printf("Order Time (YYYYMMDDHHMM): "); //note the order time
    scanf("%lld", &o.orderTime);
    printf("Item Count: ");     //how many items??
    scanf("%d", &o.itemCount);
    o.subtotal = 0; 
    for (int i = 0; i < o.itemCount; i++) {
        printf("Item %d ID: ", i+1); 
        scanf("%d", &o.itemIds[i]);
        printf("Quantity: "); 
        scanf("%d", &o.itemQty[i]);
        int sidx = findSKU(o.itemIds[i]);
        if (sidx == -1 || o.itemQty[i] <= 0) //check that item quantity and and item ids exists
        { 
            printf(" Invalid.\n"); 
            return; 
        }
        o.subtotal += SKUList[sidx].price * o.itemQty[i]; //calculate the price of the 
    }
    o.status = 0; o.deliveryTime = 0;
    OrderList[orderCount++] = o;
    printf("Order placed. Subtotal: %.2f\n", o.subtotal);
}
//Function 4
void Deliver_Order() {
    int id; 
    printf("Order ID: "); 
    scanf("%d", &id);   //check the order id
    int idx = findOrder(id);
    if (idx == -1) 
    { 
        printf(" Order not found.\n"); 
        return; 
    }
    if (OrderList[idx].status != 0) 
    { 
        printf(" Must be Placed.\n"); 
        return; 
    }
    printf("Delivery Time (YYYYMMDDHHMM): ");  //take the delivery time
    scanf("%lld", &OrderList[idx].deliveryTime);
    for (int i = 0; i < OrderList[idx].itemCount; i++) {
        int sidx = findSKU(OrderList[idx].itemIds[i]);
        if (sidx == -1 || SKUList[sidx].stock < OrderList[idx].itemQty[i]) {
            printf("Not enough stock.\n"); return;
        }
    }
    for (int i = 0; i < OrderList[idx].itemCount; i++) {
        int sidx = findSKU(OrderList[idx].itemIds[i]);
        SKUList[sidx].stock -= OrderList[idx].itemQty[i]; //decrease the stock
        SKUList[sidx].soldCount += OrderList[idx].itemQty[i]; //increase the sold count
    }
    OrderList[idx].status = 1;
    printf(" Delivered.\n");
}

//Function 5
void Cancel_Order() {
    int id; printf("Order ID: "); 
    scanf("%d", &id);
    int idx = findOrder(id);
    if (idx == -1) 
    { 
        printf(" Not found.\n"); 
        return; 
    }
    if (OrderList[idx].status == 1) 
    { 
        printf("Delivered order can't cancel.\n");   
        return; 
    }
    OrderList[idx].status = 2;  //Just update the status of the order to 2
    printf(" Cancelled.\n");
}


void Search_SKU_ByName_Substring() {
    char query[50]; //initialize an char array
    printf("Search substring: "); 
    scanf(" %s", query); //Enter the substring
    int found = 0;
    for (int i = 0; i < skuCount; i++) {
        char n[50], q[50];
        strcpy(n, SKUList[i].name); //copy the main string into n
        strcpy(q, query); //copy the query string into q
        for (int k=0;n[k];k++) n[k]=tolower(n[k]); //the loop continues till n[k] is not equal to zero
        for (int k=0;q[k];k++) q[k]=tolower(q[k]);
        if (strstr(n,q)) { //check if it is a substring
            printf("Found: %d | %s | %.2f | Stock=%d | Sold=%d\n",
                   SKUList[i].skuId, SKUList[i].name, SKUList[i].price,
                   SKUList[i].stock, SKUList[i].soldCount);
            found=1;
        } 
    }
    if(!found) printf(" No matches.\n");
}

void Sort_Orders_ByTime() {
    
    for (int i = 0; i < orderCount-1; i++){
        for (int j = 0; j < orderCount-i-1; j++){
        
            if (OrderList[j].orderTime > OrderList[j+1].orderTime ||
               (OrderList[j].orderTime==OrderList[j+1].orderTime && 
                OrderList[j].orderId > OrderList[j+1].orderId)) {
                Order tmp = OrderList[j];
                OrderList[j] = OrderList[j+1];
                OrderList[j+1] = tmp;
                
            }
        }
    }
    printf("Orders sorted by time.\n");
}

void TopK_Bestsellers() {
    int K; 
    printf("Enter K: ");
    scanf("%d", &K); 
    if(K>skuCount){
        printf("Invalid K");
        return;
    }
    
    int index[MAX_SKU];
    for (int i=0;i<skuCount;i++) 
    index[i]=i;
    for (int i=0;i<skuCount-1;i++)
        for (int j=0;j<skuCount-i-1;j++){
            int a=index[j], b=index[j+1];
            if (SKUList[a].soldCount < SKUList[b].soldCount ||
                (SKUList[a].soldCount==SKUList[b].soldCount &&
                 strcmp(SKUList[a].name,SKUList[b].name)>0)){
                int t=index[j]; index[j]=index[j+1]; index[j+1]=t;
            }
        }
    printf("\nTop %d Bestsellers:\n",K);
    for(int i=0;i<K && i<skuCount;i++){
        int id=index[i];
        printf("%d. %s (Sold=%d)\n",i+1,SKUList[id].name,SKUList[id].soldCount);
    }
}

void ABC_Analysis_SoldCount() {
    int index[MAX_SKU];
    long long total=0;
    for(int i=0;i<skuCount;i++)
    { 
        index[i]=i; 
        total+=SKUList[i].soldCount; 
    }
    for(int i=0;i<skuCount-1;i++)
        for(int j=0;j<skuCount-i-1;j++)
            if(SKUList[index[j]].soldCount < SKUList[index[j+1]].soldCount){
                int t=index[j]; index[j]=index[j+1]; index[j+1]=t;
            }
    printf("\nABC Analysis:\n");
    float cum=0;
    for(int i=0;i<skuCount;i++){
        if(total==0) break;
        cum += (SKUList[index[i]].soldCount*100.0)/total;
        char g = (cum<=80)?'A':(cum<=95?'B':'C');
        printf("%s | Sold=%d | %.2f%% | %c\n",
               SKUList[index[i]].name,SKUList[index[i]].soldCount,cum,g);
    }
}
