#include "MySysCtrl.h"

struct sysCtrlClass sysCrtl[sysCtrlLen+1];       //建立任务结构体数组
uint16_t sysCrtlLowPriorityRootinesNum = 0;		 //低优先级任务
uint16_t sysCrtlHighPriorityRootinesNum = 0;		 //高优先任务

//增加一个任务
bool addOneRootine(uint32_t delay,void (*f)(void),uint8_t priority,bool enable)
{
    struct sysCtrlClass *sys;

    if(priority == 0)   //high
    {
        if(sysCrtlHighPriorityRootinesNum < sysCtrlPriorityDivide)   //高优先级任务,sysCtrlPriorityDivide是分配给高优先级任务的资源
        {
            sys = sysCrtl + sysCrtlHighPriorityRootinesNum; 		 //计数加上偏移
            sys->enable = enable;									 //填充任务结构体
            sys->delay = delay;
            sys->timeRecord = setdelay(delay);
            sys->f = f;
            (sys+1)->enable = false;								 //清空下一个任务结构体
            (sys+1)->delay = 0;
            (sys+1)->f = NULL;
            sysCrtlHighPriorityRootinesNum++;					     //任务数量自加
            return true;
        }
    }
    else	//低优先级，放在最后，不用插入
    {
        if(sysCrtlLowPriorityRootinesNum < sysCtrlLen)				//低优先级任务,这里似乎有点不对??
        {
            sys = sysCrtl + sysCrtlLowPriorityRootinesNum + sysCtrlPriorityDivide; //计数加上偏移
            sys->enable = enable;
            sys->delay = delay;
            sys->timeRecord = setdelay(delay);
            sys->f = f;
            (sys+1)->enable = false;
            (sys+1)->delay = 0;
            (sys+1)->f = NULL;
            sysCrtlLowPriorityRootinesNum++;
            return true;
        }
    }

    return false;
}

//允许任务的同时也会设置时间
bool enableOneRootine(void(*f)(void))
{
    struct sysCtrlClass *sys;
    uint16_t rootineC = 0;

    while(rootineC<sysCrtlHighPriorityRootinesNum)//遍历列表长度的节点
    {
        sys = sysCrtl + rootineC;
        if(sys->delay == 0) break;	//检测到终止节点
        if(sys->f == f)				//搜索到目的节点
        {
            sys->enable = true;		//允许任务
            sys->timeRecord = setdelay(sys->delay);
            return true;
        }							//返回设置成功
        rootineC++;
    }

    rootineC = 0;
    while(rootineC<sysCrtlLowPriorityRootinesNum)//遍历列表长度的节点
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->delay == 0) break;	//检测到终止节点
        if(sys->f == f)				//搜索到目的节点
        {
            sys->enable = true;		//允许任务
            sys->timeRecord = setdelay(sys->delay); //设置时间
            return true;            //返回设置成功
        }
        rootineC++;
    }

    return false;
}

//只会禁止任务,不会清零时间
bool disableOneRootine(void(*f)(void))
{
    struct sysCtrlClass *sys;
    uint16_t rootineC = 0;

    while(rootineC<sysCrtlHighPriorityRootinesNum)//遍历列表长度的节点
    {
        sys = sysCrtl + rootineC;
        if(sys->delay == 0) break;	//检测到终止节点
        if(sys->f == f)				//搜索到目的节点
        {
            sys->enable = false;		//禁止任务
            return true;
        }							//返回设置成功
        rootineC++;
    }

    rootineC = 0;
    while(rootineC<sysCrtlLowPriorityRootinesNum)//遍历列表长度的节点
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->delay == 0) break;	//检测到终止节点
        if(sys->f == f)				//搜索到目的节点
        {
            sys->enable = false;		//禁止任务
            return true;            //返回设置成功
        }
        rootineC++;
    }

    return false;
}

bool deleteOneRootine(void (*f)(void))
{
    struct sysCtrlClass *sys;
    uint16_t rootineC = 0;

    while(rootineC<sysCrtlHighPriorityRootinesNum)
    {
        sys = sysCrtl + rootineC;
        if(sys->f == f)
        {
            while((sys+1)->delay != 0)//直到终止节点
            {
                *sys = *(sys+1);
                sys++;
            }
            sys->delay = 0;//重新设置末节点
            sys->f = NULL;
            sysCrtlHighPriorityRootinesNum -= 1;	//删除一个任务
            return true;
        }
        rootineC++;
    }

    rootineC = 0 ;
    while(rootineC < sysCrtlLowPriorityRootinesNum)
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->f == f)
        {
            while((sys+1)->delay != 0)//直到终止节点
            {
                *sys = *(sys+1);
                sys++;
            }
            sys->delay = 0;//重新设置末节点
            sys->f = NULL;
            sysCrtlLowPriorityRootinesNum -= 1;	//删除一个任务
            return true;
        }
        rootineC++;
    }

    return false;
}


bool changeOneRootine(void (*f)(void),int32_t period)
{
    struct sysCtrlClass *sys;
    uint16_t rootineC = 0;

    while(rootineC<sysCrtlHighPriorityRootinesNum)//遍历列表长度的节点
    {
        sys = sysCrtl + rootineC;
        if(sys->delay == 0) break;	//检测到终止节点
        if(sys->f == f)				//搜索到目的节点
        {
            sys->delay = period;	
            return true;
        }							//返回设置成功
        rootineC++;
    }

    rootineC = 0;
    while(rootineC<sysCrtlLowPriorityRootinesNum)//遍历列表长度的节点
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->delay == 0) break;	//检测到终止节点
        if(sys->f == f)				//搜索到目的节点
        {
            sys->delay = period;
            return true;            //返回设置成功
        }
        rootineC++;
    }

    return false;
}

void runLowRootine(void)
{
    struct sysCtrlClass *sys;
	if(sysCrtlLowPriorityRootinesNum == 0) return;//由于没有初始化，sys->delay在没有任何任务的时候不可知，故加判断

    sys = sysCrtl + sysCtrlPriorityDivide;
    while( sys->delay !=0 )//不是终止节点
    {
        if(sys->enable && checkdelay(sys->timeRecord))//定时时间到了
        {
            sys->timeRecord = setdelay(sys->delay); //重新定时
            sys->f();	//执行函数
        }
        sys++;
    }
}

void runHighRootine(void)
{
    struct sysCtrlClass *sys;
	
    if(sysCrtlHighPriorityRootinesNum == 0) return;//由于没有初始化，sys->delay在没有任何任务的时候不可知，故加判断
	sys = sysCrtl;
    while( sys->delay !=0 )//不是终止节点
    {
        if(sys->enable && checkdelay(sys->timeRecord))//定时时间到了
        {
            sys->timeRecord = setdelay(sys->delay); //重新定时
            sys->f();	//执行函数
        }
        sys++;
    }
}
