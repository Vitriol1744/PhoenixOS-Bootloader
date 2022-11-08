#ifndef PHOENIXOS_BOOT__VESA_H
#define PHOENIXOS_BOOT__VESA_H

#include "arch/framebuffer.h"

typedef struct
{
    uint32_t vbe_signature;
    uint16_t vbe_version;
    uint16_t oem_string_ptr[2];
    uint8_t capabilities[4];
    uint16_t video_mode_off;
    uint16_t video_mode_seg;
    uint16_t total_memory;
} __attribute__((packed)) vbe_info_block_t;

typedef struct
{
	uint16_t attributes;		    // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			    // deprecated
	uint8_t window_b;			    // deprecated
	uint16_t granularity;		    // deprecated; used while calculating bank numbers
	uint16_t window_size;   
	uint16_t segment_a; 
	uint16_t segment_b; 
	uint32_t win_func_ptr;		    // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			        // number of bytes per horizontal line
	uint16_t width;			        // width in pixels
	uint16_t height;			    // height in pixels
	uint8_t w_char;			        // unused...
	uint8_t y_char;			        // ...
	uint8_t planes; 
	uint8_t bpp;			        // bits per pixel in this mode
	uint8_t banks;			        // deprecated; total number of banks in this mode
	uint8_t memory_model;   
	uint8_t bank_size;		        // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		    // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed)) vbe_mode_info_t;

/**
 * @brief Initializes VBE framebuffer
 */
bool vbe_Initialize(struct framebuffer* framebuffer);

/**
 * @brief Gets VBE Controller Info
 * @param buffer Pointer to buffer receiving controller info
 * 
 * @return Non-zero on success
 */
bool __attribute__((cdecl)) vbe_get_controller_info(void* buffer);
/**
 * @brief Gets VBE mode info
 * @param mode_index Index of VBE mode
 * @param vbe_mode_info Pointer to structure receiving information
 * 
 * @return Non-zero on success
 */
bool __attribute__((cdecl)) vbe_get_mode_info(uint16_t mode_index, vbe_mode_info_t* vbe_mode_info);
/**
 * @brief Sets VBE mode
 * @param mode_index Index of VBE mode
 * 
 * @return Non-zero on success
 */
bool __attribute__((cdecl)) vbe_set_video_mode(uint16_t mode_index);

#endif // PHOENIXOS_BOOTLOADER__VESA_H
