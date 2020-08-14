# EmbeddedComputing

## Final Project
### 프로그램 개요
- 라즈베리파이와 MQTT 통신을 사용하여 두 사용자(플레이어)가 하나의 폭탄 해체 시뮬레이션을 실행하는 프로그램이다.
- 라즈베리파이와 연결된 여러 개의 센서들 중 랜덤으로 선택된 3개의 센서를 이용하여 문제를 해결하면 최종적으로 폭탄이 해체된다.
### 프로그램 설명

![image](https://user-images.githubusercontent.com/39904216/90207113-6f8c3200-de20-11ea-9e68-644bb8b3549a.png)

- xshell 콘솔을 보고 문제를 설명하는 사람이 publish하고, 문제를 푸는 사람이 subscribe한다.
- 다음의 5개의 센서 중 3개의 센서가 문제로 선택된다.
  - 적외선 센서
  - MPU6050(Gyro/Acc 센서)
  - 버튼 센서
  - 조도 센서
  - 피에조 부저
- 각 문제의 정답은 publish 콘솔 창에 보인다.
- publish는 subscribe가 정답을 맞출 수 있도록 정해진 시간 동안 콘솔에 설명을 적는다.
- MQTT 통신을 통해 publish가 입력한 내용이 subscribe 콘솔 창에 출력된다.
- subscribe가 3개의 문제를 모두 맞춘 경우 프로그램이 종료된다.
