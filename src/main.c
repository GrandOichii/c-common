#include <stdio.h>
#include <stdlib.h>

#include "list/list.h"
#include "stack/stack.h"
#include "json/json.h"

#define json_test_path "test.json"

const char * json_str = ""
"{\n"
"	\"value\": \"hello, world!\",\n"
"	\"second\": 123,\n"
"	\"arr\": [10, 4, \"amogus\", 51]"
"}";

// const char* json_str = "1";

void list_test() {
	list* l = new_list();
	for (int i = 0; i < 5; i++)
		list_add(l, i);

	for (int i = 0; i < list_size(l); i++)
		printf("%d: %d\n", i, list_get(l, i));

	list_free(l);
}

void stack_test() {
	stack* s = new_stack();

	stack_push(s, 2);
	stack_push(s, 4);
	stack_push(s, -1);
	stack_push(s, -10);

	int size = s->cur;
	for (int i = 0; i < size; i++)
		printf("%d\n", stack_pop(s));


	stack_free(s);
}

void json_test() {
	json_node result;
	int err = parse(json_str, &result);
	// printf("Node child count: %d\n", result.child_count);
	// printf("Root type: %d\n", result.type);
	// printf("Child 1 type: %d\n", result.children[0].value->type);
	// printf("Child 2 type: %d\n", result.children[1].value->type);

	// printf("First key: %s\n", result.children[0].string_key);
	// printf("First value: %s\n", result.children[0].value->string_val);
	// printf("Second key: %s\n", result.children[1].string_key);
	// printf("Second key: %f\n", result.children[1].value->double_val);
	printf("Err: %d\n", err);

	const char* str = stringify(&result);

	printf("Stringify result:\n%s\n", str);
}

int main() {
	json_test();
	return 0;
}
// {
// 	"key": {
// 		"values": [
// 			"v1",
// 			"v2"
// 		]
// 	},
// 	"arr": [
// 		1, 2, 3
// 	],
// 	"other_arr": [
// 		{
// 			"name": "v1"
// 		},
// 		{
// 			"name": "v1"
// 		}
// 	]
// }