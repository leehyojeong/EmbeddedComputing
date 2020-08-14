## ioctl을 통한 GPIO 제어
### Task 1
- 브레드 보드에 LED 회로를 구성한 후 ioctl을 사용하여 GPIO를 제어하는 디바이스 드라이버와 C 응용 프로그램을 만든다.
- 코드 파일
  - __Ex_06_dev.c__ : GPIO를 제어하는 ioctl이 구현된 디바이스 드라이버
  - __Ex_06_app.c__ : 위의 디바이스 드라이버를 사용하는 애플리케이션
  - __Ex_06.h__ : ioctl이 정의된 헤더
### Task 2
- Task 1에서 만든 C 응용 프로그램을 파이썬용 모듈로 만들고, 파이썬에서 호출하여 LED를 제어하도록 만든다.

![image](https://user-images.githubusercontent.com/39904216/90210995-7b7cf180-de2a-11ea-9c99-5ef9bcbde09e.png)


