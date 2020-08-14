# -*- coding: utf-8 -*-
"""
Created on Sun Oct 27 23:12:01 2019

@author: hyoju
"""

import LedControl as LC
import Timer as T
import time

pin = (18,20,21)#output으로 18,20,21번 핀을 사용한다
now = 0#불이 들어와야하는 핀번호
LR = 0 #현재 방향이 Left(1)인지 Right(0)인지
flag = True
#callback함수를 세팅했는지, 
#만약 세팅이 되어있지 않으면 True, 세팅이 되어있으면 False

def changeDirect():
    global LR
    global flag
    LR = int(input('1)Left 2)Right >>'))
    flag = True
    #콜백함수가 호출되었기 때문에 새롭게 callback함수를 호출해 주어야한다
        
if __name__ == '__main__':
    try:
        while True:
            if flag:
                #만약 콜백함수가 호출되지 않았다면
                T.add_callback(10,cb=changeDirect)
                #C모듈 TImer의 add_callback함수를 호출한다
                #callback함수로 changeDirect를 호출
                flag = False
                #콜백을 설정했으므로 false로 바꾸어준다
            if LR == 1:
                #만약 왼쪽으로 선택이 되어 있다면
                LC.Left(pin[now])
                #LedControl C모듈의 Left함수를 호출하고 
                #현재 불이 들어와야하는 핀번호를 인자로 넘겨준다
                if now==0:
                    #왼쪽으로 돌기 때문에 핀번호를 바꾸어준다
                    now=2
                elif now == 1:
                    now=0
                elif now == 2:
                    now=1
            else:
                #오른쪽으로 선택이 되었다면
                LC.Right(pin[now])
                #LedControl의 C모듈의 Right함수를 호출하고 
                #현재 불이 들어와야하는 핀번호를 인자로 넘겨준다
                if now==0:
                    #현재 불이 들어와야하는 핀번호를 변경
                    now=1
                elif now==1:
                    now=2
                elif now==2:
                    now=0
            time.sleep(2)#LED에 불이 들어와야하는 시간을 벌어주기 위해 잠시 sleep한다
    except KeyboardInterrupt:
        #키보드 인터럽트가 발생하면
        exit()#종료한다
