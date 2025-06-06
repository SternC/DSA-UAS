#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct food{
    char name[50];
    int stock;
    int price;
};

void addfood(struct food snack[], int n) {
    FILE *add = fopen("food.txt", "a");
    for (int i = 0; i < n; i++) {
        printf("Name: ");
        scanf("%49s", snack[i].name); 
        getchar(); 
        printf("Stock: ");
        scanf("%d", &snack[i].stock);
        getchar(); 
        printf("Price: ");
        scanf("%d", &snack[i].price);
        getchar();
        printf("\n");
        fprintf(add, "%s#%d#%d\n", snack[i].name, snack[i].stock, snack[i].price);
    }
    fclose(add);
}




void print(struct food snack[]){
    FILE *fp = fopen("food.txt", "r");
    if (fp == NULL) {
        printf("Error opening food file.\n");
        return;
    }

    printf("========================================\n");
    printf("                  Menu                  \n");
    printf("========================================\n");

    struct food item;
    int n = 0;
    while (fscanf(fp, "%49[^#]#%d#%d\n", item.name, &item.stock, &item.price) == 3) {
        printf("%d.  %-10s : %-20s\n", n+1, "Food", item.name);
        printf("    %-10s : %-18d\n", "Price", item.price);
        printf("========================================\n");
        n++;
    }
    fclose(fp);
}




void swap(struct food *a, struct food *b) {
    struct food temp = *a;
    *a = *b;
    *b = temp;
}

void insertionsort(struct food snack[]){
    FILE *fp = fopen("food.txt", "r");
    if (fp == NULL) {
        printf("Error opening food file.\n");
        return;
    }
    int n = 0;
    while (fscanf(fp, "%49[^#]#%d#%d\n", snack[n].name, &snack[n].stock, &snack[n].price) == 3) {
        n++;
    }
    fclose(fp);

    int i,j;
    struct food temp;
    for(i = 0; i <n; i++){
        temp = snack[i];
        for(j= i-1; j >= 0 && snack[j].price > temp.price; j--) {
            snack[j + 1] = snack[j];
        }
        snack[j + 1] = temp;
    }

    printf("========================================\n");
    printf("                  Menu                  \n");
    printf("========================================\n");

    for (int i = 0; i < n; i++) {
        printf("%d.  %-10s : %-20s\n", i + 1, "Food", snack[i].name);
        printf("    %-10s : %-18d\n", "Price", snack[i].price);
        printf("========================================\n");
    }
}

void selectionsort(struct food snack[]){
    FILE *fp = fopen("food.txt", "r");
    if (fp == NULL) {
        printf("Error opening food file.\n");
        return;
    }
    int n = 0;
    while (fscanf(fp, "%49[^#]#%d#%d\n", snack[n].name, &snack[n].stock, &snack[n].price) == 3) {
        n++;
    }
    fclose(fp);

    for (int i = 0; i < n - 1; i++){
        int min = i;
        for( int j = i+1; j < n; j++){
            if(strcmp(snack[j].name, snack[min].name) < 0){
                min = j;
            }
        }
        if (min != i) {
            swap(&snack[i], &snack[min]);
        }
    }

    printf("========================================\n");
    printf("                  Menu                  \n");
    printf("========================================\n");

    for (int i = 0; i < n; i++) {
        printf("%d.  %-10s : %-20s\n", i + 1, "Food", snack[i].name);
        printf("    %-10s : %-18d\n", "Price", snack[i].price);
        printf("========================================\n");
    }
}

int main(){
    int choice, n;
    struct food snack[100];
    while (1){
        printf("1. Add Food\n");
        printf("2. Print Food\n");
        printf("3. Sort by Price \n");
        printf("4. Sort by Name\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("How many food items to add? ");
                scanf("%d", &n);
                getchar();
                addfood(snack, n);
                break;
            case 2:
                print(snack);
                break;
            case 3:            
                insertionsort(snack);                     
                break;
            case 4:       
                selectionsort(snack);                   
                break;
            case 5:
                printf("Exiting...\n");
                return 0;                          
            default:
                printf("Invalid choice.\n");
        }
    }
}
