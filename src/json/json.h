#ifndef JSON_H
#define JSON_H

struct json_node;

typedef struct json_kvp {
	
	char* string_key;
	int int_key;
	struct json_node* value;

} json_kvp;

typedef int JSON_TYPE;

typedef struct json_node {
	
	JSON_TYPE type;
	const char* string_val;
	double double_val;

	int child_count;
	json_kvp children[50];

} json_node;

int parse(const char* string, json_node* result);
const char* stringify(const json_node* json);

#endif