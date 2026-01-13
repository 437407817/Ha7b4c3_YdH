/**
  ******************************************************************************
  * @file    bsp_touch_gtxx.c
  * @brief   该文件仅仅用来读取GTxxx触摸芯片的ID号,用以区分不同的RGB屏幕
  ******************************************************************************
  */ 
#include <stdio.h>
#include <string.h>
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/bsp_i2c_touch.h"
#include "./lcd/bsp_lcd.h"


/* 触摸IC类型默认为5寸屏的ic */
TOUCH_IC touchIC = GT9157;		

const TOUCH_PARAM_TypeDef touch_param[TOUCH_TYPE_NUM] = 
{
  /* GT9157,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT911,7寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT5688,4.3寸屏 */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  },
	 /* GT917S,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8050,
  },
  /* GT615,7寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047, //该起始地址与GT911的一样
  },

#if TOUCH_GT1151QM_LCD_4_3
  /* GT1151QM,4.3寸屏 */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  },
#elif TOUCH_GT1151QM_LCD_5
  /* GT1151QM,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8050,
  },
#endif
};				

static int8_t GTP_I2C_Test(void);


/**
  * @brief   使用IIC进行数据传输
  * @param
  *		@arg i2c_msg:数据传输结构体
  *		@arg num:数据传输结构体的个数
  * @retval  正常完成的传输结构个数，若不正常，返回0xff
  */
static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
	int im = 0;
	int ret = 0;

//	 

	for (im = 0; ret == 0 && im != num; im++)
	{
		if ((msgs[im].flags&I2C_M_RD))																//根据flag判断是读数据还是写数据
		{
			ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IIC读取数据
		} else
		{
			ret = I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IIC写入数据
		}
	}
	if(ret)
		return ret;

	return im;   													//正常完成的传输结构个数
}

/**
  * @brief   从IIC设备中读取数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 读取数据寄存器的起始地址
  *		@arg buf[2~len-1]: 存储读出来数据的缓冲buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count（寄存器地址长度+读取的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，2为成功，其它为失败
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

//     
    /*一个读数据的过程可以分为两个传输过程:
     * 1. IIC  写入 要读取的寄存器地址
     * 2. IIC  读取  数据
     * */

    msgs[0].flags = !I2C_M_RD;					//写入
    msgs[0].addr  = client_addr;					//IIC设备地址
    msgs[0].len   = GTP_ADDR_LENGTH;	//寄存器地址为2字节(即写入两字节的数据)
    msgs[0].buf   = &buf[0];						//buf[0~1]存储的是要读取的寄存器地址
    
    msgs[1].flags = I2C_M_RD;					//读取
    msgs[1].addr  = client_addr;					//IIC设备地址
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//要读取的数据长度
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]之后的缓冲区存储读出的数据

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//调用IIC数据传输过程函数，有2个传输过程
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
        GTP_ERROR("I2C Read: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);
    }
    return ret;
}



/**
  * @brief   向IIC设备写入数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 要写入的数据寄存器的起始地址
  *		@arg buf[2~len-1]: 要写入的数据
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count（寄存器地址长度+写入的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，1为成功，其它为失败
  */
static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

//     
    /*一个写数据的过程只需要一个传输过程:
     * 1. IIC连续 写入 数据寄存器地址及数据
     * */
    msg.flags = !I2C_M_RD;			//写入
    msg.addr  = client_addr;			//从设备地址
    msg.len   = len;							//长度直接等于(寄存器地址长度+写入的数据字节数)
    msg.buf   = buf;						//直接连续写入缓冲区中的数据(包括了寄存器地址)

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	//调用IIC数据传输过程函数，1个传输过程
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {

        GTP_ERROR("I2C Write: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);

    }
    return ret;
}


/**
  * @brief   使用IIC读取再次数据，检验是否正常
  * @param
  *		@arg client:设备地址
  *		@arg  addr: 寄存器地址
  *		@arg rxbuf: 存储读出的数据
  *		@arg len:    读取的字节数
  * @retval
  * 	@arg FAIL
  * 	@arg SUCCESS
  */
 int32_t GTP_I2C_Read_dbl_check(uint8_t client_addr, uint16_t addr, uint8_t *rxbuf, int len)
{
    uint8_t buf[16] = {0};
    uint8_t confirm_buf[16] = {0};
    uint8_t retry = 0;
    
//     

    while (retry++ < 3)
    {
        memset(buf, 0xAA, 16);
        buf[0] = (uint8_t)(addr >> 8);
        buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, buf, len + 2);
        
        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (uint8_t)(addr >> 8);
        confirm_buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, confirm_buf, len + 2);

      
        if (!memcmp(buf, confirm_buf, len+2))
        {
            memcpy(rxbuf, confirm_buf+2, len);
            return SUCCESS;
        }
    }    
    GTP_ERROR("I2C read 0x%04X, %d bytes, double check failed!", addr, len);
    return FAIL;
}


/**
  * @brief   关闭GT91xx中断
  * @param 无
  * @retval 无
  */
void GTP_IRQ_Disable(void)
{

//     

    I2C_GTP_IRQDisable();
}

/**
  * @brief   使能GT91xx中断
  * @param 无
  * @retval 无
  */
void GTP_IRQ_Enable(void)
{
//     
     
	  I2C_GTP_IRQEnable();    
}


static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[6] = {0};
    int32_t ret = 0;

    uint16_t abs_x_max = GTP_MAX_WIDTH;
    uint16_t abs_y_max = GTP_MAX_HEIGHT;
    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 6);
    if (ret < 0)
    {
        return FAIL;
    }
    
    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
    
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
    int_trigger_type = opr_buf[2] & 0x03;
    
    GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
            abs_x_max,abs_y_max,int_trigger_type);
    
    return SUCCESS;    
}


/* 初始化触摸芯片,然后读取触摸芯片ID以判断不同尺寸类型的屏幕
   保存当前的液晶屏类型到变量 cur_lcd */
  
int32_t GTP_Init_ReadID(void)
{
    int32_t ret = -1;

//     


    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
        GTP_ERROR("I2C communication ERROR!");
				return ret;
    } 
		
		//获取触摸IC的型号
    GTP_Read_Version(); 
    
    GTP_Get_Info();

    return 0;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //寄存器地址

//     

    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed");
        return ret;
    }

#ifdef LCD_TOUCH_IC_GT1151QM
    //GT1151QM芯片
    if (buf[2]=='1' && buf[3]=='1' && buf[4]=='5')
    {
        GTP_INFO("IC Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);
        touchIC = GT1151QM;
        
        #if TOUCH_GT1151QM_LCD_4_3
        cur_lcd = INCH_4_3;     //设置当前的液晶屏类型
        #elif TOUCH_GT1151QM_LCD_5
        cur_lcd = INCH_5;       //设置当前的液晶屏类型
        #endif
    }
    else
    {
        GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
    }
#else

    if (buf[4] == '1')
    {				
				//GT911芯片
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
        {
          GTP_INFO("IC1 Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);

					touchIC = GT911;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_7;
        }
        //GT9157芯片
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
		}
		else if (buf[4] == '5')
		{
			if( buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
        {
          GTP_INFO("IC2 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT9157;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_5;
        }
        //GT615芯片
        else if( buf[2] == '6' && buf[3] == '1' && buf[4] == '5')
        {
          GTP_INFO("IC3 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT615;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_7;
        }
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    }    
    else if (buf[4] == '8')
    {	
				//GT5688芯片
				if(buf[2] == '5' && buf[3] == '6' && buf[4] == '8' && buf[5] == '8')
        {
          GTP_INFO("IC4 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT5688;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_4_3;
        }
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    }
		else if(buf[4] == '7')
    {
			  //GT917S芯片
         GTP_INFO("IC5 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
				
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '7' && buf[5] == 'S')
				{	
					touchIC = GT917S; 
					/* 设置当前的液晶屏类型 */
          cur_lcd = INCH_5;	
				}					
    }
    else 
       GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

#endif

    return ret;
}

/*******************************************************
Function:
    Initialize gtp.
Input:
    ts: goodix private data
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
 int32_t GTP_Init_Panel(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint8_t check_sum = 0;
    int32_t retry = 0;
    uint8_t* config;

    uint8_t* cfg_info;
    uint8_t cfg_info_len  ;

    uint8_t cfg_num =0x80FE-0x8047+1 ;		//需要配置的寄存器个数

//     


    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
        GTP_ERROR("I2C communication ERROR!");
				return ret;
    } 
		
		//获取触摸IC的型号
    GTP_Read_Version(); 
    
#if UPDATE_CONFIG
		
    config = (uint8_t *)malloc (GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH);

		config[0] = GTP_REG_CONFIG_DATA >> 8;
		config[1] =  GTP_REG_CONFIG_DATA & 0xff;
    
		//根据IC的型号指向不同的配置
		if(touchIC == GT9157)
		{
			cfg_info =  CTP_CFG_GT9157; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT9157);//计算配置表的大小
		}
		else if(touchIC == GT911)
		{
			cfg_info =  CTP_CFG_GT911;//指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT911) ;//计算配置表的大小
		}
		else if(touchIC == GT5688)			
		{
			cfg_info =  CTP_CFG_GT5688; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT5688);//计算配置表的大小
		}
		else if(touchIC == GT917S)
		{
			cfg_info =  CTP_CFG_GT917S; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT917S);//计算配置表的大小
		}
    else if(touchIC == GT615)
		{
			cfg_info =  CTP_CFG_GT615; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT615);//计算配置表的大小
		}
        else if(touchIC == GT1151QM)
		{
			cfg_info =  CTP_CFG_GT1151QM; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT1151QM);//计算配置表的大小
		}

    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info, cfg_info_len);
		
		cfg_num = cfg_info_len;
		
		GTP_DEBUG("cfg_info_len = %d ",cfg_info_len);
		GTP_DEBUG("cfg_num = %d ",cfg_num);
		GTP_DEBUG_ARRAY(config,6);
		
		/*根据LCD的扫描方向设置分辨率*/
		config[GTP_ADDR_LENGTH+1] = LCD_PIXEL_WIDTH & 0xFF;
		config[GTP_ADDR_LENGTH+2] = LCD_PIXEL_WIDTH >> 8;
		config[GTP_ADDR_LENGTH+3] = LCD_PIXEL_HEIGHT & 0xFF;
		config[GTP_ADDR_LENGTH+4] = LCD_PIXEL_HEIGHT >> 8;
		
		/*根据模式设置X2Y交换*/

    //不交换
//		config[GTP_ADDR_LENGTH+6] &= ~(X2Y_LOC);

    //交换
//    config[GTP_ADDR_LENGTH+6] |= (X2Y_LOC);

    //计算要写入checksum寄存器的值
    check_sum = 0;

    /* 计算check sum校验值 */
    if(touchIC == GT911 || touchIC == GT615)
    {
        for (i = GTP_ADDR_LENGTH; i < cfg_num; i++)
        {
            check_sum += (config[i] & 0xFF);
        }
        config[ cfg_num] = (~(check_sum & 0xFF)) + 1; 	//checksum
        config[ cfg_num+1] =  1; 						//refresh 配置更新标志
    }
		else if(touchIC == GT9157)
    {
        for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
        {
            check_sum += (config[i] & 0xFF);
        }
        config[ cfg_num+GTP_ADDR_LENGTH] = (~(check_sum & 0xFF)) + 1; 	//checksum
        config[ cfg_num+GTP_ADDR_LENGTH+1] =  1; 						//refresh 配置更新标志
    }
    else if(touchIC == GT5688 || touchIC == GT917S || touchIC == GT1151QM) 
    {
      for (i = GTP_ADDR_LENGTH; i < (cfg_num+GTP_ADDR_LENGTH -3); i += 2) 
      {
        check_sum += (config[i] << 8) + config[i + 1];
      }
      
      check_sum = 0 - check_sum;
      GTP_DEBUG("Config checksum: 0x%04X", check_sum);
      //更新checksum
      config[(cfg_num+GTP_ADDR_LENGTH -3)] = (check_sum >> 8) & 0xFF;
      config[(cfg_num+GTP_ADDR_LENGTH -2)] = check_sum & 0xFF;
      config[(cfg_num+GTP_ADDR_LENGTH -1)] = 0x01;
    }
		
    //写入配置信息
    for (retry = 0; retry < 5; retry++)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }
    Delay(0xfffff);				//延迟等待芯片更新
		

		
#if 1	//读出写入的数据，检查是否正常写入
    //检验读出的数据与写入的是否相同
	{
    	    uint16_t i;
    	    uint8_t buf[300];
    	     buf[0] = config[0];
    	     buf[1] =config[1];    //寄存器地址

//    	     

    	    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
			   
					GTP_DEBUG("read ");

					GTP_DEBUG_ARRAY(buf,cfg_num);
		
			    GTP_DEBUG("write ");

					GTP_DEBUG_ARRAY(config,cfg_num);

					//不对比版本号
    	    for(i=3;i<cfg_num+GTP_ADDR_LENGTH-3;i++)
    	    {

    	    	if(config[i] != buf[i])
    	    	{
    	    		GTP_ERROR("Config fail ! i = %d ",i);
							free(config);
    	    		return -1;
    	    	}
    	    }
    	    if(i==cfg_num+GTP_ADDR_LENGTH-3)
	    		GTP_DEBUG("Config success ! i = %d ",i);
	}
#endif
	free(config);

#endif
		
	 /*使能中断，这样才能检测触摸数据*/
		I2C_GTP_IRQEnable();
	
    GTP_Get_Info();

    return 0;
}




static bool isTouch=0;//全局静态变量
//这个是LVGL触摸文件要用到的
bool Touch_isPressed(void)
{
	if(isTouch == 1)
	{
		isTouch = 0;
		return true;
	}
	return false;
}
//这个是触摸中断回调函数。
void GTP_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GTP_INT_GPIO_PIN) != RESET) //确保是否产生了EXTI Line中断
	{
//		LED2_TOGGLE;
//    GTP_TouchProcess();   
      isTouch=1;			
    __HAL_GPIO_EXTI_CLEAR_IT(GTP_INT_GPIO_PIN);     //清除中断标志位
	}  
}

/**
  * @brief  触屏检测函数，本函数作为emXGUI的定制检测函数，
   *        参考Goodix_TS_Work_Func修改而来， 只读取单个触摸点坐标
  * @param x[out] y[out] 读取到的坐标
  * @retval 坐标有效返回1，否则返回0
  */
  //这个是获取x,y触摸的坐标点函数，在lv_port_indev_template有用到
int	GTP_Execu( int16_t *x,int16_t *y)
{
    uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    //2-寄存器地址 1-状态寄存器 8*1-每个触摸点使用8个寄存器 
    uint8_t  point_data[2 + 1 + 8 * 1 + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t  touch_num = 0;
    uint8_t  finger = 0;

    uint8_t client_addr=GTP_ADDRESS;
    int32_t input_x = 0;
    int32_t input_y = 0;

    int32_t ret = -1;

     

    ret = GTP_I2C_Read(client_addr, point_data, 12);//10字节寄存器加2字节地址
    if (ret < 0)
    {
        GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
        return 0;
    }
    
    finger = point_data[GTP_ADDR_LENGTH];//状态寄存器数据

    if (finger == 0x00)		//没有数据，退出
    {
        return 0;
    }

    if((finger & 0x80) == 0)//判断buffer status位
    {
        goto exit_work_func;//坐标未就绪，数据无效
    }

    touch_num = finger & 0x0f;//坐标点数
    if (touch_num > GTP_MAX_TOUCH)
    {
        goto exit_work_func;//大于最大支持点数，错误退出
    }    
  
    if (touch_num)
    {
		input_x  = point_data[3+1] | (point_data[3+2] << 8);	//x坐标
		input_y  = point_data[3+3] | (point_data[3+4] << 8);	//y坐标
		if(input_x < GTP_MAX_WIDTH && input_y < GTP_MAX_HEIGHT)  
		{
			*x = input_x;
			*y = input_y;
		}
		else
		{
			goto exit_work_func;
		}
    }

exit_work_func:
    {
        //清空标志
        ret = GTP_I2C_Write(client_addr, end_cmd, 3);
        if (ret < 0)
        {
            GTP_INFO("I2C write end_cmd error!");
            return 0;
        }
    }

    return touch_num;
}
















/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test( void)
{
    uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
    uint8_t retry = 0;
    int8_t ret = -1;

     
  
    while(retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
        if (ret > 0)
        {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.",retry);
    }
    return ret;
}

