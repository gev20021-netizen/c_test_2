#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stddef.h>

// Universal container for list data
typedef struct Container {
    void* data;              // Pointer to data
    size_t data_size;        // Data size in bytes 
    struct Container* next;  // Pointer to next container

    // Data-specific destroy and print functions
    void (*print)(const void*);
    void (*destroy)(void*);
} Container;

// List struct
typedef struct {
    Container* head; // List head pointer
    Container* tail; // List tail pointer
    size_t size; // Size
} List;

//Basic operations

List* list_create(); // Create list
void list_destroy(List* list); // Destroy list

int list_append(List* list, const void* data, size_t data_size,
    void (*print)(const void*), void (*destroy)(void*)); // Add element

// Get info

size_t list_size(const List* list); // Get size
int list_empty(const List* list); // Is empty

void list_print(const List* list); // Print

// Seatch and filter

void* list_find(const List* list, const void* data,
    int (*compare)(const void*, const void*)); // Find first of

void** list_find_all(const List* list, const void* data,
    int (*compare)(const void*, const void*), size_t* count); // Find all

void** list_filter(const List* list, int (*condition)(const void*), size_t* count); // Find if

// Data modify

int list_update(List* list, void* old_data, const void* new_data, size_t new_data_size); // Replace container

int list_modify(void* data, void (*modifier)(void*)); // Modify container field

int list_apply_to_all(List* list, int (*condition)(const void*), void (*action)(void*)); // Apply to all

#endif