#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo.h"

void to_lowercase(char* str)
{
    for (; *str; ++str)
    {
        *str = tolower((unsigned char)*str);
    }
}

char* get_input(const char* msg, const size_t length)
{
    if (!msg)
    {
        fprintf(stderr, "msg is null, cannot get input");
        return NULL;
    }

    char* input = malloc(length + 1);
    if (!input)
    {
        fprintf(stderr, "malloc failed, cannot get input");
        return NULL;
    }

    printf("%s", msg);
    fflush(stdout);

    if (!fgets(input, length + 1, stdin))
    {
        printf("Error reading input\n");
        free(input);
        return NULL;
    }

    // Remove trailing newline if present
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n')
    {
        input[len-1] = '\0';
    }
    else
    {
        // Clear remaining characters in stdin if buffer was too small
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    return input;
}

char* read_todo()
{
    // Change from "rb" to "r" to handle line endings consistently
    FILE* file = fopen(FILE_PATH, "r"); // dynamic
    if (!file)
    {
        fprintf(stderr, "fopen failed.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = malloc(length + 1); // dynamic
    if (!buffer)
    {
        fprintf(stderr, "malloc failed.\n");
        fclose(file); // closed file - failure
        return NULL;
    }

    size_t read_len = fread(buffer, 1, length, file);
    buffer[read_len] = '\0';

    fclose(file); // closed file
    return buffer;
}

// Returns 0 if todo_item found (case-insensitive exact match), 1 otherwise
int find_todo(const char* todo_item)
{
    if (!todo_item || strlen(todo_item) == 0)
    {
        fprintf(stderr, "todo item is null, cannot find");
        return 1;
    }

    char* content = read_todo(); // dynamic

    char* content_lower = strdup(content); // dynamic
    if (!content_lower)
    {
        fprintf(stderr, "strdup failed.\n");
        free(content); // free content - failure
        return 1;
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
            free(content_lower); // free content_lower - found
            free(content); // free content - found
            return 0; // found
        }
        line = strtok(NULL, "\n");
    }

    free(content_lower); // free content_lower - not found
    free(content); // free content - found
    return 1; // not found
}

void add_todo(const char* todo_item)
{
    if (!find_todo(todo_item))
    {
        printf("Todo item already in the file!\n");
        return;
    }

    FILE* file = fopen(FILE_PATH, "a"); // dynamic
    if (!file)
    {
        fprintf(stderr, "failed to open file.\n");
        return;
    }

    fprintf(file, "%s\n", todo_item);
    fclose(file); // closed file

    printf("Added todo item.\n");
}

void delete_todo(const char* todo_item)
{
    if (find_todo(todo_item) != 0)
    {
        printf("Todo item does not exist.\n");
        return;
    }

    FILE* file = fopen(FILE_PATH, "r"); // dynamic
    if (!file)
    {
        fprintf(stderr, "failed to open file for reading.\n");
        return;
    }

    FILE* temp = fopen("../../build/temp.txt", "w"); // dynamic
    if (!temp)
    {
        fprintf(stderr, "failed to open temp file.\n");
        fclose(file); // closed file - failure
        return;
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

    fclose(file); // closed file
    fclose(temp); // closed temp

    if (!deleted)
    {
        remove("temp.txt");
        fprintf(stderr, "Failed to delete todo (unexpected): %s\n", todo_item);
        exit(1);
    }

    remove(FILE_PATH);
    rename("temp.txt", FILE_PATH);
    printf("Deleted todo item.\n");
}

void clear_todo()
{
    FILE* file = fopen(FILE_PATH, "w"); // dynamic
    if (!file)
    {
        fprintf(stderr, "failed to open file.\n");
        return;
    }
    fclose(file); // closed file
    printf("Cleared todo items.\n");
}