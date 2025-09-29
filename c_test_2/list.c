#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Main list operations

List* list_create() {
	List* list = malloc(sizeof(List));
	if (!list) return NULL;

	list->head = list->tail = NULL;
	list->size = 0;
	return list;
}

void list_destroy(List* list) {
	if (!list) return;

	Container* current = list->head;
	while (current) {
		Container* next = current->next;
		if (current->destroy) {
			current->destroy(current->data);
		} else {
			free(current->data);
		}
		free(current);

		current = next;
	}

	free(list);
}

int list_append(List* list, const void* data, size_t data_size,
	void (*print)(const void*), void (*destroy)(void*)) {
	if (!list || !data) return 0;

	Container* new_container = malloc(sizeof(Container));
	if (!new_container) return 0;

	new_container->data = malloc(data_size);
	if (!new_container->data) {
		free(new_container);
		return 0;
	}

	memcpy(new_container->data, data, data_size);
	new_container->data_size = data_size;
	new_container->print = print;
	new_container->destroy = destroy;
	new_container->next = NULL;

	if (!list->head) {
		list->head = list->tail = new_container;
	} else {
		list->tail->next = new_container;
		list->tail = new_container;
	}

	list->size++;
	return 1;
}

size_t list_size(const List* list) {
	return list ? list->size : 0;
}

int list_empty(const List* list) {
	return !list || list->size == 0;
}

void list_print(const List* list) {
	if (!list) return;

	printf("[");
	Container* current = list->head;
	int first = 1;

	while (current) {
		if (!first) printf(", ");
		if (current->print) {
			current->print(current->data);
		} else {
			printf("<no print function>");
		}
		first = 0;
		current = current->next;
	}
	printf("]\n");
}

// Search and filtering operations

void* list_find(const List* list, const void* data,
	int (*compare)(const void*, const void*)) {
	if (!list || !compare) return NULL;

	Container* current = list->head;
	while (current) {
		if (compare(current->data, data) == 0) {
			return current->data;
		}
		current = current->next;
	}
	return NULL;
}

void** list_find_all(const List* list, const void* data,
	int (*compare)(const void*, const void*), size_t* count) {
	if (!list || !compare || !count) return NULL;

	size_t match_count = 0;
	Container* current = list->head;
	while (current) {
		if (compare(current->data, data) == 0) {
			match_count++;
		}
		current = current->next;
	}

	if (match_count == 0) {
		*count = 0;
		return NULL;
	}

	void** result = malloc(match_count * sizeof(void*));
	if (!result) {
		*count = 0;
		return NULL;
	}

	size_t index = 0;
	current = list->head;
	while (current) {
		if (compare(current->data, data) == 0) {
			result[index++] = current->data;
		}
		current = current->next;
	}

	*count = match_count;
	return result;
}

void** list_filter(const List* list, int (*condition)(const void*), size_t* count) {
	if (!list || !condition || !count) return NULL;

	size_t match_count = 0;
	Container* current = list->head;
	while (current) {
		if (condition(current->data)) {
			match_count++;
		}
		current = current->next;
	}

	if (match_count == 0) {
		*count = 0;
		return NULL;
	}

	void** result = malloc(match_count * sizeof(void*));
	if (!result) {
		*count = 0;
		return NULL;
	}

	size_t index = 0;
	current = list->head;
	while (current) {
		if (condition(current->data)) {
			result[index++] = current->data;
		}
		current = current->next;
	}

	*count = match_count;
	return result;
}

// Data modify

int list_update(List* list, void* old_data, const void* new_data, size_t new_data_size) {
	if (!list || !old_data || !new_data) return 0;

	Container* current = list->head;
	while (current) {
		if (current->data == old_data) {
			

			if (current->destroy) {
				current->destroy(current->data);
			} else {
				free(current->data);
			}

			current->data = malloc(new_data_size);
			if (!current->data) return 0;

			memcpy(current->data, new_data, new_data_size);
			current->data_size = new_data_size;
			return 1;
		}
		current = current->next;
	}

	return 0;
}

int list_modify(void* data, void (*modifier)(void*)) {
	if (!data || !modifier) return 0;
	modifier(data);
	return 1;
}

int list_apply_to_all(List* list, int (*condition)(const void*), void (*action)(void*)) {
	if (!list || !action) return 0;

	int applied_count = 0;
	Container* current = list->head;

	while (current) {

		if (!condition || condition(current->data)) {
			action(current->data);
			applied_count++;
		}
		current = current->next;
	}

	return applied_count;
}