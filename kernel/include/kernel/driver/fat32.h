#ifndef _FAT32_H
#define _FAT32_H

#include <stdint.h>

/* Bios Parameter Block */
typedef struct BPB {
	unsigned char bytes_first[3];
	unsigned char OEM_ident[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t FAT_count;
	uint16_t directory_entry_count;
	uint16_t sector_count;
	uint8_t media_descriptor_type;
	uint16_t sectors_per_fat;		// Unused in fat32
	uint16_t sectors_per_track;
	uint16_t heads;
	uint32_t hidden_sector_count;
	uint32_t large_sector_count;
} __attribute__((packed)) BPB_t;

/* Extended Boot Record */
typedef struct EBR {
	uint32_t sectors_per_fat;
	uint16_t flags;
	uint16_t fat_version_number;
	uint32_t root_directory_cluster;
	uint16_t FSInfo_sector;
	uint16_t backup_boot_sector;
	unsigned char reserved[12];
	unsigned char drive_number;
	unsigned char WindowsNT_flags;
	unsigned char signature;
	uint32_t volume_id_serial;
	unsigned char volume_label[11];
	unsigned char system_identifier[8];
} __attribute__((packed)) EBR_t;

/* File System Info */
typedef struct FSInfo {
	uint32_t lead_signature;
	unsigned char reserved[480];
	uint32_t secondary_signature;
	uint32_t last_free_cluster_count;
	uint32_t search_cluster;
	unsigned char reserved2[12];
	uint32_t trail_signature;
} __attribute__((packed)) FSInfo_t;

/* Long Filename Entry */
typedef struct lfn_entry {
	unsigned char order;
	uint16_t bytes0[5];
	unsigned char attribute;
	unsigned char entry_type;
	unsigned char checksum;
	uint16_t bytes1[6];
	unsigned char zero[2];
	uint16_t bytes2;
} __attribute__((packed)) lfn_entry_t;

/* Directory Entry */
typedef struct directory_entry {
	unsigned char file_name[11];
	unsigned char file_attributes;
	unsigned char reserved;
	unsigned char create_time_tenths_second;
	uint16_t create_time;
	uint16_t create_date;
	uint16_t last_accessed_date;
	uint16_t first_cluster_high;
	uint16_t last_modified_time;
	uint16_t last_modified_date;
	uint16_t first_cluster_low;
	uint32_t file_size;

	int has_long_filename;
	lfn_entry_t lfn;
} __attribute__((packed)) directory_entry_t;

/* Fat32 struct for holding all the relevant information about a fat32 volume */
typedef struct fat32 {
	BPB_t bpb;
	EBR_t ebr;
	FSInfo_t* fsinfo;

	uint32_t fat_size;
	uint32_t first_data_sector;
	uint32_t first_fat_sector;
	uint32_t data_sectors;
	uint32_t root_cluster;
	uint8_t sectors_per_cluster;
} fat32_t;

/* Initialise a fat32 drive */
void fat32_init(int drive);

/* Read a single cluster */
unsigned char* read_cluster(int drive, uint32_t cluster);

/* Read a directory */
directory_entry_t* read_directory(int drive, uint32_t cluster);

#endif
