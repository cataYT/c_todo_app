#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* read_todo();
void add_todo(const char* todo_item);
void delete_todo(const char* todo_item);
void clear_todo();

void to_lowercase(char* str);