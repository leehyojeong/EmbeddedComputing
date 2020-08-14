#include <Python.h>
#include <wiringPi.h>

static PyObject* Left(PyObject* self, PyObject* args) {//Left�� ȣ������ �� ����Ǵ� �Լ�

	int pin = 0;

	//���̽㿡�� C���� ���� ���� �ϳ��� int������ �޾ƿ´�
	if (!PyArg_ParseTuple(args, "i", &pin))
		return NULL;

	wiringPiSetupGpio();//wiringPI�� BCM��� ����� ���
	pinMode(pin, OUTPUT);//���� �Է����� ���� ���� ��¿����� ����Ѵ�
	digitalWrite(pin, HIGH);//���� HIGH����, �� LED ON
	sleep(1);//ON�� ������ �ð��� �������ش�(1��)
	digitalWrite(pin, LOW);//���� LOW����, �� LED OFF
	sleep(1);//OFF�� ������ �ð��� �������ش�(1��)

	Py_RETURN_NONE;
}

static PyObject* Right(PyObject* self, PyObject* args) {//RIGHT�� ȣ������ �� ����Ǵ� �Լ�

	int pin = 0;

	//���̽㿡�� C���� ���� ���� �ϳ��� int������ �޾ƿ´�
	if (!PyArg_ParseTuple(args, "i", &pin))
		return NULL;

	wiringPiSetupGpio();//BCM��带 ���
	pinMode(pin, OUTPUT);//���� �Է����� �Ѿ�� ���� ��¿����� ���
	digitalWrite(pin, HIGH);//���� ����� HIGH, LED ON���� �Ѵ�
	sleep(1);
	digitalWrite(pin, LOW);//���� ����� LOW, LED OFF�� �Ѵ�
	sleep(1);
	Py_RETURN_NONE;
}

static PyMethodDef keywdarg_methods[] = {
	//���̽㿡�� Left�� �Լ��� ȣ���ϸ� C����� Left�Լ��� ȣ���Ѵ�
	//���̽㿡�� Right�� �Լ��� ȣ���ϸ� C����� Right�Լ��� ȣ���Ѵ�
   {"Left", (PyCFunction)Left, METH_VARARGS, NULL},
   {"Right", (PyCFunction)Right, METH_VARARGS, NULL},
   {NULL, NULL, 0, NULL}
}