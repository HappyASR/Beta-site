
#define FPGA_Enable_Disable 0


#ifdef CONFIG_ITCM
    #define ITCM_Enable_Disable 1
#else
    #define ITCM_Enable_Disable 0
#endif

#ifdef CONFIG_DTCM
    #define DTCM_Enable_Disable 1
#else
    #define DTCM_Enable_Disable 0
#endif

#ifdef CONFIG_MDDR
    #define MDDR_Enable_Disable 1
#else
    #define MDDR_Enable_Disable 0
#endif

#ifdef CONFIG_SRAM 
    #define SRAM_Enable_Disable      1
#else
    #define SRAM_Enable_Disable      0
#endif    

#ifdef CONFIG_DP_SRAM 
#define DP_SRAM_Enable_Disable      1  
#else
#define DP_SRAM_Enable_Disable      0
#endif

#if 0//For PPImage2
#define ANOTHER_DTCM_Enable_Disable 1 
#else
#define ANOTHER_DTCM_Enable_Disable 0 
#endif

#define ITCM1_ENABLE		0

#define DTCM1_ENABLE		0




