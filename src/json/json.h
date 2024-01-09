#ifndef JSON_H
#define JSON_H

struct json_node;

typedef struct json_kvp {
	
	const char* string_key;
	int int_key;
	struct json_node* value;

} json_kvp;

typedef struct json_node {
	
	const char* string_val;
	float float_val;

	int child_count;
	json_kvp** children;

} json_node;

int parse(const char* string, json_node* result);

#endif