#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo/todo.h"

int main(void)
{
    while (1) {
        char *input = get_string_input("Enter your choice (add/read/delete/clear/exit): ");
        if (!input) {
            continue;  // on input error retry
        }

        to_lowercase(input);

        if (strcmp(input, "add") == 0 || strcmp(input, "a") == 0) {
            char *todo_item = get_string_input("Enter your todo item: ");
            if (todo_item) {
                if (!todo_add(todo_item)) {
                    printf("Failed to add todo or it already exists.\n");
                }
            }
            free(todo_item);
        } else if (strcmp(input, "read") == 0 || strcmp(input, "r") == 0) {
            char *content = todo_read();
            if (content) {
                printf("Todo items:\n%s\n", content);
                free(content);
            } else {
                printf("Failed to read todo items.\n");
            }
        } else if (strcmp(input, "delete") == 0 || strcmp(input, "d") == 0) {
            char *todo_item = get_string_input("Enter the todo item to delete: ");
            if (todo_item) {
                char **updated_items = todo_delete(todo_item);
                if (updated_items) {
                    printf("Deleted todo item. Updated list:\n");
                    for (int i = 0; updated_items[i] != NULL; i++) {
                        printf("  %s\n", updated_items[i]);
                    }
                    items_free(updated_items);
                } else {
                    printf("Todo item not found or failed to delete.\n");
                }
            }
            free(todo_item);
        } else if (strcmp(input, "clear") == 0 || strcmp(input, "c") == 0) {
            if (todo_clear()) {
                printf("Cleared all todo items.\n");
            } else {
                printf("Failed to clear todo items.\n");
            }
        } else if (strcmp(input, "exit") == 0 || strcmp(input, "e") == 0) {
            printf("Exiting...\n");
            free(input);
            break;
        } else {
            printf("Invalid option.\n");
        }
        free(input);
    }

    return 0;
}