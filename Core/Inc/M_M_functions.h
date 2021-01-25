//#include "i2c-lcd.h"
#include "tim.h"
#include "arm_math.h"
#include "math_helper.h"
#include "i2c-lcd.h"
#include "bh1750.h"
#include "stdio.h"
#include "string.h"

//void SetValue(int in, int out, arm_pid_instance_f32 * PID_INSTANCE);
int ReadData();
void RefreshLCD(float in, float out);
void RefreshLCD_3v(float in, float out, int nr3);
