#ifndef JSONPARSER
#define JSONPARSER

void parse_json_object(const char *json_str, struct Dictionary *dictionary);

char *dictionary_to_json(struct Dictionary *dictionary);

#endif