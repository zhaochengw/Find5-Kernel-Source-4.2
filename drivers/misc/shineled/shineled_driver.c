/*
 * Driver for the PN544 NFC chip.
 *
 * Copyright (C) Nokia Corporation
 *
 * Author: Jari Vanhala <ext-jari.vanhala@nokia.com>
 * Contact: Matti Aaltonen <matti.j.aaltonen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/regulator/consumer.h>

#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/sizes.h>

#include <linux/leds.h>
#include <linux/pcb_version.h>

#define SHINELED_DEBUG 1

#if SHINELED_DEBUG
	#define D(x...) pr_info("[shineled] " x)
#else
	#define D(x...)
#endif

#define I(x...) pr_info("[shineled] " x)
#define E(x...) pr_err("[shineled] " x)

#define SLED_IOCTL_MAGIC 'l'

#define SLED_ENABLE                  _IOW(SLED_IOCTL_MAGIC, 0, unsigned)	//enable led
#define SLED_DISABLE                 _IOW(SLED_IOCTL_MAGIC, 1, unsigned)	//disable led
/*
config register 01h
*/
#define SLED_CONFIG_FEATURE      _IOW(SLED_IOCTL_MAGIC, 2, unsigned)	//  stop flash or start flah
/*
config register 02h
*/
#define SLED_SET_WORKMOD      _IOW(SLED_IOCTL_MAGIC, 3, unsigned)	//set work mod
/*
config 0ah~18h
u8 t[6];	  
t[0]:channel
t[1]:T0
t[2]:T1
t[3]:T2
t[4]:T3
t[5]:T4
 */
#define SLED_SET_BTEATHTIME  _IOW(SLED_IOCTL_MAGIC, 4, unsigned)	//set breateh time T0~T4
/*
config register 05h
*/
#define SLED_SET_RED                 _IOW(SLED_IOCTL_MAGIC, 5, unsigned)	//set red
/*
config register 06h
*/
#define SLED_SET_GREEEN            _IOW(SLED_IOCTL_MAGIC, 6, unsigned)	//set green
/*
config register 04h
*/
#define SLED_SET_BLUE               _IOW(SLED_IOCTL_MAGIC, 7, unsigned)	//set blue
//#define SLED_SET_LASTTIME       _IOW(SLED_IOCTL_MAGIC, 8, unsigned) //set flahing time
#define SET_RED_BREATHE           _IOW(SLED_IOCTL_MAGIC, 8, unsigned)	//set red
#define SET_GREEN_BREATHE       _IOW(SLED_IOCTL_MAGIC, 9, unsigned)	//set green
#define SET_BLUE_BREATHE         _IOW(SLED_IOCTL_MAGIC, 10, unsigned)	//set blue

#define RED_SLED 0
#define GREEN_SLED 1
#define BLUE_SLED 2
#define SN3193_POWER_ON 1

struct SN3193_sled {
	struct miscdevice SN3193_miscdev;
	struct i2c_client *i2c_dev;
};
struct sled_dev_sate {
	struct i2c_client *gi2c_dev;
};
struct sled_dev_sate *SN3193_sled_dev_sate;

static struct regulator *ldo121;
static struct regulator *lvs5;

extern int get_pcb_version(void);
//add the buffer to save the data to write 

static int SN3193_write_reg(u16 reg, u8 val)
{
	u8 buf[3];
	buf[0] = reg & 0xff;
//	buf[1] = (reg >> 8) & 0xff;
	buf[1] = val;

	if (i2c_master_send(SN3193_sled_dev_sate->gi2c_dev, buf, 2) != 2) {
		E("%s: i2c send failed\n", __func__);
		return -EIO;
	}

	return 0;
}

/*static int SN3193_reset_sled(struct i2c_client *client)
{
    	int ret=0;
    	ret=SN3193_write_reg(client,0x2f,0xff);
	return ret;
}*/

static int SN3193_enable_sled(int on)
{
	int ret;
	if (on) {
		ret = SN3193_write_reg(0x00, 0x20);
	} else {
		// ret=SN3193_write_reg(client,0x2f,0x00);
		ret = SN3193_write_reg(0x00, 0x01);
	}
	return ret;
}

static int SN3193_config_feature_sled(u8 feature)
{
	int ret = 0;
	ret = SN3193_write_reg(0x01, feature);
	return 0;
}

/*
in single color breath mod should work in pwm mod rgb=0
or rgb=1
*/
static int SN3193_workmod_sled(int rgb)
{
	int ret = 0;
	if (rgb) {
		ret = SN3193_write_reg(0x02, 0x20);
	} else {
		ret = SN3193_write_reg(0x02, 0x00);
	}
	return ret;
}

//set max output current
static int SN3193_setCurrent_sled(u8 cs)
{
	int ret = 0;
	ret = SN3193_write_reg(0x03, 0x08);
	return ret;
}

/*static int SN3193_setColor_sled(u8 r,u8 g,u8 b) //r,g,b 0~255
{
       int ret=0;
	ret=SN3193_write_reg(0x04,r);
	ret=SN3193_write_reg(0x05,g);
	ret=SN3193_write_reg(0x06,b);
	return ret;
}*/
static int SN3193_upData_sled(void)	//update 04~06
{
	int ret = 0;
	ret = SN3193_write_reg(0x07, 0x00);
	return ret;
}

/*************************set RGB Led breath time**********************************/
//Ch UT1,OUT2,OUT3   Ch= 1,2,3;
//T0(time of holding off) T0= 0-15;        T0(D7-D4)  
//T1&T2(time of ramping up and hold on     T1(D7-D5)|T2(D4-D1)|X
//T3&T4(time of ramping down and hold off) T3(D7-D5)|T4(D4-D1)|X
/********************************************************************************/

static int SN3193_SetBreathTime_sled(u8 Ch, u8 T0, u8 T1, u8 T2, u8 T3, u8 T4)
{
	int ret = 0;
	switch (Ch) {
	case 1:
		SN3193_write_reg(0x0a, T0 << 4);	//T0(time of holding off)
		SN3193_write_reg(0x10, (T1 << 5) | (T2 << 1));	//T1&T2(time of ramping up and hold on
		SN3193_write_reg(0x16, (T3 << 5) | (T4 << 1));	//T3&T4(time of ramping down and hold off)
		break;
	case 2:
		SN3193_write_reg(0x0b, T0 << 4);
		SN3193_write_reg(0x11, (T1 << 5) | (T2 << 1));
		SN3193_write_reg(0x17, (T3 << 5) | (T4 << 1));
		break;
	case 3:
		SN3193_write_reg(0x0c, T0 << 4);
		SN3193_write_reg(0x12, (T1 << 5) | (T2 << 1));
		SN3193_write_reg(0x18, (T3 << 5) | (T4 << 1));
		break;

	}
	return ret;
}

static int SN3193_TimeUpdate_sled(void)
{
	int ret = 0;
	ret = SN3193_write_reg(0x1c, 0x00);
	return ret;
}

static int SN3193_TurnOnRGB_sled(void)
{
	int ret = 0;
	SN3193_write_reg(0x1D, 0x07);
	return ret;
}

/*********************************************/
//open Channel 1,2,3
/********************************************/
static int SN3193_TurnOnOut_sled(u8 Ch)
{
	int ret = 0;
	SN3193_write_reg(0x1D, (1 << Ch));
	return ret;
}

/*********************************************/
//close Channel 1,2,3
/********************************************/
static int SN3193_TurnOffOut_sled(void)
{
	int ret = 0;
	SN3193_write_reg(0x1D, 0x00);
	return ret;
}

static int SN3193_SetBrightness(int color, u8 brightness)
{
	int ret = 0;
	SN3193_write_reg(0x04 + color, brightness);
	return ret;
}

/*static int SN3193_enable_diff_color_sled(int color)
{
int ret =0;
     switch(color)
     	{
	 case RED_SLED:
	          SN3193_setColor_sled(0x0,0x05,0x0); //out1 ~B,out2 ~R,out3~G
	          SN3193_upData_sled();

	          SN3193_TurnOnOut_sled(color);
		  break;
	 case GREEN_SLED:
	          SN3193_setColor_sled(0x0,0x00,0x5); //out1 ~B,out2 ~R,out3~G
	          SN3193_upData_sled();

	          SN3193_TurnOnOut_sled(color);
		  break;
        case BLUE_SLED:
	          SN3193_setColor_sled(0x05,0x00,0x00); //out1 ~B,out2 ~R,out3~G
	          SN3193_upData_sled();

	          SN3193_TurnOnOut_sled(color);
		  break;
     	}
   return ret;
}*/

static int SN3193_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	struct SN3193_sled *info = container_of(file->private_data,
						struct SN3193_sled,
						SN3193_miscdev);
	file->private_data = info;
	D("%s:sn3193 open\n", __func__);
	return ret;
}

static long SN3193_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned int val;
	/*
	   t[0]:channel
	   t[1]:T0
	   t[2]:T1
	   t[3]:T2
	   t[4]:T3
	   t[5]:T4
	 */
	u8 t[6];
	long r = 0;

	switch (cmd) {
	case SLED_ENABLE:
		SN3193_enable_sled(1);
		SN3193_setCurrent_sled(0x01);
		D("%s: enable sled\n", __func__);
		break;
	case SLED_DISABLE:
		SN3193_TurnOffOut_sled();
		SN3193_enable_sled(0);
		D("%s: disable sled\n", __func__);
		break;
	case SLED_SET_WORKMOD:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		D("%s: set sled work mod ,val:%d\n", __func__, val);
		SN3193_workmod_sled(val);	//now we should set to val to 0
		break;
	case SLED_CONFIG_FEATURE:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		D("%s: set stagnate mod,val:%d\n", __func__, val);
		SN3193_config_feature_sled(val);
		break;
	case SLED_SET_BTEATHTIME:
		if (copy_from_user(t, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		D("%s: set stagnate modl:%d,%d,%d,%d,%d\n", __func__,
			 t[1], t[2], t[3], t[4], t[5]);
		SN3193_SetBreathTime_sled(t[0], t[1], t[2], t[3], t[4], t[5]);
		SN3193_TimeUpdate_sled();
		break;
	case SLED_SET_RED:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		SN3193_TurnOnOut_sled(RED_SLED);
		SN3193_SetBrightness(RED_SLED, val);
		SN3193_upData_sled();
		D("%s: set sled red,val:%d\n", __func__, val);
		break;
	case SLED_SET_GREEEN:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		SN3193_TurnOnOut_sled(GREEN_SLED);
		SN3193_SetBrightness(GREEN_SLED, val);
		SN3193_upData_sled();
		D("%s: set sled green,val:%d\n", __func__, val);
		break;
	case SLED_SET_BLUE:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		SN3193_TurnOnOut_sled(BLUE_SLED);
		SN3193_SetBrightness(BLUE_SLED, val);
		SN3193_upData_sled();
		D("%s: set sled blue,val:%d\n", __func__, val);
		break;
	/*
	   case SET_RED_BREATHE:case SET_GREEN_BREATHE:case SET_BLUE_BREATHE: 
	   just for debug in EngineeringMode
	 */
	case SET_RED_BREATHE:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		SN3193_TurnOnOut_sled(RED_SLED);
		SN3193_SetBrightness(RED_SLED, val);

		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(1);
		SN3193_setCurrent_sled(0x01);

		SN3193_SetBreathTime_sled(1, 0, 1, 2, 1, 4);

		SN3193_TimeUpdate_sled();
		SN3193_upData_sled();
		break;
	case SET_GREEN_BREATHE:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		SN3193_TurnOnOut_sled(GREEN_SLED);
		SN3193_SetBrightness(GREEN_SLED, val);

		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(1);
		SN3193_setCurrent_sled(0x01);

		SN3193_SetBreathTime_sled(2, 0, 1, 2, 1, 4);
		SN3193_TimeUpdate_sled();
		SN3193_upData_sled();
		break;
	case SET_BLUE_BREATHE:
		if (copy_from_user
		    (&val, (void __user *)arg, sizeof(unsigned int))) {
			r = -EFAULT;
		}
		SN3193_TurnOnOut_sled(BLUE_SLED);
		SN3193_SetBrightness(BLUE_SLED, val);

		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(1);
		SN3193_setCurrent_sled(0x01);

		SN3193_SetBreathTime_sled(3, 0, 1, 2, 1, 4);
		SN3193_TimeUpdate_sled();
		SN3193_upData_sled();
		break;
	default:
		E("%s: Unknown ioctl 0x%x\n", __func__, cmd);
		r = -ENOIOCTLCMD;
		break;
	}
	return r;
}

/* OPPO 2012-07-20 liuhd Add begin for reason */
static int SN3193_power(int on)
{
	if (on) {
		if (get_pcb_version() <= PCB_VERSION_EVT) {
			//evt---lvs5
			lvs5 = regulator_get(NULL, "8921_lvs5");
			if (IS_ERR(lvs5)) {
				E("%s: VREG LVS5 get failed\n", __func__);
				lvs5 = NULL;
				goto lvs5_get_failed;
			}
			if (regulator_enable(lvs5)) {
				E("%s: VREG LVS5 enable failed\n",
				       __func__);
				goto lvs5_get_failed;
			}
		} else {
			//dvt---l21
			ldo121 = regulator_get(NULL, "8921_l21");
			if (IS_ERR(ldo121)) {
				E("%s: VREG ldo121 get failed\n",
				       __func__);
				ldo121 = NULL;
				goto ldo121_get_failed;
			}
			if (regulator_set_voltage(ldo121, 1800000, 1800000)) {
				E("%s: VREG ldo121 set voltage failed\n",
				       __func__);
				goto ldo121_get_failed;
			}
			if (regulator_enable(ldo121)) {
				E("%s: VREG ldo121 enable failed\n",
				       __func__);
				goto ldo121_get_failed;
			}
		}
	} else if (!on) {
		if (get_pcb_version() <= PCB_VERSION_EVT) {	//evt
			if (lvs5) {
				regulator_disable(lvs5);
				regulator_put(lvs5);
			}
		} else {	//dvt
			if (ldo121) {
				regulator_disable(ldo121);
				regulator_put(ldo121);
			}
		}
	}
	return 0;

 ldo121_get_failed:
	regulator_disable(ldo121);
 lvs5_get_failed:
	regulator_put(lvs5);

	return -1;
}

int totalMS, onMS;
int color_R, color_G, color_B;
u8 get_register_t(int MS)
{
	u8 t;
	static int table[] = { 130, 260, 520, 1040, 2080, 4160, 8320, 16640 };
	t = 0;

	while (MS >= table[t++] && t < 8) ;

	t--;

	if ((t > 0) && (MS << 1) < (table[t] + table[t - 1])) {
		t--;
	}
	return t;
}

static ssize_t sled_grppwm_store(struct device *dev,
				 struct device_attribute *attr, const char *buf,
				 size_t count)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	D("%s: buf = %s: count = %d\n", __func__, buf, count);

	onMS = value * totalMS / 255;

	return count;
}

static ssize_t sled_grpfreq_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	D("%s: buf = %s: count = %d\n", __func__, buf, count);

	totalMS = value * 50;

	if (totalMS > 35000)
		totalMS = 35000;
	return count;
}

static ssize_t sled_blink_store(struct device *dev,
				struct device_attribute *attr, const char *buf,
				size_t count)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	u8 t123, t4;
	
	//if(value == 1 && totalMS && onMS)
	if (~(~value) && totalMS && onMS) {

		SN3193_TurnOnRGB_sled();	//turn on the RGB color

		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(1);	//select the program mode, for breath effect.

		SN3193_setCurrent_sled(0x01);

		t123 = get_register_t(onMS / 3);
		t4 = get_register_t(totalMS - onMS) + 1;

		D("%s: t123 = %d: t4 = %d\n", __func__, t123, t4);
		SN3193_SetBreathTime_sled(1, 0, t123, t123 + 1, t123, t4);
		SN3193_SetBreathTime_sled(2, 0, t123, t123 + 1, t123, t4);
		SN3193_SetBreathTime_sled(3, 0, t123, t123 + 1, t123, t4);

		SN3193_TimeUpdate_sled();	//start breath  

		SN3193_upData_sled();	//turn on the light

	} else {
		D("%s: color_R = %d: color_G = %d: color_B = %d,\n", __func__, color_R, color_G,color_B);
		if (color_R + color_G + color_B == 0) {
			//close
			SN3193_TurnOffOut_sled();
			SN3193_enable_sled(0);
		} else {
			SN3193_TurnOnRGB_sled();	//turn on the RGB color
			//light aways
			SN3193_enable_sled(1);
			SN3193_config_feature_sled(0);
			SN3193_workmod_sled(0);	//select the RGB mode,          
			SN3193_setCurrent_sled(0x01);
			SN3193_upData_sled();	//turn on the light
		}
	}

	return count;
}

/*OPPO yuyi add */

static ssize_t sled_showing(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t count)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	//printk("yuyi----%s:   buf = %s: count = %d\n", __func__, buf, count);

	if (value >= 100) {

		color_G = 255;
		color_B = 0;
		color_R = 0;

		SN3193_SetBrightness(RED_SLED, color_R);
		SN3193_SetBrightness(GREEN_SLED, color_G);
		SN3193_SetBrightness(BLUE_SLED, color_B);

		SN3193_TurnOnRGB_sled();	//turn on the RGB color 
		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(0);	//select the RGB mode, 
		SN3193_upData_sled();	//turn on the light

		return count;
	} else if ((value < 100) && (value > 20)) {
		color_B = 255;
		color_G = 0;
		color_R = 0;

		SN3193_SetBrightness(RED_SLED, color_R);
		SN3193_SetBrightness(GREEN_SLED, color_G);
		SN3193_SetBrightness(BLUE_SLED, color_B);

		SN3193_TurnOnRGB_sled();	//turn on the RGB color 
		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(0);	//select the RGB mode, 
		SN3193_upData_sled();	//turn on the light

		return count;
	} else {
		color_B = 0;
		color_G = 0;
		color_R = 255;

		SN3193_SetBrightness(RED_SLED, color_R);
		SN3193_SetBrightness(GREEN_SLED, color_G);
		SN3193_SetBrightness(BLUE_SLED, color_B);

		SN3193_TurnOnRGB_sled();	//turn on the RGB color 
		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(0);	//select the RGB mode, 
		SN3193_upData_sled();	//turn on the light

		return count;

	}

}

static ssize_t sled_charging(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t count)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);
	u8 t123, t4;

	if (value >= 100) {

		color_G = 255;
		color_B = 0;
		color_R = 0;

		SN3193_SetBrightness(RED_SLED, color_R);
		SN3193_SetBrightness(GREEN_SLED, color_G);
		SN3193_SetBrightness(BLUE_SLED, color_B);

		SN3193_TurnOnRGB_sled();	//turn on the RGB color

		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(0);	//select the RGB mode, 
		SN3193_setCurrent_sled(0x01);
		SN3193_upData_sled();	//turn on the light

		return count;
	} else {
		color_B = 0;
		color_G = 255;
		color_R = 255;

		SN3193_SetBrightness(RED_SLED, color_R);
		SN3193_SetBrightness(GREEN_SLED, color_G);
		SN3193_SetBrightness(BLUE_SLED, color_B);

		SN3193_TurnOnRGB_sled();	//turn on the RGB color

		SN3193_enable_sled(1);
		SN3193_config_feature_sled(0);
		SN3193_workmod_sled(1);	//select the RGB mode, 
		SN3193_setCurrent_sled(0x01);

		t123 = get_register_t(428);
		t4 = get_register_t(2764) + 1;

		SN3193_SetBreathTime_sled(1, 0, t123, t123 + 1, t123, t4);
		SN3193_SetBreathTime_sled(2, 0, t123, t123 + 1, t123, t4);
		SN3193_SetBreathTime_sled(3, 0, t123, t123 + 1, t123, t4);

		SN3193_TimeUpdate_sled();	//start breath  
		SN3193_upData_sled();	//turn on the light

		return count;
	}
}

static ssize_t sled_reset(struct device *dev, struct device_attribute *attr,
			  const char *buf, size_t count)
{
	unsigned long value = simple_strtoul(buf, NULL, 10);

	if (value == 1) {
		SN3193_TurnOffOut_sled();
		SN3193_enable_sled(0);
		return count;
	} else {

		return count;

	}

}

static ssize_t sled_test(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	SN3193_write_reg(0x04, 0xff);
	SN3193_write_reg(0x05, 0xff);
	SN3193_write_reg(0x06, 0xff);

	SN3193_write_reg(0x1D, 0x07);

	SN3193_write_reg(0x00, 0x20);
	SN3193_write_reg(0x01, 0x00);
	SN3193_write_reg(0x02, 0x20);

	SN3193_write_reg(0x03, 0x08);

	SN3193_write_reg(0x0a, 0x00);
	SN3193_write_reg(0x10, 0x68);
	SN3193_write_reg(0x16, 0x68);

	SN3193_write_reg(0x0b, 0x00);
	SN3193_write_reg(0x11, 0x68);
	SN3193_write_reg(0x17, 0x68);

	SN3193_write_reg(0x0c, 0x00);
	SN3193_write_reg(0x12, 0x68);
	SN3193_write_reg(0x18, 0x68);

	SN3193_write_reg(0x1c, 0x00);

	SN3193_write_reg(0x07, 0x00);

	return count;

}

static DEVICE_ATTR(ledtest, S_IWUSR | S_IRUGO, NULL, sled_test);
static DEVICE_ATTR(ledreset, S_IWUSR | S_IRUGO, NULL, sled_reset);

static DEVICE_ATTR(showing, S_IWUSR | S_IRUGO, NULL, sled_showing);

static DEVICE_ATTR(charging, S_IWUSR | S_IRUGO, NULL, sled_charging);

static DEVICE_ATTR(grppwm, S_IWUSR | S_IRUGO, NULL, sled_grppwm_store);

static DEVICE_ATTR(grpfreq, S_IWUSR | S_IRUGO, NULL, sled_grpfreq_store);

static DEVICE_ATTR(blink, S_IWUSR | S_IRUGO, NULL, sled_blink_store);

static struct attribute *blink_attributes[] = {
	&dev_attr_grppwm.attr,
	&dev_attr_grpfreq.attr,
	&dev_attr_blink.attr,
	&dev_attr_charging.attr,
	&dev_attr_showing.attr,
	&dev_attr_ledreset.attr,
	&dev_attr_ledtest.attr,

	NULL
};

static const struct attribute_group blink_attr_group = {
	.attrs = blink_attributes,
};

static void lcds_set_brightness(struct led_classdev *led_cdev,
				enum led_brightness value)
{
	D("%s:\n", __func__);
	if (!strcmp(led_cdev->name, "red")) {
		D("%s: red - value=%d\n", __func__, value);
		SN3193_SetBrightness(RED_SLED, value);
		color_R = value;
	}
	if (!strcmp(led_cdev->name, "green")) {
		D("%s: green - value=%d\n", __func__, value);
		SN3193_SetBrightness(GREEN_SLED, value);
		color_G = value;
	}
	if (!strcmp(led_cdev->name, "blue")) {
		D("%s: blue - value=%d\n", __func__, value);
		SN3193_SetBrightness(BLUE_SLED, value);
		color_B = value;
	}

}

static const struct file_operations SN3193_fops = {
	.owner = THIS_MODULE,
	.open = SN3193_open,
	.unlocked_ioctl = SN3193_ioctl,
};

static struct led_classdev SN3193_lcds[] = {
	{
		.name = "red",
		.brightness_set = lcds_set_brightness,
	},
	{
		.name = "green",
		.brightness_set = lcds_set_brightness,
	},
	{
		.name = "blue",
		.brightness_set = lcds_set_brightness,
	},
};

static int SN3193_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int ret = 0;
	int i;
	struct SN3193_sled *sn3193_sled_dev;

	SN3193_power(SN3193_POWER_ON);
	msleep(100);
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		E("%s : need I2C_FUNC_I2C\n", __func__);
		return -ENODEV;
	}
	SN3193_sled_dev_sate->gi2c_dev = client;
	sn3193_sled_dev = kzalloc(sizeof(struct SN3193_sled), GFP_KERNEL);
	if (sn3193_sled_dev == NULL) {
		E("%s: failed to allocate memory for module data\n", __func__);
		ret = -ENOMEM;
		goto err_exit;
	}

	sn3193_sled_dev->i2c_dev = client;
	sn3193_sled_dev->SN3193_miscdev.minor = MISC_DYNAMIC_MINOR;
	sn3193_sled_dev->SN3193_miscdev.name = "SN3193";
	sn3193_sled_dev->SN3193_miscdev.fops = &SN3193_fops;

	ret = misc_register(&sn3193_sled_dev->SN3193_miscdev);
	if (ret) {
		E("%s : misc_register failed\n", __func__);
		goto err_misc_register;
	}

	for (i = 0; i < 3; i++) {
		if (led_classdev_register(&client->dev, &SN3193_lcds[i])) {
			E("%s: led_classdev_register failed of SN3193_lcds!\n", __func__);
			goto err_classdev_register;
		}
	}

	ret = sysfs_create_group(&client->dev.kobj, &blink_attr_group);
	if (ret) {
		E("%s : sysfs_create_group failed!\n", __func__);
		goto err_group_register;
	}

	i2c_set_clientdata(client, sn3193_sled_dev);

	//SN3193_config_feature_sled(0);                //dvt not breath

	//close, light as need
	SN3193_SetBrightness(RED_SLED, 0);
	SN3193_SetBrightness(GREEN_SLED, 0);
	SN3193_SetBrightness(BLUE_SLED, 0);
	SN3193_upData_sled();
	SN3193_TurnOffOut_sled();
	SN3193_enable_sled(0);


	I("%s: OK\n", __func__);
	return 0;
 err_group_register:
	for (i = 0; i < 3; i++)
		led_classdev_unregister(&SN3193_lcds[i]);
 err_classdev_register:
	misc_deregister(&sn3193_sled_dev->SN3193_miscdev);
 err_misc_register:
	kfree(sn3193_sled_dev);
 err_exit:
	return ret;
}

static int __devexit SN3193_remove(struct i2c_client *client)
{
	struct SN3193_sled *sn3193_sled_dev;
	int i;
	sn3193_sled_dev = i2c_get_clientdata(client);
	sysfs_remove_group(&client->dev.kobj, &blink_attr_group);
	for (i = 0; i < 3; i++)
		led_classdev_unregister(&SN3193_lcds[i]);
	misc_deregister(&sn3193_sled_dev->SN3193_miscdev);
	kfree(sn3193_sled_dev);

	kfree(SN3193_sled_dev_sate);
	return 0;
}

static const struct i2c_device_id SN3193_id[] = {
	{"SN3193", 0},
	{}
};

static struct i2c_driver SN3193_driver = {
	.id_table = SN3193_id,
	.probe = SN3193_probe,
	.remove = SN3193_remove,
	.driver = {
		   .owner = THIS_MODULE,
		   .name = "SN3193",
		   },
};

/*
 * module load/unload record keeping
 */

static int __init SN3193_dev_init(void)
{
	int ret = 0;
	SN3193_sled_dev_sate = kzalloc(sizeof(struct SN3193_sled), GFP_KERNEL);
	if (SN3193_sled_dev_sate == NULL) {
		ret = -ENOMEM;
		goto err_exit;
	}
	I("%s:\n", __func__);
	return i2c_add_driver(&SN3193_driver);
 err_exit:
	return ret;
}

module_init(SN3193_dev_init);

static void __exit SN3193_dev_exit(void)
{
	I("%s:\n", __func__);
	i2c_del_driver(&SN3193_driver);
}

module_exit(SN3193_dev_exit);

MODULE_AUTHOR("yanghai");
MODULE_DESCRIPTION("SN3193LED driver");
MODULE_LICENSE("GPL");
