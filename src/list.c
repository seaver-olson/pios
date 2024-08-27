#include <stdio.h>
#include "list.h"

void list_add(struct list_element *list_head, struct list_element 
*new_element){
	while (list_head.next != NULL){
		list_head = list_head.next;
	}
	list_head.next = new_element;
}
void list_remove(struct list_element *element);

int main(){
	struct c = {NULL, 5};
	struct b = {&c, 4};
	struct a = {&b , 4};
	struct new = {NULL, 7};
	list_add(head,new);
	printf("%s","testing:");
	struct list_element *head = &a;
	while(true){
		printf("%d",head.data);
		head.next;
	}
	return 0;
}
	

