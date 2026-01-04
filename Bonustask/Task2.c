#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct product {
    int productid;
    char name[30];
    float price;
    int qty;
    int taxable;
    float tax_amount;
};

struct setting_conf {
    float tax_rate;
};

struct product bill[20];
int bill_count = 0;
float subtotal = 0;
float total_tax = 0;
float discount = 0;
float final_amount = 0;
struct setting_conf config = {0.13};

void* calculate_tax(void* arg) {
    for(int i=0;i<bill_count;i++) {
        if(bill[i].taxable)
            bill[i].tax_amount = bill[i].price * bill[i].qty * config.tax_rate;
        else
            bill[i].tax_amount = 0;
        total_tax += bill[i].tax_amount;
    }
    return NULL;
}

void* calculate_total(void* arg) {
    for(int i=0;i<bill_count;i++)
        subtotal += bill[i].price * bill[i].qty;
    return NULL;
}

void* final_summary(void* arg) {
    final_amount = subtotal + total_tax - discount;
    return NULL;
}

int main() {
    struct product products[10] = {
        {1,"Pen",10,0,1,0},
        {2,"Pencil",5,0,0,0},
        {3,"Notebook",50,0,1,0},
        {4,"Eraser",3,0,0,0},
        {5,"Marker",25,0,1,0},
        {6,"Scale",15,0,0,0},
        {7,"Bag",800,0,1,0},
        {8,"Bottle",200,0,1,0},
        {9,"File",30,0,0,0},
        {10,"Diary",120,0,1,0}
    };

    int pid, qty, choice;

    do {
        printf("\nProduct List:\n");
        printf("ID\tName\tPrice\n");
        for(int i=0;i<10;i++)
            printf("%d\t%s\t%.2f\n", products[i].productid, products[i].name, products[i].price);

        printf("Enter Product ID: ");
        scanf("%d", &pid);
        printf("Enter Quantity: ");
        scanf("%d", &qty);

        for(int i=0;i<10;i++) {
            if(products[i].productid == pid) {
                bill[bill_count] = products[i];
                bill[bill_count].qty = qty;
                bill_count++;
            }
        }

        printf("Add another product? (1-Yes / 0-No): ");
        scanf("%d", &choice);

    } while(choice == 1);

    printf("Enter Discount (0-99): ");
    scanf("%f", &discount);

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, calculate_total, NULL);
    pthread_create(&t2, NULL, calculate_tax, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_create(&t3, NULL, final_summary, NULL);
    pthread_join(t3, NULL);

    printf("\nSN\tName\tPrice\tQty\tTotal\tTax\tFinal\n");
    for(int i=0;i<bill_count;i++) {
        float total = bill[i].price * bill[i].qty;
        printf("%d\t%s\t%.2f\t%d\t%.2f\t%.2f\t%.2f\n",
               i+1,
               bill[i].name,
               bill[i].price,
               bill[i].qty,
               total,
               bill[i].tax_amount,
               total + bill[i].tax_amount);
    }

    printf("\nSubtotal: %.2f", subtotal);
    printf("\nTotal Tax: %.2f", total_tax);
    printf("\nDiscount: %.2f", discount);
    printf("\nFinal Payable Amount: %.2f\n", final_amount);

    return 0;
}