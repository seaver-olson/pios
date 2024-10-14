#include "fat.h"
#include "sd.h"
#include "rprintf.h"
#include "serial.h"
struct boot_sector *bs;
char bootSector[512];
char fat_table[8*SECTOR_SIZE];
unsigned int actual_data_sector;
unsigned int root_sector;

int fatInit(){	
	sd_init();
	sd_readblock(0, bootSector, 1);
	bs = bootSector;
	esp_printf(putc, "fs type: ");
	//8 because fs_type[8]
	for (int i = 0; i < 8; i++){
		esp_printf(putc, "%c", bs->fs_type[i];
	}
	if (bs->boot_signature != 0xaa55){
		esp_printf(putc, "[WARNING]: boot_signature is not 0xaa55");
	} else {
		esp_printf(putc, "boot_signature : 0xaa55");
	}
	if (strcmp(bs->fs_type, "FAT12") == 0){
		esp_printf(putc, "fs_type : FAT12");
	} else {
		esp_printf(putc, "[WARNING]: fs_type is not FAT12");
	}
	esp_printf(putc, "num_fat_tables: %d\n", bs->num_fat_tables);
	esp_printf(putc, "num_sectors_per_fat: %d\n", bs->num_sectors_per_fat);
	esp_printf(putc, "num_reserved_sectors: %d\n", bs->num_reserved_sectors);
	esp_printf(putc, "num_hidden_sectors: %d\n",bs->num_hidden_sectors);
	root_sector = (bs->num_fat_tables + bs->num_sectors_per_fat + bs->num_reserved_sectors + bs->num_hidden_sectors);
	if (sd_readblock(0, boot_sector, 1) == 0){
		esp_printf(putc, "[WARNING]: Failed to read boot sector\n");
	}
	sd_readblock(root_sector,fat_table,bs->num_sectors_per_fat);	
	
	
}
