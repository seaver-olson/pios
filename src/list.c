
#include <stdio.h>
#include "list.h"

void list_items(struct list_element *head){
        while (head != NULL){
                printf("%d",head->data);
                head = head->next;
        }
        printf("\n");
}


void list_add(struct list_element *list_head, struct list_element *new_element){
	if (list_head == NULL){
		list_head = new_element;
	}
	while (list_head->next != NULL){
		list_head = list_head->next;
	}
	list_head->next = new_element;
}

void list_remove(struct list_element *head, struct list_element *element){
	struct list_element *list_head = head;
	while(list_head != NULL){
		if ((list_head->next) == element){
			element = element->next;
			list_head->next = element;//replace link to next item, will set to NULL if next doesn't exist AKA it's at the end of the list
			list_head = list_head->next;
			return;
		}
		else{
			list_head = list_head->next;
		}
	}
}


int main(){
	/* test for all functions: cleared all 3 tests
	struct list_element c = {NULL, 5};
	struct list_element b = {&c, 3};
	struct list_element a = {&b , 4};
	struct list_element d = {NULL, 7};
	struct list_element e = {NULL, 9};
	struct list_element *new_element = &d;
	struct list_element *head = &a;
	list_add(head,new_element);//shows test of adding one element
	new_element = &e;//sets pointer to next new element
	list_items(head);//should print out 3457
	list_add(head,new_element);//tests adding multiple elements
	list_items(head);//should print out 34579
	new_element = &d;
	list_remove(head,new_element);//tests list_remove;
	list_items(head);//should print out 3459
	*/
	return 0;
}
	

