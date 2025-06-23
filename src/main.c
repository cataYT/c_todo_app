#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo/todo.h"

#define MAX_INPUT_LEN 64
#define MAX_TODO_LEN 256

int main(void)
{
    char input[MAX_INPUT_LEN] = {'\0'};

    while (1) {
        if (!get_string_input("Enter your choice (add/read/delete/clear/exit): ", input, MAX_INPUT_LEN)) {
            continue;  // on input error retry
        }

        to_lowercase(input);

        if (strcmp(input, "add") == 0 || strcmp(input, "a") == 0) {
            char todo_item[MAX_TODO_LEN] = {'\0'};
            if (get_string_input("Enter your todo item: ", todo_item, MAX_TODO_LEN)) {
                if (!todo_add(todo_item)) {
                    printf("Failed to add todo or it already exists.\n");
                }
            }
        } else if (strcmp(input, "read") == 0 || strcmp(input, "r") == 0) {
            char *content = NULL;
            if (todo_read(&content)) {
                printf("Todo items:\n%s\n", content);
                free(content);
            } else {
                printf("Failed to read todo items.\n");
            }
        } else if (strcmp(input, "delete") == 0 || strcmp(input, "d") == 0) {
            char todo_item[MAX_TODO_LEN] = {'\0'};
            if (get_string_input("Enter the todo item to delete: ", todo_item, MAX_TODO_LEN)) {
                char **updated_items = NULL;
                if (todo_delete(todo_item, &updated_items)) {
                    printf("Deleted todo item. Updated list:\n");
                    if (updated_items) {
                        for (int i = 0; updated_items[i] != NULL; i++) {
                            printf("  %s\n", updated_items[i]);
                        }
                        items_free(updated_items);
                    }
                } else {
                    printf("Todo item not found or failed to delete.\n");
                }
            }
        } else if (strcmp(input, "clear") == 0 || strcmp(input, "c") == 0) {
            if (todo_clear()) {
                printf("Cleared all todo items.\n");
            } else {
                printf("Failed to clear todo items.\n");
            }
        } else if (strcmp(input, "exit") == 0 || strcmp(input, "e") == 0) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

    return 0;
}