#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
    char name[50];
    int age;
} Person;

void print_person(const void* data) {
    const Person* p = (const Person*)data;
    printf("%s(%d)", p->name, p->age);
}

void destroy_person(void* data) {
    free(data);
}

int compare_person_by_name(const void* a, const void* b) {
    const Person* pa = (const Person*)a;
    const Person* pb = (const Person*)b;
    return strcmp(pa->name, pb->name);
}

int is_adult(const void* data) {
    const Person* p = (const Person*)data;
    return p->age >= 18;
}

void make_birthday(void* data) {
    Person* p = (Person*)data;
    p->age++;
    printf("[Happy birthday %s!] ", p->name);
}

Person* create_person(const char* name, int age) {
    Person* p = malloc(sizeof(Person));
    if (p) {
        strcpy(p->name, name);
        p->age = age;
    }
    return p;
}


typedef struct {
    int id;
    char name[50];
    float price;
} Product;

void print_product(const void* data) {
    const Product* p = (const Product*)data;
    printf("%s:$%.2f", p->name, p->price);
}

void destroy_product(void* data) {
    free(data);
}

int compare_product_by_id(const void* a, const void* b) {
    const Product* pa = (const Product*)a;
    const Product* pb = (const Product*)b;
    return pa->id - pb->id;
}

int is_expensive(const void* data) {
    const Product* p = (const Product*)data;
    return p->price > 50.0f;
}

void apply_discount(void* data) {
    Product* p = (Product*)data;
    p->price *= 0.9f; // 10% discount
    printf("[Applied discount to %s] ", p->name);
}

Product* create_product(int id, const char* name, float price) {
    Product* p = malloc(sizeof(Product));
    if (p) {
        p->id = id;
        strcpy(p->name, name);
        p->price = price;
    }
    return p;
}

void demonstrate_person_operations() {
    printf("=== PERSON LIST DEMONSTRATION ===\n");

    // Create person list
    List* person_list = list_create();

    // Add persons
    Person* alice = create_person("Alice", 25);
    Person* bob = create_person("Bob", 17);
    Person* charlie = create_person("Charlie", 30);

    list_append(person_list, alice, sizeof(Person), print_person, destroy_person);
    list_append(person_list, bob, sizeof(Person), print_person, destroy_person);
    list_append(person_list, charlie, sizeof(Person), print_person, destroy_person);

    printf("Initial person list: ");
    list_print(person_list);

    // Search operations
    printf("\n--- Search Operations ---\n");

    Person search_alice = { "A", 0 };
    Person* found_person = (Person*)list_find(person_list, &search_alice, compare_person_by_name);
    if (found_person) {
        printf("Found person: ");
        print_person(found_person);
        printf("\n");
    } else printf("Not found ");

    // Filter operations
    size_t adults_count = 0;
    Person** adults = (Person**)list_filter(person_list, is_adult, &adults_count);
    printf("Adults (%zu): ", adults_count);
    for (size_t i = 0; i < adults_count; i++) {
        print_person(adults[i]);
        if (i < adults_count - 1) printf(", ");
    }
    printf("\n");
    free(adults);

    // Modification operations
    printf("\n--- Modification Operations ---\n");

    // Update Bobs age
    Person new_bob_data = { "Bob", 18 };
    list_update(person_list, bob, &new_bob_data, sizeof(Person));
    printf("After updating Bob: ");
    list_print(person_list);

    // Apply birthday to all
    printf("Applying birthdays: ");
    list_apply_to_all(person_list, NULL, make_birthday);
    printf("\nAfter birthdays: ");
    list_print(person_list);

    list_destroy(person_list);
}

void demonstrate_product_operations() {
    printf("\n=== PRODUCT LIST DEMONSTRATION ===\n");

    // Create product list
    List* product_list = list_create();

    // Add products
    Product* laptop = create_product(1, "Laptop", 999.99);
    Product* mouse = create_product(2, "Mouse", 29.99);
    Product* keyboard = create_product(3, "Keyboard", 79.99);

    list_append(product_list, laptop, sizeof(Product), print_product, destroy_product);
    list_append(product_list, mouse, sizeof(Product), print_product, destroy_product);
    list_append(product_list, keyboard, sizeof(Product), print_product, destroy_product);

    printf("Initial product list: ");
    list_print(product_list);

    // Search operations
    printf("\n--- Search Operations ---\n");

    Product search_mouse = { 2, "", 0 };
    Product* found_product = (Product*)list_find(product_list, &search_mouse, compare_product_by_id);
    if (found_product) {
        printf("Found product: ");
        print_product(found_product);
        printf("\n");
    }

    // Filter operations
    size_t expensive_count = 0;
    Product** expensive = (Product**)list_filter(product_list, is_expensive, &expensive_count);
    printf("Expensive products (%zu): ", expensive_count);
    for (size_t i = 0; i < expensive_count; i++) {
        print_product(expensive[i]);
        if (i < expensive_count - 1) printf(", ");
    }
    printf("\n");
    free(expensive);

    // Modification operations
    printf("\n--- Modification Operations ---\n");

    // Apply discount to expensive products
    printf("Applying discounts: ");
    list_apply_to_all(product_list, is_expensive, apply_discount);
    printf("\nAfter discounts: ");
    list_print(product_list);

    // Find all after modifications
    size_t final_expensive_count = 0;
    Product** final_expensive = (Product**)list_filter(product_list, is_expensive, &final_expensive_count);
    printf("Still expensive after discount (%zu): ", final_expensive_count);
    for (size_t i = 0; i < final_expensive_count; i++) {
        print_product(final_expensive[i]);
        if (i < final_expensive_count - 1) printf(", ");
    }
    printf("\n");
    free(final_expensive);

    list_destroy(product_list);
}

int main() {

    demonstrate_person_operations();
    demonstrate_product_operations();

    printf("\n TESTED \n");
    return 0;
}