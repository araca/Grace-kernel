

#ifndef __CDS_BST_PROC_H__
#define __CDS_BST_PROC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif
#if (CDS_FEATURE_ON == CDS_FEATURE_BASTET)
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define CDS_BST_HOOK_LEVEL_NONE         ( 0x00U )
#define CDS_BST_HOOK_LEVEL_HEAD         ( 0x01U )
#define CDS_BST_HOOK_LEVEL_FULL         ( 0x02U )
#define CDS_BST_HOOK_LEVEL_FLAGS        ( CDS_BST_HOOK_LEVEL_HEAD )

#define CDS_BST_HOOK_TYPE_NO            ( 0x00U )
#define CDS_BST_HOOK_TYPE_UL            ( 0x01U )
#define CDS_BST_HOOK_TYPE_DL            ( 0x02U )
#define CDS_BST_HOOK_TYPE_FW            ( 0x04U )
/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
  4 枚举定义
*****************************************************************************/

/*****************************************************************************
   5 STRUCT定义
*****************************************************************************/

/*****************************************************************************
 结构名    : CDS_BST_IP_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CDS_BST_IP_INFO_STRU结构
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usProtocol;                 /*协议号*/
    VOS_UINT16                          usDstPort;                  /*目的端口号*/
}CDS_BST_IP_INFO_STRU;

/*****************************************************************************
 结构名    : CDS_BST_ENTITY_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CDS_BST_ENTITY_STRU结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucNvFlag;                   /*NV开关状态*/
    VOS_UINT8                           ucHookFlag;                 /*钩包开关配置*/
    VOS_UINT8                           ucRabId;                    /*默认承载RAB*/
    VOS_UINT8                           ucRptEnable;                /*是否允许上报数据状态*/
    VOS_UINT16                          usNetState;                 /*当前ACORE网络状态*/
    VOS_UINT16                          usModemId;                  /*当前CDS所用MODEM-ID*/
    CDS_BST_RECV_DL_DATA_FUNC           pfRcvDlPacket;              /*BASTET接收包回调函数*/
    CDS_BST_PORT_CHECK_FUNC             pfPortCheck;                /*BASTET校验端口值回调*/
}CDS_BST_ENTITY_STRU;

/*****************************************************************************
  6 UNION定义
*****************************************************************************/

/*****************************************************************************
  7 全局变量声明
*****************************************************************************/

/*****************************************************************************
  8 函数声明
*****************************************************************************/
extern VOS_UINT32 CDS_IsBastetPacket    ( TTF_MEM_ST   *pstIpPkt );
extern VOS_UINT32 CDS_UlProcBastetData  ( VOS_VOID );
extern VOS_VOID   CDS_DlProcBastetData  ( TTF_MEM_ST   *pstSrc,
                                          VOS_UINT8     ucDeftRabId,
                                          VOS_UINT16    usModemId,
                                          VOS_UINT32    ulSduLen );

extern VOS_VOID   CDS_BstSendTxRxInd    ( VOS_VOID );
extern VOS_VOID   CDS_BST_MsgProc       ( MsgBlock *pstMsg );
extern PS_BOOL_ENUM_UINT8   CDS_GetBastetSupportFlag(VOS_VOID);
extern VOS_VOID             CDS_BstInit(VOS_VOID);

/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
#endif

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

