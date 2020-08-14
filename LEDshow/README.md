## C 모듈을 이용한 LED_Show

### Task 1
#### Timer Module
- 파이썬에서 원하는 주기와 콜백 함수를 등록하면 해당 주기에 맞추어 등록된 함수를 계속 호출한다.
#### LED Control Module
- 브레드 보드 상의 LED 3개를 GPIO를 사용하여 제어한다.
- 파이썬에서는 Left, Right 함수만 호출하고 GPIO 제어는 C에서 실행한다.
  - __Left 함수__ : 왼쪽부터 순서대로 LED가 켜지도록 한다.
  - __Right 함수__ : 오른쪽부터 순서대로 LED가 켜지도록 한다.
  
### Task 2
- Task 1에서 구현한 두 모듈을 사용하여 사용자의 입력이 0이면 LED를 시계 방향으로 점멸하고, 1이면 반시계 방향으로 점멸하도록 만든다.

### 프로그램 세부 설명
- 흰색선은 GPIO 18, 회색선은 GPIO 20, 보라색선은 GPIO 21에 연결했다.
- 항상 특정 LED에서 시작하는 것이 아니라 이전에 멈추었던 핀 번호부터 시작하기 때문에 시작 시 불이 켜지는 LED 위치가 다를 수 있다.
- 21번부터 점등된 경우 왼쪽에 해당하는 방향은 21-20-18-21-20-18 ... 이 반복됨을 의미한다.
- 18번부터 점등된 경우 오른쪽에 해당하는 방향은 18-20-21-18-20-21 ... 이 반복됨을 의미한다.
![image](https://user-images.githubusercontent.com/39904216/90211956-2a223180-de2d-11ea-9090-900f01364b16.png)
![image](https://user-images.githubusercontent.com/39904216/90211968-2e4e4f00-de2d-11ea-869a-1e4fd7689749.png)
![image](https://user-images.githubusercontent.com/39904216/90211988-3c9c6b00-de2d-11ea-984e-a64c73767788.png)

### 실행 결과
- Left
![image](https://user-images.githubusercontent.com/39904216/90212185-ac125a80-de2d-11ea-8370-15e82877a2ac.png)

- Right
