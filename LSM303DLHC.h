#ifndef __LSM303DLHC_H__
#define __LSM303DLHC_H__

/**
 * @brief  LSM303DLHC main working structure
 */
typedef struct
{
  int16_t X;
  int16_t Y;
  volatile int16_t Z;
}LSM303DLHC_Data_t;


typedef enum {READ, WRITE}read_write_typedef;
typedef enum {ACK, NACK}ack_typedef;

/* Fuctions prototypes */
static void I2C_Start (void);
static void I2C_Stop (void);
static void I2C_Addr(uint8_t, read_write_typedef);
static void I2C_Clear_Addr_Flag(void);
static void I2C_Write (uint8_t);
static uint8_t I2C_Read (ack_typedef ack_nack);
static void LSM303DLHC_set_reg(uint8_t);
void LSM303DLHC_write_reg (uint8_t, uint8_t);
void LSM303DLHC_read_reg (uint8_t, uint8_t*, uint8_t);
void LSM303DLHC_init (void);
void LSM303DLHC_read_rates (LSM303DLHC_Data_t*);

/* LSM303DLHC registers */
#define LSM303DLHC_CTRL_REG1_A			0x20
#define LSM303DLHC_CTRL_REG2_A			0x21
#define LSM303DLHC_CTRL_REG3_A			0x22
#define LSM303DLHC_CTRL_REG4_A			0x23
#define LSM303DLHC_CTRL_REG5_A			0x24
#define LSM303DLHC_CTRL_REG6_A			0x25
#define LSM303DLHC_REFFERENCE_A			0x26
#define LSM303DLHC_STATUS_REG_A			0x27
#define LSM303DLHC_OUT_X_L_A			0x28
#define LSM303DLHC_OUT_X_H_A			0x29
#define LSM303DLHC_OUT_Y_L_A			0x2A
#define LSM303DLHC_OUT_Y_H_A			0x2B
#define LSM303DLHC_OUT_Z_L_A			0x2C
#define LSM303DLHC_OUT_Z_H_A			0x2D
#define LSM303DLHC_FIFO_CTRL_REG_A		0x2E
#define LSM303DLHC_FIFO_SRC_REG_A		0x2F
#define LSM303DLHC_INT1_CFG_A   		0x30
#define LSM303DLHC_INT1_SOURCE_A   		0x31
#define LSM303DLHC_INT1_THS_A   		0x32
#define LSM303DLHC_INT1_DURATION_A   		0x33
#define LSM303DLHC_INT2_CFG_A   		0x34
#define LSM303DLHC_INT2_SOURCE_A   		0x35
#define LSM303DLHC_INT2_THS_A   		0x36
#define LSM303DLHC_INT2_DURATION_A   		0x37
#define LSM303DLHC_CLICK_CFG_A   		0x38
#define LSM303DLHC_CLICK_SRC_A   		0x39
#define LSM303DLHC_CLICK_THS_A   		0x3A
#define LSM303DLHC_TIME_LIMIT_A   		0x3B
#define LSM303DLHC_TIME_LATENCY_A   		0x3C
#define LSM303DLHC_TIME_WINDOW_A   		0x3D
#define LSM303DLHC_CRA_REG_M       		0x00
#define LSM303DLHC_CRB_REG_M       		0x01
#define LSM303DLHC_MR_REG_M       		0x02
#define LSM303DLHC_OUT_X_L_M			0x03
#define LSM303DLHC_OUT_X_H_M			0x04
#define LSM303DLHC_OUT_Y_L_M			0x05
#define LSM303DLHC_OUT_Y_H_M			0x06
#define LSM303DLHC_OUT_Z_L_M			0x07
#define LSM303DLHC_OUT_Z_H_M			0x08
#define LSM303DLHC_SR_REG_Mg			0x09
#define LSM303DLHC_IRA_REG_Mg			0x0A
#define LSM303DLHC_IRB_REG_Mg			0x0B
#define LSM303DLHC_IRC_REG_Mg			0x0C
#define LSM303DLHC_TEMP_OUT_H_M			0x31
#define LSM303DLHC_TEMP_OUT_L_M			0x32

#define LSM303D_ADDR			        0x32
#define X_A_Enable                              0x01
#define Y_A_Enable                              0x02
#define Z_A_Enable                              0x04
#define Data_rate_100Hz                         0x50
#define HIGH_RESOLUTION_EN                      0X08


#endif //__LSM303DLHC_H__