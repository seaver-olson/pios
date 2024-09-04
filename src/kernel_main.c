
extern unsigned char __bss_start;
extern unsigned char __bss_end;

void kernel_main() {
	char *begin_bss = &__bss_start;
	char *end_bss = &__bss_end;
	//testing where the bss begins and ends, vars aren't working
	printf("%d", &begin_bss);
	printf("%d", &end_bss);
	while(1){
    }
}
