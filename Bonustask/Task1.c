#include <stdio.h>
#include <string.h>

struct product {
    int productid;
    char name[30];
    float price;
    int qty;
    int taxable;
};

int main() {
    struct product products[10] = {
        {1,"Pen",10,0,1},
        {2,"Pencil",5,0,0},
        {3,"Notebook",50,0,1},
        {4,"Eraser",3,0,0},
        {5,"Marker",25,0,1},
        {6,"Scale",15,0,0},
        {7,"Bag",800,0,1},
        {8,"Bottle",200,0,1},
        {9,"File",30,0,0},
        {10,"Diary",120,0,1}
    };

    int invoice, choice, pid, qty;
    float grand_total = 0;
    int sn = 1;

    printf("Enter Invoice Number: ");
    scanf("%d", &invoice);

    do {
        printf("\nProduct List:\n");
        printf("ID\tName\t\tPrice\n");
        for(int i=0;i<10;i++)
            printf("%d\t%s\t\t%.2f\n", products[i].productid, products[i].name, products[i].price);

        printf("Enter Product ID: ");
        scanf("%d", &pid);
        printf("Enter Quantity: ");
        scanf("%d", &qty);

        for(int i=0;i<10;i++) {
            if(products[i].productid == pid) {
                float total = products[i].price * qty;
                grand_total += total;
                printf("%d\t%s\t%.2f\t%d\t%.2f\n", sn++, products[i].name, products[i].price, qty, total);
            }
        }

        printf("Add another product? (1-Yes / 0-No): ");
        scanf("%d", &choice);

    } while(choice == 1);

    printf("\nGrand Total: %.2f\n", grand_total);
    return 0;
}
