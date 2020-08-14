#ifndef __SIMPLE__
#define __SIMPLE__

typedef struct _ledCtl{
    int pin;//LED조작을 할 GPIO포트번호
    int funcNum;
}__attribute__((packed))ledCtl;

#define MY_IOC_MAGIC 'c'

#define MY_IOC_SET _IOW(MY_IOC_MAGIC, 0, ledCtl)//SETTING
#define MY_IOC_GPIO_SETFUNC _IO(MY_IOC_MAGIC, 1)
#define MY_IOC_GPIO_ACTIVE _IO(MY_IOC_MAGIC, 2)
#define MY_ON _IO(MY_IOC_MAGIC,3)//LED ON
#define MY_OFF _IO(MY_IOC_MAGIC,4)//LED OFF

#define MY_IOC_MAXNR 5

#endif
