/* 
 * File:   mcal_ccp.h
 * Author: Ahmed Aseel
 *
 * Created on November 5, 2023, 6:18 PM
 */

#ifndef MCAL_CCP_H
#define	MCAL_CCP_H

/* Section: Includes */
#include "mcal_ccp_cfg.h"

/* Section: Macro Declarations */
#define CCP_MODULE_DISABLE                 ((uint8)0x00)
#define CCP_CAPTURE_MODE_1_FALLING_EDGE    ((uint8)0x04)
#define CCP_CAPTURE_MODE_1_RISING_EDGE     ((uint8)0x05)
#define CCP_CAPTURE_MODE_4_RISING_EDGE     ((uint8)0x06)
#define CCP_CAPTURE_MODE_16_RISING_EDGE    ((uint8)0x07)
#define CCP_COMPARE_MODE_SET_PIN_LOW       ((uint8)0x08)
#define CCP_COMPARE_MODE_SET_PIN_HIGH      ((uint8)0x09)
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH   ((uint8)0x02)
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT  ((uint8)0x0A)
#define CCP_COMPARE_MODE_GEN_EVENT         ((uint8)0x0B)
#define CCP_PWM_MODE                       ((uint8)0x0C)

#define CCP_CAPTURE_NOT_READY              0x00
#define CCP_CAPTURE_READY                  0x01

#define CCP_COMPARE_NOT_READY              0x00
#define CCP_COMPARE_READY                  0x01

#define CCP_TIMER2_POSTSCALER_DIV_BY_1       1
#define CCP_TIMER2_POSTSCALER_DIV_BY_2       2
#define CCP_TIMER2_POSTSCALER_DIV_BY_3       3
#define CCP_TIMER2_POSTSCALER_DIV_BY_4       4
#define CCP_TIMER2_POSTSCALER_DIV_BY_5       5
#define CCP_TIMER2_POSTSCALER_DIV_BY_6       6
#define CCP_TIMER2_POSTSCALER_DIV_BY_7       7
#define CCP_TIMER2_POSTSCALER_DIV_BY_8       8
#define CCP_TIMER2_POSTSCALER_DIV_BY_9       9
#define CCP_TIMER2_POSTSCALER_DIV_BY_10      10
#define CCP_TIMER2_POSTSCALER_DIV_BY_11      11
#define CCP_TIMER2_POSTSCALER_DIV_BY_12      12
#define CCP_TIMER2_POSTSCALER_DIV_BY_13      13
#define CCP_TIMER2_POSTSCALER_DIV_BY_14      14
#define CCP_TIMER2_POSTSCALER_DIV_BY_15      15
#define CCP_TIMER2_POSTSCALER_DIV_BY_16      16

#define CCP_TIMER2_PRESCALER_DIV_BY_1        1
#define CCP_TIMER2_PRESCALER_DIV_BY_4        4
#define CCP_TIMER2_PRESCALER_DIV_BY_16       16

/* Section: Macro Functions Declarations */
#define CCP1_SET_MODE(_CONFIG)             (CCP1CONbits.CCP1M = _CONFIG)
#define CCP2_SET_MODE(_CONFIG)             (CCP2CONbits.CCP2M = _CONFIG)

#define CCP1_CCP2_TIMER1_SELECT()          do{T3CONbits.T3CCP1 = 0;\
                                              T3CONbits.T3CCP2 = 0;\
                                           }while(0)

#define CCP1_TIMER1_CCP2_TIMER3_SELECT()   do{T3CONbits.T3CCP1 = 1;\
                                              T3CONbits.T3CCP2 = 0;\
                                           }while(0)

#define CCP1_CCP2_TIMER3_SELECT()          do{T3CONbits.T3CCP1 = 1;\
                                              T3CONbits.T3CCP2 = 1;\
                                           }while(0)

/* Section: Data type Declarations */
typedef union {
    struct {
        uint8 ccpr_low;
        uint8 ccpr_high;        
    };
    struct {
        uint16 ccpr_16bits;       
    };
}CCP_REG_t;

typedef enum {
    CCP1_INST = 0,
    CCP2_INST     
}ccp_inst_t;

typedef enum {
    CCP_CAPTURE_MODE_SELECT = 0,
    CCP_COMPARE_MODE_SELECT,
    CCP_PWM_MODE_SELECT      
}ccp_mode_t;

typedef enum {
    CCP1_CCP2_TIMER1 = 0,
    CCP1_TIMER1_CCP2_TIMER3,
    CCP1_CCP2_TIMER3      
}ccp_cpt_cmp_tmr_t;

typedef struct {
    #if (CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE) || \
        (CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE)
        InterruptHandler CCP_InterruptHandler;
        interrupt_priority_t priority;
    #endif    

    ccp_inst_t ccp_inst;    
    ccp_mode_t ccp_mode;
    uint8 ccp_mode_variant;
    ccp_cpt_cmp_tmr_t ccp_timer;
    pin_config_t ccp_pin;
    
    #if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED) || \
        (CCP2_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
        uint32 PWM_freq;
        uint8 timer2_prescaler;
        uint8 timer2_postscaler;
    #endif      
}ccp_config_t;

/* Section: Function Declarations */
Std_ReturnType mcal_CCP_Init(const ccp_config_t* _ccp_config);
Std_ReturnType mcal_CCP_DeInit(const ccp_config_t* _ccp_config);

#if (CCP1_CFG_MODE_SELECTED == CCP_CFG_CAPTURE_MODE_SELECTED) || \
    (CCP2_CFG_MODE_SELECTED == CCP_CFG_CAPTURE_MODE_SELECTED)
Std_ReturnType mcal_CCP_IsCapturedDataReady(const ccp_config_t* _ccp_config, uint8* capture_status);
Std_ReturnType mcal_CCP_Capture_Mode_Read_Value(const ccp_config_t* _ccp_config, uint16* capture_value);
#endif

#if (CCP1_CFG_MODE_SELECTED == CCP_CFG_COMPARE_MODE_SELECTED) || \
    (CCP2_CFG_MODE_SELECTED == CCP_CFG_COMPARE_MODE_SELECTED)
Std_ReturnType mcal_CCP_Compare_Mode_Set_Value(const ccp_config_t* _ccp_config, uint16 compare_value);
Std_ReturnType mcal_CCP_IsCompareComplete(const ccp_config_t* _ccp_config, uint8* compare_status);
#endif

#if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED) || \
    (CCP2_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
Std_ReturnType mcal_CCP_PWM_Set_Duty(const ccp_config_t* _ccp_config, const uint8 duty);
Std_ReturnType mcal_CCP_PWM_Start(const ccp_config_t* _ccp_config);
Std_ReturnType mcal_CCP_PWM_Stop(const ccp_config_t* _ccp_config);
#endif

#endif	/* MCAL_CCP_H */

