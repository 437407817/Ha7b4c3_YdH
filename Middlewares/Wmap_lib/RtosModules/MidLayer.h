#ifndef _MIDLAYER_H_
#define _MIDLAYER_H_

#include "mmc.h"

typedef struct 
        {
				 int temprature;
				} midlayer_sample_temprature_stru;

typedef struct 
        {
				 int bright;
				} midlayer_sample_bright_stru;
				
typedef struct 
        {
				 int light_sw;
				} midlayer_switch_light_stru;

typedef struct 
        {
				 int fan_sw;
				} midlayer_switch_fan_stru;
				
extern mmc_ctrl_stru midlayer_sample_tempraure_mmc_ctrl;
extern mmc_ctrl_stru midlayer_sample_bright_mmc_ctrl;
extern mmc_ctrl_stru midlayer_switch_light_mmc_ctrl;
extern mmc_ctrl_stru midlayer_switch_fan_mmc_ctrl;

extern midlayer_sample_temprature_stru midlayer_sample_temprature_values;
extern midlayer_sample_bright_stru     midlayer_sample_bright_values;

extern midlayer_switch_light_stru      midlayer_switch_light_sw;
extern midlayer_switch_fan_stru        midlayer_switch_fan_sw;
	

#endif