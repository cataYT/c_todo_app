#include <stdlib.h>
#include "todo/todo.h"

// dynamic

int main()
{
    while (1)
    {
        char* input = get_input("Enter your choice (add/read/delete/clear/exit): ", 6); // dynamic
        if (!input) continue;  // Skip on error

        to_lowercase(input);

        if (strcmp(input, "add") == 0 || strcmp(input, "a") == 0)
        {
            char* todo_item = get_input("Enter your todo item: ", 50);
            if (todo_item)
            {
                add_todo(todo_item);
                free(todo_item);
            }
        }
        else if (strcmp(input, "read") == 0 || strcmp(input, "r") == 0)
        {
            char* items = read_todo();
            printf("Items are: %s\n", items);
            free(items);
        }
        else if (strcmp(input, "delete") == 0 || strcmp(input, "d") == 0)
        {
            char* delete_entry = get_input("Enter the todo item to delete: ", 50);
            if (delete_entry)
            {
                delete_todo(delete_entry);
                free(delete_entry);
            }
        }
        else if (strcmp(input, "clear") == 0 || strcmp(input, "c") == 0)
        {
            clear_todo();
        }
        else if (strcmp(input, "exit") == 0 || strcmp(input, "e") == 0)
        {
            free(input);
            printf("Exiting...\n");
            break;
        }
        else
        {
            printf("Invalid option.\n");
        }
        free(input);  // Free here for all cases
    }
    return 0;
}