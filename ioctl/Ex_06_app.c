#include <stdio.h>    
#include <sys/types.h>    
#include <sys/stat.h>    
#include <sys/ioctl.h>    
#include <fcntl.h>    
#include <termios.h>
#include <unistd.h>    
#include "Ex_06.h"
#include <string.h>
#include <stdlib.h>

#define DEVICE_FILENAME  "/dev/Ex_06"    

#define LED1 26
#define LED2 19
#define LED3 21
#define LED4 20
#define LED5 16

int kbhit(void);

int main(int argc, char* argv[])    
{    
    int fd=0;
    int i=0;
    char ch = 0;
    int pins[5]={ LED1, LED2, LED3, LED4, LED5};    
    ledCtl ledctl;

    // init ledctl
    memset(&ledctl, 0, sizeof(ledCtl));    
    int size = sizeof(ledctl);    
     
    int pin = atoi(argv[1]);
    printf("%d\n",pin);
    int k=0;
    int isOk=0;
    for(k=0;k<5;k++){
        if(pins[k]==pin){
            isOk=1;
			//�Է��� pin��ȣ�� ��Ʈ�� ����� pin��ȣ �� �ִ� ���
        }
    }
    
    // init device driver
    fd = open( DEVICE_FILENAME, O_RDWR|O_NDELAY );    
    
    if( fd >= 0 ){  
        // ledctl 

        memcpy(&ledctl,&pin, sizeof(ledCtl));
        
        ledctl.pin=pin;
        ledctl.funcNum = 1;
		//�Է����� ���� pin��ȣ�� �Ѱ��ݴϴ�.

        printf("send ledctl to device driver\n");
        ioctl(fd, MY_IOC_SET, &ledctl);    
        ioctl(fd, MY_IOC_GPIO_SETFUNC);   
		//�⺻���� GPIO������ ���ݴϴ�.
    }

    if(isOk){
        if(!strcmp(argv[2],"ON")){
			//��ɾ ON�� ��� ioctl MY_ON��ɾ �Ѱ��ݴϴ�.
            ioctl(fd,MY_ON);
        }
        else if(!strcmp(argv[2],"OFF")){
			//��ɾ OFF�� ��� ioctl MY_OFF��ɾ �Ѱ��ݴϴ�.
            ioctl(fd,MY_OFF);
        }
    }
    close(fd);    
    return 0;    
}

int kbhit(){
    struct termios oldt, newt;
    int ch;

    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );

    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}
