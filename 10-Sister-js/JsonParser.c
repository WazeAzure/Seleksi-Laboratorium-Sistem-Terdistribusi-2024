#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./DataStructures/Dictionary/Dictionary.h"  // Include your custom Dictionary header

/**
 *  JSON TO DICTIONARY
 * 
 *  */

// Function prototypes
void parse_json_object(const char *json_str, struct Dictionary *dictionary);
void parse_json_value(const char *json_str, const char **end_ptr, struct Dictionary *dictionary, char *key);
char *parse_json_string(const char *json_str, const char **end_ptr);
void skip_whitespace(const char **json_str);

// Helper macros
#define JSON_PARSE_ERROR(msg) { printf("JSON Parse Error: %s\n", msg); }

// Function to parse a JSON object
void parse_json_object(const char *json_str, struct Dictionary *dictionary) {
    skip_whitespace(&json_str);
    if (*json_str != '{') {
        JSON_PARSE_ERROR("Expected '{' at the start of JSON object");
        return;
    }
    json_str++;  // Skip '{'

    while (*json_str != '\0') {
        skip_whitespace(&json_str);
        if (*json_str == '}') {
            json_str++;  // Skip '}'
            break;
        }

        // Parse key
        char *key = parse_json_string(json_str, &json_str);
        skip_whitespace(&json_str);

        if (*json_str != ':') {
            JSON_PARSE_ERROR("Expected ':' after key");
            return;
        }
        json_str++;  // Skip ':'
        skip_whitespace(&json_str);

        // Parse value
        parse_json_value(json_str, &json_str, dictionary, key);

        // Insert key-value pair into dictionary
        // dictionary->insert(dictionary, key, strlen(key) + 1, (void *)json_str, sizeof(json_str)); 

        skip_whitespace(&json_str);
        if (*json_str == ',') {
            json_str++;  // Skip ','
        } else if (*json_str == '}') {
            continue;  // End of object
        } else {
            JSON_PARSE_ERROR("Expected ',' or '}' after value");
            return;
        }

        free(key);  // Free allocated memory for key
    }
}

// Function to parse a JSON value
void parse_json_value(const char *json_str, const char **end_ptr, struct Dictionary *dictionary, char *key) {
    skip_whitespace(&json_str);
    
    if (*json_str == '"') {
        // Parse string value
        char *value = parse_json_string(json_str, &json_str);
        *end_ptr = json_str;
        
        // Insert string into dictionary
        dictionary->insert(dictionary, key, strlen(key) + 1, (void *)value, strlen(value) + 1);
        
        free(value);  // Free allocated memory for value
    } else if (isdigit(*json_str) || *json_str == '-') {
        // Parse number (as string for simplicity)
        char *end;
        double number = strtod(json_str, &end);
        *end_ptr = end;
        
        // Convert number to string and insert into dictionary
        char value[64];
        snprintf(value, sizeof(value), "%f", number);
        dictionary->insert(dictionary, key, strlen(key) + 1, (void *)value, strlen(value) + 1);
    } else if (strncmp(json_str, "true", 4) == 0) {
        // Parse boolean true
        *end_ptr = json_str + 4;
        dictionary->insert(dictionary, key, strlen(key) + 1, "true", 5);
    } else if (strncmp(json_str, "false", 5) == 0) {
        // Parse boolean false
        *end_ptr = json_str + 5;
        dictionary->insert(dictionary, key, strlen(key) + 1, "false", 6);
    } else if (strncmp(json_str, "null", 4) == 0) {
        // Parse null value
        *end_ptr = json_str + 4;
        dictionary->insert(dictionary, key, strlen(key) + 1, "null", 5);
    } else if (*json_str == '{') {
        // Parse nested object
        struct Dictionary nested_dict = dictionary_constructor(dictionary->binary_search_tree.compare);
        parse_json_object(json_str, &nested_dict);
        *end_ptr = json_str;
        
        // Insert nested dictionary (pointer) into the main dictionary
        dictionary->insert(dictionary, key, strlen(key) + 1, &nested_dict, sizeof(struct Dictionary));
    } else {
        JSON_PARSE_ERROR("Unknown value type");
        return;
    }
}


// Function to parse a JSON string
char *parse_json_string(const char *json_str, const char **end_ptr) {
    if (*json_str != '"') {
        JSON_PARSE_ERROR("Expected '\"' at the start of JSON string");
        return "";
    }
    json_str++;  // Skip the opening quote

    const char *start = json_str;
    while (*json_str != '"' && *json_str != '\0') {
        json_str++;
    }

    if (*json_str == '\0') {
        JSON_PARSE_ERROR("Unterminated JSON string");
        return "";
    }

    size_t length = json_str - start;
    char *result = (char *)malloc(length + 1);
    memset(result, 0, sizeof(length + 1));
    strncpy(result, start, length);
    result[length] = '\0';

    *end_ptr = json_str + 1;  // Skip the closing quote
    return result;
}

// Function to skip whitespace characters in the JSON string
void skip_whitespace(const char **json_str) {
    while (isspace(**json_str)) {
        (*json_str)++;
    }
}


/**
 *  DICTIONARY TO JSON 
 * 
 *  */

// Function to escape quotes in JSON values
void escape_json_string(char *dest, const char *src) {
    while (*src) {
        if (*src == '\"') {
            *dest++ = '\\';
        }
        *dest++ = *src++;
    }
    *dest = '\0';
}

// Helper function to recursively traverse the BST and append key-value pairs to the JSON string
void traverse_tree_to_json(struct Node *node, char *json) {
    if (node == NULL) return;

    // Get the current entry (key-value pair)
    struct Entry *entry = (struct Entry *)node->data;

    // Allocate space for the escaped key and value strings
    char escaped_key[256];
    char escaped_value[256];
    escape_json_string(escaped_key, (char *)entry->key);
    escape_json_string(escaped_value, (char *)entry->value);

    // Append the key-value pair to the JSON string
    char pair[512];
    sprintf(pair, "\"%s\":\"%s\"", escaped_key, escaped_value);

    // Check if we need a comma before appending this pair
    if (strlen(json) > 1) {
        strcat(json, ",");
    }
    strcat(json, pair);

    // Recur for left and right children
    traverse_tree_to_json(node->previous, json);
    traverse_tree_to_json(node->next, json);
}

// Function to convert the Dictionary to JSON
char *dictionary_to_json(struct Dictionary *dictionary) {
    // Allocate initial memory for the JSON string
    char *json = malloc(4096 * sizeof(char)); // Adjust size as needed
    memset(json, 0, 4096 * sizeof(char));
    strcpy(json, "{");

    // Traverse the binary search tree and collect the key-value pairs
    traverse_tree_to_json(dictionary->binary_search_tree.head, json);

    // Close the JSON string
    strcat(json, "}");

    return json;
}