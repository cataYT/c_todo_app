#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper: lowercase a string in place
void to_lowercase(char* str)
{
    for (; *str; ++str)
    {
        *str = tolower((unsigned char)*str);
    }
}

// Reads entire todo file, returns dynamically allocated string (must free)
char* read_todo()
{
    // Change from "rb" to "r" to handle line endings consistently
    FILE* file = fopen("todo_items.txt", "r");
    if (!file)
    {
        perror("fopen failed.\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = malloc(length + 1);
    if (!buffer)
    {
        perror("malloc failed.\n");
        fclose(file);
        exit(1);
    }

    size_t read_len = fread(buffer, 1, length, file);
    buffer[read_len] = '\0';

    fclose(file);
    return buffer;
}

// Returns 0 if todo_item found (case-insensitive exact match), 1 otherwise
int find_todo(const char* todo_item)
{
    char* content = read_todo();

    char* content_lower = strdup(content);
    if (!content_lower)
    {
        perror("strdup failed.\n");
        free(content);
        exit(1);
    }
    to_lowercase(content_lower);

    char todo_lower[1024];
    snprintf(todo_lower, sizeof(todo_lower), "%s", todo_item);
    to_lowercase(todo_lower);

    // tokenize by lines and compare
    char* line = strtok(content_lower, "\n");
    while (line)
    {
        if (strcmp(line, todo_lower) == 0)
        {
            free(content_lower);
            free(content);
            return 0; // found
        }
        line = strtok(NULL, "\n");
    }

    free(content_lower);
    free(content);
    return 1; // not found
}

void add_todo(const char* todo_item)
{
    if (!find_todo(todo_item))
    {
        printf("Todo item already in the file!\n");
        return;
    }

    FILE* file = fopen("todo_items.txt", "a");
    if (!file)
    {
        perror("failed to open file.\n");
        exit(1);
    }

    fprintf(file, "%s\n", todo_item);
    fclose(file);

    printf("Added todo item.\n");
}

void delete_todo(const char* todo_item)
{
    if (find_todo(todo_item) != 0)
    {
        printf("Todo item does not exist.\n");
        return;
    }

    FILE* file = fopen("todo_items.txt", "r");
    if (!file)
    {
        perror("failed to open file for reading.\n");
        exit(1);
    }

    FILE* temp = fopen("temp.txt", "w");
    if (!temp)
    {
        perror("failed to open temp file.\n");
        fclose(file);
        exit(1);
    }

    char line[1024];
    int deleted = 0;
    char todo_lower[1024];

    snprintf(todo_lower, sizeof(todo_lower), "%s", todo_item);  // Always null-terminates
    to_lowercase(todo_lower);

    while (fgets(line, sizeof(line), file))
    {
        size_t new_line_index = strcspn(line, "\n\r");
        line[new_line_index] = '\0';

        char line_lower[1024];
        snprintf(line_lower, sizeof(line_lower), "%s", line);
        to_lowercase(line_lower);

        if (strcmp(line_lower, todo_lower) == 0)
        {
            deleted = 1;
            continue;
        }

        fprintf(temp, "%s\n", line);
    }

    fclose(file);
    fclose(temp);

    if (!deleted)
    {
        remove("temp.txt");
        printf("Failed to delete todo (unexpected): %s\n", todo_item);
        exit(1);
    }

    remove("todo_items.txt");
    rename("temp.txt", "todo_items.txt");
    printf("Deleted todo item.\n");
}

void clear_todo()
{
    FILE* file = fopen("todo_items.txt", "w");
    if (!file)
    {
        perror("failed to open file.\n");
        exit(1);
    }
    fclose(file);
    printf("Cleared todo items.\n");
}