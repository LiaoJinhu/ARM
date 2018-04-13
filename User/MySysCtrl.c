#include "MySysCtrl.h"

struct sysCtrlClass sysCrtl[sysCtrlLen+1];       //��������ṹ������
uint16_t sysCrtlLowPriorityRootinesNum = 0;		 //�����ȼ�����
uint16_t sysCrtlHighPriorityRootinesNum = 0;		 //����������

//����һ������
bool addOneRootine(uint32_t delay,void (*f)(void),uint8_t priority,bool enable)
{
    struct sysCtrlClass *sys;

    if(priority == 0)   //high
    {
        if(sysCrtlHighPriorityRootinesNum < sysCtrlPriorityDivide)   //�����ȼ�����,sysCtrlPriorityDivide�Ƿ���������ȼ��������Դ
        {
            sys = sysCrtl + sysCrtlHighPriorityRootinesNum; 		 //��������ƫ��
            sys->enable = enable;									 //�������ṹ��
            sys->delay = delay;
            sys->timeRecord = setdelay(delay);
            sys->f = f;
            (sys+1)->enable = false;								 //�����һ������ṹ��
            (sys+1)->delay = 0;
            (sys+1)->f = NULL;
            sysCrtlHighPriorityRootinesNum++;					     //���������Լ�
            return true;
        }
    }
    else	//�����ȼ���������󣬲��ò���
    {
        if(sysCrtlLowPriorityRootinesNum < sysCtrlLen)				//�����ȼ�����,�����ƺ��е㲻��??
        {
            sys = sysCrtl + sysCrtlLowPriorityRootinesNum + sysCtrlPriorityDivide; //��������ƫ��
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

//���������ͬʱҲ������ʱ��
bool enableOneRootine(void(*f)(void))
{
    struct sysCtrlClass *sys;
    uint16_t rootineC = 0;

    while(rootineC<sysCrtlHighPriorityRootinesNum)//�����б��ȵĽڵ�
    {
        sys = sysCrtl + rootineC;
        if(sys->delay == 0) break;	//��⵽��ֹ�ڵ�
        if(sys->f == f)				//������Ŀ�Ľڵ�
        {
            sys->enable = true;		//��������
            sys->timeRecord = setdelay(sys->delay);
            return true;
        }							//�������óɹ�
        rootineC++;
    }

    rootineC = 0;
    while(rootineC<sysCrtlLowPriorityRootinesNum)//�����б��ȵĽڵ�
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->delay == 0) break;	//��⵽��ֹ�ڵ�
        if(sys->f == f)				//������Ŀ�Ľڵ�
        {
            sys->enable = true;		//��������
            sys->timeRecord = setdelay(sys->delay); //����ʱ��
            return true;            //�������óɹ�
        }
        rootineC++;
    }

    return false;
}

//ֻ���ֹ����,��������ʱ��
bool disableOneRootine(void(*f)(void))
{
    struct sysCtrlClass *sys;
    uint16_t rootineC = 0;

    while(rootineC<sysCrtlHighPriorityRootinesNum)//�����б��ȵĽڵ�
    {
        sys = sysCrtl + rootineC;
        if(sys->delay == 0) break;	//��⵽��ֹ�ڵ�
        if(sys->f == f)				//������Ŀ�Ľڵ�
        {
            sys->enable = false;		//��ֹ����
            return true;
        }							//�������óɹ�
        rootineC++;
    }

    rootineC = 0;
    while(rootineC<sysCrtlLowPriorityRootinesNum)//�����б��ȵĽڵ�
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->delay == 0) break;	//��⵽��ֹ�ڵ�
        if(sys->f == f)				//������Ŀ�Ľڵ�
        {
            sys->enable = false;		//��ֹ����
            return true;            //�������óɹ�
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
            while((sys+1)->delay != 0)//ֱ����ֹ�ڵ�
            {
                *sys = *(sys+1);
                sys++;
            }
            sys->delay = 0;//��������ĩ�ڵ�
            sys->f = NULL;
            sysCrtlHighPriorityRootinesNum -= 1;	//ɾ��һ������
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
            while((sys+1)->delay != 0)//ֱ����ֹ�ڵ�
            {
                *sys = *(sys+1);
                sys++;
            }
            sys->delay = 0;//��������ĩ�ڵ�
            sys->f = NULL;
            sysCrtlLowPriorityRootinesNum -= 1;	//ɾ��һ������
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

    while(rootineC<sysCrtlHighPriorityRootinesNum)//�����б��ȵĽڵ�
    {
        sys = sysCrtl + rootineC;
        if(sys->delay == 0) break;	//��⵽��ֹ�ڵ�
        if(sys->f == f)				//������Ŀ�Ľڵ�
        {
            sys->delay = period;	
            return true;
        }							//�������óɹ�
        rootineC++;
    }

    rootineC = 0;
    while(rootineC<sysCrtlLowPriorityRootinesNum)//�����б��ȵĽڵ�
    {
        sys = sysCrtl + rootineC + sysCtrlPriorityDivide;
        if(sys->delay == 0) break;	//��⵽��ֹ�ڵ�
        if(sys->f == f)				//������Ŀ�Ľڵ�
        {
            sys->delay = period;
            return true;            //�������óɹ�
        }
        rootineC++;
    }

    return false;
}

void runLowRootine(void)
{
    struct sysCtrlClass *sys;
	if(sysCrtlLowPriorityRootinesNum == 0) return;//����û�г�ʼ����sys->delay��û���κ������ʱ�򲻿�֪���ʼ��ж�

    sys = sysCrtl + sysCtrlPriorityDivide;
    while( sys->delay !=0 )//������ֹ�ڵ�
    {
        if(sys->enable && checkdelay(sys->timeRecord))//��ʱʱ�䵽��
        {
            sys->timeRecord = setdelay(sys->delay); //���¶�ʱ
            sys->f();	//ִ�к���
        }
        sys++;
    }
}

void runHighRootine(void)
{
    struct sysCtrlClass *sys;
	
    if(sysCrtlHighPriorityRootinesNum == 0) return;//����û�г�ʼ����sys->delay��û���κ������ʱ�򲻿�֪���ʼ��ж�
	sys = sysCrtl;
    while( sys->delay !=0 )//������ֹ�ڵ�
    {
        if(sys->enable && checkdelay(sys->timeRecord))//��ʱʱ�䵽��
        {
            sys->timeRecord = setdelay(sys->delay); //���¶�ʱ
            sys->f();	//ִ�к���
        }
        sys++;
    }
}
