import Ex06 as ex6
import sys

pin = int(sys.argv[1])#첫번째 인자인 pin번호를 받습니다.
isOn = sys.argv[2]#두번째 인자인 ON/OFF를 받습니다.

if(isOn=="ON"):
    #만약 입력된 명령어가 ON이면
    #Ex_06모듈의 ON함수를 호출합니다.
    ex6.ON(pin)
elif(isOn=="OFF"):
    #만약 입력된 명령어가 OFF이면
    #Ex_06모듈의 OFF함수를 호출합니다.
    ex6.OFF(pin)


