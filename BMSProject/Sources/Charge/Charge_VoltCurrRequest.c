#include  "Task_ChargeCfg.h"
#include  "BattInfoConfig.h"
#include  "Task_CurrentLimit.h"
#include  "Task_SOC.h"
#include  "BattInfoParaStructure.h"
#include  "Task_FaultLevelJudge.h"

static 
float Charge_CurrInit(float currlimt, float chargeInit);
static
float ChargeEnd_CurrentOut(uint16 cellvoltmax, float curr); 

static uint8 ChargeEndJudge(uint8,float,uint16,uint8,float);
/*=======================================================================
 *函数名:      BmsToChargePileInfo(void) 
 *功能:        搜集发送至充电机的信息
 *参数:        无       
 *返回：       无
 
 *说明：       配置需要发送给充电机的信息,提高程序可移植性
========================================================================*/ 
void Charge_VoltCurrRequest(void)
{
  float curr;
  //最大电压请求
  BMSChargePile.Volt_Max_ChargePile = CELL_VOLT_NOMINAL * SYS_SERIES_YiDongLi + 5;
  //最大电流请求
  curr = Charge_CurrInit(CurrentLimit.Current_Charge_Constant, BMSChargePile.Curr_Max_ChargePile);
  BMSChargePile.Curr_Max_ChargePile = ChargeEnd_CurrentOut(VoltInfo.CellVolt_Max, curr);
  
  
  BMSChargePile.VoltC_Max           = VoltInfo.CellVolt_Max ;
  BMSChargePile.VoltC_Min           = VoltInfo.CellVolt_Min;
  BMSChargePile.SOC                 = SOCInfo.SOC_ValueRead;
  BMSChargePile.Temp_Max            = TempInfo.CellTemp_Max;
  BMSChargePile.VoltS               = DataColletInfo.SysVolt_Total;
  
  BMSChargePile_State.TempH_Cell    = Fault_Charge.Level_Temp_High;
  BMSChargePile_State.TempL_Cell    = Fault_Charge.Level_Temp_Low;
  BMSChargePile_State.CurrH_Cell    = Fault_Charge.Level_Current_Charge_High;
  BMSChargePile_State.Insul         = 0;
  BMSChargePile_State.BMSGetMsg     = 0;
  BMSChargePile_State.FaultFlag     = 0;
}

/*=======================================================================
 *函数名:      ChargeEndJudge(void) 
 *功能:        充电中止判断
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
static
uint8 ChargeEndJudge(uint8 soc,float curr_end,uint16 cellvoltmax,uint8 temph,uint8 templ, uint8 BMSFlt, uint8 chargeFlt)
{
  static uint8 cnt[5];
  //SOC到达100%
  if(soc+0.002>=1)        
  {
    if(++cnt[0]>5)
    {
      cnt[0] = 0;
      return (1);
    }
  }
  else
  {
    cnt[0] = 0;
  }
  //充电末期电流小于0.03C
  if(curr_end < 0.03*SYS_CAPACITY)
  {
    if(++cnt[1]>5)
    {
      cnt[1] = 0;
      return (1);  
    }
  }
  else
  {
     cnt[1] = 0;
  }
  //到达单体截止电压
  if(cellvoltmax > CELL_VOLT_MAX)
  {
    if(++cnt[2]>3)
    {
      cnt[2] = 0;
      return (1); 
    }
  }
  else
  {
     cnt[2] = 0;
  }
  //到达最高温度
  if(temph > CELL_TEMP_MAX_CHARGE + 40)
  {
    if(++cnt[3]>3)
    {
      cnt[3] = 0;
      return (1);  
    }
  }
  else
  {
     cnt[3] = 0;
  }
  //低温停止
  if(templ < CELL_TEMP_MIN_CHARGE + 40)
  {
    if(++cnt[4]>3)
    {
      cnt[4] = 0;
      return (1);
    }
  }
  else
  {
     cnt[4] = 0;
  }
  //BMS故障和充电机输出故障
  if((BMSFlt == 1)||(chargeFlt == 1))
  {
    return 1;
  }

  return (0);
}


/*=======================================================================
 *函数名:      Charge_Curr(uint16, float curr) 
 *功能:        充电充电末端的电流请求
 *参数:        chargeInit：初始化时的电流值
               curr:电流
 *返回：       float:充电枪电流值
 *说明：       判断电池是否到达3.65V,若到达则按原来请求电流的0.7倍再请求
               电流减小低于3.63V时，就按原来的电流大小请求
========================================================================*/
static 
float Charge_CurrInit(float currlimt, float chargeInit)
{
   float currIn;
   if (chargeInit <= 0.03*SYS_CAPACITY)//初始值的确定
   {
      chargeInit = currlimt;
   }
   if(chargeInit > currlimt)//当到达充电末期后变电流的值大于电流限制值时按限制值
   {  
      currIn = currlimt;
   }
   else//否则按变电流的值
   {
      currIn = chargeInit; 
   }
   return currIn;
}

/*=======================================================================
 *函数名:      ChargeEnd_CurrentOut(uint16, float curr) 
 *功能:        充电末端的变电流请求
 *参数:        cellvoltmax：最大单体电压
               currIn:电流
 *返回：       float:充电枪电流值
 *说明：       判断电池是否到达3.63V,若到达则按原来请求电流的0.7倍再请求
               电流减小低于3.63V时，就按原来的电流大小请求
========================================================================*/
static 
float ChargeEnd_CurrentOut(uint16 cellvoltmax, float currIn)
{
  float current;
  static uint8 currflag;
  static uint8 cnt;
  if((cellvoltmax >= 36300)&&(currflag == 0)) //currflag的作用为避免重复的降电流
  {
    currflag = 1;
    current = 0.7*currIn;
  }
  else
  {
    if(++cnt>3)//3s之后再判断电压情况再决定是否进行降电流
    {
      cnt = 0;
      currflag = 0;
    }
    current = currIn; 
  }
  return current;
}




