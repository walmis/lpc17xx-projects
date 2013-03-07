#ifndef  _SBL_IAP_H
#define  _SBL_IAP_H

extern const unsigned sector_start_map[];
extern const unsigned sector_end_map[];


unsigned write_flash(unsigned * dst, char * src, unsigned no_of_bytes);
void execute_user_code(void);
int user_code_present(void);
void erase_user_flash(void);
void check_isp_entry_pin(void);
void erase_user_flash(void);

typedef enum
{
	PREPARE_SECTOR_FOR_WRITE	=50,
	COPY_RAM_TO_FLASH			=51,
	ERASE_SECTOR				=52,
	BLANK_CHECK_SECTOR			=53,
	READ_PART_ID				=54,
	READ_BOOT_VER				=55,
	COMPARE						=56,
	REINVOKE_ISP				=57
}IAP_Command_Code;

#define CMD_SUCCESS 0
#define CMD_FAILURE 1
#define IAP_ADDRESS 0x1FFF1FF1

#endif /* _SBL_IAP_H */
