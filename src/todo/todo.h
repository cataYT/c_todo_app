#pragma once

#include <stdlib.h>

#define FILE_PATH "../../build/bin/todo_items.txt"

// Reads entire todo file, returns dynamically allocated string (must free)
char* read_todo();
void add_todo(const char* todo_item);
void delete_todo(const char* todo_item);
void clear_todo();

// Helper: lowercase a string in place
void to_lowercase(char* str);
// Helper: get input safely
char* get_input(const char* msg, const size_t length);