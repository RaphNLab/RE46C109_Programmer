#ifndef CS43L22_CODEC_H_
#define CS43L22_CODEC_H_

#include "global.h"

#define CS43L22_CODEC_ID 			0x4AU

#define CS43L22_CHIP_ID				0x01U
#define CS43L22_PWR_CTL_1			0x02U
#define CS43L22_PWR_CTL_2			0x04U
#define CS43L22_CLK_CTL				0x05U
#define CS43L22_IF_CTL_1			0x06U
#define CS43L22_IF_CTL_2			0x07U
#define CS43L22_PASSTH_SEL_A		0x08U
#define CS43L22_PASSTH_SEL_B		0x09U
#define CS43L22_ANALOG_ZC_SR		0x0AU
#define CS43L22_PASSTH_G_CTL		0x0CU
#define CS43L22_PLAYBACK_CTL_1		0x0DU
#define CS43L22_MISC_CTL			0x0EU
#define CS43L22_PLAYBACK_CTL_2		0x0FU
#define CS43L22_PASSTH_VOL_A		0x14U
#define CS43L22_PASSTH_VOL_b		0x15U
#define CS43L22_PCMB_VOL_A			0x1AU
#define CS43L22_PCMB_VOL_B			0x1BU
#define CS43L22_BEEP_FREQ_ON_TIME	0x1CU
#define CS43L22_BEEP_VOL_OFF_TIME	0x1DU
#define CS43L22_BEEP_TONE_CFG		0x1EU
#define CS43L22_BEEP_TONE_CTL		0x1FU
#define CS43L22_MASTER_VOL_A		0x20U
#define CS43L22_MASTER_VOL_B		0x21U
#define CS43L22_HP_VOL_A			0x22U
#define CS43L22_HP_VOL_B			0x23U
#define CS43L22_SPEAK_VOL_A			0x24U
#define CS43L22_SPEAK_VOL_B			0x25U
#define CS43L22_CH_MIX_SWAP			0x26U
#define CS43L22_LIM_CTL1_THR		0x27U
#define CS43L22_LIM_CTL2_R_RATE		0x28U
#define CS43L22_LIM_ATTACK_RATE		0x29U
#define CS43L22_OV_CLK_STATUS		0x2EU
#define CS43L22_BAT_COMP			0x2FU
#define CS43L22_VP_BAT_LEVEL		0x30U
#define CS43L22_SPEAKER_STATUS		0x31U
#define CS43L22_CHARGE_PUMP_FREQ	0x32U



#define I2C_SCL		GPIO6	//PB6
#define I2C_SDA		GPIO9	//PB9

#define I2S_MCLK	GPIO7	//PC7
#define I2S_SCLK	GPIO10	//PC10
#define I2S_SDIN	GPIO12	//PC12
#define CS43L22_RESET_PIN	GPIO4	//PD4
#define CS43L22_LRCK_PIN	GPIO4	//PA4



void codec_init(void);
void codec_configure(void);
void codec_generate_MCLK(void);
uint8_t codec_read(void);
void codec_write(void);
void codec_set_volume(uint16_t vol);
uint32_t codec_get_volume(void);
void codec_decrease_volume(void);
void codec_increase_volume(void);
void codec_set_mute(void);

void i2c_config(void);
void i2s_config(void);


#endif
