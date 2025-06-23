#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Reads the todo from todo_items.txt and dynamically allocate it.
 * 
 * @param[out] out_content Pointer to string for holding the todo content.
 * @return true on successful, false otherwise.
 */
bool todo_read(char **out_content);

/**
 * @brief Loads todo items for todo_read(). Use items_free() to deallocate it.
 * 
 * @param[out] out_items Pointer to caller allocated array of strings for holding the todo items.
 * @return true on successful, false otherwise.
 */
bool todo_load(char ***out_items);

/**
 * @brief Used for freeing items by todo_load() and todo_delete().
 * 
 * @param[in] items Pointer to string for freeing the items. 
 */
void items_free(char **items);

/**
 * @brief Adds an item into the todo.
 * 
 * @param[in] todo_item Item to add into the todo.
 * @return true if successful, false otherwise.
 */
bool todo_add(const char *todo_item);

/**
 * @brief Deletes an item from todo and dynamically allocate it. Use items_free() to deallocate it.
 * 
 * @param[in] todo_item Item to delete from todo. 
 * @param[out] out_items Pointer to array of strings for holding the deleted item.
 * @return true if successful, false otherwise.
 */
bool todo_delete(const char *todo_item, char ***out_items);

/**
 * @brief Clears the whole todo.
 * 
 * @return true if sucessful, false otherwise.
 */
bool todo_clear(void);

/**
 * @brief Converts a string to lowercase in place.
 * 
 * @param[in,out] str String to convert to lowercase.
 * @return true if successful, false otherwise. 
 */
bool to_lowercase(char *str);

/**
 * @brief Used for getting string user input.
 * 
 * @param[in] msg Message to present to user.
 * @param[out] out Caller allocated string for holding what the user inputted.
 * @param[in] max_length Max length for the user to input the string.
 * @return true if successful, false otherwise. 
 */
bool get_string_input(const char *msg, char *out, const size_t max_length);