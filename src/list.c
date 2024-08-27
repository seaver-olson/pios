#include <stdio.h>
#include "list.h"

void list_add(struct list_element *list_head, struct list_element new_element){
	if (list_head->next == NULL){
		list_head->next = &new_element;
		return;
	}
	while (list_head->next != NULL){
		//printf("%d",list_head->data);
		list_head = list_head->next;
	}
	list_head->next = &new_element;
}
void list_remove(struct list_element *element);

void list_items(struct list_element *head){
	while (head->next != NULL){
		printf("%d",head->data);
		head = head->next;
	}
	printf("%d",head->data);
}
int main(){
	struct list_element c = {NULL, 5};
	struct list_element b = {&c, 3};
	struct list_element a = {&b , 4};
	struct list_element d = {NULL, 7};
	struct list_element e = {NULL, 9};
	struct list_element *head = &a;
	list_add(head,d);
	list_items(head);
	list_add(head,e);
	list_items(head);
	for (int i=0;i < 4;i++){
		printf("%d",head->data);
		head = head->next;
	}
	return 0;
}
	

