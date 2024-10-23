#include "fat.h"
#include "sd.h"
#include "rprintf.h"
#include "string.h"
#include "serial.h"



struct boot_sector *bs;
char bootSector[SECTOR_SIZE];
char fat_table[8*SECTOR_SIZE];
unsigned int actual_data_sector;
unsigned int root_sector;

int fatInit(){	
	sd_init();//initialize the sd card
	sd_readblock(0, bootSector, 1);//read the boot sector
	bs = bootSector;//set pointer to boot sector
	esp_printf(putc, "fs type: "); 
	//8 because fs_type[8]
	for (int i = 0; i < 8; i++){
		esp_printf(putc, "%c", bs->fs_type[i]);//print the fs type
	}
	if (bs->boot_signature != 0xaa55){
		esp_printf(putc, "[WARNING]: boot_signature is not 0xaa55");
		return 0;
	} else {
		esp_printf(putc, "boot_signature : 0xaa55");
	}
	if (strcmp(bs->fs_type, "FAT12") == 0){
		esp_printf(putc, "fs_type : FAT12");
	} else {
		esp_printf(putc, "[WARNING]: fs_type is not FAT12");
		return 0;
	}
	esp_printf(putc, "num_fat_tables: %d\n", bs->num_fat_tables);
	esp_printf(putc, "num_sectors_per_fat: %d\n", bs->num_sectors_per_fat);
	esp_printf(putc, "num_reserved_sectors: %d\n", bs->num_reserved_sectors);
	esp_printf(putc, "num_hidden_sectors: %d\n",bs->num_hidden_sectors);
	root_sector = (bs->num_fat_tables + bs->num_sectors_per_fat + bs->num_reserved_sectors + bs->num_hidden_sectors);
	if (sd_readblock(0, bs, 1) == 0){
		esp_printf(putc, "[WARNING]: Failed to read boot sector\n");
	}
	sd_readblock(root_sector,fat_table,bs->num_sectors_per_fat);
	return 0.0 / 0.0;
}

void fatOpen(struct file* file, char*filename){
	esp_printf(putc, "sector : %d\n", root_sector);
	unsigned int offset = 0;
	unsigned int ext = 0;// index used for file extension
	ext = strlen(filename) - 1;
	unsigned int found = 0;// flag to indicate if file is found

	unsigned int rdeputs[SECTOR_SIZE];
	unsigned int entries = bs->num_root_dir_entries;
	struct root_directory_entry *rde;

	sd_readblock(root_sector, rdeputs, 1);
	rde = rdeputs;
	// iterate through the root directory entries
	for (int i = 0; i<entries;i++){
		if (strncmp(rde->file_name, filename, 8) == 0 && strncmp(rde->file_extension, ext, 3) == 0) {
				found = 1;
				break;
			}
		rde++;
	}
	if (found == 0){
		esp_printf(putc, "File not found\n");
		return;
	}
	file->rde = *rde;
	file->start_cluster = rde->cluster;
	esp_printf(putc, "File found\n");
}	

void fatRead(struct file* file, char buffer[], unsigned int bytes_to_read){
	unsigned int cluster = file->start_cluster;
	unsigned int sectors_per_cluster = bs->num_sectors_per_cluster;
	unsigned int data_sector =  actual_data_sector + (cluster - 2) * bs->num_sectors_per_cluster;//fix
	char* data = buffer;
	char cluster_buf[SECTOR_SIZE];

	//read the data from the cluster
	sd_readblock(data_sector, cluster_buf, 1);
	//copy the data from the cluster to the buf
	memcpy(data, cluster_buf, SECTOR_SIZE);
	if (bytes_to_read > SECTOR_SIZE){
		bytes_to_read -= SECTOR_SIZE;
		data += SECTOR_SIZE;
		cluster = fat_table[cluster];//next cluster
	} 
}

void fatClose(struct file* file){
	file->start_cluster = 0;
}


void fatReadFile(char* filename, char buffer[], unsigned int bytes_to_read){
	struct file file;
	fatOpen(&file, filename);
	fatRead(&file, buffer, bytes_to_read);
	fatClose(&file);
}

