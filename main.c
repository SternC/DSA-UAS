#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Movie
{
    int day;
    int month;
    char movieName[50];
    char genre[50];
    char director[50];
    int duration;
    char age[10];
    struct Movie *left, *right;
} Movie;

struct food
{
    char name[50];
    int stock;
    int price;
};

// MOVIES MENU (SOAL NOMOR 1)
void loadMovies(Movie **root)
{
    FILE *fp = fopen("movie.txt", "r");
    if (!fp)
    {
        printf("Failed to open movie.txt\n");
        return;
    }

    *root = NULL;
    int count = 0;

    while ((!feof(fp)))
    {
        Movie *newNode = (Movie *)malloc(sizeof(Movie));
        if (!newNode)
        {
            printf("Memory allocation failed\n");
            break;
        }

        if (fscanf(fp, "%d-%d#%[^#]#%[^#]#%d#%[^#]#%[^\n]\n",
                   &newNode->day,
                   &newNode->month,
                   newNode->movieName,
                   newNode->genre,
                   &newNode->duration,
                   newNode->director,
                   newNode->age) == 7)
        {

            newNode->left = newNode->right = NULL;
            count++;

            if (*root == NULL)
            {
                *root = newNode;
            }
            else
            {
                Movie *curr = *root;
                while (1)
                {
                    if (newNode->month < curr->month ||
                        (newNode->month == curr->month && newNode->day < curr->day))
                    {
                        if (curr->left == NULL)
                        {
                            curr->left = newNode;
                            break;
                        }
                        curr = curr->left;
                    }
                    else
                    {
                        if (curr->right == NULL)
                        {
                            curr->right = newNode;
                            break;
                        }
                        curr = curr->right;
                    }
                }
            }
        }
        else
        {
            printf("Error reading movie data from file.\n");
            break;
        }
    }
    fclose(fp);
    printf("%d movie(s) successfully loaded.\n", count);
}

void viewMovies(Movie *root)
{
    if (root == NULL)
        return;
    else
    {
        viewMovies(root->left);
        printf("|   %02d-%02d   | %-45s | %-18s | %-20s | %5d mins | %8s |\n",
               root->day, root->month,
               root->movieName,
               root->genre,
               root->director,
               root->duration,
               root->age);
        viewMovies(root->right);
    }
}

void addMovie(Movie **root)
{
    Movie *newNode = (Movie *)malloc(sizeof(Movie));
    if (!newNode)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\nRelease Date (1-30): ");
    scanf("%d", &newNode->day);
    printf("Release Month (1-12): ");
    scanf("%d", &newNode->month);
    getchar();
    printf("Movie Name          : ");
    scanf(" %[^\n]", newNode->movieName);
    printf("Genre               : ");
    scanf(" %[^\n]", newNode->genre);
    printf("Duration            : ");
    scanf("%d", &newNode->duration);
    getchar();
    printf("Director            : ");
    scanf(" %[^\n]", newNode->director);
    printf("Age Rating          : ");
    scanf(" %[^\n]", newNode->age);

    newNode->left = newNode->right = NULL;

    FILE *fp = fopen("movie.txt", "a");
    if (!fp)
    {
        printf("Failed to open movie.txt for appending.\n");
        free(newNode);
        return;
    }
    fprintf(fp, "\n%d-%d#%s#%s#%d#%s#%s\n",
            newNode->day, newNode->month,
            newNode->movieName,
            newNode->genre,
            newNode->duration,
            newNode->director,
            newNode->age);
    fclose(fp);

    if (*root == NULL)
    {
        *root = newNode;
    }
    else
    {
        Movie *curr = *root;
        while (1)
        {
            if (newNode->month < curr->month ||
                (newNode->month == curr->month && newNode->day < curr->day))
            {
                if (curr->left == NULL)
                {
                    curr->left = newNode;
                    break;
                }
                curr = curr->left;
            }
            else
            {
                if (curr->right == NULL)
                {
                    curr->right = newNode;
                    break;
                }
                curr = curr->right;
            }
        }
    }

    printf("Movie added successfully.\n");
}

void searchMovieByDate(Movie *root, int day, int month)
{
    if (root == NULL)
        return;

    if (month < root->month || (month == root->month && day < root->day))
    {
        searchMovieByDate(root->left, day, month);
    }
    else if (month > root->month || (month == root->month && day > root->day))
    {
        searchMovieByDate(root->right, day, month);
    }
    else
    {
        printf("\nMovie Found!\n");
        printf("------------------------------\n");
        printf("Release Date : %02d-%02d\n", root->day, root->month);
        printf("Movie Name   : %s\n", root->movieName);
        printf("Genre        : %s\n", root->genre);
        printf("Duration     : %d minutes\n", root->duration);
        printf("Director     : %s\n", root->director);
        printf("Age Rating   : %s\n", root->age);
        printf("------------------------------\n");
    }
}

void rewriteMovies(FILE *fp, Movie *root)
{
    if (root == NULL)
        return;

    rewriteMovies(fp, root->left);

    fprintf(fp, "%d-%d#%s#%s#%d#%s#%s\n",
            root->day,
            root->month,
            root->movieName,
            root->genre,
            root->duration,
            root->director,
            root->age);

    rewriteMovies(fp, root->right);
}

int removeMovie(Movie **root, int day, int month)
{
    if (*root == NULL)
    {
        printf("Movie not found!\n");
        return 0;
    }

    Movie *node = *root;

    if (month < node->month || (month == node->month && day < node->day))
    {
        removeMovie(&node->left, day, month);
    }
    else if (month > node->month || (month == node->month && day > node->day))
    {
        removeMovie(&node->right, day, month);
    }
    else
    {
        // 1. Leaf node (tidak punya anak)
        if (node->left == NULL && node->right == NULL)
        {
            free(node);
            *root = NULL;
        }

        // 2. Hanya punya satu anak (kanan)
        else if (node->left == NULL)
        {
            Movie *temp = node->right;
            free(node);
            *root = temp;
        }

        // 2. Hanya punya satu anak (kiri)
        else if (node->right == NULL)
        {
            Movie *temp = node->left;
            free(node);
            *root = temp;
        }

        // 3. Punya dua anak
        else
        {
            Movie *successorParent = node;
            Movie *successor = node->right;

            while (successor->left != NULL)
            {
                successorParent = successor;
                successor = successor->left;
            }

            node->day = successor->day;
            node->month = successor->month;
            strcpy(node->movieName, successor->movieName);
            strcpy(node->genre, successor->genre);
            strcpy(node->director, successor->director);
            node->duration = successor->duration;
            strcpy(node->age, successor->age);

            printf("Removing movie: %s\n", successor->movieName);

            if (successorParent == node)
            {
                successorParent->right = successor->right;
            }
            else
            {
                successorParent->left = successor->right;
            }
            free(successor);
        }

        return 1;
    }
}

void deleteNodes(Movie *root)
{
    if (root == NULL)
        return;

    deleteNodes(root->left);
    deleteNodes(root->right);

    free(root);
}

// CINEMA LOCATION (SOAL NOMOR 2)
void swap2(char data[][2][101], int i, int j)
{
    char temp[101];
    for (int k = 0; k < 2; k++)
    {
        strcpy(temp, data[i][k]);
        strcpy(data[i][k], data[j][k]);
        strcpy(data[j][k], temp);
    }
}

int compare2(char arr[][2][101], int i, int j)
{
    return strcmp(arr[i][0], arr[j][0]);
}

void minHeapify2(char arr[][2][101], int counter, int i)
{
    int smallest = i;
    int left = 2 * i;
    int right = 2 * i + 1;

    if (left <= counter && compare2(arr, left, smallest) < 0)
    {
        smallest = left;
    }

    if (right <= counter && compare2(arr, right, smallest) < 0)
    {
        smallest = right;
    }

    if (smallest != i)
    {
        swap2(arr, i, smallest);
        minHeapify2(arr, counter, smallest);
    }
}

void minHeap2(char arr[][2][101], int counter)
{
    for (int i = counter / 2; i >= 1; i--)
    {
        minHeapify2(arr, counter, i);
    }
}

void heapSort2(char arr[][2][101], int counter)
{
    minHeap2(arr, counter);

    for (int i = counter; i > 1; i--)
    {
        swap2(arr, 1, i);
        minHeapify2(arr, i - 1, 1);
    }

    for (int i = 1; i <= counter / 2; i++)
    {
        swap2(arr, i, counter - i + 1);
    }
}

void printHeap2(char arr[][2][101], int counter)
{
    for (int i = 1; i <= counter; i++)
    {
        printf("%s#%s\n", arr[i][0], arr[i][1]);
    }
}

void deleteMinHeap2(char arr[][2][101], int *counter, int i)
{
    if (*counter < 1 || i > *counter)
    {
        return;
    }
    swap2(arr, i, *counter);
    (*counter)--;
    minHeapify2(arr, *counter, i);
}

void swap3(char arr[][3][101], int i, int j)
{
    char temp[101];
    for (int k = 0; k < 3; k++)
    {
        strcpy(temp, arr[i][k]);
        strcpy(arr[i][k], arr[j][k]);
        strcpy(arr[j][k], temp);
    }
}

int compare3(char arr[][3][101], int i, int j)
{
    return strcmp(arr[i][0], arr[j][0]);
}

void minHeapify3(char arr[][3][101], int counter, int i)
{
    int smallest = i;
    int left = 2 * i;
    int right = 2 * i + 1;

    if (left <= counter && compare3(arr, left, smallest) < 0)
        smallest = left;

    if (right <= counter && compare3(arr, right, smallest) < 0)
        smallest = right;

    if (smallest != i)
    {
        swap3(arr, i, smallest);
        minHeapify3(arr, counter, smallest);
    }
}

void minHeap3(char arr[][3][101], int counter)
{
    for (int i = counter / 2; i >= 1; i--)
    {
        minHeapify3(arr, counter, i);
    }
}

void heapSort3(char arr[][3][101], int counter)
{
    minHeap3(arr, counter);

    for (int i = counter; i > 1; i--)
    {
        swap3(arr, 1, i);
        minHeapify3(arr, i - 1, 1);
    }

    for (int i = 1; i <= counter / 2; i++)
    {
        swap3(arr, i, counter - i + 1);
    }
}

void printHeap3(char arr[][3][101], int counter)
{
    for (int i = 1; i <= counter; i++)
    {
        printf("%s#%s\n", arr[i][0], arr[i][2]);
    }
}

void deleteMinHeap3(char arr[][3][101], int *counter, int i)
{
    if (*counter < 1 || i > *counter)
    {
        return;
    }
    swap3(arr, i, *counter);
    (*counter)--;
    minHeapify3(arr, *counter, i);
}

void printHeap4(char arr[][3][101], int counter, char selectedProvinceCode[])
{
    for (int i = 0; i < counter; i++)
    {
        if (strcmp(arr[i][1], selectedProvinceCode) == 0)
        {
            printf("%s#%s\n", arr[i][0], arr[i][2]);
        }
    }
}

void addCinemaLocationAndNewProvince(int counterProv)
{
    char newProvince[51], newLocation[51];

    FILE *cinProvFile = fopen("cinema_province.txt", "a");
    FILE *cinLocFile = fopen("cinema_location.txt", "a");

    printf("Please Use Uppercase To Input!\n");
    printf("Input New Province: ");
    scanf(" %[^\n]", newProvince);
    fprintf(cinProvFile, "%s#%d\n", newProvince, counterProv + 1);

    printf("Input New Location: ");
    scanf(" %[^\n]", newLocation);
    fprintf(cinLocFile, "%s#%d#1\n", newLocation, counterProv + 1);
    printf("\n");

    fclose(cinProvFile);
    fclose(cinLocFile);
}

void addCinemaLocationFromExistingProvince(char provinceAndCode[100][2][101], char locationAndCodeAndNum[100][3][101])
{
    char provinceCode[101];
    char newLocation[51];
    int newNum, counter;

    newNum = counter = 0;

    FILE *cinProvFile = fopen("cinema_province.txt", "r");
    FILE *cinLocFile = fopen("cinema_location.txt", "a+");

    printf("\nPlease Input The Code!\n");
    printf("Choose Province's Code: ");
    scanf("%s", provinceCode);
    printf("\n");

    while (1)
    {
        if (strcmp(locationAndCodeAndNum[counter][1], provinceCode) == 0)
        {
            newNum++;
        }
        counter++;
        if (strcmp(locationAndCodeAndNum[counter][0], "") == 0)
        {
            break;
        }
    }

    printf("Please Use Uppercase To Input!\n");
    printf("Input New Location: ");
    scanf(" %[^\n]", newLocation);
    fprintf(cinLocFile, "%s#%s#%d\n", newLocation, provinceCode, newNum + 1);
    printf("\n");

    fclose(cinProvFile);
    fclose(cinLocFile);
}

void deleteCinemaLocation(int *counterLoc, char provinceAndCode[100][2][101], char locationAndCodeAndNum[100][3][101])
{
    char provinceCode[101], cinemaNum[101];
    char filteredCinema[100][3][101];
    int counter;

    FILE *cinProvFile = fopen("cinema_province.txt", "r");
    FILE *cinLocFile = fopen("cinema_location.txt", "w");

    printf("\nPlease Input The Code!\n");
    printf("Choose Province's Code: ");
    scanf("%s", provinceCode);
    printf("\n");

    counter = 1;

    for (int i = 1; i <= *counterLoc; i++)
    {
        if (strcmp(locationAndCodeAndNum[i][1], provinceCode) == 0)
        {
            for (int j = 0; j < 3; j++)
            {
                strcpy(filteredCinema[counter][j], locationAndCodeAndNum[i][j]);
            }

            counter++;
        }
    }

    heapSort3(filteredCinema, counter - 1);
    printHeap3(filteredCinema, counter - 1);

    printf("Choose Unwanted Cinema: ");
    scanf("%s", &cinemaNum);
    printf("\n");

    counter = 1;
    while (strcmp(locationAndCodeAndNum[counter][1], provinceCode) != 0 || strcmp(locationAndCodeAndNum[counter][2], cinemaNum) != 0)
    {
        counter++;
    }

    deleteMinHeap3(locationAndCodeAndNum, counterLoc, counter);

    counter = 1;
    (*counterLoc)++;
    for (int i = 1; i < *counterLoc; i++)
    {
        fprintf(cinLocFile, "%s#%s#%s\n", locationAndCodeAndNum[counter][0], locationAndCodeAndNum[counter][1], locationAndCodeAndNum[counter][2]);
        counter++;
    }

    fclose(cinProvFile);
    fclose(cinLocFile);
}

void deleteCinemaProvinceAndLocation(int *counterLoc, int *counterProv, char provinceAndCode[100][2][101], char locationAndCodeAndNum[100][3][101])
{
    char provinceCode[101];
    char provinceDisplay[100][2][101];
    int counter, counterCode;

    FILE *cinProvFile = fopen("cinema_province.txt", "w");
    FILE *cinLocFile = fopen("cinema_location.txt", "w");

    counter = 1;

    for (int i = 1; i <= *counterProv; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            strcpy(provinceDisplay[counter][j], provinceAndCode[i][j]);
        }

        counter++;
    }

    heapSort2(provinceDisplay, counter - 1);
    printHeap2(provinceDisplay, counter - 1);

    printf("\nPlease Input The Code!\n");
    printf("Choose Province's Code: ");
    scanf("%s", provinceCode);
    printf("\n");

    counter = 1;
    while (strcmp(provinceAndCode[counter][1], provinceCode) != 0)
    {
        counter++;
    }

    deleteMinHeap2(provinceAndCode, counterProv, counter - 1);

    for (int i = 1; i < *counterProv; i++)
    {
        fprintf(cinProvFile, "%s#%s\n", provinceAndCode[i][0], provinceAndCode[i][1]);
    }

    fclose(cinProvFile);

    counter = 1;
    counterCode = 0;
    while (1)
    {
        if (strcmp(locationAndCodeAndNum[counter][1], provinceCode) == 0)
        {
            counterCode++;
        }
        counter++;
        if (strcmp(locationAndCodeAndNum[counter][1], "") == 0)
        {
            break;
        }
    }

    for (int i = 1; i < counterCode; i++)
    {
        counter = 1;
        while (strcmp(locationAndCodeAndNum[counter][1], provinceCode) != 0)
        {
            counter++;
        }

        deleteMinHeap3(locationAndCodeAndNum, counterLoc, counter);
    }

    for (int i = 1; i < *counterLoc; i++)
    {
        fprintf(cinLocFile, "%s#%s#%s\n", locationAndCodeAndNum[i][0], locationAndCodeAndNum[i][1], locationAndCodeAndNum[i][2]);
    }

    fclose(cinLocFile);
}

void deleteAllLocationAndProvince(int *counterProv, int *counterLoc, char provinceAndCode[][2][101], char locationAndCodeAndNum[][3][101])
{
    while (*counterProv > 0)
    {
        deleteMinHeap2(provinceAndCode, counterProv, 0);
    }

    while (*counterLoc > 0)
    {
        deleteMinHeap3(locationAndCodeAndNum, counterLoc, 0);
    }
}

int binarySearchProvince(char target[], char provinceAndCode[][2][101], int n)
{
    int left = 0, right = n - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        int cmp = strcmp(target, provinceAndCode[mid][0]);
        if (cmp == 0)
            return mid; // found
        else if (cmp < 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1; // not found
}

int binarySearchCinema(char target[], char locationAndCodeAndNum[][3][101], int n)
{
    int left = 0, right = n - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        int cmp = strcmp(target, locationAndCodeAndNum[mid][0]);
        if (cmp == 0)
            return mid; // found
        else if (cmp < 0)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1; // not found
}

void cinemaOwner()
{
    int userActivity;
    int counterProv, counterLoc;

    char provinceAndCode[100][2][101];
    char locationAndCodeAndNum[100][3][101];

    while (1)
    {
        counterProv = counterLoc = 1;

        FILE *cinProvFile = fopen("cinema_province.txt", "r");
        FILE *cinLocFile = fopen("cinema_location.txt", "r");

        while (!feof(cinProvFile))
        {
            fscanf(cinProvFile, " %[^#]#%[^\n]\n", provinceAndCode[counterProv][0], provinceAndCode[counterProv][1]);
            counterProv++;
        }

        while (!feof(cinLocFile))
        {
            fscanf(cinLocFile, " %[^#]#%[^#]#%[^\n]\n", locationAndCodeAndNum[counterLoc][0], locationAndCodeAndNum[counterLoc][1], locationAndCodeAndNum[counterLoc][2]);
            counterLoc++;
        }

        fclose(cinProvFile);
        fclose(cinLocFile);

        printf("==========================\n");
        printf("Cinema Menu\n");
        printf("==========================\n");
        printf("1. Add New Cinema's Location and New Province\n");
        printf("2. Add New Cinema's Location From Existing Province\n");
        printf("3. Delete Cinema's Location From a Province\n");
        printf("4. Delete Cinema's Province\n");
        printf("5. Exit Owner Mode\n");
        printf("What Do You Want To Do? ");
        scanf("%d", &userActivity);
        printf("\n");

        counterLoc -= 1;
        counterProv -= 1;

        switch (userActivity)
        {
        case 1:
            addCinemaLocationAndNewProvince(counterProv);
            break;

        case 2:
            heapSort2(provinceAndCode, counterProv);
            printHeap2(provinceAndCode, counterProv);
            addCinemaLocationFromExistingProvince(provinceAndCode, locationAndCodeAndNum);
            break;

        case 3:
            heapSort2(provinceAndCode, counterProv);
            printHeap2(provinceAndCode, counterProv);
            deleteCinemaLocation(&counterLoc, provinceAndCode, locationAndCodeAndNum);
            break;

        case 4:
            deleteCinemaProvinceAndLocation(&counterLoc, &counterProv, provinceAndCode, locationAndCodeAndNum);
            break;

        default:
            deleteAllLocationAndProvince(&counterProv, &counterLoc, provinceAndCode, locationAndCodeAndNum);
            return;
        }
        deleteAllLocationAndProvince(&counterProv, &counterLoc, provinceAndCode, locationAndCodeAndNum);
    }
}

void chooseCinema(char chosenProv[], char chosenCinema[])
{
    char userProv[101], userCinema[101];
    int counterProv = 1, counterLoc = 1, filteredCount = 1;

    char provinceAndCode[100][2][101];
    char locationAndCodeAndNum[100][3][101];
    char filteredCinema[100][3][101];

    FILE *cinProvFile = fopen("cinema_province.txt", "r");
    FILE *cinLocFile = fopen("cinema_location.txt", "r");

    while (fscanf(cinProvFile, " %[^#]#%[^\n]\n", provinceAndCode[counterProv][0], provinceAndCode[counterProv][1]) == 2)
    {
        counterProv++;
    }

    while (fscanf(cinLocFile, " %[^#]#%[^#]#%[^\n]\n", locationAndCodeAndNum[counterLoc][0], locationAndCodeAndNum[counterLoc][1], locationAndCodeAndNum[counterLoc][2]) == 3)
    {
        counterLoc++;
    }

    fclose(cinProvFile);
    fclose(cinLocFile);

    heapSort2(provinceAndCode, counterProv - 1);
    printHeap2(provinceAndCode, counterProv - 1);
    printf("\n");

    printf("Choose Your Province [CASE SENSITIVE]: ");
    scanf(" %[^\n]", userProv);

    int provIndex = binarySearchProvince(userProv, provinceAndCode, counterProv);
    if (provIndex == -1)
    {
        printf("Province not found!\n");
        return;
    }

    strcpy(chosenProv, provinceAndCode[provIndex][0]);
    char *selectedProvCode = provinceAndCode[provIndex][1];
    printf("Selected Province: %s\n\n", chosenProv);

    for (int i = 1; i < counterLoc; i++)
    {
        if (strcmp(locationAndCodeAndNum[i][1], selectedProvCode) == 0)
        {
            for (int j = 0; j < 3; j++)
            {
                strcpy(filteredCinema[filteredCount][j], locationAndCodeAndNum[i][j]);
            }
            filteredCount++;
        }
    }

    heapSort3(filteredCinema, filteredCount - 1);
    printHeap3(filteredCinema, filteredCount - 1);

    printf("\nChoose Your Cinema [CASE SENSITIVE]: ");
    scanf(" %[^\n]", userCinema);

    int found = 0;
    for (int i = 1; i < filteredCount; i++)
    {
        if (strcmp(filteredCinema[i][0], userCinema) == 0)
        {
            strcpy(chosenCinema, filteredCinema[i][0]);
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("Selected Cinema: %s\n\n", chosenCinema);
    }
    else
    {
        printf("Cinema not found!\n\n");
    }
}

// FOOD AREA (SOAL NOMOR 3)

void addFood(struct food snack[], int n)
{
    FILE *add = fopen("food.txt", "a");
    for (int i = 0; i < n; i++)
    {
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

void foodPrint(struct food snack[])
{
    FILE *fp = fopen("food.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening food file.\n");
        return;
    }

    printf("========================================\n");
    printf("                  Menu                  \n");
    printf("========================================\n");

    struct food item;
    int n = 0;
    while (fscanf(fp, "%49[^#]#%d#%d\n", item.name, &item.stock, &item.price) == 3)
    {
        printf("%d.  %-10s : %-20s\n", n + 1, "Food", item.name);
        printf("    %-10s : %-18d\n", "Price", item.price);
        printf("========================================\n");
        n++;
    }
    fclose(fp);
}

void swap(struct food *a, struct food *b)
{
    struct food temp = *a;
    *a = *b;
    *b = temp;
}

void insertionSort(struct food snack[])
{
    FILE *fp = fopen("food.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening food file.\n");
        return;
    }
    int n = 0;
    while (fscanf(fp, "%49[^#]#%d#%d\n", snack[n].name, &snack[n].stock, &snack[n].price) == 3)
    {
        n++;
    }
    fclose(fp);

    int i, j;
    struct food temp;
    for (i = 0; i < n; i++)
    {
        temp = snack[i];
        for (j = i - 1; j >= 0 && snack[j].price > temp.price; j--)
        {
            snack[j + 1] = snack[j];
        }
        snack[j + 1] = temp;
    }

    printf("========================================\n");
    printf("                  Menu                  \n");
    printf("========================================\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d.  %-10s : %-20s\n", i + 1, "Food", snack[i].name);
        printf("    %-10s : %-18d\n", "Price", snack[i].price);
        printf("========================================\n");
    }
}

void selectionSort(struct food snack[])
{
    FILE *fp = fopen("food.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening food file.\n");
        return;
    }
    int n = 0;
    while (fscanf(fp, "%49[^#]#%d#%d\n", snack[n].name, &snack[n].stock, &snack[n].price) == 3)
    {
        n++;
    }
    fclose(fp);

    for (int i = 0; i < n - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
        {
            if (strcmp(snack[j].name, snack[min].name) < 0)
            {
                min = j;
            }
        }
        if (min != i)
        {
            swap(&snack[i], &snack[min]);
        }
    }

    printf("========================================\n");
    printf("                  Menu                  \n");
    printf("========================================\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d.  %-10s : %-20s\n", i + 1, "Food", snack[i].name);
        printf("    %-10s : %-18d\n", "Price", snack[i].price);
        printf("========================================\n");
    }
}

void movieOwner()
{
    Movie *root = NULL;
    int menu = 0;
    int day, month;
    char confirm;

    loadMovies(&root);

    while (menu != 3)
    {
        printf("==========================\n");
        printf("Movie Menu\n");
        printf("==========================\n");
        printf("1. Add Movie\n2. Remove Movie\n3. Exit\nChoose: ");
        scanf("%d", &menu);
        getchar();

        if (menu == 1)
        {
            addMovie(&root);
            printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
            printf(" Date-Month |                  Movie Name                   |        Genre       |        Director      |  Duration  |Age Rating|\n");
            printf("---------------------------------------------------------------------------------------------------------------------------------\n");
            viewMovies(root);
        }
        else if (menu == 2)
        {
            printf("Input Release Day to Delete  : ");
            scanf("%d", &day);
            printf("Input Release Month to Delete: ");
            scanf("%d", &month);

            searchMovieByDate(root, day, month);

            printf("Are you sure you want to delete this movie? (y/n): ");
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y')
            {
                int removed = removeMovie(&root, day, month);
                if (removed)
                {
                    printf("Movie removed successfully.\n");

                    FILE *fp = fopen("movie.txt", "w");
                    if (!fp || fp == NULL)
                    {
                        printf("Failed to open movie.txt for writing.\n");
                    }
                    else
                    {
                        rewriteMovies(fp, root);
                        fclose(fp);
                    }
                }
                else
                {
                    printf("Movie not found.\n");
                }
            }
            else
            {
                printf("Delete cancelled.\n");
            }
            printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
            printf(" Date-Month |                  Movie Name                   |        Genre       |        Director      |  Duration  |Age Rating|\n");
            printf("---------------------------------------------------------------------------------------------------------------------------------\n");
            viewMovies(root);
        }
    }
    printf("Thank you! We appreciate your choice.\n");
}

void foodOwner()
{
    int choice, n;
    struct food snack[100];

    while (1)
    {
        printf("==========================\n");
        printf("Food Owner Menu\n");
        printf("==========================\n");
        printf("1. Add Food\n");
        printf("2. Print Food\n");
        printf("3. Sort by Price \n");
        printf("4. Sort by Name\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1:
            printf("How many food items to add? ");
            scanf(" %d", &n);
            addFood(snack, n);
            break;
        case 2:
            foodPrint(snack);
            break;
        case 3:
            insertionSort(snack);
            break;
        case 4:
            selectionSort(snack);
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    }
}

void foodMenu()
{
    struct food snack[100];
    int menuChoice = 0;

    while (menuChoice != 4)
    {
        printf("==========================\n");
        printf("Food Menu || To check menu only, not order\n");
        printf("==========================\n");
        printf("1. View Food Menu\n");
        printf("2. View Food Menu Sorted by Price\n");
        printf("3. View Food Menu Sorted by Name\n");
        printf("4. Exit\n");
        printf("Choose your option: ");
        scanf(" %d", &menuChoice);

        if (menuChoice == 1)
        {
            foodPrint(snack);
        }
        else if (menuChoice == 2)
        {
            insertionSort(snack);
        }
        else if (menuChoice == 3)
        {
            selectionSort(snack);
        }
        else if (menuChoice == 4)
        {
            printf("Exiting Food Menu.\n");
            break;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

int main()
{
    int login, attempt = 0;
    int option = 0, ownerOption = 0;
    char correctUsername[] = {"admin"}, inputtedUsername[50];
    char correctPassword[] = {"admin"}, inputtedPassword[50];

    // CINEMA LOCATION
    char provinceAndCode[100][2][101];
    char locationAndCodeAndNum[100][3][101];
    char userProv[100], userCinema[100];

    // MOVIE
    Movie *root = NULL;
    int day, month;

    // FOOD
    struct food snack[100];

    loadMovies(&root);

    printf("1. Customer\n");
    printf("2. Owner\n");
    printf("Choose Your Mode: ");
    scanf(" %d", &login);
    printf("\n");

    if (login == 1)
    {
        chooseCinema(userProv, userCinema);

        while (option != 4)
        {
            printf("==========================\n");
            printf("Province: %s\n", userProv);
            printf("Cinema: %s\n", userCinema);
            printf("==========================\n");
            printf("Cinema Menu\n");
            printf("1. View Movies\n");
            printf("2. Search Movie\n");
            printf("3. Food Menu\n");
            printf("4. Exit\n");
            printf("Choose your option: ");
            scanf(" %d", &option);

            if (option == 1)
            {
                printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
                printf(" Date-Month |                  Movie Name                   |        Genre       |        Director      |  Duration  |Age Rating|\n");
                printf("---------------------------------------------------------------------------------------------------------------------------------\n");
                viewMovies(root);
            }
            else if (option == 2)
            {
                viewMovies(root);
                printf("\n");
                printf("Please enter the date (e.g any valid number from 1-31) of the movie release date:  ");
                scanf(" %d", &day);
                printf("Please enter the month (e.g any valid number from 01-12) of the movie release date:  ");
                scanf(" %d", &month);

                searchMovieByDate(root, day, month);
            }
            else if (option == 3)
            {
                foodMenu();
            }
            else if (option == 4)
            {
                printf("Thank you for using our service!\n");
                deleteNodes(root);
            }
            else
            {
                printf("Invalid option. Please try again.\n");
            }
        }
    }
    else if (login == 2)
    {
        while (attempt < 3)
        {
            printf("Username: ");
            scanf(" %[^\n]", inputtedUsername);
            printf("Password: ");
            scanf(" %[^\n]", inputtedPassword);
            if (strcmp(correctUsername, inputtedUsername) == 0 && strcmp(correctPassword, inputtedPassword) == 0)
            {
                printf("\n");
                break;
            }
            attempt++;
            if (attempt < 3)
            {
                printf("\nUsername or Password Incorrect (Remaining Attempt: %d)\n\n", 3 - attempt);
            }
            else
            {
                printf("\nTry Again Next Time");
            }
        }

        if (attempt != 3)
        {
            while (ownerOption != 4)
            {
                printf("==========================\n");
                printf("Owner Menu\n");
                printf("==========================\n");
                printf("1. Cinema Location Menu\n");
                printf("2. Cinema Movies Menu\n");
                printf("3. Cinema Food Menu\n");
                printf("4. Exit\n");
                printf("Choose your option: ");
                scanf("%d", &ownerOption);

                if (ownerOption == 1)
                {
                    cinemaOwner();
                }
                else if (ownerOption == 2)
                {
                    movieOwner();
                }
                else if (ownerOption == 3)
                {
                    foodOwner();
                }
                else if (ownerOption == 4)
                {
                    printf("Thank you for using our service!\n");
                    deleteNodes(root);
                    return 0;
                }
                else
                {
                    printf("Invalid option. Please try again.\n");
                }
            }
        }
    }
    return 0;
}