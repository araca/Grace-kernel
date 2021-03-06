

/*****************************************************************************
   1 头文件包含
*****************************************************************************/
#include "Rabm2GInclude.h"
#include "NasRabmMain.h"
#include "NasRabmMsgProc.h"
#include "NVIM_Interface.h"
#include "NasUtranCtrlInterface.h"
#include "WasNvInterface.h"
#include "NasRabmFastdorm.h"


#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*lint -e958*/

/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RABM_ASSISTANT_C

/*****************************************************************************
   2 全局变量定义
*****************************************************************************/
/*2G中RABM实体的结构体数组*/
RABM_2G_ENTITY_ST                                           gastRabm2GEntity[RABM_2G_MAX_ENT_NUM];

/*用以记录3G中发生3G到2G切换时，在发生切换前所处的状态*/
VOS_UCHAR                                                   gaucRabm3GPreState[RABM_3G_PS_MAX_ENT_NUM];

/*标识当前是否正在进行2G到3G的切换*/
RABM_2G_TO_3G_SWITCH_ENUM                                   gRabm2GTo3GSwitch;

/*标识当前是否正在进行3G到2G的切换*/
RABM_3G_TO_2G_SWITCH_ENUM                                   gRabm3GTo2GSwitch;

/*标识2G中路由区更新定时器是否启动*/
RABM_RA_T1_SWITCH_ENUM_UINT8                                gRabmRAT1Switch;

/*标识Rabm.Act.Req.T1定时器是否启动*/
RABM_ACT_REQ_T1_SWITCH_ENUM_UINT8                           gaRabmActReqT1Switch[RABM_2G_MAX_ENT_NUM];

/*标识Rabm.2To3.T1定时器是否启动*/
RABM_2G_TO_3G_T1_SWITCH_ENUM                                gRabm2GTo3GT1Switch;

/*标识Rabm.3To2.T1定时器是否启动*/
RABM_3G_TO_2G_T1_SWITCH_ENUM                                gRabm3GTo2GT1Switch;

/*标识Rabm.3To2.T3定时器是否启动*/
RABM_3G_TO_2G_T3_SWITCH_ENUM_UINT8                          gRabm3GTo2GT3Switch;

/*标识Rabm.3To2.RA.Update定时器是否启动*/
RABM_3G_TO_2G_RA_UPDATE_SWITCH_ENUM                         gRabm3GTo2GRAUpdateSwitch;

/*TLLI的全局变量*/
VOS_UINT32                                                  gulRabmTLLI;

/*定时器时长数组(以定时器的PARA枚举作为索引)*/
VOS_UINT32                                                  gaulRabmTimerLength[RABM_TIMER_PARA_BUTT];

/*实体定时器(以定时器的NAME枚举作为索引)*/
HTIMER                                                      gastRabmEntityTimer[RABM_MAX_ENT_NUM];

/*公用定时器(以定时器的PARA枚举作为索引)*/
HTIMER                                                      gastRabmCommonTimer[RABM_COMMON_TIMER_NUMBER];

/*标识第一次获取网络是否完成*/
RABM_GET_CUR_NET_FIRSTTIME_ENUM                             gRabmGetCurNetFirsttime;

/*标识当前所处的网络模式*/
RABM_CUR_NET_ENUM                                           gRabmCurNet;

/*2G向3G切换后，数传恢复的标志*/
VOS_UINT32                                                  gRabm2GTo3GDataResume;

/*3G向2G切换时，RABM保存NSAPI和RB的映射关系*/
SNDCP_NSAPI_MAP_RB_INFO                                     gastNsapiMapRb[SNDCP_RABM_MAX_NSAPI_NUM];

NAS_RABM_TIMER_STATUS_ENUM_UINT8                           g_ulNasRabmFDFluxDetectTimer;
NAS_RABM_TIMER_STATUS_ENUM_UINT8                           g_ulNasRabmFDRetryTimer;
NAS_RABM_TIMER_STATUS_ENUM_UINT8                           g_ulNasRabmFDWaitGmmQryRsltTimer;
NAS_RABM_TIMER_STATUS_ENUM_UINT8                           g_ulNasRabmResendEstTimer;

NAS_RABM_TIMER_STATUS_ENUM_UINT8                           g_ulNasRabmVPDelayTimer;
NAS_RABM_TIMER_STATUS_ENUM_UINT8                           g_ulNasRabmVPReQryGmmDelayTimer;

/*RABM从PDCP处获知的RCV NPDU NUM 信息*/
RABM_PDCP_GET_N_PDU_NUM_LIST_ST                             gPdcpRcvNpduNum;

#if (FEATURE_ON == FEATURE_LTE)
/* L->G标记, 用于标记L->G的流程 */
VOS_UINT32                              g_ulNasRabmL2GFlg = VOS_FALSE;
#endif

RAB_AS_UL_DATA_CALLBACK_STRU g_astRabAsUlDataCallBack[] =
{
  /*====ucMode===,  =========ucState=========,  usRsv   =====pAsUlDataCallBack=====*/

    {RABM_3G_ENUM,  RABM_NULL,                  0,      VOS_NULL_PTR                    },
    {RABM_3G_ENUM,  RABM_NSAPI_ACTIVE_PENDING,  0,      RABM_SndRABMUlDataCallBack},
    {RABM_3G_ENUM,  RABM_NSAPI_ACTIVE_NO_RAB,   0,      RABM_SndRABMUlDataCallBack},
    {RABM_3G_ENUM,  RABM_NSAPI_ACTIVE_WITH_RAB, 0,      RABM_SndPdcpUlDataCallBack     },
    {RABM_3G_ENUM,  RABM_DATA_TRANSFER_STOP,    0,      RABM_SndRABMUlDataCallBack},
    {RABM_3G_ENUM,  RABM_ACT_PENDING_WITH_RAB,  0,      RABM_SndRABMUlDataCallBack},
    {RABM_3G_ENUM,  RABM_TC_STATE_NULL,         0,      VOS_NULL_PTR                    },
    {RABM_3G_ENUM,  RABM_TC_STATE_WITH_RAB,     0,      VOS_NULL_PTR                    },
    {RABM_2G_ENUM,  RABM_2G_NULL,               0,      VOS_NULL_PTR                    },
    {RABM_2G_ENUM,  RABM_NSAPI_OK_TRANSMODE_NO, 0,      RABM_SndRABMUlDataCallBack },
    {RABM_2G_ENUM,  RABM_DATA_TRANS_READY,      0,      RABM_SndSndcpUlDataCallBack    },
    {RABM_2G_ENUM,  RABM_ROUT_AREA_UPDATE,      0,      RABM_SndRABMUlDataCallBack },
    {RABM_2G_ENUM,  RABM_DATA_TRANS_STOP,       0,      RABM_SndRABMUlDataCallBack }
};

/* ======================== */    /* RAB映射实体，以RAB ID为索引下标，有效区间[5,15] */
RAB_MAP_STRU    g_astRabMap[PS_WUE_MAX_RB_ID + 1];


/* ======================== */    /* RB ID到RAB ID的快速索引表,提高效率，有效范围[5,32] */
VOS_UINT8       g_aucRb2Rab[PS_WUE_MAX_RB_ID + 1];


/*****************************************************************************
   3 函数实现
*****************************************************************************/


VOS_VOID  NAS_Rabm2GInit( VOS_VOID )
{
    VOS_UINT8                   ucLoop;
    NVIM_FASTDORM_PARA_STRU     stFastDormPara;
    VOS_UINT32                  ulRslt;


    PS_MEM_SET(&stFastDormPara, 0x00, sizeof(stFastDormPara));


    /*打印流程信息---2G中RABM模块的初始化:*/
    RABM_LOG_NORMAL( "NAS_Rabm2GInit:NORMAL:Start to Initial RABM module in 2G mode" );

    /*初始化定时器时长数组(临时定义)*/
    gaulRabmTimerLength[RABM_TIMER_PARA_ROUT_AREA_UPDATE_T1] = 30000;

    gaulRabmTimerLength[RABM_TIMER_PARA_3_TO_2_T3]           = 30000;
    gaulRabmTimerLength[RABM_TIMER_PARA_ACT_REQ_T1]          = 30000;

    gaulRabmTimerLength[RABM_TIMER_FASTDORM_FLUX_DETECT]            = 1000;
    gaulRabmTimerLength[RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT]    = 2000;

    gaulRabmTimerLength[RABM_TIMER_FASTDORM_RETRY]                  = 2000;

    gaulRabmTimerLength[RABM_TIMER_RESEND_EST_REQ]                  = 500;

    gaulRabmTimerLength[RABM_TIMER_VOICEPREFER_DELAY] = 4000;
    gaulRabmTimerLength[RABM_TIMER_VOICEPREFER_REQRY_GMM_DELAY] = 4000;

    /* 从NVIM中读取W下FASTDORM操作相关参数 */
    ulRslt = NV_Read(en_NV_Item_Fastdorm_Para,
                     &stFastDormPara,
                     sizeof(NVIM_FASTDORM_PARA_STRU));
    /* 如果读取失败，则设置各种参数为默认值，返回失败 */
    if (NV_OK != ulRslt)
    {
        RABM_LOG1_WARNING( "NAS_Rabm2GInit:Read NVIM Failed,ulRslt:", (VOS_INT32)ulRslt);
    }
    else
    {
        gaulRabmTimerLength[RABM_TIMER_FASTDORM_RETRY] = stFastDormPara.ucNasRetryInterval * 1000;
    }

    for ( ucLoop = 0; ucLoop < RABM_MAX_ENT_NUM; ucLoop++ )
    {
        gastRabmEntityTimer[ucLoop] = VOS_NULL_PTR;
    }
    for ( ucLoop = 0; ucLoop < RABM_COMMON_TIMER_NUMBER; ucLoop++ )
    {
        gastRabmCommonTimer[ucLoop] = VOS_NULL_PTR;
    }

    for ( ucLoop = 0; ucLoop < RABM_2G_MAX_ENT_NUM; ucLoop++ )
    {
        /*初始化RABM状态机:*/
        RABM_SetGState((RABM_NSAPI_ENUM)ucLoop,RABM_2G_NULL);
        gastRabm2GEntity[ucLoop].ucNsapi           = ( VOS_UINT8 )( ucLoop + RABM_2G_NSAPI_OFFSET );
        gastRabm2GEntity[ucLoop].TransMode         = RABM_ADM_ENUM;
        gastRabm2GEntity[ucLoop].stQos.ulQosLength = 0;
        gastRabm2GEntity[ucLoop].RAUpdateCause     = RABM_RA_UPDATE_CAUSE_BUTT;

        /*初始化Rabm.Act.Req.T1定时器标志:*/
        gaRabmActReqT1Switch[ucLoop] = RABM_ACT_REQ_T1_OFF;

        PS_MEM_SET(gastRabm2GEntity[ucLoop].stQos.aucQosValue,
                   0x00,
                   sizeof(gastRabm2GEntity[ucLoop].stQos.aucQosValue));

    }

    /*初始化2G中所使用的其它定时器标志:*/
    gRabmRAT1Switch           = RABM_ROUTING_AREA_UPDATE_T1_OFF;
    gRabm2GTo3GT1Switch       = RABM_2G_TO_3G_T1_OFF;
    gRabm3GTo2GT3Switch       = RABM_3G_TO_2G_T3_OFF;
    gRabm3GTo2GRAUpdateSwitch = RABM_3G_TO_2G_RA_UPDATE_OFF;

    /*初始化2G到3G的切换标识:*/
    gRabm2GTo3GSwitch          = RABM_2G_TO_3G_SWITCH_OFF;

    /*初始化第一次获取网络是否完成的标识:*/
    gRabmGetCurNetFirsttime = RABM_GET_CUR_NET_FIRSTTIME_OFF;

    /*初始化NSAPI和RB的映射关系:*/
    PS_MEM_SET( (VOS_VOID *)gastNsapiMapRb, 0, sizeof(SNDCP_NSAPI_MAP_RB_INFO) * SNDCP_RABM_MAX_NSAPI_NUM );

    /*初始化RCV NPDU NUM 信息:*/
    PS_MEM_SET( (VOS_VOID *)(&gPdcpRcvNpduNum), 0, sizeof(RABM_PDCP_GET_N_PDU_NUM_LIST_ST) );

    /*打印流程信息---2G中RABM模块的初始化成功:*/
    RABM_LOG_NORMAL( "NAS_Rabm2GInit:NORMAL:Initial RABM module in 2G mode SUCCESS" );

    /*初始化RAB_MAP所使用的相关信息:*/
    RABM_RabMapInit();

    return;
}
VOS_UINT32  NAS_RabmStartTimer( RABM_TIMER_NAME_ENUM Name, RABM_TIMER_PARA_ENUM Para )
{
    RABM_START_TIMER_ST                 *pstRabmStartTimer;
    RABM_START_TIMER_ST                  stRabmStartTimer;
    RABM_TIMER_NAME_ENUM                 EntityTimerName = ( RABM_TIMER_NAME_ENUM )0;

    pstRabmStartTimer = &stRabmStartTimer;

    if ((RABM_TIMER_NAME_COMMON == Name)
     || (RABM_TIMER_NAME_FD_RETRY == Name)
     || (RABM_TIMER_NAME_FD_FLUX_DETECT == Name))
    {
        pstRabmStartTimer->pstHTimer  = &gastRabmCommonTimer[Para];
        if ( RABM_TIMER_FASTDORM_FLUX_DETECT == Para )
        {
            pstRabmStartTimer->ucMode = VOS_RELTIMER_LOOP;
        }
        else
        {
            pstRabmStartTimer->ucMode = VOS_RELTIMER_NOLOOP;
        }
    }
    else
    {
        EntityTimerName               = Name;
        pstRabmStartTimer->pstHTimer  = &gastRabmEntityTimer[Name];
        pstRabmStartTimer->ucMode     = VOS_RELTIMER_NOLOOP;
    }

    /*设置定时器参数:*/
    pstRabmStartTimer->ulPid     = WUEPS_PID_RABM;
    pstRabmStartTimer->Name      = Name;
    pstRabmStartTimer->Param     = Para;
    pstRabmStartTimer->ulLength  = gaulRabmTimerLength[Para];

    /*启动指定定时器:*/
    if ( VOS_OK != NAS_StartRelTimer( pstRabmStartTimer->pstHTimer,
                                      pstRabmStartTimer->ulPid,
                                      pstRabmStartTimer->ulLength,
                                      pstRabmStartTimer->Name,
                                      pstRabmStartTimer->Param,
                                      pstRabmStartTimer->ucMode ) )
    {
        return RABM_FAILURE;
    }

    /*置位定时器启动标志:*/
    switch ( Para )
    {
        case RABM_TIMER_PARA_ROUT_AREA_UPDATE_T1:
            gRabmRAT1Switch = RABM_ROUTING_AREA_UPDATE_T1_ON;
            break;

        case RABM_TIMER_PARA_3_TO_2_T3:
            gRabm3GTo2GT3Switch = RABM_3G_TO_2G_T3_ON;
            break;

        case RABM_TIMER_PARA_ACT_REQ_T1:
            gaRabmActReqT1Switch[EntityTimerName] = RABM_ACT_REQ_T1_ON;
            break;

        case RABM_TIMER_FASTDORM_FLUX_DETECT:                                            /* 周期性流量检测定时器，超时后检查是否有流量变化 */
            g_ulNasRabmFDFluxDetectTimer = NAS_RABM_TIMER_STATUS_RUNING;
            break;

        case RABM_TIMER_FASTDORM_RETRY:                                                  /* RETRY定时器，超时后重新尝试FD */
            g_ulNasRabmFDRetryTimer = NAS_RABM_TIMER_STATUS_RUNING;
            break;

        case RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT:
            g_ulNasRabmFDWaitGmmQryRsltTimer = NAS_RABM_TIMER_STATUS_RUNING;
            break;

        case RABM_TIMER_RESEND_EST_REQ:
            g_ulNasRabmResendEstTimer = NAS_RABM_TIMER_STATUS_RUNING;

        case RABM_TIMER_VOICEPREFER_DELAY:
            g_ulNasRabmVPDelayTimer = NAS_RABM_TIMER_STATUS_RUNING;
            break;

        case RABM_TIMER_VOICEPREFER_REQRY_GMM_DELAY:
            g_ulNasRabmVPReQryGmmDelayTimer = NAS_RABM_TIMER_STATUS_RUNING;
            break;

        default:
            break;
    }
    return RABM_SUCCESS;
}


NAS_RABM_TIMER_STATUS_ENUM_UINT8  NAS_RABM_CheckTimerStatus(
    RABM_TIMER_NAME_ENUM                enEntityTimerName,
    RABM_TIMER_PARA_ENUM                enTimerPara
)
{
    switch ( enTimerPara )
    {
        case RABM_TIMER_PARA_ROUT_AREA_UPDATE_T1:
            return gRabmRAT1Switch;

        case RABM_TIMER_PARA_3_TO_2_T3:
            return gRabm3GTo2GT3Switch;

        case RABM_TIMER_PARA_ACT_REQ_T1:
            return gaRabmActReqT1Switch[enEntityTimerName];

        case RABM_TIMER_FASTDORM_FLUX_DETECT:                                            /* 周期性流量检测定时器，超时后检查是否有流量变化 */
            return g_ulNasRabmFDFluxDetectTimer;

        case RABM_TIMER_FASTDORM_RETRY:                                                  /* RETRY定时器，超时后重新尝试FD */
            return g_ulNasRabmFDRetryTimer;

        case RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT:
            return g_ulNasRabmFDWaitGmmQryRsltTimer;

        case RABM_TIMER_RESEND_EST_REQ:
            return g_ulNasRabmResendEstTimer;

        case RABM_TIMER_VOICEPREFER_DELAY:
            return g_ulNasRabmVPDelayTimer;

        case RABM_TIMER_VOICEPREFER_REQRY_GMM_DELAY:
            return g_ulNasRabmVPReQryGmmDelayTimer;


        default:
            return NAS_RABM_TIMER_STATUS_STOP;
    }
}


VOS_UINT32  NAS_RabmStopTimer( RABM_TIMER_NAME_ENUM Name, RABM_TIMER_PARA_ENUM Para )
{
    VOS_UINT32                           ulResult          = RABM_SUCCESS;
    RABM_TIMER_NAME_ENUM                 EntityTimerName   = ( RABM_TIMER_NAME_ENUM )0;
    RABM_TIMER_PARA_ENUM                 CommonTimerPara   = ( RABM_TIMER_PARA_ENUM )0;
    HTIMER                              *pstHTimer;


    if ((RABM_TIMER_NAME_COMMON == Name)
     || (RABM_TIMER_NAME_FD_RETRY == Name)
     || (RABM_TIMER_NAME_FD_FLUX_DETECT == Name))
    {
        CommonTimerPara = Para;
    }
    else
    {
        EntityTimerName = Name;
    }

    if (NAS_RABM_TIMER_STATUS_STOP == NAS_RABM_CheckTimerStatus(EntityTimerName,Para))
    {
        return RABM_SUCCESS;
    }

    /*获取所要关闭的定时器的HTIMER指针:*/

    if ((RABM_TIMER_NAME_COMMON == Name)
     || (RABM_TIMER_NAME_FD_RETRY == Name)
     || (RABM_TIMER_NAME_FD_FLUX_DETECT == Name))
    {
        pstHTimer  = &gastRabmCommonTimer[CommonTimerPara];
    }
    else
    {
        pstHTimer  = &gastRabmEntityTimer[EntityTimerName];
    }

    /*关闭指定定时器:*/
    if ( VOS_OK != NAS_StopRelTimer(WUEPS_PID_RABM, Para, pstHTimer ) )
    {
        ulResult = RABM_FAILURE;
    }

    /*复位定时器启动标志:*/
    switch ( Para )
    {
        case RABM_TIMER_PARA_ROUT_AREA_UPDATE_T1:
            gRabmRAT1Switch = RABM_ROUTING_AREA_UPDATE_T1_OFF;
            break;
        case RABM_TIMER_PARA_3_TO_2_T3:
            gRabm3GTo2GT3Switch = RABM_3G_TO_2G_T3_OFF;
            break;
        case RABM_TIMER_PARA_ACT_REQ_T1:
            gaRabmActReqT1Switch[EntityTimerName] = RABM_ACT_REQ_T1_OFF;
            break;

        case RABM_TIMER_FASTDORM_FLUX_DETECT:                                            /* 周期性流量检测定时器，超时后检查是否有流量变化 */
            g_ulNasRabmFDFluxDetectTimer = NAS_RABM_TIMER_STATUS_STOP;
            break;

        case RABM_TIMER_FASTDORM_RETRY:                                                  /* RETRY定时器，超时后重新尝试FD */
            g_ulNasRabmFDRetryTimer = NAS_RABM_TIMER_STATUS_STOP;
            break;

        case RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT:
            g_ulNasRabmFDWaitGmmQryRsltTimer = NAS_RABM_TIMER_STATUS_STOP;
            break;

        case RABM_TIMER_RESEND_EST_REQ:
            g_ulNasRabmResendEstTimer = NAS_RABM_TIMER_STATUS_STOP;

        case RABM_TIMER_VOICEPREFER_DELAY:
            g_ulNasRabmVPDelayTimer = NAS_RABM_TIMER_STATUS_STOP;
            break;

        case RABM_TIMER_VOICEPREFER_REQRY_GMM_DELAY:
            g_ulNasRabmVPReQryGmmDelayTimer = NAS_RABM_TIMER_STATUS_STOP;
            break;


        default:
            break;
    }
    return ulResult;
}
VOS_UINT32  NAS_RabmQueryTransmodeNoState( VOS_VOID )
{
    VOS_UINT8       ucLoop;

    /*遍历所有NSAPI:*/
    for ( ucLoop = 0; ucLoop < RABM_2G_MAX_ENT_NUM; ucLoop++ )
    {
        if ( RABM_NSAPI_OK_TRANSMODE_NO == gastRabm2GEntity[ucLoop].State )   /*存在RABM_NSAPI_OK_TRANSMODE_NO状态*/
        {
            return RABM_YES;
        }
    }
    return RABM_NO;
}


VOS_VOID  NAS_RabmDecodeNPdu( GMMRABM_ROUTING_AREA_UPDATE_IND_MSG *pRAUpdateIndMsg,
                                          RABM_SNDCP_RECV_N_PDU_NUM_LIST_ST   *pstNPdu )
{
    VOS_UINT8       ucLoop,ucLeft,ucRight,ucNumber,ucNsapi,ucNpduNum;

    /*初始化pstNPdu指向的结构体:*/
    for ( ucLoop = 0; ucLoop <= RABM_MAX_NSAPI_NUMBER; ucLoop++ )
    {
        pstNPdu->aucNPDUNumList[ucLoop][0] = 0;
    }

    /*获得N-PDU的个数:*/
    ucNumber = ( VOS_UINT8 )( ( ( VOS_UINT16 )pRAUpdateIndMsg->aucRecvNPDUNumListIE[1] * ( VOS_UINT16 )8 ) / ( VOS_UINT16 )12 );

    /*遍历N-PDU:*/
    for ( ucLoop = 1; ucLoop <= ucNumber; ucLoop++ )
    {
        if ( 1 == (ucLoop % 2) )
        {
            /*得到N-PDU value在数组中的索引(序号为奇数):*/
            ucLeft  = ( VOS_UINT8 )( (( ( ucLoop - 1 ) * 12 ) / 8) + RABM_N_PDU_OFFSET );
            ucRight = ucLeft + 1;
            if (ucRight >= RABM_RECV_N_PDU_NUM_LIST_IE_MAXLEN)
            {
                continue;
            }

            /*获取NSAPI号:*/
            ucNsapi = ( pRAUpdateIndMsg->aucRecvNPDUNumListIE[ucLeft] >> 4 ) & 0x0f;

            /*获取NPDU Number:*/
            ucNpduNum =( ( pRAUpdateIndMsg->aucRecvNPDUNumListIE[ucLeft]  << 4 ) & 0xf0 )
                        | ( ( pRAUpdateIndMsg->aucRecvNPDUNumListIE[ucRight] >> 4 ) & 0x0f );

            if ( RABM_2G_NULL == gastRabm2GEntity[ucNsapi - RABM_2G_NSAPI_OFFSET].State )
            {
                /*打印警告信息---该NSAPI未激活:*/
                RABM_LOG1_WARNING( "NAS_RabmDecodeNPdu:WARNING:This NSAPI hasn't been activated but also receive a N-PDU number!: NSAPI:", ucNsapi );
            }

            /*填写pstNPdu所指向的结构体:*/
            pstNPdu->aucNPDUNumList[ucNsapi][0] = 1;
            pstNPdu->aucNPDUNumList[ucNsapi][1] = ucNpduNum;
        }
        else
        {
            /*得到N-PDU value在数组中的索引(序号为偶数):*/
            ucLeft  = ( VOS_UINT8 )( ((( ucLoop * 12 ) / 8) - 2) + RABM_N_PDU_OFFSET );
            ucRight = ucLeft + 1;
            if (ucRight >= RABM_RECV_N_PDU_NUM_LIST_IE_MAXLEN)
            {
                continue;
            }

            /*获取NSAPI号:*/
            ucNsapi = pRAUpdateIndMsg->aucRecvNPDUNumListIE[ucLeft] & 0x0f;

            /*获取NPDU Number:*/
            ucNpduNum = pRAUpdateIndMsg->aucRecvNPDUNumListIE[ucRight];

            if ( RABM_2G_NULL == gastRabm2GEntity[ucNsapi - RABM_2G_NSAPI_OFFSET].State )
            {
                /*打印警告信息---该NSAPI未激活:*/
                RABM_LOG1_WARNING( "NAS_RabmDecodeNPdu:WARNING:This NSAPI hasn't been activated but also receive a N-PDU number!: NSAPI:", ucNsapi );
            }

            /*填写pstNPdu所指向的结构体:*/
            pstNPdu->aucNPDUNumList[ucNsapi][0] = 1;
            pstNPdu->aucNPDUNumList[ucNsapi][1] = ucNpduNum;
        }
    }
}
VOS_VOID    NAS_RabmEncodeNPdu( GMMRABM_ROUTING_AREA_UPDATE_RSP_MSG *PRAUpdateRspMsg,
                                            RABM_SNDCP_RECV_N_PDU_NUM_LIST_ST   *pstNPdu )
{
    VOS_UINT8       ucLoop,ucNumber,ucLeft,ucRight;

    /*填写N-PDU码流的IEI:*/
    PRAUpdateRspMsg->aucRecvNPDUNumListIE[0] = 0x26;

    /*遍历各NSAPI对应的N-PDU number:*/
    for ( ucNumber = 0, (ucLoop = 5); ucLoop <= RABM_MAX_NSAPI_NUMBER; ucLoop++ )
    {
        if( 1 == pstNPdu->aucNPDUNumList[ucLoop][0] )
        {
            ucNumber++;

            if ( 1 == (ucNumber % 2) )      /*第奇数个N-PDU Number*/
            {
                /*得到N-PDU value在数组中的索引(序号为奇数):*/
                ucLeft  = ( VOS_UINT8 )( (( ( ucNumber - 1 ) * 12 ) / 8) + RABM_N_PDU_OFFSET );
                ucRight = ucLeft + 1;

                /*填入NSAPI:*/
                PRAUpdateRspMsg->aucRecvNPDUNumListIE[ucLeft]  = ( ucLoop << 4 ) &0xf0;

                /*填入N-PDU号高4位:*/
                PRAUpdateRspMsg->aucRecvNPDUNumListIE[ucLeft] |= ( pstNPdu->aucNPDUNumList[ucLoop][1] >> 4 ) & 0x0f;
                /*填入N-PDU号低4位:*/
                PRAUpdateRspMsg->aucRecvNPDUNumListIE[ucRight] = ( pstNPdu->aucNPDUNumList[ucLoop][1] << 4 ) & 0xf0;
            }
            else                          /*第偶数个N-PDU Number*/
            {
                /*得到N-PDU value在数组中的索引(序号为偶数):*/
                ucLeft  = ( VOS_UINT8 )( ((( ucNumber * 12 ) / 8) - 2) + RABM_N_PDU_OFFSET );
                ucRight = ucLeft + 1;

                /*填入NSAPI:*/
                PRAUpdateRspMsg->aucRecvNPDUNumListIE[ucLeft] |= ucLoop;

                /*填入N-PDU号:*/
                PRAUpdateRspMsg->aucRecvNPDUNumListIE[ucRight] = pstNPdu->aucNPDUNumList[ucLoop][1];
            }
        }
        else
        {
            if ( 0 != pstNPdu->aucNPDUNumList[ucLoop][0] )
            {
                /*打印出错信息---NSAPI是否激活的标志位出错:*/
                RABM_LOG1_WARNING( "NAS_RabmEncodeNPdu:WARNING:This NSAPI's flag(indicate the NSAPI has received N-PDU Number or not)is ERROR!: NSAPI:", ucLoop );
            }
        }
    }

    /*填写N-PDU码流的Length字段:*/
    PRAUpdateRspMsg->aucRecvNPDUNumListIE[1] =( VOS_UINT8 )( ( (ucNumber * 12) + 4 ) / 8 );

}
VOS_UINT32    NAS_RabmQueryRAUState( VOS_UINT8 ucNsapiIndex )
{
    VOS_UINT8 ucLoop;

    for ( ucLoop = 0; ucLoop < RABM_2G_MAX_ENT_NUM; ucLoop++ )
    {
        if ( ucLoop == ucNsapiIndex )
        {
            continue;
        }

        if ( RABM_ROUT_AREA_UPDATE == gastRabm2GEntity[ucLoop].State )
        {
            return RABM_YES;
        }
    }

    return RABM_NO;
}



VOS_UINT32    NAS_RabmQueryNoOrReadyState( VOS_UINT8 ucNsapiIndex )
{
    VOS_UINT8 ucLoop;

    for ( ucLoop = 0; ucLoop < RABM_2G_MAX_ENT_NUM; ucLoop++ )
    {
        if ( ucLoop == ucNsapiIndex )
        {
            continue;
        }

        if ( ( RABM_NSAPI_OK_TRANSMODE_NO == gastRabm2GEntity[ucLoop].State )
             || ( RABM_DATA_TRANS_READY == gastRabm2GEntity[ucLoop].State ) )
        {
            return RABM_YES;
        }
    }

    return RABM_NO;
}


VOS_VOID    NAS_RabmDeactive3GPdp(VOS_UINT8 ucNsapiIndex)
{
    RABM_APS_LOCAL_PDP_DEACT_REQ    *pMsg;
    VOS_UINT32                       ulLength;

    if ( ucNsapiIndex >= RABM_MAX_ENT_NUM )
    {
        /*打印警告信息---该NSAPI未激活:*/
        RABM_LOG_WARNING( "NAS_RabmDeactive3GPdp:WARNING:This NSAPI is beyond range!" );
        return;
    }

    /*清空QoS:*/
    g_aRabmPsEnt[ucNsapiIndex].QoS.ulQosLength = 0;
    PS_MEM_SET(g_aRabmPsEnt[ucNsapiIndex].QoS.aucQosValue,
               0x00,
               sizeof(g_aRabmPsEnt[ucNsapiIndex].QoS.aucQosValue));

    g_aRabmPsEnt[ucNsapiIndex].RabInfo.ucRbNum = 0;                /*初始化RB的个数为0*/
    g_aRabmPsEnt[ucNsapiIndex].ucPppFlg        = RABM_SM_IP_PROT;  /*上层协议类型初始化为IP*/

    RABM_RelSaveData(&g_aRabmPsEnt[ucNsapiIndex].DlDataSave, RABM_DOWNLINK);       /*释放存储的下行数据*/
    RABM_RelSaveData(&g_aRabmPsEnt[ucNsapiIndex].UlDataSave, RABM_UPLINK);         /*释放存储的上行数据*/


    /*发送RABM_APS_LOCAL_PDP_DEACT_REQ消息:*/
    ulLength = sizeof( RABM_APS_LOCAL_PDP_DEACT_REQ ) - VOS_MSG_HEAD_LENGTH;
    pMsg     = ( RABM_APS_LOCAL_PDP_DEACT_REQ * )PS_ALLOC_MSG( WUEPS_PID_RABM, ulLength );
    if ( VOS_NULL_PTR == pMsg )
    {
        /*打印出错信息---申请消息包失败:*/
        RABM_LOG_ERROR( "NAS_RabmDeactive3GPdp:ERROR:Allocates a message packet for RABM_APS_LOCAL_PDP_DEACT_REQ msg FAIL!" );
        return;
    }
    /*填写消息头:*/
    pMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pMsg->ulSenderPid     = WUEPS_PID_RABM;
    pMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pMsg->ulReceiverPid   = WUEPS_PID_TAF;
    pMsg->ulLength        = ulLength;
    /*填写消息体:*/
    pMsg->ulMsgType       = RABM_APS_LOCAL_PDP_DEACT_REQ_MSG;
    pMsg->ucNsapi         = ucNsapiIndex + RABM_NSAPI_OFFSET;
    /*发送该消息:*/
    if ( VOS_OK != PS_SEND_MSG( WUEPS_PID_RABM, pMsg ) )
    {
        /*打印警告信息---发送消息失败:*/
        RABM_LOG_WARNING( "NAS_RabmDeactive3GPdp:WARNING:SEND RABM_APS_LOCAL_PDP_DEACT_REQ msg FAIL!" );
    }
    else
    {
        /*打印流程信息---发送了消息:*/
        RABM_LOG_NORMAL( "NAS_RabmDeactive3GPdp:NORMAL:SEND RABM_APS_LOCAL_PDP_DEACT_REQ Msg" );
    }

    return;
}
VOS_VOID  NAS_RabmSndHandoverRabRsp(VOS_UINT8  ucRabNum, RRRABM_HANDOVER_RAB_IND_STRU *pMsg)
{
    RRRABM_HANDOVER_RAB_RSP_STRU   *pSndMsg;
    VOS_UINT32                      ulLength;
    VOS_UINT8                       i;

    ulLength = sizeof(RRRABM_HANDOVER_RAB_RSP_STRU) - VOS_MSG_HEAD_LENGTH;
    pSndMsg  = (RRRABM_HANDOVER_RAB_RSP_STRU *)PS_ALLOC_MSG(WUEPS_PID_RABM, ulLength);
    if ( VOS_NULL_PTR == pSndMsg )
    {
        /*打印出错信息---申请消息包失败:*/
        RABM_LOG_ERROR( "NAS_RabmSndHandoverRabRsp:ERROR:Allocates a message packet for RRRABM_HANDOVER_RAB_IND_STRU msg FAIL!" );
        return;
    }

    RABM_Memset((VOS_VOID *)pSndMsg,0,sizeof(RRRABM_HANDOVER_RAB_RSP_STRU));

    /*填写消息头:*/
    pSndMsg->MsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pSndMsg->MsgHeader.ulSenderPid     = WUEPS_PID_RABM;
    pSndMsg->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pSndMsg->MsgHeader.ulReceiverPid   = WUEPS_PID_WRR;
    pSndMsg->MsgHeader.ulLength        = ulLength;
    pSndMsg->MsgHeader.ulMsgName       = RRRABM_HANDOVER_RAB_RSP;

    /*填写消息体:*/
    pSndMsg->ulOpId     = pMsg->ulOpId;
    pSndMsg->ulRabCnt   = (VOS_UINT32)ucRabNum;

    for ( i = 0; i <pSndMsg->ulRabCnt; i++ )
    {
        pSndMsg->aRabInfo[i].enCnDomainId     = pMsg->aRabInfo[i].ulCnDomainId;
        pSndMsg->aRabInfo[i].ulRabId          = pMsg->aRabInfo[i].ulRabId;
        pSndMsg->aRabInfo[i].ulPppValid       = WUEPS_TRUE;
        pSndMsg->aRabInfo[i].ulDeliveryErrSdu = RABM_ERROR_SDU_DELIVERY_NO_DET;
    }

    /*发送消息:*/
    if ( VOS_OK != NAS_UTRANCTRL_SndAsMsg(WUEPS_PID_RABM, (struct MsgCB **)&pSndMsg) )
    {
        /*打印警告信息---发送消息失败:*/
        RABM_LOG_WARNING( "NAS_RabmSndHandoverRabRsp:WARNING:SEND RRRABM_HANDOVER_RAB_RSP msg FAIL!" );
    }
    else
    {
        /*打印流程信息---发送了消息:*/
        RABM_LOG_NORMAL( "NAS_RabmSndHandoverRabRsp:NORMAL:SEND RRRABM_HANDOVER_RAB_RSP Msg" );
    }
}





VOS_VOID  NAS_RabmDealHandoverRabInd(RRRABM_HANDOVER_RAB_IND_STRU *pMsg)
{
    VOS_UINT8  ucRabNum = 0;
    VOS_UINT8  i;

    switch ( pMsg->ulRabChangeType )
    {
        case NAS_RRC_RAB_SETUP_REQUEST:
            for( i = 0; i < pMsg->ulRabCnt; i++ )
            {
                if( RABM_CN_CS == pMsg->aRabInfo[i].ulCnDomainId )
                {
                    if( ucRabNum >= RABM_CS_MAX_ENT_NUM )
                    {
                        ucRabNum = 0;
                        /*打印警告信息---:ucRabNum超过14个*/
                        RABM_LOG_WARNING( "NAS_RabmDealHandoverRabInd:WARNING: ucRabNum is bigger than 14!" );
                        break;
                    }
                    else
                    {
                        ucRabNum++;
                    }
                }
                else if ( RABM_CN_PS == pMsg->aRabInfo[i].ulCnDomainId )
                {
                    if (ucRabNum >= RABM_PS_MAX_ENT_NUM)
                    {
                        /* RabNum超过11个 */
                        ucRabNum = 0;

                        NAS_WARNING_LOG(WUEPS_PID_RABM,
                            "NAS_RabmDealHandoverRabInd: RabNum is bigger than 11.");
                        break;
                    }
                    else
                    {
                        ucRabNum++;
                    }
                }
                else
                {
                    /*打印警告信息---CN域值错误:*/
                    RABM_LOG_WARNING( "NAS_RabmDealHandoverRabInd:WARNING:Domain is Error!" );
                    ucRabNum = 0;
                    break;
                }
            }
            /*向RR发送RSP消息*/
            NAS_RabmSndHandoverRabRsp( ucRabNum, pMsg );
            break;
        case NAS_RRC_RAB_SETUP_SUCCESS:
        case NAS_RRC_RAB_SETUP_FAILURE:
        case NAS_RRC_RAB_RECONFIG:
        case NAS_RRC_RAB_RELEASE:
            /*打印警告信息---收到不合逻辑消息:*/
            RABM_LOG_WARNING( "NAS_RabmDealHandoverRabInd:WARNING:Receive Illegal Msg!" );
            break;
        default:
            /*打印警告信息---收到的消息名字出错:*/
            RABM_LOG_WARNING( "NAS_RabmDealHandoverRabInd:WARNING:The name of receive Msg is Error!" );
            break;
    }

}





VOS_VOID  NAS_RabmPowerOff(VOS_VOID)
{
    VOS_UINT8                           ucLoop;

    if (NAS_MML_NET_RAT_TYPE_GSM == NAS_RABM_GetCurrentSysMode())
    {
        /*2G 的资源回收:*/
        /*公共定时器的关闭:*/
        if ( RABM_ROUTING_AREA_UPDATE_T1_ON == gRabmRAT1Switch )   /*若启动了rout.area.update.T1定时器*/
        {
            /*关闭rout.area.update.T1定时器:*/
            if ( RABM_SUCCESS != NAS_RabmStopTimer( RABM_TIMER_NAME_COMMON, RABM_TIMER_PARA_ROUT_AREA_UPDATE_T1 ) )
            {
                /*打印警告信息---关闭rout.area.update.T1定时器失败:*/
                RABM_LOG_WARNING( "NAS_RabmPowerOff:WARNING:Stop rout.area.update.T1 Timer FAIL!" );
            }
            else
            {
                /*打印流程信息---关闭了rout.area.update.T1定时器:*/
                RABM_LOG_NORMAL( "NAS_RabmPowerOff:NORMAL:Stop rout.area.update.T1 Timer SUCCESS!" );
            }
        }

        if ( RABM_3G_TO_2G_T3_ON == gRabm3GTo2GT3Switch )    /*若打开了Rabm.sys.3To2.T3定时器*/
        {
            /*关闭Rabm.sys.3To2.T3定时器:*/
            if ( RABM_SUCCESS != NAS_RabmStopTimer( RABM_TIMER_NAME_COMMON, RABM_TIMER_PARA_3_TO_2_T3 ) )
            {
                /*打印警告信息---关闭Rabm.sys.3To2.T3定时器失败:*/
                RABM_LOG_WARNING( "NAS_RabmPowerOff:WARNING:Stop Rabm.sys.3To2.T3 Timer FAIL!" );
            }
            else
            {
                /*打印流程信息---关闭了Rabm.sys.3To2.T3定时器:*/
                RABM_LOG_NORMAL( "NAS_RabmPowerOff:NORMAL:Stop Rabm.sys.3To2.T3 Timer SUCCESS!" );
            }
        }

        /*关闭实体定时器及回收动态分配的内存:*/
        for ( ucLoop = 0; ucLoop < RABM_2G_MAX_ENT_NUM; ucLoop++ )
        {
            /*回收动态内存:*/
            switch ( gastRabm2GEntity[ucLoop].State )    /*该NSAPI所对应的状态*/
            {
                case RABM_2G_NULL:
                    break;
                case RABM_NSAPI_OK_TRANSMODE_NO:
                    if ( RABM_ACT_REQ_T1_ON == gaRabmActReqT1Switch[ucLoop] )   /*若打开了Rabm.act.req.T1定时器*/
                    {
                        /*关闭Rabm.act.req.T1定时器:*/
                        if ( RABM_SUCCESS != NAS_RabmStopTimer( ( RABM_TIMER_NAME_ENUM )ucLoop, RABM_TIMER_PARA_ACT_REQ_T1 ) )
                        {
                            /*打印警告信息---关闭Rabm.act.req.T1定时器失败:*/
                            RABM_LOG1_WARNING( "NAS_RabmPowerOff:WARNING:Stop Rabm.act.req.T1 Timer FAIL!: NSAPI:", ucLoop + RABM_NSAPI_OFFSET );
                        }
                        else
                        {
                            /*打印流程信息---关闭了Rabm.act.req.T1定时器:*/
                            RABM_LOG1_NORMAL( "NAS_RabmPowerOff:NORMAL:Stop Rabm.act.req.T1 Timer SUCCESS!: NSAPI:", ucLoop + RABM_NSAPI_OFFSET );
                        }
                    }

                    /*将状态设置为RABM_2G_NULL状态:*/
                    RABM_SetGState((RABM_NSAPI_ENUM)ucLoop, RABM_2G_NULL);

                    /*清空QoS:*/
                    gastRabm2GEntity[ucLoop].stQos.ulQosLength = 0;
                    PS_MEM_SET(gastRabm2GEntity[ucLoop].stQos.aucQosValue,
                               0x00,
                               sizeof(gastRabm2GEntity[ucLoop].stQos.aucQosValue));

                    break;
                case RABM_DATA_TRANS_READY:
                case RABM_ROUT_AREA_UPDATE:
                case RABM_DATA_TRANS_STOP:
                    /*将状态设置为RABM_2G_NULL状态:*/
                    RABM_SetGState((RABM_NSAPI_ENUM)ucLoop, RABM_2G_NULL);

                    /*清空QoS:*/
                    gastRabm2GEntity[ucLoop].stQos.ulQosLength = 0;
                    PS_MEM_SET(gastRabm2GEntity[ucLoop].stQos.aucQosValue,
                               0x00,
                               sizeof(gastRabm2GEntity[ucLoop].stQos.aucQosValue));

                    break;
                default:
                    /*打印出错信息--- 该NSAPI所对应的状态出错:*/
                    RABM_LOG1_ERROR( "NAS_RabmPowerOff:ERROR:This NSAPI's state is WRONG!: NSAPI:", ucLoop + RABM_NSAPI_OFFSET );
                    break;
            }
        }
    }
    else
    {
        /*3G 的资源回收:*/
        /*若开了Rabm.ReestTimer,则关闭它，并复位此定时器标志:*/
        if ( RABM_TRUE == g_ucReestTimerFlg )
        {
            RABM_TimerStop(0);
            NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON, RABM_TIMER_RESEND_EST_REQ);
            g_ucReestTimerFlg = RABM_FALSE;
        }

        /*回收动态内存:*/
        for ( ucLoop = 0; ucLoop < RABM_3G_PS_MAX_ENT_NUM; ucLoop++ )   /*遍历3G的每个NSAPI*/
        {
            switch ( g_aRabmPsEnt[ucLoop].ucState )    /*该NSAPI所对应的状态*/
            {
                case RABM_NULL:
                    break;
                case RABM_NSAPI_ACTIVE_PENDING:
                case RABM_ACT_PENDING_WITH_RAB:             /*这两种状态的处理方式一样*/
                case RABM_NSAPI_ACTIVE_WITH_RAB:
                case RABM_DATA_TRANSFER_STOP:
                case RABM_NSAPI_ACTIVE_NO_RAB:
                    RABM_SetWState(ucLoop, RABM_NULL);    /*状态清空 */

                    g_aRabmPsEnt[ucLoop].RabInfo.ucRbNum = 0;                /*初始化RB的个数为0*/
                    g_aRabmPsEnt[ucLoop].ucPppFlg        = RABM_SM_IP_PROT;  /*上层协议类型初始化为IP*/
                    g_aRabmPsEnt[ucLoop].QoS.ulQosLength = 0;                /*清空QoS*/
                    PS_MEM_SET(g_aRabmPsEnt[ucLoop].QoS.aucQosValue,
                               0x00,
                               sizeof(g_aRabmPsEnt[ucLoop].QoS.aucQosValue));

                    RABM_RelSaveData(&g_aRabmPsEnt[ucLoop].DlDataSave, RABM_DOWNLINK);       /*释放存储的下行数据*/
                    RABM_RelSaveData(&g_aRabmPsEnt[ucLoop].UlDataSave, RABM_UPLINK);         /*释放存储的上行数据*/

                    /* 停止RB建立保护定时器 */
                    if (VOS_TRUE == NAS_RABM_GetWPsEntRabReestFlg(ucLoop))
                    {
                        NAS_RABM_StopReestRabPendingTmr(ucLoop);
                        NAS_RABM_ClearWPsEntRabReestFlg(ucLoop);
                    }

                    break;
                default:
                    /*打印出错信息--- 该NSAPI所对应的状态出错:*/
                    RABM_LOG1_ERROR( "NAS_RabmPowerOff:ERROR:This NSAPI's state is WRONG!: NSAPI:", ucLoop + RABM_NSAPI_OFFSET );
                    break;
            }
        }
    }

    if (NAS_RABM_TIMER_STATUS_RUNING == g_ulNasRabmFDFluxDetectTimer)
    {
        NAS_RabmStopTimer(RABM_TIMER_NAME_FD_FLUX_DETECT,RABM_TIMER_FASTDORM_FLUX_DETECT);
    }

    if (NAS_RABM_TIMER_STATUS_RUNING == g_ulNasRabmFDRetryTimer)
    {
        NAS_RabmStopTimer(RABM_TIMER_NAME_FD_RETRY, RABM_TIMER_FASTDORM_RETRY);
    }

    if (NAS_RABM_TIMER_STATUS_RUNING == g_ulNasRabmFDWaitGmmQryRsltTimer)
    {
        NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON,RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT);
    }

    if (NAS_RABM_TIMER_STATUS_RUNING == g_ulNasRabmResendEstTimer)
    {
        NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON,RABM_TIMER_RESEND_EST_REQ);
    }

    if (NAS_RABM_TIMER_STATUS_RUNING == g_ulNasRabmVPDelayTimer)
    {
        NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON, RABM_TIMER_VOICEPREFER_DELAY);
    }
    if (NAS_RABM_TIMER_STATUS_RUNING == g_ulNasRabmVPReQryGmmDelayTimer)
    {
        NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON, RABM_TIMER_VOICEPREFER_REQRY_GMM_DELAY);
    }

    /*调用初始化函数:*/
    Rabm_Init();
    NAS_Rabm2GInit();

#if (FEATURE_ON == FEATURE_LTE)
    NAS_RABM_SetSysMode(NAS_MML_NET_RAT_TYPE_LTE);
#endif

    return;
}
VOS_UINT8  NAS_GetRbTxMode(VOS_UINT32 ulDomain, VOS_INT8    cRbId)
{
    VOS_UINT8    ucEntId,ucLoop;

    if ( RRC_NAS_PS_DOMAIN == ulDomain )
    {
        for(ucEntId = 0; ucEntId < RABM_PS_MAX_ENT_NUM; ucEntId++)
        {
            if((RABM_NULL != g_aRabmPsEnt[ucEntId].ucState)
                &&(RABM_NSAPI_ACTIVE_PENDING != g_aRabmPsEnt[ucEntId].ucState)
                &&(RABM_NSAPI_ACTIVE_NO_RAB != g_aRabmPsEnt[ucEntId].ucState))
            {
                if(cRbId == g_aRabmPsEnt[ucEntId].RabInfo.aucRbId[0])
                {
                    break;
                }
            }
        }

        if ( RABM_PS_MAX_ENT_NUM == ucEntId )
        {
            return WUE_RLC_MODE_BUTT;
        }
        else
        {
            return g_aRabmPsEnt[ucEntId].RabInfo.aucRbTxMode[0];
        }
    }
    else
    {
        for(ucEntId = 0; ucEntId < RABM_CS_MAX_ENT_NUM; ucEntId++)
        {
            for(ucLoop = 0; ucLoop < g_aRabmCsEnt[ucEntId].RabInfo.ucRbNum; ucLoop++)
            {
                if(g_aRabmCsEnt[ucEntId].RabInfo.aucRbId[ucLoop] == (VOS_UINT8)cRbId)
                {
                    return g_aRabmCsEnt[ucEntId].RabInfo.aucRbTxMode[ucLoop];
                }
            }
        }

        return WUE_RLC_MODE_BUTT;
    }

}


VOS_VOID NAS_RABM_MNTN_TracePktLostEvt(
    NAS_RABM_PKT_FLOW_DIR_ENUM_UINT8    enPtkFlowDir,
    VOS_UINT8                           ucPoint
)
{
    NAS_RABM_MNTN_PKT_LOST_STRU        *pstPktLostEvt = VOS_NULL_PTR;

    /* 构造消息 */
    pstPktLostEvt = (NAS_RABM_MNTN_PKT_LOST_STRU*)PS_ALLOC_MSG_WITH_HEADER_LEN(
                        WUEPS_PID_RABM,
                        sizeof(NAS_RABM_MNTN_PKT_LOST_STRU));
    if (VOS_NULL_PTR == pstPktLostEvt)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM, "NAS_RABM_MNTN_TracePktLostEvt: Memory alloc failed.");
        return;
    }

    /* 填写消息 */
    pstPktLostEvt->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstPktLostEvt->ulReceiverPid   = WUEPS_PID_RABM;
    pstPktLostEvt->enMsgId         = (NAS_RABM_PKT_FLOW_DIR_UPLINK == enPtkFlowDir) ?
                                     ID_NAS_RABM_MNTN_EVT_UL_PKT_LOST : ID_NAS_RABM_MNTN_EVT_DL_PKT_LOST;
    pstPktLostEvt->ucPoint         = ucPoint;

    OM_TraceMsgHook(pstPktLostEvt);

    PS_FREE_MSG(WUEPS_PID_RABM, pstPktLostEvt);

    return;
}


VOS_VOID RABM_UpdateCallBack(VOS_UINT8 ucEntId)
{
    VOS_UINT8                           ucState;
    VOS_UINT8                           ucChgMask;
    VOS_UINT32                          ulLoop;
    RABM_CUR_NET_ENUM                   enRabmCurrNet;

    /*检查g_astRabMap的实体是否已创建*/
    if (PS_TRUE !=  g_astRabMap[ucEntId + RABM_NSAPI_OFFSET].enucExistInd )
    {
        RABM_LOG1_WARNING( "RABM, RABM_UpdateCallBack, WARNING, RAB MAP Not Exist, RabId <1>", ucEntId + RABM_NSAPI_OFFSET);
        return;
    }

    switch (NAS_RABM_GetCurrentSysMode())
    {
        case NAS_MML_NET_RAT_TYPE_GSM:
            enRabmCurrNet = RABM_2G_ENUM;
            ucState = NAS_RABM_GetGPsEntState(ucEntId);
            break;

        case NAS_MML_NET_RAT_TYPE_WCDMA:
        case NAS_MML_NET_RAT_TYPE_LTE:
            enRabmCurrNet = RABM_3G_ENUM;
            ucState = NAS_RABM_GetWPsEntState(ucEntId);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "RABM_UpdateCallBack: Invalid system mode." );
            return;
    }

    /*配置相应上行回调*/
    for (ulLoop = 0; ulLoop < (sizeof(g_astRabAsUlDataCallBack)/sizeof(RAB_AS_UL_DATA_CALLBACK_STRU)); ulLoop++)
    {
        if ( (g_astRabAsUlDataCallBack[ulLoop].ucMode == enRabmCurrNet)
            && (g_astRabAsUlDataCallBack[ulLoop].ucState == ucState) )
        {
            /* 配置相应RAB_MAP实体的上行回调 */
            ucChgMask = 0;
            ucChgMask |= RAB_MAP_CHG_MASK_UL_CALLBACK;
            RABM_ChgRabMap( ucEntId + RABM_NSAPI_OFFSET,
                            ucChgMask,
                            0,
                            g_astRabAsUlDataCallBack[ulLoop].pAsUlDataCallBack,
                            VOS_NULL_PTR );
            break;
        }
    }

    /*配置相应下行回调(下行回调不依赖于状态而转移，但若状态迁入IDLE态，则需将下行回调指针置为空指针)*/
    if (RABM_2G_ENUM == enRabmCurrNet)
    {
        if ( RABM_2G_NULL == gastRabm2GEntity[ucEntId].State )
        {
            /*将相应实体的下行回调置空*/
            ucChgMask = 0;
            ucChgMask |= RAB_MAP_CHG_MASK_DL_CALLBACK;
            RABM_ChgRabMap( ucEntId + RABM_2G_NSAPI_OFFSET,
                            ucChgMask,
                            0,
                            VOS_NULL_PTR,
                            VOS_NULL_PTR );
        }
    }
    else
    {
        if ( RABM_NULL == g_aRabmPsEnt[ucEntId].ucState )
        {
            /*将相应实体的下行回调置空*/
            ucChgMask = 0;
            ucChgMask |= RAB_MAP_CHG_MASK_DL_CALLBACK;
            RABM_ChgRabMap( ucEntId + RABM_NSAPI_OFFSET,
                            ucChgMask,
                            0,
                            VOS_NULL_PTR,
                            VOS_NULL_PTR );
        }
    }

    RABM_LOG1_NORMAL( "RABM, RABM_UpdateCallBack, NORMAL, Update RabId <1> CallBack", ucEntId + RABM_NSAPI_OFFSET );

    return;
}



VOS_UINT32  RABM_SetGState(RABM_NSAPI_ENUM  EntId, RABM_2G_STATE_ENUM  DestState)
{
    VOS_UINT8   ucChgMask;

    /*检查EntId的合法性*/
    if (RABM_NSAPI_BUTT <= EntId)
    {
        RABM_LOG1_WARNING( "RABM_SetGState:WARNING:EntId is beyond range!",
                           (VOS_INT32)EntId );
        return VOS_ERR;
    }

    /*检查DestState的合法性*/
    if (RABM_STATE_BUTT <= DestState )
    {
        RABM_LOG2_WARNING( "RABM_SetGState:WARNING:DestState is beyond range!",
                           (VOS_INT32)EntId,
                           (VOS_INT32)DestState );
        return VOS_ERR;
    }

    /*执行状态迁移操作*/
    gastRabm2GEntity[EntId].State = DestState;

    if (NAS_MML_NET_RAT_TYPE_GSM == NAS_RABM_GetCurrentSysMode())
    {
        if (RABM_2G_NULL != DestState)
        {
            /*更新RAB_MAP表中的ucAsId*/
            ucChgMask = 0;
            ucChgMask |= RAB_MAP_CHG_MASK_AS_ID;
            RABM_ChgRabMap(EntId + RABM_NSAPI_OFFSET,
                           ucChgMask,
                           EntId + RABM_NSAPI_OFFSET,
                           VOS_NULL_PTR,
                           VOS_NULL_PTR);
        }

        /*更新RAB_MAP表中的上下行回调函数*/
        RABM_UpdateCallBack(EntId);
    }

    return VOS_OK;
}
VOS_UINT32  RABM_SetWState(VOS_UINT8    ucEntId, VOS_UINT8   ucDestState)
{
    /*检查EntId的合法性*/
    if (RABM_PS_MAX_ENT_NUM <= ucEntId)
    {
        RABM_LOG1_WARNING( "RABM_SetWState:WARNING:ucEntId is beyond range!",
                           (VOS_INT32)ucEntId );
        return VOS_ERR;
    }

    /*检查DestState的合法性*/
    if (RABM_TC_STATE_WITH_RAB < ucDestState)
    {
        RABM_LOG2_WARNING( "RABM_SetWState:WARNING:ucDestState is beyond range!",
                           (VOS_INT32)ucEntId,
                           (VOS_INT32)ucDestState );
        return VOS_ERR;
    }

    /*执行状态迁移操作*/
    g_aRabmPsEnt[ucEntId].ucState = ucDestState;

    if ( (NAS_MML_NET_RAT_TYPE_WCDMA == NAS_RABM_GetCurrentSysMode())
      || (NAS_MML_NET_RAT_TYPE_LTE == NAS_RABM_GetCurrentSysMode()) )
    {
        RABM_UpdateCallBack(ucEntId);
    }

    return VOS_OK;
}
VOS_VOID RABM_TraceRabMapInfo(VOS_UINT8 ucRabId)
{
    RABM_TRACE_RAB_MAP_INFO_STRU    stTraceRabMapInfoMsg;


    /*====================*/    /* 参数检查 */
    if ( !RAB_MAP_RAB_IS_VALID(ucRabId) )
    {
        PS_LOG1(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_TraceRabMapInfo, WARNING, Invalid RabId, RabId <1>",
            ucRabId);
        return;
    }

    PS_MEM_SET(&stTraceRabMapInfoMsg, 0, sizeof(RABM_TRACE_RAB_MAP_INFO_STRU));

    stTraceRabMapInfoMsg.ulSenderCpuId      = VOS_LOCAL_CPUID;
    stTraceRabMapInfoMsg.ulSenderPid        = WUEPS_PID_RABM;
    stTraceRabMapInfoMsg.ulReceiverCpuId    = VOS_LOCAL_CPUID;
    stTraceRabMapInfoMsg.ulReceiverPid      = WUEPS_PID_RABM;
    stTraceRabMapInfoMsg.ulLength           = sizeof(RABM_TRACE_RAB_MAP_INFO_STRU) - VOS_MSG_HEAD_LENGTH;

    stTraceRabMapInfoMsg.ulMsgName          = RABM_TRACE_RAB_MAP_INFO_MSG;
    stTraceRabMapInfoMsg.enucExistInd       = g_astRabMap[ucRabId].enucExistInd;
    stTraceRabMapInfoMsg.ucRabId            = g_astRabMap[ucRabId].ucRabId;
    stTraceRabMapInfoMsg.ucAsId             = g_astRabMap[ucRabId].ucAsId;

    if (VOS_NULL_PTR == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        stTraceRabMapInfoMsg.enUlCbName = RAB_UL_CB_NAME_NULL;
    }
    else if (RABM_SndRABMUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        stTraceRabMapInfoMsg.enUlCbName = RAB_UL_CB_NAME_SND_RABM;
    }
    else if (RABM_SndPdcpUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        stTraceRabMapInfoMsg.enUlCbName = RAB_UL_CB_NAME_SND_PDCP;
    }
    else if (RABM_SndSndcpUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        stTraceRabMapInfoMsg.enUlCbName = RAB_UL_CB_NAME_SND_SNDCP;
    }
    else
    {
        stTraceRabMapInfoMsg.enUlCbName = RAB_UL_CB_NAME_BUTT;
    }

    if (VOS_NULL_PTR == g_astRabMap[ucRabId].pDlDataCallBack)
    {
        stTraceRabMapInfoMsg.enDlCbName = RAB_DL_CB_NAME_NULL;
    }
    else if (RABM_SndAppDlDataCallBack == g_astRabMap[ucRabId].pDlDataCallBack)
    {
        stTraceRabMapInfoMsg.enDlCbName = RAB_DL_CB_NAME_DEFAULT;
    }
    else
    {
        stTraceRabMapInfoMsg.enDlCbName = RAB_DL_CB_NAME_APP_REG;
    }

    OM_TraceMsgHook(&stTraceRabMapInfoMsg);
} /* RABM_TraceRabMapInfo */



VOS_UINT32 RABM_CreateRabMap(
    VOS_UINT8                           ucRabId,
    VOS_UINT8                           ucLinkdRabId,
    VOS_UINT8                           ucAsId,
    RAB_UL_DATA_CALLBACK                pRabUlDataCallBack,
    RAB_DL_DATA_CALLBACK                pRabDlDataCallBack
)
{
    /*====================*/    /* 参数检查 */
    /* RabId值，范围为:[5,15] */
    if ( !RAB_MAP_RAB_IS_VALID(ucRabId) )
    {
        PS_LOG4(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_CreateRabMap, WARNING, Invalid RabId, RabId <1>, AsId <2>, UlCallBack <3 >, DLCallBack <4>",
            ucRabId, ucAsId, (VOS_INT32)pRabUlDataCallBack, (VOS_INT32)pRabDlDataCallBack);
        return PS_FAIL;
    }

    if ( !RAB_MAP_ASID_IS_VALID(ucAsId) )
    {
        PS_LOG4(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_CreateRabMap, WARNING, Invalid ucAsId, RabId <1>, AsId <2>, UlCallBack <3 >, DLCallBack <4>",
            ucRabId, ucAsId, (VOS_INT32)pRabUlDataCallBack, (VOS_INT32)pRabDlDataCallBack);
        return PS_FAIL;
    }

    /* 回调函数支持注册空函数，不检查 */


    /* 如果映射实体已经存在，则出错 */
    if (PS_TRUE == g_astRabMap[ucRabId].enucExistInd)
    {
        PS_LOG4(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_CreateRabMap, WARNING, This RAB MAP Is Already Exist, RabId <1>, AsId <2>, UlCallBack <3 >, DLCallBack <4>",
            ucRabId, ucAsId, (VOS_INT32)pRabUlDataCallBack, (VOS_INT32)pRabDlDataCallBack);
        return PS_FAIL;
    }


    /*====================*/    /* 创建RAB映射实体 */
    g_astRabMap[ucRabId].enucExistInd       = PS_TRUE;
    g_astRabMap[ucRabId].ucRabId            = ucRabId;
    g_astRabMap[ucRabId].ucLinkdRabId       = ucLinkdRabId;
    g_astRabMap[ucRabId].ucAsId             = ucAsId;
    g_astRabMap[ucRabId].pUlDataCallBack    = pRabUlDataCallBack;

    /*PDP激活成功时，若处于可以数传的状态，则更改R接口流控门限*/
    if(RABM_SndPdcpUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        R_ITF_SetWFlowLev();
    }
    else if(RABM_SndSndcpUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        R_ITF_SetGFlowLev();
    }
    else
    {
        ;
    }

    g_astRabMap[ucRabId].pDlDataCallBack    = pRabDlDataCallBack;

    /* 设置RB到映射实体的快速索引 */
    g_aucRb2Rab[ucAsId]                     = ucRabId;

    PS_LOG4(WUEPS_PID_RABM, 0, PS_PRINT_NORMAL,
        "RABM, RABM_CreateRabMap, NORMAL, CreatRabMap Success, RabId <1>, AsId <2>, UlCallBack <3 >, DLCallBack <4>",
        ucRabId, ucAsId, (VOS_INT32)pRabUlDataCallBack, (VOS_INT32)pRabDlDataCallBack);

    RABM_TraceRabMapInfo(ucRabId);

    return PS_SUCC;

} /* RABM_CreateRabMap */



VOS_UINT32 RABM_DelRabMap(VOS_UINT8 ucRabId)
{
    VOS_UINT8   ucAsId;


    /*====================*/    /* 参数检查 */
    if ( !RAB_MAP_RAB_IS_VALID(ucRabId) )
    {
        PS_LOG1(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_DelRabMap, WARNING, Invalid RabId, RabId <1>",
            ucRabId);
        return PS_FAIL;
    }

    /* 如果映射实体已经不存在，则出错 */
    if (PS_TRUE != g_astRabMap[ucRabId].enucExistInd)
    {
        PS_LOG1(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_DelRabMap, WARNING, This RAB MAP Is Not Exist, RabId <1>",
            ucRabId);
        return PS_FAIL;
    }


    /*====================*/    /*  删除该RAB映射实体 */
    ucAsId  = g_astRabMap[ucRabId].ucAsId;  /* 获取AS_ID */

    g_astRabMap[ucRabId].enucExistInd       = PS_FALSE;
    g_astRabMap[ucRabId].ucRabId            = RAB_MAP_INVALID_RAB_ID;
    g_astRabMap[ucRabId].ucLinkdRabId       = RAB_MAP_INVALID_RAB_ID;
    g_astRabMap[ucRabId].ucAsId             = RAB_MAP_INVALID_AS_ID;
    g_astRabMap[ucRabId].pUlDataCallBack    = VOS_NULL_PTR;
    g_astRabMap[ucRabId].pDlDataCallBack    = VOS_NULL_PTR;

    if ( RAB_MAP_ASID_IS_VALID(ucAsId) )
    {
        /* 清除RB到映射实体的快速索引 */
        g_aucRb2Rab[ucAsId]                 = RAB_MAP_INVALID_RAB_ID;
    }
    else
    {
        PS_LOG2(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_DelRabMap, WARNING, Invalid RbId, RabId <1>, AsId <2>",
            ucRabId, ucAsId);
    }

    PS_LOG2(WUEPS_PID_RABM, 0, PS_PRINT_NORMAL,
        "RABM, RABM_DelRabMap, NORMAL, DelRabMap Success, RabId <1>, AsId <2>",
        ucRabId, ucAsId);

    RABM_TraceRabMapInfo(ucRabId);

    return PS_SUCC;

} /* RABM_DelRabMap */



VOS_UINT32 RABM_ChgRabMap
(
    VOS_UINT8       ucRabId,
    VOS_UINT8       ucChgMask,
    VOS_UINT8       ucAsId,
    RAB_UL_DATA_CALLBACK pRabUlDataCallBack,
    RAB_DL_DATA_CALLBACK pRabDlDataCallBack
)
{
    VOS_UINT8   ucOldAsId;

    /*====================*/    /* 参数检查 */
    if ( !RAB_MAP_RAB_IS_VALID(ucRabId) )
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_ChgRabMap, WARNING, Invalid RabId, RabId <1>, ucChgMask <2> AsId <3>",
            ucRabId, ucChgMask, ucAsId);
        return PS_FAIL;
    }

    /* RAB映射实体如果不存在，异常 */
    if (PS_TRUE != g_astRabMap[ucRabId].enucExistInd)
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_ChgRabMap, WARNING, RAB MAP Not Exist, RabId <1>, ucChgMask <2> AsId <3>",
            ucRabId, ucChgMask, ucAsId);
        return PS_FAIL;
    }

    /*====================*/    /* 根据掩码修改RAB映射实体的元素 */
    if ( 0 != (RAB_MAP_CHG_MASK_AS_ID & ucChgMask) )
    {
        /* 清除原来RB到映射实体的快速索引 */
        ucOldAsId   = g_astRabMap[ucRabId].ucAsId;

        if ( !RAB_MAP_ASID_IS_VALID(ucAsId) )
        {
            PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
                "RABM, RABM_ChgRabMap, WARNING, Invalid RbId, RabId <1>, ucChgMask <2> AsId <3>",
                ucRabId, ucChgMask, ucAsId);
            return PS_FAIL;
        }

        g_astRabMap[ucRabId].ucAsId     = ucAsId;

        /* 更新RB到映射实体的快速索引 */
        g_aucRb2Rab[ucAsId] = ucRabId;

        /*检查ucOldAsId的合法性(之所以不用RAB_MAP_ASID_IS_VALID来判断，是因为在W/G异系统流程中,W模式下的ucOldAsId可能大于
           G下的[5,15]的合法范围)，故只判断ucOldAsId在W和G模式下的整体合法的取值范围[5,32]*/
        if ( (PS_WUE_MIN_T_RB_ID <= ucOldAsId) && (PS_WUE_MAX_RB_ID >= ucOldAsId) )
        {
            if ( (ucOldAsId != ucAsId) && (g_aucRb2Rab[ucOldAsId] == g_aucRb2Rab[ucAsId]) )
            {
                g_aucRb2Rab[ucOldAsId] = RAB_MAP_INVALID_RAB_ID;
            }
        }
    }

    if ( 0 != (RAB_MAP_CHG_MASK_UL_CALLBACK & ucChgMask) )
    {
        g_astRabMap[ucRabId].pUlDataCallBack    = pRabUlDataCallBack;

        /*双模切换时，更改R接口流控门限*/
        if(RABM_SndPdcpUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
        {
            R_ITF_SetWFlowLev();
        }
        else if(RABM_SndSndcpUlDataCallBack == g_astRabMap[ucRabId].pUlDataCallBack)
        {
            R_ITF_SetGFlowLev();
        }
        else
        {
            ;
        }
    }

    if ( 0 != (RAB_MAP_CHG_MASK_DL_CALLBACK & ucChgMask) )
    {
        g_astRabMap[ucRabId].pDlDataCallBack    = pRabDlDataCallBack;
    }

    PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_NORMAL,
        "RABM, RABM_ChgRabMap, NORMAL, Change RabMap Success, RabId <1>, ucChgMask <2> AsId <3>",
        ucRabId, ucChgMask, ucAsId);

    RABM_TraceRabMapInfo(ucRabId);

    return PS_SUCC;

} /* RABM_ChgRabMap */



VOS_UINT32 RABM_SendUlData(VOS_UINT8 ucRabId, TTF_MEM_ST *pstData)
{
    VOS_UINT8   ucAsId;
    VOS_UINT32  ulRtn;

    /*====================*/    /* 参数检查 */
    if ( !RAB_MAP_RAB_IS_VALID(ucRabId) )
    {
        PS_LOG2(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendUlData, WARNING, Invalid RabId, RabId <1>, pData <2>",
            ucRabId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_UPLINK, 1);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /* RAB映射实体如果不存在，异常 */
    if (PS_TRUE != g_astRabMap[ucRabId].enucExistInd)
    {
        PS_LOG2(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendUlData, WARNING, RAB MAP Not Exist, RabId <1>, pData <2>",
            ucRabId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_UPLINK, 2);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /* 获取RB_ID */
    ucAsId  = g_astRabMap[ucRabId].ucAsId;

    if ( !RAB_MAP_ASID_IS_VALID(ucAsId) )
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendUlData, WARNING, Invalid RbId, RabId <1>, AsId <2>, pData <3>",
            ucRabId, ucAsId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_UPLINK, 3);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }


    /*====================*/    /* 根据RAB_ID寻找RAB_MAP表，并调用相应的回调 */
    if (VOS_NULL_PTR == g_astRabMap[ucRabId].pUlDataCallBack)
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendUlData, WARNING, UlCallBack Is NULL, RabId <1>, ucAsId <2>, pData <3>",
            ucRabId, ucAsId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_UPLINK, 4);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /* 向接入层分发 */
    ulRtn = g_astRabMap[ucRabId].pUlDataCallBack(ucRabId, ucAsId, pstData);
    if (PS_SUCC != ulRtn)
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendUlData, WARNING, Call UlCallBack Fail, RabId <1>, ucAsId <2>, pData <3>",
            ucRabId, ucAsId, (VOS_INT32)pstData);
        return PS_FAIL;
    }

    return PS_SUCC;

} /* RABM_SendUlData */


VOS_UINT32 RABM_SendDlData(VOS_UINT8 ucAsId, TTF_MEM_ST *pstData)
{
    VOS_UINT8   ucRabId;
    VOS_UINT32  ulRtn;

    /*====================*/    /* 参数检查 */
    if ( !RAB_MAP_ASID_IS_VALID(ucAsId) )
    {
        PS_LOG2(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendDlData, WARNING, Invalid RbId, AsId <1>, pData",
            ucAsId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_DOWNLINK, 1);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /* 根据快速索引表获取RAB_ID和映射实体 */
    ucRabId = g_aucRb2Rab[ucAsId];

    if ( !RAB_MAP_RAB_IS_VALID(ucRabId) )
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendDlData, WARNING, Invalid RabId, RabId <1>, AsId <2>, pData",
            ucRabId, ucAsId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_DOWNLINK, 2);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /* RAB映射实体如果不存在，异常 */
    if (PS_TRUE != g_astRabMap[ucRabId].enucExistInd)
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendDlData, WARNING, RAB MAP Is Free, RabId <1>, AsId <2>, pData",
            ucRabId, ucAsId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_DOWNLINK, 3);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }


    /*====================*/    /* 根据RAB_ID寻找RAB_MAP表，并调用相应的下行回调 */
    if (VOS_NULL_PTR == g_astRabMap[ucRabId].pDlDataCallBack)
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendDlData, WARNING, DlCallBack Is NULL, RabId <1>, AsId <2>, pData",
            ucRabId, ucAsId, (VOS_INT32)pstData);

        NAS_RABM_MNTN_TracePktLostEvt(NAS_RABM_PKT_FLOW_DIR_DOWNLINK, 4);
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /* 向应用分发 */
    ulRtn = g_astRabMap[ucRabId].pDlDataCallBack(ucRabId, pstData);
    if (PS_SUCC != ulRtn)
    {
        PS_LOG3(WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
            "RABM, RABM_SendDlData, WARNING, Call DlCallBack Fail, RabId <1>, AsId <2>, pData",
            ucRabId, ucAsId, (VOS_INT32)pstData);
        return PS_FAIL;
    }

    /* W下对FD下行数据计数器进行累加 */
    if (NAS_MML_NET_RAT_TYPE_WCDMA == NAS_RABM_GetCurrentSysMode())
    {
        NAS_RABM_INC_FD_DLDATA_CNT();

        NAS_RABM_ProcFastDormStatusInTransfer();

        NAS_RABM_AbortRelRrcProcedure();
    }

    return PS_SUCC;
} /* RABM_SendDlData */

VOS_UINT32 RABM_SndPdcpUlDataCallBack(VOS_UINT8 ucRabId, VOS_UINT8 ucRbId, TTF_MEM_ST *pstData)
{
    VOS_UINT16                          usDataLen;
    VOS_UINT32                          ulDataSendRslt;

    usDataLen       = TTF_MemGetLen(WUEPS_PID_RABM, pstData);

    ulDataSendRslt  = NAS_UTRANCTRL_RABM_AdaptPdcpDataReq(ucRbId, pstData, usDataLen * 8);
    if ( PS_SUCC != ulDataSendRslt )
    {
        return PS_FAIL;
    }

    NAS_RABM_INC_FD_ULDATA_CNT();

    NAS_RABM_ProcFastDormStatusInTransfer();

    NAS_RABM_AbortRelRrcProcedure();

    return PS_SUCC;
}
VOS_UINT32 RABM_SndSndcpUlDataCallBack(VOS_UINT8 ucRabId, VOS_UINT8 ucNsapi, TTF_MEM_ST *pstData)
{
    RABM_NSAPI_ENUM         NsapiIndex;
    SN_UNITDATA_REQ_MSG    *pUnitDataReqMsg;
    SN_DATA_REQ_MSG        *pDataReqMsg;
    VOS_UINT16              usLen;

    NsapiIndex  = ( RABM_NSAPI_ENUM )( ucRabId - RABM_2G_NSAPI_OFFSET );
    usLen       = TTF_MemGetLen(WUEPS_PID_RABM, pstData);

    if ( RABM_ABM_ENUM == gastRabm2GEntity[NsapiIndex].TransMode )
    {
        /* 如果是确认模式 */
        pDataReqMsg = (SN_DATA_REQ_MSG *)
            PS_ALLOC_MSG( WUEPS_PID_RABM, sizeof(SN_DATA_REQ_MSG) - VOS_MSG_HEAD_LENGTH);
        if ( VOS_NULL_PTR == pDataReqMsg )
        {
            PS_LOG( WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
                "RABM, RABM_SndSndcpUlDataCallBack, WARNING, Alloc msg fail!");
            TTF_MemFree(WUEPS_PID_RABM, pstData);
            return PS_FAIL;
        }

        pDataReqMsg->ulReceiverCpuId    = VOS_LOCAL_CPUID;
        pDataReqMsg->ulSenderPid        = WUEPS_PID_RABM;
        pDataReqMsg->ulReceiverPid      = UEPS_PID_SN;
        pDataReqMsg->usMsgType          = ID_SN_DATA_REQ;
        pDataReqMsg->usPId              = ucNsapi;
        pDataReqMsg->ucMask             = 0;
        pDataReqMsg->usPduNum           = 0;
        pDataReqMsg->usPduLen           = usLen;
        pDataReqMsg->pMemPt             = pstData;

        if (VOS_OK != PS_SEND_MSG(WUEPS_PID_RABM, pDataReqMsg))
        {
            PS_LOG( WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
                "RABM, RABM_SndSndcpUlDataCallBack, WARNING, Send msg fail!");
            TTF_MemFree(WUEPS_PID_RABM, pstData);
            return PS_FAIL;
        }
    }
    else    /* 如果是非确认模式 */
    {
        pUnitDataReqMsg = (SN_UNITDATA_REQ_MSG *)PS_ALLOC_MSG
            (WUEPS_PID_RABM, sizeof(SN_UNITDATA_REQ_MSG) - VOS_MSG_HEAD_LENGTH);
        if ( VOS_NULL_PTR == pUnitDataReqMsg )
        {
            PS_LOG( WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
                "RABM, RABM_SndSndcpUlDataCallBack, WARNING, Alloc msg fail!");
            TTF_MemFree(WUEPS_PID_RABM, pstData);
            return PS_FAIL;
        }

        pUnitDataReqMsg->ulReceiverCpuId    = VOS_LOCAL_CPUID;
        pUnitDataReqMsg->ulSenderPid        = WUEPS_PID_RABM;
        pUnitDataReqMsg->ulReceiverPid      = UEPS_PID_SN;
        pUnitDataReqMsg->usMsgType          = ID_SN_UNITDATA_REQ;
        pUnitDataReqMsg->usPId              = ucNsapi;
        pUnitDataReqMsg->usPduLen           = usLen;
        pUnitDataReqMsg->pMemPt             = pstData;


        if (VOS_OK != PS_SEND_MSG(WUEPS_PID_RABM, pUnitDataReqMsg))
        {
            PS_LOG( WUEPS_PID_RABM, 0, PS_PRINT_WARNING,
                "RABM, RABM_SndSndcpUlDataCallBack, WARNING, Send msg fail!");
            TTF_MemFree(WUEPS_PID_RABM, pstData);
            return PS_FAIL;
        }
    }

    return PS_SUCC;

}





VOS_UINT32 RABM_SndRABMUlDataCallBack(VOS_UINT8 ucRabId, VOS_UINT8 ucRbId, TTF_MEM_ST *pstData)
{
    /*释放零拷贝数据块*/
    TTF_MemFree(WUEPS_PID_RABM, pstData);

    return PS_SUCC;
}
VOS_UINT32 RABM_SndAppDlDataCallBack(VOS_UINT8 ucRabId, TTF_MEM_ST *pstData)
{
    VOS_UINT32                  ulDataLen;
    ST_APP_RABM_DATA            AppRabmData;

    ulDataLen = TTF_MemGetLen(WUEPS_PID_RABM, pstData);

    AppRabmData.pucData = (VOS_UINT8*)PS_MEM_ALLOC(WUEPS_PID_RABM, ulDataLen);
    if (VOS_NULL_PTR == AppRabmData.pucData)
    {
        /*打印出错信息---申请消息包失败:*/
        RABM_LOG_ERROR( "RABM_SndAppDlDataCallBack:ERROR:Allocates a dynamic mem fail!" );

        /*释放零拷贝数据块*/
        TTF_MemFree(WUEPS_PID_RABM, pstData);
        return PS_FAIL;
    }

    /*从TTF MEM零拷贝结构中拷贝数据至线性内存区域*/
    TTF_MemGet(WUEPS_PID_RABM, pstData, 0, AppRabmData.pucData, (VOS_UINT16)ulDataLen);

    /*释放零拷贝数据块*/
    TTF_MemFree(WUEPS_PID_RABM, pstData);

    /*调用APP注册的回调函数*/
    AppRabmData.ucDomain  = TAF_DATA_PS_DOMAIN;
    AppRabmData.ucRabId   = ucRabId;
    AppRabmData.ulDataLen = ulDataLen;
    Api_AppRabmDataInd(&AppRabmData);

    /*释放线性内存块*/
    PS_MEM_FREE(WUEPS_PID_RABM, AppRabmData.pucData);
    return PS_SUCC;
}




VOS_VOID    RABM_RabMapInit(VOS_VOID)
{
    VOS_UINT8   ucLoop;

    for (ucLoop = 0; ucLoop <= PS_WUE_MAX_RB_ID; ucLoop++)
    {
        g_astRabMap[ucLoop].enucExistInd    = PS_FALSE;
        g_astRabMap[ucLoop].ucRabId         = RAB_MAP_INVALID_RAB_ID;
        g_astRabMap[ucLoop].ucLinkdRabId    = RAB_MAP_INVALID_RAB_ID;
        g_astRabMap[ucLoop].ucAsId          = RAB_MAP_INVALID_AS_ID;
        g_astRabMap[ucLoop].pUlDataCallBack = VOS_NULL_PTR;
        g_astRabMap[ucLoop].pDlDataCallBack = VOS_NULL_PTR;
    }

    for (ucLoop = 0; ucLoop <= PS_WUE_MAX_RB_ID; ucLoop++)
    {
        g_aucRb2Rab[ucLoop] = RAB_MAP_INVALID_RAB_ID;
    }

    return;
}




VOS_UINT32  RABM_IsExistBufferUplinkData(VOS_VOID)
{
    VOS_UINT8   ucEntId;

    for(ucEntId = 0; ucEntId < RABM_PS_MAX_ENT_NUM; ucEntId++)
    {
        if((RABM_NULL != g_aRabmPsEnt[ucEntId].ucState) && (0 != g_aRabmPsEnt[ucEntId].UlDataSave.ucNum))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32  RABM_ResumeFirstActIndStatus(RABM_CUR_NET_ENUM enCurNetType, VOS_UINT8 ucEntId)
{
    RABMSM_ACTIVATE_IND_STRU    ActivateInd;
    RABMSM_ACTIVATE_IND_STRU    *pActivateInd;

    if ( (RABM_3G_ENUM != enCurNetType) && (RABM_2G_ENUM != enCurNetType) )
    {
        RABM_LOG_WARNING( "RABM_ResumeFirstActIndStatus:WARNING:enCurNetType is Error" );
        return VOS_ERR;
    }

    if ( ucEntId >= RABM_2G_MAX_ENT_NUM )
    {
        RABM_LOG_WARNING( "RABM_ResumeFirstActIndStatus:WARNING:ucEntId is Abnormal" );
        return VOS_ERR;
    }

    pActivateInd = &ActivateInd;
    if (VOS_OK != SM_RabmGetFirstActIndMsgInfo( (VOS_UINT8)(ucEntId + RABM_2G_NSAPI_OFFSET), pActivateInd) )
    {
        RABM_LOG_WARNING( "RABM_ResumeFirstActIndStatus:WARNING:SM_RabmGetFirstActIndMsgInfo invoke Error" );
        return VOS_ERR;
    }

    if (RABM_3G_ENUM == enCurNetType)
    {
        g_aRabmPsEnt[ucEntId].QoS.ulQosLength = pActivateInd->Qos.ulLength;

        PS_MEM_CPY(g_aRabmPsEnt[ucEntId].QoS.aucQosValue,
                   pActivateInd->Qos.aucQosValue,
                   NAS_RABM_MAX_QOS_LEN);


        if(RABM_SM_PPP_PROT == pActivateInd->ulPppFlag)
        {
            g_aRabmPsEnt[ucEntId].ucPppFlg = RABM_SM_PPP_PROT;
        }
        else
        {
            g_aRabmPsEnt[ucEntId].ucPppFlg = RABM_SM_IP_PROT;
        }
    }
    else
    {
        /*获取ucPppFlg:*/
        if( RABM_SM_PPP_PROT == pActivateInd->ulPppFlag )
        {
            gastRabm2GEntity[ucEntId].ucPppFlg = RABM_SM_PPP_PROT;
        }
        else
        {
            gastRabm2GEntity[ucEntId].ucPppFlg = RABM_SM_IP_PROT;
        }
    }

    return VOS_OK;
}
VOS_VOID NAS_RABM_CreateRabMapEntity(
    VOS_UINT8                           ucRabId,
    VOS_UINT8                           ucLinkdRabId,
    VOS_UINT8                           ucAsId
)
{
    RAB_UL_DATA_CALLBACK                pRabUlDataCallBack;
    RAB_DL_DATA_CALLBACK                pRabDlDataCallBack;
    VOS_UINT8                           ucState;
    VOS_UINT32                          ulLoop;
    RABM_CUR_NET_ENUM                   enRabmCurrNet;

    pRabUlDataCallBack = VOS_NULL_PTR;

    switch (NAS_RABM_GetCurrentSysMode())
    {
        case NAS_MML_NET_RAT_TYPE_GSM:
            enRabmCurrNet = RABM_2G_ENUM;
            ucState = NAS_RABM_GetGPsEntState(ucRabId - RABM_2G_NSAPI_OFFSET);
            break;

        case NAS_MML_NET_RAT_TYPE_WCDMA:
        case NAS_MML_NET_RAT_TYPE_LTE:
            enRabmCurrNet = RABM_3G_ENUM;
            ucState = NAS_RABM_GetWPsEntState(ucRabId - RABM_NSAPI_OFFSET);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_CreateRabMapEntity: Invalid system mode." );
            return;
    }

    /* 配置相应上行回调 */
    for (ulLoop = 0; ulLoop < (sizeof(g_astRabAsUlDataCallBack)/sizeof(RAB_AS_UL_DATA_CALLBACK_STRU)); ulLoop++)
    {
        if ( (g_astRabAsUlDataCallBack[ulLoop].ucMode == enRabmCurrNet)
          && (g_astRabAsUlDataCallBack[ulLoop].ucState == ucState) )
        {
            /* 找到相应RAB_MAP实体的上行回调 */
            pRabUlDataCallBack = g_astRabAsUlDataCallBack[ulLoop].pAsUlDataCallBack;
            break;
        }
    }

    /* 未找到匹配的上行回调 */
    if (ulLoop == (sizeof(g_astRabAsUlDataCallBack)/sizeof(RAB_AS_UL_DATA_CALLBACK_STRU)))
    {
        NAS_WARNING_LOG(WUEPS_PID_RABM,
            "NAS_RABM_CreateRabMapEntity: Can't find match pAsUlDataCallBack." );
        pRabUlDataCallBack = VOS_NULL_PTR;
    }

    /* 配置相应下行回调，统一使用CDS提供的下行回调 */
    pRabDlDataCallBack = NAS_RABM_DL_DataInd;

    RABM_CreateRabMap(ucRabId, ucLinkdRabId, ucAsId, pRabUlDataCallBack, pRabDlDataCallBack);

}

/*lint +e958*/

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif
