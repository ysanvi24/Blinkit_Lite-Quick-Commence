#include <stdio.h>
#include "blinkit.h"
#include "blinkit.c" //include the files where all the functions have be 

int main() {
    loadData(); //loads the past data into the functions 
    int choice = -1; //Initialise choice to any integer less than 0.
    //Main menu of the project
    while(choice!=0){
        printf("\n \t \t BLINKIT-LITE MENU \t \t \n");
        printf("1. Add SKU\n");
        printf("2. Update/Delete SKU\n");
        printf("3. Place Order\n");
        printf("4. Deliver Order\n");
        printf("5. Cancel Order\n");
        printf("6. Search SKU by Name\n");
        printf("7. Sort Orders by Time\n");
        printf("8. Top-K Bestsellers\n");
        printf("9. ABC Analysis\n");
        printf("10. Save the Data\n");
        printf("0. Save and Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: Add_SKU(); break;
            case 2: Update_or_Delete_SKU(); break;
            case 3: Place_Order(); break;
            case 4: Deliver_Order(); break;
            case 5: Cancel_Order(); break;
            case 6: Search_SKU_ByName_Substring(); break;
            case 7: Sort_Orders_ByTime(); break;
            case 8: TopK_Bestsellers(); break;
            case 9: ABC_Analysis_SoldCount(); break;
            case 10: printf("Saving the data..."); saveData(); break;
            case 0: 
            saveData(); 
            printf("Data saved. Thank you for using Blinkit-Lite\n"); 
            break;
            default: printf(" Invalid option. Choose a value from (0-9) please!\n");
            int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                break;  //To clear the input buffer if it is a non-number
        }
    } 
    return 0;
}
