#include <Python.h>
#include <wiringPi.h>

static PyObject* Left(PyObject* self, PyObject* args) {//Left를 호출했을 때 실행되는 함수

	int pin = 0;

	//파이썬에서 C모듈로 보낸 인자 하나를 int형으로 받아온다
	if (!PyArg_ParseTuple(args, "i", &pin))
		return NULL;

	wiringPiSetupGpio();//wiringPI로 BCM모드 사용을 허용
	pinMode(pin, OUTPUT);//현재 입력으로 들어온 핀을 출력용으로 사용한다
	digitalWrite(pin, HIGH);//핀을 HIGH전력, 즉 LED ON
	sleep(1);//ON이 유지될 시간을 설정해준다(1초)
	digitalWrite(pin, LOW);//핀을 LOW전력, 즉 LED OFF
	sleep(1);//OFF가 유지될 시간을 설정해준다(1초)

	Py_RETURN_NONE;
}

static PyObject* Right(PyObject* self, PyObject* args) {//RIGHT를 호출했을 때 실행되는 함수

	int pin = 0;

	//파이썬에서 C모듈로 보낸 인자 하나를 int형으로 받아온다
	if (!PyArg_ParseTuple(args, "i", &pin))
		return NULL;

	wiringPiSetupGpio();//BCM모드를 사용
	pinMode(pin, OUTPUT);//현재 입력으로 넘어온 핀을 출력용으로 사용
	digitalWrite(pin, HIGH);//핀의 출력을 HIGH, LED ON으로 한다
	sleep(1);
	digitalWrite(pin, LOW);//핀의 출력을 LOW, LED OFF로 한다
	sleep(1);
	Py_RETURN_NONE;
}

static PyMethodDef keywdarg_methods[] = {
	//파이썬에서 Left로 함수를 호출하면 C모듈의 Left함수를 호출한다
	//파이썬에서 Right로 함수를 호출하면 C모듈의 Right함수를 호출한다
   {"Left", (PyCFunction)Left, METH_VARARGS, NULL},
   {"Right", (PyCFunction)Right, METH_VARARGS, NULL},
   {NULL, NULL, 0, NULL}
}