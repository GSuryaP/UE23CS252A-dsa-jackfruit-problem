#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 100

typedef struct Product {
    char category[50];
    char name[50];
    int price;
    struct Product* next;
} Product;

Product* hash_table[HASH_TABLE_SIZE];

int hash(char* category) {
    return strlen(category) % HASH_TABLE_SIZE;
}

void add_product(char* category, char* name, int price) {
    int index = hash(category);
    Product* new_product = (Product*)malloc(sizeof(Product));
    strcpy(new_product->category, category);
    strcpy(new_product->name, name);
    new_product->price = price;
    new_product->next = hash_table[index];
    hash_table[index] = new_product;
}

typedef struct Purchase {
    char product[50];
    int amount;
    char date[11];
    struct Purchase* next;
} Purchase;

typedef struct Browse {
    char category[50];
    char date[11];
    struct Browse* next;
} Browse;

typedef struct UserNode {
    char name[50];
    struct Purchase* purchases;
    struct Browse* browsing;
    struct UserNode* left;
    struct UserNode* right;
} UserNode;

UserNode* root = NULL;

UserNode* create_user(char* name) {
    UserNode* new_user = (UserNode*)malloc(sizeof(UserNode));
    strcpy(new_user->name, name);
    new_user->purchases = NULL;
    new_user->browsing = NULL;
    new_user->left = NULL;
    new_user->right = NULL;
    return new_user;
}

void add_user(char* name) {
    UserNode* new_user = create_user(name);
    
    if (root == NULL) {
        root = new_user;
    } else {
        UserNode* current = root;
        while (1) {
            if (strcmp(current->name, name) < 0) {
                if (current->right == NULL) {
                    current->right = new_user;
                    break;
                }
                current = current->right;
            } else {
                if (current->left == NULL) {
                    current->left = new_user;
                    break;
                }
                current = current->left;
            }
        }
    }
}

void add_purchase(UserNode* user, char* product, int amount, char* date) {
    Purchase* new_purchase = (Purchase*)malloc(sizeof(Purchase));
    strcpy(new_purchase->product, product);
    new_purchase->amount = amount;
    strcpy(new_purchase->date, date);
    new_purchase->next = user->purchases;
    user->purchases = new_purchase;
}

UserNode* find_user(char* name) {
    UserNode* current = root;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        } else if (strcmp(current->name, name) < 0) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return NULL;
}

void recommend_products(char* purchased_product) {
    Product* current = NULL;
    int purchased_product_category_index = -1;

    // Find the category of the purchased product
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        current = hash_table[i];
        while (current != NULL) {
            if (strcmp(current->name, purchased_product) == 0) {
                purchased_product_category_index = i;
                break;
            }
            current = current->next;
        }
        if (purchased_product_category_index != -1) {
            break;
        }
    }

    if (purchased_product_category_index != -1) {
        printf("Recommendations based on %s purchase:\n", purchased_product);
        current = hash_table[purchased_product_category_index];
        while (current != NULL) {
            if (strcmp(current->name, purchased_product) != 0) {
                printf("- %s ($%d)\n", current->name, current->price);
            }
            current = current->next;
        }
    } else {
        printf("Unable to find the category for the purchased product %s.\n", purchased_product);
    }
}

void get_browsing_history(UserNode* user) {
    Browse* current = user->browsing;
    printf("%s's browsing history:\n", user->name);
    while (current != NULL) {
        printf("- Browsed %s on %s\n", current->category, current->date);
        current = current->next;
    }
}

void get_purchase_history(UserNode* user) {
    Purchase* current = user->purchases;
    printf("%s's purchase history:\n", user->name);
    while (current != NULL) {
        printf("- Bought %s for $%d on %s\n", current->product, current->amount, current->date);
        current = current->next;
    }
}

int main() {
    int choice;
    char category[50], name[50];
    int price;
    char user_name[50], product[50], date[11];
    int amount;
    UserNode* user_node;

    // Add some initial products
    add_product("electronics", "Laptop", 1000);
    add_product("electronics", "Smartphone", 800);
    add_product("electronics", "Headphones", 150);
    add_product("home", "Vacuum", 300);
    add_product("kitchen", "Blender", 120);

    while (1) {
        printf("\nUser and Product Management System\n");
        printf("1. Add Product\n");
        printf("2. Add User\n");
        printf("3. Add Purchase\n");
        printf("4. Recommend Products\n");
        printf("5. View Browsing History\n");
        printf("6. View Purchase History\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter product category: ");
                scanf("%s", category);
                printf("Enter product name: ");
                scanf("%s", name);
                printf("Enter product price: ");
                scanf("%d", &price);
                add_product(category, name, price);
                printf("Product added.\n");
                break;

            case 2:
                printf("Enter user name: ");
                scanf("%s", user_name);
                add_user(user_name);
                printf("User added.\n");
                break;

            case 3:
                printf("Enter user name: ");
                scanf("%s", user_name);
                user_node = find_user(user_name);
                if (user_node == NULL) {
                    printf("User not found.\n");
                    break;
                }
                printf("Enter product name: ");
                scanf("%s", product);
                printf("Enter purchase amount: ");
                scanf("%d", &amount);
                printf("Enter purchase date (YYYY-MM-DD): ");
                scanf("%s", date);
                add_purchase(user_node, product, amount, date);
                printf("Purchase added.\n");
                break;

            case 4:
                printf("Enter purchased product: ");
                scanf("%s", product);
                recommend_products(product);
                break;

            case 5:
                printf("Enter user name: ");
                scanf("%s", user_name);
                user_node = find_user(user_name);
                if (user_node == NULL) {
                    printf("User not found.\n");
                    break;
                }
                get_browsing_history(user_node);
                break;

            case 6:
                printf("Enter user name: ");
                scanf("%s", user_name);
                user_node = find_user(user_name);
                if (user_node == NULL) {
                    printf("User not found.\n");
                    break;
                }
                get_purchase_history(user_node);
                break;

            case 7:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}