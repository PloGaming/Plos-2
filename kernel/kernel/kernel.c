#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <graphics/vga.h>
#include <graphics/framebuffer.h>
#include <kernel/multiboot2.h>

void kernel_main(uint32_t magicNumber, struct multiboot_tag *boot_information) 
{
	// Initialize the VGA output
	vga_terminal_initialize();

	// Check for being boooted by the GRUB bootloader with multiboot2
	if (magicNumber != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
		printf ("[ERROR] Invalid magic number: 0x%x\n", (uint32_t) magicNumber);
		return;
    }

	// Parsing the multiboot info structure
	struct multiboot_tag *ctrTag;
	int numTags = 0;
	for(ctrTag = (struct multiboot_tag *)(uint8_t *)boot_information + 8; 
		ctrTag->type != MULTIBOOT_TAG_TYPE_END; 
		ctrTag = (struct multiboot_tag *)((uint8_t *)ctrTag + ((ctrTag->size + 7) & ~7)))
		{
			switch(ctrTag->type)
			{
				case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
					struct multiboot_tag_basic_meminfo *basicMemInfo = (struct multiboot_tag_basic_meminfo *)ctrTag; 

					printf("MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:\n");
					printf ("mem_lower = %uKB, mem_upper = %uKB\n",
                  		basicMemInfo->mem_lower,
                  		basicMemInfo->mem_upper);

					break;
				case MULTIBOOT_TAG_TYPE_MMAP:
					struct multiboot_mmap_entry *memoryMap;
					
					printf("MULTIBOOT_TAG_TYPE_MMAP:\n");
					for(memoryMap = ((struct multiboot_tag_mmap *)ctrTag)->entries; 
						(uint8_t *)memoryMap < (uint8_t *)ctrTag + ctrTag->size; 
						memoryMap = (struct multiboot_mmap_entry *)((uint8_t *)memoryMap + ((struct multiboot_tag_mmap *)ctrTag)->entry_size))
						{
							printf (" base_addr = 0x%x%x,"
						" length = 0x%x%x, type = 0x%x\n",
						(uint32_t) (memoryMap->addr >> 32),
						(uint32_t) (memoryMap->addr & 0xffffffff),
						(uint32_t) (memoryMap->len >> 32),
						(uint32_t) (memoryMap->len & 0xffffffff),
						(uint32_t) memoryMap->type);
						}
					break;
				case MULTIBOOT_TAG_TYPE_VBE:
					struct multiboot_tag_vbe *vbeInfo = (struct multiboot_tag_vbe * )ctrTag;

					printf("MULTIBOOT_TAG_TYPE_VBE:\n");
					printf("Current video type: %hu\n", vbeInfo->vbe_mode);
					break;
				case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
					struct multiboot_tag_framebuffer *framebufferInfo = (struct multiboot_tag_framebuffer *) ctrTag;

					printf("MULTIBOOT_TAG_TYPE_FRAMEBUFFER:\n");
					printf("Framebuffer addr: 0x%x%x\n", 
						(uint32_t)(framebufferInfo->common.framebuffer_addr >> 32),
						(uint32_t)(framebufferInfo->common.framebuffer_addr & 0xffffffff));
					printf("Graphics mode: %u\n", framebufferInfo->common.framebuffer_type);
					printf("Height: %lu; Width: %lu\n", framebufferInfo->common.framebuffer_height, framebufferInfo->common.framebuffer_width);

					switch(framebufferInfo->common.framebuffer_type)
					{
						case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
							break;
						case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
							break;
						case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:		
							break;
						default:
							break;
					}
					break;
				case MULTIBOOT_TAG_TYPE_EFI32:
				case MULTIBOOT_TAG_TYPE_EFI64:
				case MULTIBOOT_TAG_TYPE_SMBIOS:
				case MULTIBOOT_TAG_TYPE_ACPI_OLD:
				case MULTIBOOT_TAG_TYPE_ACPI_NEW:
				case MULTIBOOT_TAG_TYPE_NETWORK:
				case MULTIBOOT_TAG_TYPE_EFI_BS:
				case MULTIBOOT_TAG_TYPE_EFI32_IH:
				case MULTIBOOT_TAG_TYPE_EFI64_IH:
				case MULTIBOOT_TAG_TYPE_LOAD_BASE_ADDR:
				case MULTIBOOT_TAG_TYPE_CMDLINE:
				case MULTIBOOT_TAG_TYPE_MODULE:
				case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
				case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				case MULTIBOOT_TAG_TYPE_APM:
					printf("multiboot useless TAG type: %lu\n", ctrTag->type);
					break;
				default:
					printf("multiboot unrecognized TAG type: %lu\n", ctrTag->type);
					break;
			}
			numTags++;
		}
		printf("Num of tags: %d\n", numTags);
}
