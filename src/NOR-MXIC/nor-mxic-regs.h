#include <platform.h>
#include <irqs.h>
#include <type.h>

#ifdef SQ_NOR_FLASH_BANK0
#define SOCLE_NOR_BASE0	SQ_NOR_FLASH_BANK0;
#else 
#error we dont define IP address 
#endif

#if defined SOCLE_NOR_FLASH_BANK1
	#define SOCLE_NOR_BASE1	SOCLE_NOR_FLASH_BANK1
#else
	#define SOCLE_NOR_BASE1	-1
#endif

//#define FLASH_TOTAL_SEC_NUM       71
/* added by morganlin */
#define FLASH_TOTAL_SEC_NUM       128


//#define FLASH_SIZE   0x400000
/* added by morganlin */
#define FLASH_SIZE      0x1000000 

/* #define FLASH_SECTOR_SIZE  0x10000 */
/* added by morganlin */
#define FLASH_SECTOR_SIZE  0x20000 

#if defined(CONFIG_PC9223)
 	#define MODE_BASE (SQ_SDRSTMC0+0x8020)
#endif

/*define errror code*/
#define FLASH_NOERROR				 0
#define FLASH_ERROR				  	 1
#define FLASH_NOTSUPPORT			-1/*the flash memory is not support in this version*/
#define FLASH_INIT_ERROR			-2/*read flash identifier error*/
#define FLASH_VPP_ERROR 			-3/*Vpp range error*/
#define FLASH_CMDSEQ_ERROR		-4/*command sequence error*/
#define FLASH_ERASE_ERROR			-5/*erase error*/
#define FLASH_PROGRAM_ERROR		-6/*flash program error*/
#define FLASH_OVERFLOW				-7/*the size of the data exceeds the size of flash*/
#define FLASH_NOT_INIT				-8/*the flash is not initialized*/

