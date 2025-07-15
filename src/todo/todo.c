#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "todo.h"

#define FILE_PATH "todo_items.txt"

char *todo_read()
{
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
    // File doesn't exist, create it
        file = fopen(FILE_PATH, "w+");  // create new file for read/write
        if (!file) {
            fprintf(stderr, "Failed to create file.\n");
            return NULL;
        }
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    size_t read_len = fread(buffer, 1, length, file);
    buffer[read_len] = '\0';
    fclose(file);

    return buffer;
}

// Helper: convert string to lowercase in-place
bool to_lowercase(char *str)
{
    if (!str) {
        fprintf(stderr, "string is null at to_lowercase()\n");
        return false;
    }
    for (; *str; ++str) {
        *str = (char)tolower((unsigned char)*str);
    }
    return true;
}

/**
 * @brief Helper function to count the number of lines in todo.
 * 
 * @param[in] file_name File name to count the number of lines for. 
 * @return Number of lines if successful, 0 otherwise.
 */
static size_t count_lines(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (!file) {
    // File doesn't exist, create it
        file = fopen(FILE_PATH, "w+");  // create new file for read/write
        if (!file) {
            fprintf(stderr, "Failed to create file.\n");
            return 0;
        }
    }

    size_t lines = 0;
    int ch = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') lines++;
    }
    fclose(file);
    return lines;
}

char **todo_load()
{
    size_t num_lines = count_lines(FILE_PATH);
    if (num_lines == 0) {
        // Empty file, return empty list (null-terminated)
        char **out_items = malloc(sizeof(*out_items));
        if (!out_items) return NULL;
        out_items[0] = NULL;
        return out_items;
    }

    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
    // File doesn't exist, create it
        file = fopen(FILE_PATH, "w+");  // create new file for read/write
        if (!file) {
            fprintf(stderr, "Failed to create file.\n");
            return NULL;
        }
    }

    char **items = malloc((num_lines + 1) * sizeof(char *));
    if (!items) {
        fclose(file);
        return NULL;
    }

    char buffer[1024];
    if (!buffer) {
        fclose(file);
        free(items);
        return NULL;
    }
    size_t i = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n\r")] = '\0'; // remove newline

        items[i] = strdup(buffer);
        if (!items[i]) {
            for (size_t j = 0; j < i; j++) 
                free(items[j]);
            free(items);
            fclose(file);
            return NULL;
        }
        i++;
    }
    items[i] = NULL;

    fclose(file);
    return items;
}

void items_free(char **items)
{
    if (!items) return;
    for (size_t i = 0; items[i] != NULL; i++) {
        free(items[i]);
    }
    free(items);
}

/**
 * @brief Checks if a todo item is already in the todo or not.
 * 
 * @param[in] todo_item Todo item to check for.
 * @return true if item exists, false otherwise.
 */
bool todo_exists(const char *todo_item)
{
    if (!todo_item || strlen(todo_item) == 0) {
        fprintf(stderr, "todo item is null or empty\n");
        return false;
    }

    char todo_lower[1024];
    snprintf(todo_lower, sizeof(todo_lower), "%s", todo_item);
    to_lowercase(todo_lower);

    char **items = todo_load();
    if (!items) return false;

    bool found = false;
    for (size_t i = 0; items[i] != NULL; i++) {
        char line_lower[1024];
        snprintf(line_lower, sizeof(line_lower), "%s", items[i]);
        to_lowercase(line_lower);
        if (strcmp(line_lower, todo_lower) == 0) {
            found = true;
            break;
        }
    }

    items_free(items);
    return found;
}

bool todo_add(const char *todo_item)
{
    if (!todo_item || strlen(todo_item) == 0) {
        fprintf(stderr, "Invalid todo item to add\n");
        return false;
    }

    if (todo_exists(todo_item)) {
        printf("Todo item already in the file!\n");
        return false;
    }

    FILE *file = fopen(FILE_PATH, "a+");
    if (!file) {
        fprintf(stderr, "Failed to open file for appending\n");
        return false;
    }

    fprintf(file, "%s\n", todo_item);
    fclose(file);

    printf("Added todo item.\n");
    return true;
}

char **todo_delete(const char *todo_item)
{
    if (!todo_item) return NULL;

    if (!todo_exists(todo_item)) {
        printf("Todo item does not exist.\n");
        return NULL;
    }

    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
    // File doesn't exist, create it
        file = fopen(FILE_PATH, "w+");  // create new file for read/write
        if (!file) {
            fprintf(stderr, "Failed to create file.\n");
            return NULL;
        }
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        fprintf(stderr, "Failed to open temp file for writing\n");
        fclose(file);
        return NULL;
    }

    char todo_lower[1024];
    snprintf(todo_lower, sizeof(todo_lower), "%s", todo_item);
    to_lowercase(todo_lower);

    char line[1024];
    bool deleted = false;

    // We'll store lines except the one deleted in a dynamic array
    size_t capacity = 16;
    size_t count = 0;
    char **new_items = malloc(capacity * sizeof(*new_items));
    if (!new_items) {
        fclose(file);
        fclose(temp);
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n\r")] = '\0';

        char line_lower[1024];
        snprintf(line_lower, sizeof(line_lower), "%s", line);
        to_lowercase(line_lower);

        if (strcmp(line_lower, todo_lower) == 0) {
            // skip this line (delete)
            deleted = true;
            continue;
        }

        fprintf(temp, "%s\n", line);

        // Store in new_items
        if (count >= capacity - 1) {
            capacity *= 2;
            char **tmp = realloc(new_items, capacity * sizeof(char *));
            if (!tmp) {
                for (size_t i = 0; i < count; i++)
                    free(new_items[i]);
                free(new_items);
                fclose(file);
                fclose(temp);
                return NULL;
            }
            new_items = tmp;
        }
        new_items[count] = strdup(line);
        if (!new_items[count]) {
            for (size_t i = 0; i < count; i++)
                free(new_items[i]);
            free(new_items);
            fclose(file);
            fclose(temp);
            return NULL;
        }
        count++;
    }
    new_items[count] = NULL;

    fclose(file);
    fclose(temp);

    if (!deleted) {
        remove("temp.txt");
        fprintf(stderr, "Failed to delete todo (unexpected)\n");
        for (size_t i = 0; i < count; i++) {
            free(new_items[i]);
        }
        free(new_items);
        return NULL;
    }

    if (remove(FILE_PATH) != 0 || rename("temp.txt", FILE_PATH) != 0) {
        fprintf(stderr, "Failed to replace todo file\n");
        for (size_t i = 0; i < count; i++) {
            free(new_items[i]);
        }
        free(new_items);
        return NULL;
    }

    printf("Deleted todo item.\n");
    return new_items;
}

bool todo_clear()
{
    FILE *file = fopen(FILE_PATH, "w");
    if (!file) {
        fprintf(stderr, "Failed to open file to clear\n");
        return false;
    }
    fclose(file);
    printf("Cleared todo items.\n");
    return true;
}

char *get_string_input(const char *msg)
{
    if (!msg) return NULL;

    printf("%s", msg);
    fflush(stdout);

    size_t buffer_size = 256;
    char *buffer = malloc(buffer_size + 1);

    if (!fgets(buffer, buffer_size + 1, stdin)) {
        if (feof(stdin)) {
            fprintf(stderr, "EOF reached, no input\n");
        } else {
            fprintf(stderr, "failed to read input\n");
        }
        free(buffer);
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strlen(buffer) == buffer_size && buffer[buffer_size - 1] != '\0') {
        int ch = 0;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    return buffer;
}