#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_TABLE_SIZE 10
#define PRODUCT_TABLE_SIZE 10

typedef struct User {
    char name[50];
    int id; // Unique user ID
    int flag; // 0: Empty, 1: Occupied
} User;

typedef struct Product {
    char name[50];
    int id; // Unique product ID
    int flag; // 0: Empty, 1: Occupied
} Product;

typedef struct HistoryNode {
    int product_id;
    struct HistoryNode *next;
} HistoryNode;

typedef struct {
    int user_id;
    HistoryNode *history;
} UserHistory;

typedef struct TreeNode {
    int product_id;
    struct TreeNode *left, *right;
} TreeNode;

// Global variables
User userTable[USER_TABLE_SIZE];
Product productTable[PRODUCT_TABLE_SIZE];
UserHistory historyGraph[USER_TABLE_SIZE];

// Utility functions
void initTables() {
    for (int i = 0; i < USER_TABLE_SIZE; i++) {
        userTable[i].flag = 0;
        productTable[i].flag = 0;
        historyGraph[i].user_id = -1;
        historyGraph[i].history = NULL;
    }
}

// Hash function
int hash(int id, int size) {
    return id % size;
}

// User Management
void addUser() {
    int id;
    char name[50];
    printf("Enter User ID: ");
    scanf("%d", &id);
    printf("Enter User Name: ");
    scanf("%s", name);

    int index = hash(id, USER_TABLE_SIZE);
    while (userTable[index].flag == 1) {
        index = (index + 1) % USER_TABLE_SIZE;
    }

    userTable[index].id = id;
    strcpy(userTable[index].name, name);
    userTable[index].flag = 1;

    printf("User added successfully!\n");
}

void displayUsers() {
    printf("\nUsers:\n");
    for (int i = 0; i < USER_TABLE_SIZE; i++) {
        if (userTable[i].flag == 1) {
            printf("ID: %d, Name: %s\n", userTable[i].id, userTable[i].name);
        }
    }
}

// Product Management
void addProduct() {
    int id;
    char name[50];
    printf("Enter Product ID: ");
    scanf("%d", &id);
    printf("Enter Product Name: ");
    scanf("%s", name);

    int index = hash(id, PRODUCT_TABLE_SIZE);
    while (productTable[index].flag == 1) {
        index = (index + 1) % PRODUCT_TABLE_SIZE;
    }

    productTable[index].id = id;
    strcpy(productTable[index].name, name);
    productTable[index].flag = 1;

    printf("Product added successfully!\n");
}

void displayProducts() {
    printf("\nProducts:\n");
    for (int i = 0; i < PRODUCT_TABLE_SIZE; i++) {
        if (productTable[i].flag == 1) {
            printf("ID: %d, Name: %s\n", productTable[i].id, productTable[i].name);
        }
    }
}

// Browsing and Purchase History
void addHistory(int user_id, int product_id) {
    int index = hash(user_id, USER_TABLE_SIZE);
    while (userTable[index].id != user_id && userTable[index].flag == 1) {
        index = (index + 1) % USER_TABLE_SIZE;
    }

    if (userTable[index].flag == 0) {
        printf("User not found!\n");
        return;
    }

    HistoryNode *newNode = (HistoryNode *)malloc(sizeof(HistoryNode));
    newNode->product_id = product_id;
    newNode->next = historyGraph[index].history;
    historyGraph[index].history = newNode;

    printf("History added successfully!\n");
}

void displayHistory() {
    for (int i = 0; i < USER_TABLE_SIZE; i++) {
        if (userTable[i].flag == 1) {
            printf("History for User ID %d (%s): ", userTable[i].id, userTable[i].name);
            HistoryNode *temp = historyGraph[i].history;
            while (temp != NULL) {
                printf("%d -> ", temp->product_id);
                temp = temp->next;
            }
            printf("NULL\n");
        }
    }
}

// Recommendation Engine
TreeNode *insertIntoTree(TreeNode *root, int product_id) {
    if (root == NULL) {
        TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
        newNode->product_id = product_id;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (product_id < root->product_id) {
        root->left = insertIntoTree(root->left, product_id);
    } else {
        root->right = insertIntoTree(root->right, product_id);
    }
    return root;
}

void recommendProducts(TreeNode *root) {
    if (root != NULL) {
        recommendProducts(root->left);
        printf("%d ", root->product_id);
        recommendProducts(root->right);
    }
}

// Main Function
int main() {
    int choice;
    TreeNode *recommendationTree = NULL;

    initTables();

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add User\n2. Display Users\n3. Add Product\n4. Display Products\n");
        printf("5. Add Browsing/Purchase History\n6. Display Browsing History\n");
        printf("7. Add Product to Recommendation Tree\n8. Display Recommendations\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); break;
            case 2: displayUsers(); break;
            case 3: addProduct(); break;
            case 4: displayProducts(); break;
            case 5: {
                int user_id, product_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                printf("Enter Product ID: ");
                scanf("%d", &product_id);
                addHistory(user_id, product_id);
                break;
            }
            case 6: displayHistory(); break;
            case 7: {
                int product_id;
                printf("Enter Product ID to add to recommendation tree: ");
                scanf("%d", &product_id);
                recommendationTree = insertIntoTree(recommendationTree, product_id);
                break;
            }
            case 8: {
                printf("Recommended Products: ");
                recommendProducts(recommendationTree);
                printf("\n");
                break;
            }
            case 9: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}