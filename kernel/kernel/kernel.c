#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <graphics/terminal.h>
#include <kernel/multiboot2.h>
#include <interrupts/idt.h>

void kernel_main(uint32_t magicNumber, struct multiboot_tag *boot_information) 
{
	// Check for being boooted by the GRUB bootloader with multiboot2
	if (magicNumber != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
		printf ("[ERROR] Invalid magic number: 0x%x\n", (uint32_t) magicNumber);
		return;
    }

	// Initialize the VGA output
	struct multiboot_tag *ctrTag;
	for(ctrTag = (struct multiboot_tag *)((uint8_t *)boot_information + MULTIBOOT_TAG_ALIGN); 
		ctrTag->type != MULTIBOOT_TAG_TYPE_END; 
		ctrTag = (struct multiboot_tag *)((uint8_t *)ctrTag + ((ctrTag->size + 7) & ~7)))
		{
			if(ctrTag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
			{
				terminal_initialize((struct multiboot_tag_framebuffer *)ctrTag);
			}
		}

	struct multiboot_tag_basic_meminfo *basicMemInfo;
	struct multiboot_mmap_entry *memoryMap;
	struct multiboot_tag_framebuffer *framebufferInfo;

	// Parsing the multiboot info structure
	for(ctrTag = (struct multiboot_tag *)((uint8_t *)boot_information + MULTIBOOT_TAG_ALIGN); 
		ctrTag->type != MULTIBOOT_TAG_TYPE_END; 
		ctrTag = (struct multiboot_tag *)((uint8_t *)ctrTag + ((ctrTag->size + 7) & ~7)))
		{
			switch(ctrTag->type)
			{
				case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
					basicMemInfo = (struct multiboot_tag_basic_meminfo *)ctrTag; 

					printf("MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:\n");
					printf ("mem_lower = %uKB, mem_upper = %uKB\n",
                  		basicMemInfo->mem_lower,
                  		basicMemInfo->mem_upper);
					break;

				case MULTIBOOT_TAG_TYPE_MMAP:
					printf("MULTIBOOT_TAG_TYPE_MMAP:\n");
					for(memoryMap = ((struct multiboot_tag_mmap *)ctrTag)->entries; 
						(uint8_t *)memoryMap < (uint8_t *)ctrTag + ctrTag->size; 
						memoryMap = (struct multiboot_mmap_entry *)((uint8_t *)memoryMap + ((struct multiboot_tag_mmap *)ctrTag)->entry_size))
						{
							printf (" base_addr = 0x%x%x,"
								" length = 0x%x%x bytes;",
								(uint32_t) (memoryMap->addr >> 32),
								(uint32_t) (memoryMap->addr & 0xffffffff),
								(uint32_t) (memoryMap->len >> 32),
								(uint32_t) (memoryMap->len & 0xffffffff));

							switch((uint32_t)memoryMap->type)
							{
								case MULTIBOOT_MEMORY_AVAILABLE:
									printf(" free memory");
									break;
								case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
									printf(" ACPI information");
									break;
								case MULTIBOOT_MEMORY_BADRAM:
									printf(" bad RAM");
									break;
								case MULTIBOOT_MEMORY_NVS:
									printf(" memory preserved for hibernation");
									break;
								case MULTIBOOT_MEMORY_RESERVED:
									printf(" reserved memory");
									break;
							}
							putchar('\n');
						}
					break;

				case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
					framebufferInfo = (struct multiboot_tag_framebuffer *) ctrTag;

					printf("MULTIBOOT_TAG_TYPE_FRAMEBUFFER:\n");
					printf("Framebuffer addr: 0x%x%x\n", 
						(uint32_t)(framebufferInfo->common.framebuffer_addr >> 32),
						(uint32_t)(framebufferInfo->common.framebuffer_addr & 0xffffffff));
					printf("Graphics mode: %u\n", framebufferInfo->common.framebuffer_type);
					printf("Height: %u; Width: %u\n", framebufferInfo->common.framebuffer_height, framebufferInfo->common.framebuffer_width);
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
				case MULTIBOOT_TAG_TYPE_BOOTDEV:
				case MULTIBOOT_TAG_TYPE_VBE:
					printf("multiboot useless TAG type: %u\n", ctrTag->type);
					break;

				default:
					printf("multiboot unrecognized TAG type: %u\n", ctrTag->type);
					break;
			}
		}

	idt_table_init();
}
