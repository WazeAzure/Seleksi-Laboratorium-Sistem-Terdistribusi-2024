#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// JSON value types
typedef enum {
    JSON_NULL, JSON_BOOLEAN, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT
} JsonValueType;

// JSON value structure
typedef struct JsonValue {
    JsonValueType type;
    union {
        int boolean;
        double number;
        char *string;
        struct JsonValue **array;
        struct JsonObject *object;
    };
    int array_size;
} JsonValue;

// JSON key-value pair structure for objects
typedef struct JsonObject {
    char *key;
    JsonValue *value;
    struct JsonObject *next;
} JsonObject;

// Forward declarations
JsonValue *parse_json_value(const char **json);
JsonObject *parse_json_object(const char **json);
JsonValue *parse_json_array(const char **json);

// Helper function to skip whitespace
void skip_whitespace(const char **json) {
    while (isspace(**json)) {
        (*json)++;
    }
}

// Function to parse JSON null
JsonValue *parse_json_null(const char **json) {
    if (strncmp(*json, "null", 4) == 0) {
        *json += 4;
        JsonValue *value = malloc(sizeof(JsonValue));
        value->type = JSON_NULL;
        return value;
    }
    return NULL;
}

// Function to parse JSON boolean
JsonValue *parse_json_boolean(const char **json) {
    JsonValue *value = malloc(sizeof(JsonValue));
    if (strncmp(*json, "true", 4) == 0) {
        *json += 4;
        value->type = JSON_BOOLEAN;
        value->boolean = 1;
        return value;
    } else if (strncmp(*json, "false", 5) == 0) {
        *json += 5;
        value->type = JSON_BOOLEAN;
        value->boolean = 0;
        return value;
    }
    free(value);
    return NULL;
}

// Function to parse JSON number
JsonValue *parse_json_number(const char **json) {
    char *end;
    double number = strtod(*json, &end);
    if (*json != end) {
        *json = end;
        JsonValue *value = malloc(sizeof(JsonValue));
        value->type = JSON_NUMBER;
        value->number = number;
        return value;
    }
    return NULL;
}

// Function to parse JSON string
JsonValue *parse_json_string(const char **json) {
    if (**json == '"') {
        (*json)++;
        const char *start = *json;
        while (**json != '"') {
            (*json)++;
        }
        size_t len = *json - start;
        char *str = malloc(len + 1);
        strncpy(str, start, len);
        str[len] = '\0';
        (*json)++;
        JsonValue *value = malloc(sizeof(JsonValue));
        value->type = JSON_STRING;
        value->string = str;
        return value;
    }
    return NULL;
}

// Function to parse JSON array
JsonValue *parse_json_array(const char **json) {
    if (**json == '[') {
        (*json)++;
        skip_whitespace(json);
        JsonValue *value = malloc(sizeof(JsonValue));
        value->type = JSON_ARRAY;
        value->array_size = 0;
        value->array = NULL;

        if (**json == ']') {
            (*json)++;
            return value;
        }

        while (1) {
            value->array = realloc(value->array, (value->array_size + 1) * sizeof(JsonValue *));
            value->array[value->array_size] = parse_json_value(json);
            value->array_size++;
            skip_whitespace(json);

            if (**json == ',') {
                (*json)++;
                skip_whitespace(json);
            } else if (**json == ']') {
                (*json)++;
                break;
            }
        }

        return value;
    }
    return NULL;
}

// Function to parse JSON object
JsonObject *parse_json_object(const char **json) {
    if (**json == '{') {
        (*json)++;
        skip_whitespace(json);
        JsonObject *head = NULL;
        JsonObject *current = NULL;

        if (**json == '}') {
            (*json)++;
            return head;
        }

        while (1) {
            JsonObject *obj = malloc(sizeof(JsonObject));
            obj->key = parse_json_string(json)->string;
            skip_whitespace(json);
            if (**json == ':') {
                (*json)++;
                skip_whitespace(json);
                obj->value = parse_json_value(json);
                obj->next = NULL;

                if (!head) {
                    head = obj;
                    current = obj;
                } else {
                    current->next = obj;
                    current = obj;
                }

                skip_whitespace(json);
                if (**json == ',') {
                    (*json)++;
                    skip_whitespace(json);
                } else if (**json == '}') {
                    (*json)++;
                    break;
                }
            }
        }

        return head;
    }
    return NULL;
}

// Function to parse any JSON value
JsonValue *parse_json_value(const char **json) {
    skip_whitespace(json);
    if (**json == '{') {
        JsonValue *value = malloc(sizeof(JsonValue));
        value->type = JSON_OBJECT;
        value->object = parse_json_object(json);
        return value;
    } else if (**json == '[') {
        return parse_json_array(json);
    } else if (**json == '"') {
        return parse_json_string(json);
    } else if (**json == 't' || **json == 'f') {
        return parse_json_boolean(json);
    } else if (**json == 'n') {
        return parse_json_null(json);
    } else if (isdigit(**json) || **json == '-') {
        return parse_json_number(json);
    }
    return NULL;
}

// Function to free the JSON value memory
void free_json_value(JsonValue *value) {
    if (value->type == JSON_STRING) {
        free(value->string);
    } else if (value->type == JSON_ARRAY) {
        for (int i = 0; i < value->array_size; i++) {
            free_json_value(value->array[i]);
        }
        free(value->array);
    } else if (value->type == JSON_OBJECT) {
        JsonObject *obj = value->object;
        while (obj) {
            free(obj->key);
            free_json_value(obj->value);
            JsonObject *temp = obj;
            obj = obj->next;
            free(temp);
        }
    }
    free(value);
}

// Function to print JSON value (for testing purposes)
void print_json_value(JsonValue *value) {
    switch (value->type) {
        case JSON_NULL:
            printf("null");
            break;
        case JSON_BOOLEAN:
            printf(value->boolean ? "true" : "false");
            break;
        case JSON_NUMBER:
            printf("%f", value->number);
            break;
        case JSON_STRING:
            printf("\"%s\"", value->string);
            break;
        case JSON_ARRAY:
            printf("[");
            for (int i = 0; i < value->array_size; i++) {
                print_json_value(value->array[i]);
                if (i < value->array_size - 1) {
                    printf(", ");
                }
            }
            printf("]");
            break;
        case JSON_OBJECT:
            printf("{");
            JsonObject *obj = value->object;
            while (obj) {
                printf("\"%s\": ", obj->key);
                print_json_value(obj->value);
                if (obj->next) {
                    printf(", ");
                }
                obj = obj->next;
            }
            printf("}");
            break;
    }
}

// Main function for testing
int main() {
    const char *json_text = "{\"name\": \"ChatGPT\", \"type\": \"AI\", \"versions\": [3.5, 4.0], \"is_active\": true, \"details\": null}";
    JsonValue *json_value = parse_json_value(&json_text);
    print_json_value(json_value);
    printf("\n");
    free_json_value(json_value);
    return 0;
}
