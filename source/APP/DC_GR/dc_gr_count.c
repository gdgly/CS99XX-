
#include "SPI_CPLD.H"
#include "cs99xx_relay_motion.h"
#include "cs99xx_struct.h"
#include "dc_gr_relay.h"
#include "dc_gr_count.h"
#include "dc_gr_test.h"
#include "test_com.h"



static int8_t dc_gr_judge_err(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data);

void dc_gr_count_dis_value(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
	uint16_t temp_cur = 0;
	uint16_t temp_vol = 0;
    
    temp_vol = test_data->sample_vol_ad;
    temp_cur = test_data->sample_cur_ad;
    
    if(temp_cur == 0)
    {
        test_data->cur_value = 0;
    }
    else
    {
        test_data->cur_value = temp_cur * ratio_gr.adc_i_k[cur_gear] + ratio_gr.adc_i_b[cur_gear];
    }
    
    if(temp_vol == 0)
    {
        vol_ave = 0;
    }
    else
    {
        vol_ave = temp_vol * ratio_gr.adc_v_k[cur_gear] + ratio_gr.adc_v_b[cur_gear] + ROUND / 100;
    }
    test_data->res_value = temp_cur * test_data->cur_adc_k;
    
    /* 引入偏移值 */
    if(test_data->res_value >= dc_gr_par->offset_res)
    {
        test_data->res_value -= dc_gr_par->offset_res;
    }
    else
    {
        test_data->res_value = 0;
    }
    
    /* 异常判断 */
    if(dc_gr_test_flag.judge_err_en == ENABLE)
    {
        test_data->fail_num = dc_gr_judge_err(dc_gr_par, test_data);
        
        if(test_data->fail_num != ST_ERR_NONE)
        {
            dc_gr_test_flag.judge_err_en == DISABLE;
            close_test_timer();/* 关定时器 */
            irq_stop_relay_motion();///<关闭高压
            test_data->test_over = 1;
        }
    }
}

uint8_t judge_dc_gr_vol_exception(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
	/* 电压上升 阶段 缓变阶段 */
	if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
    {
        if(test_data->vol_value > (dc_gr_par->testing_cur * 11 / 10 + 10))
        {
            if(++test_data->vol_err_count > 2)
            {
                return ST_ERR_VOL_ABNORMAL;
            }
        }
        else
        {
            test_data->vol_err_count = 0;
            return ST_ERR_NONE;/* 测试时间到报警 */
        }
    }
    /* 测试阶段 */
    else if(test_data->gradation == STAGE_TEST)
    {
        if(test_data->dis_time >= 5 || test_data->dis_time == dc_gr_par->testing_time)
        {
            /* 电压低于1V */
            if(test_data->vol_value < 1)
            {
                return ST_ERR_SHORT;
            }
        }
        
        /* 测试时间到 */
        if(dc_gr_par->testing_time > 0 && test_data->dis_time + 1 >= dc_gr_par->testing_time)
        {
            return ST_ERR_NONE;
        }
        
        if((test_data->vol_value < dc_gr_par->testing_cur / 2
            || test_data->vol_value > (dc_gr_par->testing_cur * 11 / 10 + 10)))
        {
            return ST_ERR_VOL_ABNORMAL;
        }
        else
        {
            return ST_ERR_NONE;/* 测试时间到报警 */
        }
    }
    
    return ST_ERR_NONE;
}

int8_t dc_gr_judge_err(GR_STRUCT *dc_gr_par, TEST_DATA_STRUCT *test_data)
{
    int8_t err = ST_ERR_NONE;
	
//    /* 判断真实电流 */
//    if(test_data->real_value > dc_gr_par->ac_real_cur && dc_gr_par->ac_real_cur > 0)
//    {
//        err = ST_ERR_REAL;
//    }
//    /* 当电流值大于上限值时就将电流值强行改为上限值并设置上限报警 */
//    else 
        if(test_data->cur_value > dc_gr_par->upper_limit)
    {
        err = ST_ERR_H;
    }
	/* 电压上升 阶段 缓变阶段 */
    else if(test_data->gradation == STAGE_RISE || test_data->gradation == STAGE_CHANGE)
	{
        err = judge_dc_gr_vol_exception(dc_gr_par, test_data);
	}
	/* 测试阶段 */
	else if(test_data->gradation == STAGE_TEST && test_data->dis_time > 2)
	{
        /* 电压异常判断 */
        err = judge_dc_gr_vol_exception(dc_gr_par, test_data);
        
        if(ST_ERR_NONE == err)
        {
            /* 电流下限报警 */
            if(test_data->cur_value < dc_gr_par->lower_limit)
            {
                /* 下限报警判断三次 */
                if(++test_data->low_err_count >= 2)
                {
                    err = ST_ERR_L;
                }
            }
            else
            {
                test_data->low_err_count = 0;
            }
        }
        
        if(ST_ERR_NONE == err)
        {
            /* 测试时间到 */
            if(dc_gr_par->testing_time && test_data->dis_time + 1 >= dc_gr_par->testing_time)
            {
                /* 电流下限报警 */
                if(test_data->cur_value < dc_gr_par->lower_limit)
                {
                    test_data->test_over_fail = ST_ERR_L;
                }
                else
                {
                    test_data->test_over_fail = ST_ERR_NONE;
                }
            }
        }
	}
	
	return err;
}

/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
