#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Reads the todo from todo_items.txt.
 * 
 * @return All todo items if successful, NULL otherwise.
 */
char *todo_read();

/**
 * @brief Loads todo items for todo_read(). Use items_free() to deallocate it.
 * 
 * @return Loaded items on successful, NULL otherwise.
 */
char **todo_load();

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
 * 
 * @return true if successful, false otherwise.
 */
bool todo_add(const char *todo_item);

/**
 * @brief Deletes an item from todo and dynamically allocate it. Use items_free() to deallocate it.
 * 
 * @param[in] todo_item Item to delete from todo.
 * 
 * @return Deleted todo item(s) if successful, NULL otherwise otherwise.
 */
char **todo_delete(const char *todo_item);

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
 * 
 * @return true if successful, false otherwise. 
 */
bool to_lowercase(char *str);

/**
 * @brief Used for getting string user input.
 * 
 * @param[in] msg Message to present to user.

 * @return User's entered if successful, NULL otherwise.
 */
char *get_string_input(const char *msg);