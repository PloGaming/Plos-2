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
	for(ctrTag = boot_information;ctrTag->type != MULTIBOOT_TAG_TYPE_END; 
				ctrTag = (struct multiboot_tag *) ((void *) ctrTag + ((ctrTag->size + 7) & ~7)))
	{
		switch(ctrTag->type)
		{
			case MULTIBOOT_TAG_TYPE_MMAP:
				// TODO use it
				break;
			case MULTIBOOT_TAG_TYPE_VBE:
				struct multiboot_tag_vbe *vbeInfo = (struct multiboot_tag_vbe * )ctrTag;
				break;
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *) ctrTag;
				// framebuffer_putpixel(tagfb, 1,1,2);
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
			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			case MULTIBOOT_TAG_TYPE_CMDLINE:
			case MULTIBOOT_TAG_TYPE_MODULE:
			case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
			case MULTIBOOT_TAG_TYPE_APM:
				break;
			default:
				// TODO handle the case
				break;
		}
	}
}
