
#include "page.h"
#include <stdio.h>
//#define NULL    ((void*)0)//some how I just found out NULL doesn't come with C, I feel very stupid

struct ppage physical_page_array[128];
struct ppage *physical_frame_allocation = &physical_page_array[0];

void init_pfa_list(void) {
	physical_frame_allocation->prev = NULL;
	struct ppage *current_physical_page;
	for (int i = 0; i < 127;i++){
		current_physical_page = &physical_page_array[i+1]; //swapped the initizaitions  of curr_p_p and free_p_p, fixed seg fault
		physical_frame_allocation->next = current_physical_page;
		current_physical_page->prev = physical_frame_allocation;
		physical_frame_allocation->physical_addr  = (void *) &physical_page_array[i];
		physical_frame_allocation = current_physical_page;
	}
	physical_frame_allocation->next = NULL;//end of free physical pages
	physical_frame_allocation->physical_addr  = (void *)  &physical_page_array[127];
}

struct ppage *allocate_physical_pages(unsigned int npages){
	struct ppage *allocd_list,  *current_physical_page;
	allocd_list = physical_frame_allocation;
	for (int i = 0; i < npages; i++){
		allocd_list = allocd_list->next;
		if (i+1 == npages){
			physical_frame_allocation =  allocd_list->next;
			physical_frame_allocation->prev = NULL;
			allocd_list->next = NULL;
		}
	}
	return  allocd_list;
}

void free_physical_pages(struct ppage *ppage_list){

}


int main(){
	init_pfa_list();
	struct ppage *allocd_list;
	allocd_list = allocate_physical_pages(2);
	return 0;
}
