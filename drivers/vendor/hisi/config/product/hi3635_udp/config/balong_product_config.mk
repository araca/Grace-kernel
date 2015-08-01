# MD5: 5568f1622f3c4c51fc80cf43bae1c11b
CFG_BALONG_OBUILD_VERSION:=10
CFG_PRODUCT_NAME                      :="hi3635_udp"
CFG_PRODUCT_CFG_CHIP_SOLUTION_NAME    :="Balong"
CFG_BUILD_TYPE                        :=RELEASE
CFG_PRODUCT_VERSION                   :=K300V300C00B000
CFG_PRODUCT_VERSION_STR               :="K300V300C00B000"
CFG_PRODUCT_FULL_VERSION_STR          :="Seattle V100R001C00B293"
CFG_PRODUCT_FILE_VERSION_STR          :="2.1.6.8"
CFG_PRODUCT_DLOAD_SOFTWARE_VER                := "21.293.17.00.000"
CFG_FEATURE_OBJ_CMP 				:=NO
CFG_FEATURE_OBJ_DUMP				:=NO
CFG_CONFIG_VERSION_STUB             :=NO
CFG_HW_VERSION_STUB                 :=0x3e000702
CFG_CONFIG_VER_MASK                 :=0x3e000000
CFG_USE_USBLOADER_MERGE 			:=YES
CFG_VRL_TOOL 					:=vrl_creater_for_k3v5
CFG_PLATFORM :=hi3630
CFG_FEATURE_MULTIMODE_GUL := YES
CFG_OS_PATH :=config/product/hi3635_udp/os
CFG_OS_IMG_FILE:=VXWORKS.6.8.3.IMG.wpj
CFG_OS_LIB:=VXWORKS.6.8.3.LIB.RELEASE
CFG_FTP_REPLY_TIME_OUT:=0
CFG_OS_LINUX_PRODUCT_NAME          :=hisi_3635_defconfig
CFG_OS_ANDROID_MODEM_DECONFIG      :=hi3635_modem_defconfig
CFG_OS_ANDROID_TEMP_DECONFIG       :=merge_hisi_k3v3fpga_defconfig
CFG_OS_ANDROID_PRODUCT_NAME        :=hi3635
CFG_AP_CHIP_NAME                   :=hi3635
CFG_OS_ANDROID_SYNC_FILE_LIST      :=include.list
OBB_ANDROID_DIR                    :=$(BALONG_TOPDIR)/../..
CFG_OS_ANDROID_MODE                :=normal
CFG_OS_ANDROID_USE_K3V3_KERNEL     :=YES
CFG_OS_ANDROID_AARCH64             :=YES
CFG_OS_K3V3_USE_LPM3_API           :=YES
CFG_PRODUCT_CONTROL_VERSION                 :="121"
CFG_PRODUCT_HISILICON_VERSION               :=hi6930
CFG_PRODUCT_NV_SPLITFILE                    :=/binfilesplit "0"
CFG_PRODUCT_NV_FILE_MAX_SIZE                :=/binfileLimitSize "100"
CFG_PRODUCT_NV_BIN_MODEM_NUNBER             :=/bin_modem_number "2"
CFG_PRODUCT_NV_XNV_MODEM_NUNBER             :=/xnv_modem_number "2"
CFG_PRODUCT_NV_XNV_SUPPORT_PRODUCT_CAT      :=/xnv_support_product_cat "1"
CFG_LNV_PRO_NAME :=PhoneSeattle
CFG_XTENSA_CORE				:=k3v3_bbe16
CFG_XTENSA_SYSTEM			:=$(ROOT_XTENSA_PATH_W)/XtDevTools/install/builds/RD-2012.5-linux/k3v3_bbe16/config
LPHY_LD_MAP_PUB_PATH        :=modem/phy/lt/Tensilica_LSP/BBE16_K3V3P_LSP/mainlsp
LPHY_LD_MAP_LTE_PATH        :=modem/phy/lt/Tensilica_LSP/BBE16_K3V3P_LSP/overlaylsp
LPHY_LD_MAP_TDS_PATH        :=modem/phy/lt/Tensilica_LSP/BBE16_K3V3P_LSP/overlaylsp
LPHY_LD_SDR_LIB_FILE        :=BBE16_K3V3_O2
LPHY_XTENSA_RULES           :=cc_tensilica4.0.5_rules.mk
CFG_LPHY_UNI_DTCM_BASE	    :=0xe2780000
CFG_LPHY_UNI_ITCM_BASE	    :=0xe2800000
CFG_LPHY_PUB_DTCM_BASE		:=0xe2780000
CFG_LPHY_PUB_ITCM_BASE		:=0xe2800000
CFG_LPHY_PRV_DTCM_BASE		:=0xe27c4000
CFG_LPHY_PRV_ITCM_BASE		:=0xe283e000
CFG_LPHY_UNI_DTCM_SIZE		:=0x80000
CFG_LPHY_UNI_ITCM_SIZE		:=0x80000
CFG_LPHY_PUB_DTCM_SIZE		:=0x44000
CFG_LPHY_PUB_ITCM_SIZE		:=0x3e000
CFG_LPHY_PRV_DTCM_SIZE		:=0x3c000
CFG_LPHY_PRV_ITCM_SIZE		:=0x42000
CFG_TL_PHY_LALG_PATH    :=include/phy
CFG_TL_PHY_ASIC_K3V3_PLUS     := YES
CFG_TL_PHY_6930         := YES
CFG_TL_PHY_FEATURE_LTE_LCS  := NO
CFG_GU_INC_PATH :=COMM_CODE_GU/Balong_GU_Inc/GUL_CS
CFG_GU_PRODUCT_VERSION :=VERSION_V7R1
CFG_RAT_GU:=0
CFG_RAT_GUL:=1
CFG_RAT_MODE :=RAT_GUL
CFG_GU_RAT_MODE :=RAT_GUL
CFG_PLATFORM_HISI_BALONG :=hi3630
CFG_TTF_SKB_EXP := NO
CFG_GU_FEATURE_CONFIG_PATH :=config/product/hi3635_udp/include_gu
CFG_HIFI_LINK_DIR_NAME :=hifi_ld_V7R1_CS_UDP_STICK
CFG_BSP_CONFIG_K3V3_ASIC := YES
CFG_HI3630_FASTBOOT_MODEM := NO
CFG_BSP_CONFIG_HI3630 := YES
CFG_BSP_CONFIG_HI3635 := YES
CFG_BSP_CONFIG_BOARD_UDP := YES
CFG_BSP_CONFIG_EDA := NO
CFG_BSP_CONFIG_NOT_DDR_BYPASSPLL := NO
CFG_BSP_ENABLE_SEC_VERIFY   := NO
CFG_BSP_ENBALE_PACK_IMAGE   := YES
CFG_BSP_USB_BURN            := NO
CFG_BSP_HAS_SEC_FEATURE     := NO
CFG_BSP_ICC_MCHANNEL_USE_LPM3TCM := YES
CFG_CONFIG_FASTBOOT_DEBUG := NO
CFG_ROM_COMPRESS := NO
CFG_CONFIG_COMPRESS_CCORE_IMAGE := NO
CFG_CONFIG_MODULE_VIC := NO
CFG_CONFIG_PASTAR_DSM := YES
CFG_CONFIG_CIPHER := YES
CFG_CONFIG_CIPHER_ENABLE_BBPPLL := YES
CFG_CONFIG_CSHELL := YES
CFG_CONFIG_IPF    := YES
CFG_CONFIG_MODULE_BUSSTRESS := NO
CFG_CONFIG_ICC := YES
CFG_CONFIG_PM_OM := YES
CFG_CONFIG_BALONG_RTC := NO
CFG_CONFIG_HI6551_RTC := NO
CFG_CONFIG_HI3630_RTC0 := YES
CFG_CONFIG_MEM := YES
CFG_CONFIG_CPUFREQ := YES
CFG_CONFIG_REGULATOR := YES
CFG_CONFIG_CCORE_WDT := YES
CFG_CONFIG_ACORE_WDT := NO
CFG_CONFIG_MODEM_PERF := NO
CFG_CONFIG_PMIC_HI6451 := NO
CFG_CONFIG_PMIC_HI6551 := NO
CFG_CONFIG_COUL := NO
CFG_CONFIG_SPI := NO
CFG_CONFIG_SFLASH := NO
CFG_FEATURE_USB_ZERO_COPY := YES
CFG_CONFIG_WM8990 := NO
CFG_CONFIG_AUDIO := NO
CFG_CONFIG_SIO := NO
CFG_CONFIG_ABB := YES
CFG_CONFIG_DSP := YES
CFG_CONFIG_HKADC := YES
CFG_CONFIG_ONOFF := YES
CFG_CONFIG_TUNER := YES
CFG_CONFIG_HW_SPINLOCK := YES
CFG_CONFIG_BBP_INT := YES
CFG_FEATURE_WIFI_ON := NO
CFG_FEATURE_BCM_VERSION := 43241
CFG_CONFIG_NVIM := YES
CFG_FEATURE_NV_FLASH_ON := NO
CFG_FEATURE_NV_EMMC_ON  := YES
CFG_FEATURE_NV_LFILE_ON := NO
CFG_FEATURE_NV_RFILE_ON := NO
CFG_FEATURE_UPGRADE_TL := YES
CFG_FEATURE_TLPHY_MAILBOX :=YES
CFG_CONFIG_MAILBOX_TYPE := YES
CFG_CONFIG_PASTAR := YES
CFG_CONFIG_PASTAR_DPM_M3 := YES
CFG_CONFIG_MIPI := YES
CFG_MODEM_MEM_REPAIR := NO
CFG_ENABLE_SHELL_SYM :=YES
CFG_ENABLE_DEBUG :=YES
CFG_CCORE_COMPONENTS :=bsp_ccore.o gumsp_mcore.o nas_ccore.o rabm_ccore.o gas_ccore.o was_ccore.o ttf_ccore.o ascomm_ccore.o pscomm_ccore.o taf_ccore.o tlas_ccore.o tlnas_ccore.o lmsp_ccore.o gudsp_mcore.o gudsp_comm_RT.o gudsp_mcore_RT.o os_ccore.o
CFG_CCORE_MODEM1_COMPONENTS :=bsp_ccore1.o nas1_ccore.o rabm1_ccore.o gas1_ccore.o taf1_ccore.o ascomm1_ccore.o pscomm1_ccore.o gumsp1_mcore.o lmsp1_ccore.o gudsp1_mcore.o gudsp1_mcore_RT.o ttf1_ccore.o
CFG_CCORE_MODEMUNION_COMPONENTS :=modemcore1_union.o
CFG_BUILD_DRV_ONLY          := NO
CFG_ENABLE_TEST_CODE := NO
CFG_ENABLE_BUILD_VARS := YES
CFG_ENABLE_BUILD_OM := YES
CFG_ENABLE_BUILD_SYSVIEW := NO
CFG_ENABLE_BUILD_CPUVIEW := YES
CFG_ENABLE_BUILD_MEMVIEW := NO
CFG_ENABLE_BUILD_AMON := NO
CFG_ENABLE_BUILD_UTRACE := YES
CFG_ENABLE_BUILD_SOCP := YES
CFG_CONFIG_CCORE_CPU_IDLE := YES
CFG_CONFIG_BSP_TEST_CCORE := NO
CFG_CONFIG_BALONG_CCLK := YES
CFG_FEATURE_E5_ONOFF := NO
CFG_CONFIG_CCORE_PM := YES
CFG_CONFIG_MODULE_IPC := YES
CFG_CONFIG_MODULE_TIMER := YES
CFG_CONFIG_MODULE_SYNC := YES
CFG_K3_TIMER_FEATURE := YES
CFG_CONFIG_RSE := YES
CFG_CONFIG_HAS_CCORE_WAKELOCK := YES
CFG_CONFIG_CCORE_BALONG_PM := YES
CFG_CONFIG_BALONG_EDMA := YES
CFG_CONFIG_BALONG_EDMA_TEST := NO
CFG_CONFIG_DUAL_MODEM := YES
CFG_CONFIG_IPF_TEST := NO
CFG_CONFIG_FB_SPI_BALONG := NO
CFG_CONFIG_FB_EMI_BALONG := NO
CFG_CONFIG_FB_1_4_5_INCH_BALONG:=NO
CFG_CONFIG_FB_2_4_INCH_BALONG:=NO
CFG_CONFIG_TEMPERATURE_PROTECT := YES
CFG_CONFIG_IOS_MBB_BALONG := NO
CFG_CONFIG_MODEM_PINTRL := YES
CFG_CONFIG_ANTEN := YES
CFG_CONFIG_EFUSE := YES
CFG_CONFIG_DDM := YES
CFG_CONFIG_TSENSOR := NO
CFG_CONFIG_USB_DWC3_VBUS_DISCONNECT:=NO
CFG_USB3_SYNOPSYS_PHY:=NO
CFG_CONFIG_USB_FORCE_HIGHSPEED:=NO
CFG_CONFIG_PWC_MNTN_CCORE := YES
CFG_CONFIG_TCXO_BALONG := YES
CFG_CONFIG_BALONG_DPM := NO
CFG_CONFIG_BALONG_MODEM_RESET := YES
CFG_CONFIG_LOAD_SEC_IMAGE := YES
CFG_MCORE_TEXT_START_ADDR               := 0x30D10000
CFG_MCORE_TEXT_START_ADDR_COMPRESSED    := 0x34000000
CFG_FASTBOOT_ENTRY                      := 0xE0800000
CFG_FASTBOOT_DDR_ENTRY                  := 0x37F00000
CFG_RTX_KERNEL_ENTRY                    := 0x10000000
CFG_HI_SRAM_MEM_ADDR                    := 0xE0800000
CFG_DDR_MEM_ADDR                        := 0x30000000
CFG_DDR_MEM_SIZE                        := 0x08000000
CFG_DDR_APP_ACP_ADDR                    := (DDR_MEM_ADDR)
CFG_DDR_APP_ACP_SIZE                    := 0x00000000
CFG_DDR_GU_ADDR                         := ((DDR_APP_ACP_ADDR) + (DDR_APP_ACP_SIZE))
CFG_DDR_GU_SIZE                         := 0x00B00000
CFG_DDR_TLPHY_IMAGE_ADDR                := ((DDR_GU_ADDR) + (DDR_GU_SIZE))
CFG_DDR_TLPHY_IMAGE_SIZE                := 0x00200000
CFG_DDR_MCORE_ADDR                      := ((DDR_TLPHY_IMAGE_ADDR) + (DDR_TLPHY_IMAGE_SIZE))
CFG_DDR_MCORE_SIZE                      := 0x05000000
CFG_DDR_LPHY_SDR_ADDR                   := ((DDR_MCORE_ADDR) + (DDR_MCORE_SIZE))
CFG_DDR_LPHY_SDR_SIZE                   := 0x200000
CFG_DDR_LCS_ADDR                        := ((DDR_LPHY_SDR_ADDR) + (DDR_LPHY_SDR_SIZE))
CFG_DDR_LCS_SIZE                        := 0x280000
CFG_DDR_RESERVED_ADDR                   := ((DDR_LCS_ADDR) + (DDR_LCS_SIZE))
CFG_DDR_RESERVED_SIZE                   := 0x80000
CFG_DDR_ACORE_ADDR                      := ((DDR_RESERVED_ADDR) + (DDR_RESERVED_SIZE))
CFG_DDR_ACORE_SIZE                       := 0x0
CFG_DDR_MDM_ACP_ADDR                    := ((DDR_ACORE_ADDR) + (DDR_ACORE_SIZE))
CFG_DDR_MDM_ACP_SIZE                    := 0x0
CFG_DDR_SHARED_MEM_ADDR                 := ((DDR_MDM_ACP_ADDR) + (DDR_MDM_ACP_SIZE))
CFG_DDR_SHARED_MEM_SIZE                 := 0x00300000
CFG_DDR_MNTN_ADDR                       := (0x3f800000)
CFG_DDR_MNTN_SIZE                       := (0x00800000-0x00008000)
CFG_DDR_SOCP_ADDR                       := ((DDR_SHARED_MEM_ADDR) + (DDR_SHARED_MEM_SIZE) + 0x00100000)
CFG_DDR_SOCP_SIZE                       := 0x01100000
CFG_DDR_HIFI_ADDR                       := ((DDR_SOCP_ADDR) + (DDR_SOCP_SIZE))
CFG_DDR_HIFI_SIZE                       := 0x00900000
CFG_CORESHARE_MEM_TENCILICA_MULT_BAND_SIZE := 0x8000
CFG_CORESHARE_MEM_TENCILICA_MULT_BAND_ADDR := DDR_SHARED_MEM_ADDR
CFG_THUMB_COMPILE := NO
CFG_FEATURE_ON                                  := 1
CFG_FEATURE_OFF                                 := 0
CFG_MEMORY_SIZE_32M                             := 1
CFG_MEMORY_SIZE_64M                             := 2
CFG_MEMORY_SIZE_128M                            := 3
CFG_MEMORY_SIZE_256M                            := 4
CFG_MEMORY_SIZE_512M                            := 5
CFG_PS_PTL_VER_DANAMIC                          := 20
CFG_PS_PTL_VER_PRE_R99                          := (-2)
CFG_PS_PTL_VER_R99                              := (-1)
CFG_PS_PTL_VER_R3                               := 0
CFG_PS_PTL_VER_R4                               := 1
CFG_PS_PTL_VER_R5                               := 2
CFG_PS_PTL_VER_R6                               := 3
CFG_PS_PTL_VER_R7                               := 4
CFG_PS_PTL_VER_R8                               := 5
CFG_PS_PTL_VER_R9                               := 6
CFG_PS_UE_REL_VER                               := PS_PTL_VER_R9
CFG_FEATURE_UE_MODE_G                           := FEATURE_ON
CFG_FEATURE_UE_MODE_W                           := FEATURE_ON
CFG_FEATURE_CS                                  := FEATURE_ON
CFG_FEATURE_GFAX                                := FEATURE_OFF
CFG_FEATURE_WFAX                                := FEATURE_OFF
CFG_FEATURE_GCBS                                := FEATURE_ON
CFG_FEATURE_WCBS                                := FEATURE_ON
CFG_FEATURE_MERGE_OM_CHAN						:= FEATURE_ON
CFG_FEATURE_AP                                  := FEATURE_OFF
CFG_BALONG_UDP_V1R1                             := FEATURE_OFF
CFG_BALONG_UDP_V1R2                             := FEATURE_OFF
CFG_FEATURE_PTABLE_UDP                          := FEATURE_ON
CFG_FEATURE_STICK								:= FEATURE_OFF
CFG_FEATURE_E5                                  := FEATURE_OFF
CFG_FEATURE_E5_UDP                              := FEATURE_OFF
CFG_FEATURE_PC_VOICE                            := FEATURE_OFF
CFG_FEATURE_HILINK                              := FEATURE_OFF
CFG_FEATURE_SEC_BOOT                            := FEATURE_OFF
CFG_FEATURE_LTE                                 := FEATURE_ON
CFG_FEATURE_CSD                                 := FEATURE_OFF
CFG_FEATURE_IPV6                                := FEATURE_ON
CFG_FEATURE_PPP                                 := FEATURE_OFF
CFG_FEATURE_PPPOE                               := FEATURE_OFF
CFG_FEATURE_VCOM_EXT                            := FEATURE_ON
CFG_FEATURE_UPDATEONLINE                        := FEATURE_OFF
CFG_FEATURE_WIFI                                := FEATURE_OFF
CFG_FEATURE_HIFI                                := FEATURE_ON
CFG_FEATURE_SDIO                                := FEATURE_OFF
CFG_FEATURE_KEYBOARD                            := FEATURE_OFF
CFG_FEATURE_CHARGE                              := FEATURE_OFF
CFG_FEATURE_ICC_DEBUG                           := FEATURE_ON
CFG_FEATURE_POWER_ON_OFF                        := FEATURE_OFF
CFG_FEATURE_OLED                                := FEATURE_OFF
CFG_FEATURE_TFT                                 := FEATURE_OFF
CFG_FEATURE_MMI_TEST                            := FEATURE_OFF
CFG_FEATURE_DL_E_CELL_FACH                      := FEATURE_ON
CFG_FEATURE_INTERPEAK                           := FEATURE_OFF
CFG_FEATURE_INTERPEAK_MINI                      := FEATURE_OFF
CFG_IPWEBS_FEATURE_E5                           := FEATURE_OFF
CFG_IPWEBS_FEATURE_WIRELESS_DONGLE              := FEATURE_OFF
CFG_IPWEBS_FEATURE_LIGHTNING_CARD               := FEATURE_OFF
CFG_FEATURE_WEBNAS                              := FEATURE_OFF
CFG_FEATURE_WIRELESS_DONGLE                     := FEATURE_OFF
CFG_FEATURE_PMU_PROTECT                         := FEATURE_OFF
CFG_FEATURE_VXWORKS_TCPIP                       := FEATURE_OFF
CFG_FEATURE_M2                                  := FEATURE_OFF
CFG_FEATURE_MEMORY_SIZE                         := MEMORY_SIZE_64M
CFG_FEATURE_ECM_RNDIS                           := FEATURE_OFF
CFG_FEATURE_RNIC                                := FEATURE_OFF
CFG_FEATURE_NFEXT                               := FEATURE_ON
CFG_FEATURE_UL_E_CELL_FACH                      := FEATURE_ON
CFG_FEATURE_BREATH_LIGHT                        := FEATURE_OFF
CFG_FEATURE_LEDSTATUS                           := FEATURE_OFF
CFG_FEATURE_E5_LED                              := FEATURE_OFF
CFG_FEATURE_RTC                                 := FEATURE_OFF
CFG_FEATURE_SMALL_MAILBOX                       := FEATURE_OFF
CFG_FEATURE_RECONFIG                            := FEATURE_ON
CFG_FEATURE_HWENCRY_REWORK                      := FEATURE_OFF
CFG_FEATURE_MMU_BIG                             := FEATURE_OFF
CFG_FEATURE_MMU_MEDIUM                          := FEATURE_ON
CFG_FEATURE_MMU_SMALL                           := FEATURE_OFF
CFG_FEATURE_TTFMEM_CACHE                        := FEATURE_OFF
CFG_FEATURE_SDUPDATE                            := FEATURE_OFF
CFG_FEATURE_SECURITY_SHELL          			:= FEATURE_ON
CFG_FEATURE_EPAD                                := FEATURE_OFF
CFG_FEATURE_HSIC_SLAVE                          := FEATURE_OFF
CFG_FEATURE_MANUFACTURE_LOG                     := FEATURE_OFF
CFG_FEATURE_MEM_MONITOR                         := FEATURE_ON
CFG_FEATURE_DRV_REPLAY_DUMP						:= FEATURE_OFF
CFG_FEATURE_SKB_EXP                             := FEATURE_OFF
CFG_FEATURE_MULTI_FS_PARTITION                  := FEATURE_ON
CFG_FEATURE_MULTI_MODEM                         := FEATURE_ON
CFG_FEATURE_SOCP_CHANNEL_REDUCE                 := FEATURE_OFF
CFG_FEATURE_SOCP_DECODE_INT_TIMEOUT                 := FEATURE_OFF
CFG_FEATURE_UE_MODE_TDS                         := FEATURE_ON
CFG_FEATURE_ETWS                                := FEATURE_ON
CFG_FEATURE_AGPS                                := FEATURE_ON
CFG_FEATRUE_XML_PARSER                          := FEATURE_ON
CFG_FEATURE_PTM                                 := FEATURE_ON
CFG_CONFIG_HISI_PTM                             := FEATURE_OFF
CFG_FEATURE_COMPRESS_WRITE_LOG_FILE             := FEATURE_OFF
CFG_FEATURE_CBT_LOG                             := FEATURE_ON
CFG_FEATURE_IMS                                 := FEATURE_ON
CFG_FEATURE_SAMPLE_LTE_CHAN 			:= FEATURE_OFF
CFG_FEATURE_AT_HSUART                           := FEATURE_OFF
CFG_FEATURE_HUAWEI_VP                           := FEATURE_ON
CFG_NAS_FEATURE_SMS_NVIM_SMSEXIST                       := FEATURE_ON
CFG_NAS_FEATURE_SMS_FLASH_SMSEXIST                      := FEATURE_ON
CFG_FEATURE_RMNET_CUSTOM                                := FEATURE_OFF
CFG_GAS_PTL_VER_PRE_R99             := (PS_PTL_VER_PRE_R99)
CFG_GAS_PTL_VER_R99                 := (PS_PTL_VER_R99)
CFG_GAS_PTL_VER_R3                  := (PS_PTL_VER_R3)
CFG_GAS_PTL_VER_R4                  := (PS_PTL_VER_R4)
CFG_GAS_PTL_VER_R5                  := (PS_PTL_VER_R5)
CFG_GAS_PTL_VER_R6                  := (PS_PTL_VER_R6)
CFG_GAS_PTL_VER_R7                  := (PS_PTL_VER_R7)
CFG_GAS_PTL_VER_R8                  := (PS_PTL_VER_R8)
CFG_GAS_PTL_VER_R9                  := (PS_PTL_VER_R9)
CFG_GAS_UE_REL_VER                  := (GAS_PTL_VER_R9)
CFG_FEATURE_BBP_MASTER                          := FEATURE_ON
CFG_FEATURE_BBPMST_FREQUENCY_REDUCE             := FEATURE_OFF
CFG_TTF_CF_FEATURE                              := FEATURE_OFF
CFG_FEATURE_TTF_MEM_DEBUG                       := FEATURE_ON
CFG_FEATURE_TTF_RLC_PIGGY_BACKED                := FEATURE_OFF
CFG_FEATURE_W_R99_DL_DUAL_MAILBOX               := FEATURE_ON
CFG_FEATURE_ASSEMBLY_MEM_CPY                    := FEATURE_ON
CFG_FEATURE_HSPA_PERF_IMP                       := FEATURE_ON
CFG_FEATURE_RACH_NO_ACK_DEBUG                   := FEATURE_OFF
CFG_FEATURE_RLC_REASSEMBLE                      := FEATURE_ON
CFG_FEATURE_TCP_ACK_IN_FRONT                    := FEATURE_ON
CFG_FEATURE_CST_ASYN_OR_NOT_TRANS               := FEATURE_OFF
CFG_FEATURE_CIPHER_MASTER                       := FEATURE_ON
CFG_FEATURE_HDLC_ENHANCE                        := FEATURE_ON
CFG_FEATURE_ACPU_STAT                           := FEATURE_OFF
CFG_FEATURE_ACPU_FC_POINT_REG                   := FEATURE_OFF
CFG_SC_CTRL_MOD_6620_FPGA                       := 1
CFG_SC_CTRL_MOD_6620_SFT                        := 2
CFG_SC_CTRL_MOD_6758_FPGA                       := 3
CFG_SC_CTRL_MOD_6758_SFT                        := 4
CFG_SC_CTRL_MOD_6930_FPGA                       := 5
CFG_SC_CTRL_MOD_6930_SFT                        := 6
CFG_SC_CTRL_MOD_3630_UDP                        := 7
CFG_SC_CTRL_MOD                                 :=(SC_CTRL_MOD_3630_UDP)
CFG_FEATURE_EDMAC                               := FEATURE_OFF
CFG_ZSP_DSP_CHIP_BB_TYPE		        :=8
CFG_ZSP_DSP_PRODUCT_FORM		        :=4
CFG_BOARD := ASIC
CFG_FEATURE_DC_DPA			        := FEATURE_ON
CFG_FEATURE_DC_UPA                              := FEATURE_OFF
CFG_FEATURE_DC_MIMO                             := FEATURE_OFF
CFG_FEATURE_HARQ_OUT                            := FEATURE_OFF
CFG_FEATURE_RFIC_RESET_GPIO_ON			:= FEATURE_ON
CFG_FEATURE_EXTERNAL_32K_CLK			:= FEATURE_ON
CFG_FEATURE_UPHY_SIO_WPLL_ELUSION_ON		:= FEATURE_OFF
CFG_FEATURE_MULTI_RFIC_MEAS			:= FEATURE_ON
CFG_FEATURE_SRAM_400K                           := FEATURE_OFF
CFG_FEATURE_TEMP_DSP_CORE_POWER_DOWN            := FEATURE_OFF
CFG_FEATURE_TEMP_MULTI_MODE_LP                  := FEATURE_ON
CFG_FEATURE_TUNER				:= FEATURE_ON
CFG_FEATURE_POWER_DRV                           := FEATURE_OFF
CFG_FEATURE_SIM_NOTIFY                          := FEATURE_ON
CFG_FEATURE_HW_CODEC                            := FEATURE_ON
CFG_FEATURE_ANT_SHARE				:= FEATURE_ON
CFG_FEATURE_DSDS                                := FEATURE_OFF
CFG_FEATURE_VIRTUAL_BAND			:= FEATURE_ON
CFG_FEATURE_VSIM                                := FEATURE_ON
CFG_FEATURE_SECURITY_PHONE                      := FEATURE_OFF
CFG_FEATURE_VOICE_UP                            := FEATURE_OFF
CFG_FEATURE_SOCP_ON_DEMAND			:= FEATURE_ON
CFG_FEATURE_MULTI_CHANNEL			:= FEATURE_OFF
CFG_FEATURE_AT_HSIC                             := FEATURE_OFF
CFG_FEATURE_POWER_TIMER                         := FEATURE_ON
CFG_FEATURE_CL_INTERWORK                        := FEATURE_ON
CFG_FEATURE_BSP_LCH_OM                          := FEATURE_ON
CFG_FEATURE_HISOCKET				:=FEATURE_OFF
CFG_FEATURE_DFS_SYNC                            :=FEATURE_OFF
CFG_FEATURE_UE_MODE_CDMA						:=FEATURE_OFF
CFG_FEATURE_TAS                     := FEATURE_ON
CFG_FEATURE_BASTET                              := FEATURE_ON
