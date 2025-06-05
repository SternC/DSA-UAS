#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Movie {
    int day;
    int month;
    char movieName[50];
    char genre[50];
    char director[50];
    int duration;
    char age[10];
    struct Movie *left, *right;
} Movie;

void loadMovies(Movie **root) {
    FILE *fp = fopen("movie.txt", "r");
    if (!fp) {
        printf("Failed to open movie.txt\n");
        return;
    }
    
    *root = NULL;
    int count = 0;

    while ((!feof(fp))) {
        Movie *newNode = (Movie *)malloc(sizeof(Movie));
        if (!newNode) {
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
                   newNode->age) == 7) {

        newNode->left = newNode->right = NULL;
        count++;

         if (*root == NULL) {
                *root = newNode;
            } 
            else {
                Movie *curr = *root;
                while (1) {
                    if (newNode->month < curr->month ||
                    (newNode->month == curr->month && newNode->day < curr->day)) {
                        if (curr->left == NULL) {
                            curr->left = newNode;
                            break;
                        }
                        curr = curr->left;
                    } 
                    else {
                        if (curr->right == NULL) {
                            curr->right = newNode;
                            break;
                        }
                        curr = curr->right;
                    }
                }
            }
        }
        else {
                printf("Error reading movie data from file.\n");
                break;
            }
    }
    fclose(fp);
    printf("%d movie(s) successfully loaded.\n", count);
}

void viewMovies(Movie *root) {
    if (root == NULL) return;
    else{
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

void addMovie(Movie **root) {
    Movie *newNode = (Movie *)malloc(sizeof(Movie));
    if (!newNode) {
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
    if (!fp) {
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

    if (*root == NULL) {
        *root = newNode;
    } 
    else {
        Movie *curr = *root;
        while (1) {
            if (newNode->month < curr->month ||
                (newNode->month == curr->month && newNode->day < curr->day)) {
                if (curr->left == NULL) 
                {
                    curr->left = newNode;
                    break;
                }
                curr = curr->left;
            } 
            else {
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

void searchMovieByDate(Movie *root, int day, int month) {
    if (root == NULL) return;

    if (month < root->month || (month == root->month && day < root->day)) {
        searchMovieByDate(root->left, day, month);
    } else if (month > root->month || (month == root->month && day > root->day)) {
        searchMovieByDate(root->right, day, month);
    } else {
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


void rewriteMovies(FILE *fp, Movie *root) {
    if (root == NULL) return;

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

int removeMovie(Movie **root, int day, int month) {
    if (*root == NULL) {
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
    else {
        // 1. Leaf node (tidak punya anak)
        if (node->left == NULL && node->right == NULL) {
            free(node);
            *root = NULL;
        }

        // 2. Hanya punya satu anak (kanan)
        else if (node->left == NULL) {
            Movie *temp = node->right;
            free(node);
            *root = temp;
        }

        // 2. Hanya punya satu anak (kiri)
        else if (node->right == NULL) {
            Movie *temp = node->left;
            free(node);
            *root = temp;
        }

        // 3. Punya dua anak
        else {
            Movie *successorParent = node;
            Movie *successor = node->right;

            while (successor->left != NULL) {
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

            if (successorParent == node) {
                successorParent->right = successor->right;
            } 
            else {
                successorParent->left = successor->right;
            }
            free(successor);
        }
        
        return 1; 
    }
}

void deleteNodes(Movie *root) {
    if (root == NULL) return;

    deleteNodes(root->left);
    deleteNodes(root->right);

    free(root);
}

int main() {
    Movie *root = NULL;
    int choose = 0;
    int menu = 0;
    int list = 0;
    int day, month;
    char confirm;

    loadMovies(&root);

    while (choose != 3) {
        printf("\nOwner (1) or User (2) or Exit (3): ");
        scanf("%d", &choose);
        getchar();

        if (choose == 1) {
            while (menu != 3) {
                printf("\n1. Add Movie\n2. Remove Movie\n3. Exit\nChoose: ");
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
                else if (menu == 2) {
                    printf("Input Release Day to Delete  : ");
                    scanf("%d", &day);
                    printf("Input Release Month to Delete: ");
                    scanf("%d", &month);

                    searchMovieByDate(root, day, month);

                    printf("Are you sure you want to delete this movie? (y/n): ");
                    scanf(" %c", &confirm);

                    if (confirm == 'y' || confirm == 'Y') {
                        int removed = removeMovie(&root, day, month);
                        if (removed) {
                            printf("Movie removed successfully.\n");

                            FILE *fp = fopen("movie.txt", "w");
                            if (!fp || fp == NULL) {
                                printf("Failed to open movie.txt for writing.\n");
                            } 
                            else {
                                rewriteMovies(fp, root);
                                fclose(fp);
                            }
                        } 
                        else {
                            printf("Movie not found.\n");
                        }
                    } 
                    else {
                        printf("Delete cancelled.\n");
                    }
                    printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
                    printf(" Date-Month |                  Movie Name                   |        Genre       |        Director      |  Duration  |Age Rating|\n");
                    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
                    viewMovies(root);
                }
            }
            deleteNodes(root);
            printf("Thank you! We appreciate your choice.\n");
        } 
        else if (choose == 2) {
            while (list != 3) {
                printf("\n1. View All Movies\n2. Search Movie\n3. Exit\nChoose: ");
                scanf("%d", &list);
                getchar();

                if (list == 1) {
                    printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
                    printf(" Date-Month |                  Movie Name                   |        Genre       |        Director      |  Duration  |Age Rating|\n");
                    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
                    viewMovies(root);
                }
                else if (list == 2) {
                    printf("Input Release Day to Delete  : ");
                    scanf("%d", &day);
                    printf("Input Release Month to Delete: ");
                    scanf("%d", &month);
                    searchMovieByDate(root, day, month);
                    printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
                    printf(" Date-Month |                  Movie Name                   |        Genre       |        Director      |  Duration  |Age Rating|\n");
                    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
                    viewMovies(root);
                }
            }
            deleteNodes(root);
            printf("Thank you! We appreciate your choice.\n");
        }
    }

    printf("\nThank you for using our feature. Have a good day!\n");
    return 0;
}
