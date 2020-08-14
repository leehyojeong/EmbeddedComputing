#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/cdev.h>    
#include <linux/device.h>    
#include <linux/fs.h>              
#include <linux/slab.h>    
#include <linux/delay.h>
#include <linux/uaccess.h>    
//#include <asm/uaccess.h>    
#include <asm/io.h>
#include "Ex_06.h"
// timer header
#include <linux/timer.h>

// GPIO 
typedef struct {
        unsigned long   GPFSEL[6];      ///< Function selection registers.
        unsigned long   Reserved_1;
        unsigned long   GPSET[2];
        unsigned long   Reserved_2;
        unsigned long   GPCLR[2];
        unsigned long   Reserved_3;
        unsigned long   GPLEV[2];
        unsigned long   Reserved_4;
        unsigned long   GPEDS[2];
        unsigned long   Reserved_5;
        unsigned long   GPREN[2];
        unsigned long   Reserved_6;
        unsigned long   GPFEN[2];
        unsigned long   Reserved_7;
        unsigned long   GPHEN[2];
        unsigned long   Reserved_8;
        unsigned long   GPLEN[2];
        unsigned long   Reserved_9;
        unsigned long   GPAREN[2];
        unsigned long   Reserved_A;
        unsigned long   GPAFEN[2];
        unsigned long   Reserved_B;
        unsigned long   GPPUD[1];
        unsigned long   GPPUDCLK[2];
        //Ignoring the reserved and test bytes
} BCM2837_GPIO_REGS;

#define BCM2837_GPIO_BASE 0x3F200000
#define BCM2837_GPIO_SIZE sizeof(BCM2837_GPIO_REGS)
#define DEVICE_NAME "Ex_06"    

void __iomem *mem = NULL;
volatile BCM2837_GPIO_REGS * pRegs = NULL;

//void my_timer_callback(struct timer_list * timer);
void ledAllClear(void);//불이 들어온 LED를 끄는 함수
void ledSet(void);//LED의 불을 켜는 함수
//void ledSet2(void);

dev_t id;    
struct cdev cdev;    
struct class *class;    
struct device *dev;    

ledCtl ledctl;        
int scenario=0, pause=0, pos=0, d=1;
static struct timer_list my_timer;
       
int simple_open (struct inode *inode, struct file *filp)    
{    
    printk( "open\n" );    
    return 0;    
}    
    
int simple_close (struct inode *inode, struct file *filp)    
{    
    printk( "close\n" );  
    return 0;    
}    
        
ssize_t simple_read(struct file *filp, char *buf, size_t size, loff_t *offset)    
{    
    printk( "simple_read\n" );    
    return 0;    
}    
        
ssize_t simple_write (struct file *filp, const char *buf, size_t size, loff_t *offset)    
{    
    printk( "simple_write\n" );    
    return 0;    
}    

long simple_ioctl ( struct file *filp, unsigned int cmd, unsigned long arg)    
{
    if(_IOC_TYPE(cmd) != MY_IOC_MAGIC)
        return -EINVAL;
    if(_IOC_NR(cmd) >= MY_IOC_MAXNR)
        return -EINVAL;

    switch(cmd){
        case MY_IOC_SET:
        {
            printk("MY_IOC_SET\n");
            copy_from_user((void *)&ledctl, (void *)arg, sizeof(ledCtl));
            printk("%d\n", ledctl.pin);
            scenario=0; pause=0; pos=-1; d=1;
            break;
        }
        case MY_IOC_GPIO_SETFUNC:
        {
            int  i=0;
            //unsigned long val=0, offset=0;

            printk("MY_IOC_GPIO_SETFUNC\n");
            
                unsigned long offset = ledctl.pin/10;
                unsigned long val = (u32)ioread32(&(pRegs->GPFSEL[offset]));
                int item = ledctl.pin % 10;

                val &= ~(0x7 << (item*3));
                val |= ((ledctl.funcNum & 0x7) << (item*3));
                iowrite32((u32)val, &(pRegs->GPFSEL[offset]));
            //현재 들어온 pin을 설정해준다.
            
            break;
        }
        case MY_ON:
        { 
            ledSet();
			//ioctl명령어로 MY_ON이 들어온 경우
            //ledSet함수를 통해 LED의 불을 켠다
            break;
        }
        case MY_OFF:
        {
            ledAllClear();
			//ioctl명령어로 MY_OFF가 들어온 경우
			//ledAllClear함수를 통해 해당 LED의 불을 끈다
            break;
        }
    }    
    return 0;    
}    
        
struct file_operations simple_fops =    
{    
    .owner           = THIS_MODULE,    
    .read            = simple_read,         
    .write           = simple_write,        
    .unlocked_ioctl  = simple_ioctl,        
    .open            = simple_open,         
    .release         = simple_close,      
};    
        
int simple_init(void)    
{    
    int ret;    
        
    ret = alloc_chrdev_region( &id, 0, 1, DEVICE_NAME );    
    if ( ret ){    
        printk( "alloc_chrdev_region error %d\n", ret );    
        return ret;    
    }    
        
    cdev_init( &cdev, &simple_fops );    
    cdev.owner = THIS_MODULE;    
        
    ret = cdev_add( &cdev, id, 1 );    
    if (ret){    
        printk( "cdev_add error %d\n", ret );    
        unregister_chrdev_region( id, 1 );    
        return ret;    
    }    
        
    class = class_create( THIS_MODULE, DEVICE_NAME );    
    if ( IS_ERR(class)){    
        ret = PTR_ERR( class );    
        printk( "class_create error %d\n", ret );    
        
        cdev_del( &cdev );    
        unregister_chrdev_region( id, 1 );    
        return ret;    
    }    
        
    dev = device_create( class, NULL, id, NULL, DEVICE_NAME );    
    if ( IS_ERR(dev) ){    
        ret = PTR_ERR(dev);    
        printk( "device_create error %d\n", ret );    
        
        class_destroy(class);    
        cdev_del( &cdev );    
        unregister_chrdev_region( id, 1 );    
        return ret;    
    }
    // gpio memory mapping
    mem = ioremap(BCM2837_GPIO_BASE, BCM2837_GPIO_SIZE);
    pRegs = mem;
    return 0;    
}    
        
    
void simple_exit(void)    
{    
    iounmap(mem);
    device_destroy(class, id );    
    class_destroy(class);    
    cdev_del( &cdev );    
    unregister_chrdev_region( id, 1 );    
}    

// led All Clear
void ledAllClear(void){
    int i=0;
    unsigned long offset=0, mask=0;
 
        offset = (ledctl.pin)/32;
        mask = (1<<((ledctl.pin)%32));
        iowrite32((u32)mask, &(pRegs->GPCLR[offset]));

		//해당 pin의 레지스터를 이용하여 불을 끕니다.

}

// led Set
void ledSet(void){
    unsigned long offset = ledctl.pin/32;
    unsigned long mask = (1<<(ledctl.pin%32));
    iowrite32((u32)mask, &(pRegs->GPSET[offset]));
	//해당 pin의 레지스터를 이용하여 불을 켭니다
}

//void ledSet2(void){
//    unsigned long offset = 0;
//    unsigned long mask[2] = {0,0};
//    int i=0;
//
//        offset = ledctl.pin/32;
//        mask[offset] |= (1<<(ledctl.pin%32));
//    
//    iowrite32((u32)mask[0], &(pRegs->GPSET[0]));
//    iowrite32((u32)mask[1], &(pRegs->GPSET[1]));
//}

//// timer callback
//void my_timer_callback(struct timer_list * timer){
//    switch(scenario){
//        case 0:// scenario 1
//            ledAllClear();
//            pos++;
//            pos = pos%5;
//            ledSet();
//            break;
//        case 1:// scenario 2
//            ledAllClear();
//            if(pos==0)
//                d = 1;
//            if(pos==4)
//                d = -1;
//            pos+=d;
//            ledSet();
//            break;
//        case 2:// scenario 3
//            ledAllClear();
//            if(pos==0)
//                d = 1;
//            if(pos==4)
//                d = -1;
//            pos+=d;
//            ledSet2();
//            break;  
//    }
//    if(!pause){
//        my_timer.expires = jiffies + 150;
//        add_timer(&my_timer);
//    }
//}
        
module_init(simple_init);    
module_exit(simple_exit);  
    
MODULE_LICENSE("Dual BSD/GPL");
