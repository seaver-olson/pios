#include <stdio.h>
#include "list.h"

void list_add(struct list_element *list_head, struct list_element new_element){
	while (list_head->next != NULL){
		list_head = list_head->next;
	}
	list_head->next = &new_element;
}
void list_remove(struct list_element *element);

int main(){
	struct list_element c = {NULL, 5};
	struct list_element b = {&c, 4};
	struct list_element a = {&b , 4};
	struct list_element d = {NULL, 7};
	struct list_element *head = &a;
	list_add(head,d);
	printf("%s","testing:");
	while(head != NULL){
		printf("%d",head->data);
		head = head->next;
	}
	return 0;
}
	

