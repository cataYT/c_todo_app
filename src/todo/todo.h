#pragma once

#include <stdlib.h>
#include <stdbool.h>

// Reads entire todo file content into a dynamically allocated string.
// Caller must free *out_content.
// Returns true on success, false on failure.
bool read_todo(char **out_content);

// Loads all todo items into a NULL-terminated array of strings.
// Caller must free array and strings via free_items().
// Returns true on success, false on failure.
bool load_todos(char ***out_items);

// Frees an array of strings returned by load_todos() or delete_todo().
void free_items(char **items);

// Adds a todo item to the file.
// Returns true on success, false on failure or if item already exists.
bool add_todo(const char *todo_item);

// Deletes a todo item from the file.
// Returns true on success, false if item not found or error.
// Updated list of todos returned via out_items (caller must free).
bool delete_todo(const char *todo_item, char ***out_items);

// Clears all todo items from the file.
// Returns true on success, false on failure.
bool clear_todo(void);

// Converts a string to lowercase in-place.
// Returns true on success, false if input is NULL.
bool to_lowercase(char *str);

// Safely gets string input from the user with a prompt.
// Stores result in out buffer (max_length).
// Returns true on success, false on EOF or error.
bool get_string_input(const char *msg, char *out, size_t max_length);