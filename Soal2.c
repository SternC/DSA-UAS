#include <stdio.h>
#include <string.h>

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
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < counter && compare2(arr, left, smallest) < 0)
    {
        smallest = left;
    }

    if (right < counter && compare2(arr, right, smallest) < 0)
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
    for (int i = counter / 2 - 1; i >= 0; i--)
    {
        minHeapify2(arr, counter, i);
    }
}

void heapSort2(char arr[][2][101], int counter)
{
    minHeap2(arr, counter);

    for (int i = counter - 1; i > 0; i--)
    {
        swap2(arr, 0, i);
        minHeapify2(arr, i, 0);
    }

    for (int i = 0; i < counter / 2; i++)
    {
        swap2(arr, i, counter - 1 - i);
    }
}

void printHeap2(char arr[][2][101], int counter)
{
    for (int i = 0; i < counter; i++)
    {
        printf("%s#%s\n", arr[i][0], arr[i][1]);
    }
}

void deleteMinHeap2(char arr[][2][101], int *counter, int i)
{
    if (*counter <= 0 || i >= *counter)
    {
        swap2(arr, i, *counter - 1);
        (*counter)--;
        minHeapify2(arr, *counter, i);
    }
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
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < counter && compare3(arr, left, smallest) < 0)
        smallest = left;

    if (right < counter && compare3(arr, right, smallest) < 0)
        smallest = right;

    if (smallest != i)
    {
        swap3(arr, i, smallest);
        minHeapify3(arr, counter, smallest);
    }
}

void heapSort3(char arr[][3][101], int counter)
{
    for (int i = counter / 2 - 1; i >= 0; i--)
    {
        minHeapify3(arr, counter, i);
    }

    for (int i = counter - 1; i > 0; i--)
    {
        swap3(arr, 0, i);
        minHeapify3(arr, i, 0);
    }

    for (int i = 0; i < counter / 2; i++)
    {
        swap3(arr, i, counter - 1 - i);
    }
}

void printHeap3(char arr[][3][101], int counter)
{
    for (int i = 0; i < counter; i++)
    {
        printf("%s#%s\n", arr[i][0], arr[i][2]);
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
    int counter, counterShift;

    FILE *cinProvFile = fopen("cinema_province.txt", "r");
    FILE *cinLocFile = fopen("cinema_location.txt", "w");

    printf("\nPlease Input The Code!\n");
    printf("Choose Province's Code: ");
    scanf("%s", provinceCode);
    printf("\n");

    counter = 0;

    for (int i = 0; i < *counterLoc; i++)
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

    heapSort3(filteredCinema, counter);
    printHeap3(filteredCinema, counter);

    printf("Choose Unwanted Cinema: ");
    scanf("%s", &cinemaNum);
    printf("\n");

    counter = 0;
    while (strcmp(locationAndCodeAndNum[counter][1], provinceCode) != 0 || strcmp(locationAndCodeAndNum[counter][2], cinemaNum) != 0)
    {
        counter++;
    }

    (*counterLoc)--;
    counterShift = *counterLoc - counter;

    for (int i = 0; i < counterShift; i++)
    {
        strcpy(locationAndCodeAndNum[counter][0], locationAndCodeAndNum[counter + 1][0]);
        strcpy(locationAndCodeAndNum[counter][1], locationAndCodeAndNum[counter + 1][1]);
        strcpy(locationAndCodeAndNum[counter][2], locationAndCodeAndNum[counter + 1][2]);
        counter++;
    }

    counter = 0;
    for (int i = 0; i < *counterLoc; i++)
    {
        fprintf(cinLocFile, "%s#%s#%s\n", locationAndCodeAndNum[counter][0], locationAndCodeAndNum[counter][1], locationAndCodeAndNum[counter][2]);
        counter++;
    }

    fclose(cinProvFile);
    fclose(cinLocFile);
}

void owner()
{
    int userActivity;
    int counterProv, counterLoc;

    char provinceAndCode[100][2][101];
    char locationAndCodeAndNum[100][3][101];

    while (1)
    {
        counterProv = counterLoc = 0;

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

        printf("1. Add New Cinema's Location and New Province\n");
        printf("2. Add New Cinema's Location From Existing Province\n");
        printf("3. Delete Cinema's Location From a Province\n");
        printf("4. Delete Cinema's Province\n");
        printf("5. Exit Owner Mode\n");
        printf("What Do You Want To Do? ");
        scanf("%d", &userActivity);
        printf("\n");

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

            // deleteCinemaProvinceAndLocation();
            break;

        default:
            // deleteAllLocationAndProvince();
            return;
        }
        // deleteAllLocationAndProvince();
    }
}

int main()
{
    owner();

    return 0;
}