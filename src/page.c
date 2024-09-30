#include "page.h"

struct ppage physical_page_array[128];

void init_pfa_list(void) {
	struct ppage *free_physical_pages;
	struct ppage *current_physical_page = &physical_page_array[0];
	for (int i = 0; i < 128;i++){
		free_physical_pages->next = current_physical_page;
		current_physical_page->prev = free_physical_pages;
	}
}	

struct ppage *allocate_physical_pages(unsigned int npages){

}

void free_physical_pages(struct ppage *ppage_list){

}


int main(){
	init_pfa_list();
	return 0;
}
