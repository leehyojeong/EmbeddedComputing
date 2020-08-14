#include <Python.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "Ex_06.h"

 

#define DEVICE_FILENAME "/dev/Ex_06"
 

static PyObject* On(PyObject* self, PyObject* args) {

        int ledPin = 0, pinNum = 0;

        ledCtl ledctl;
        memset(&ledctl, 0, sizeof(ledCtl));         
		//����ü ��ŭ�� �޸� ������ Ȯ���մϴ�.
		//���̽㿡�� ON�� ȣ���ϴ� ��� ����Ǵ� �Լ��Դϴ�.
        if (!PyArg_ParseTuple(args, "i", &pinNum)) {
			//���̽㿡�� �Ѱ��� ����(pin��ȣ)�� pinNum������ �����մϴ�.
            return NULL;

        }

        ledPin = pinNum;                 

        int fd = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
		//�ش� ����̹��� �����Ͽ� GPIO��� �մϴ�.
        if (fd >= 0) {

            memcpy(&ledctl, &ledPin, sizeof(ledCtl));

            ledctl.pin = pinNum;
            ledctl.funcNum = 1;

                                                             

            ioctl(fd, MY_IOC_SET, &ledctl);

            ioctl(fd, MY_IOC_GPIO_SETFUNC);

        }
		//�ش� ����̹��� ��ϵ� MY_ON��ɾ ���� 
		//LED�� ���� ���ݴϴ�.
        ioctl(fd, MY_ON);

}

 

static PyObject* Off(PyObject* self, PyObject* args) {

        int ledPin = 0, pinNum = 0;

        ledCtl ledctl;
        memset(&ledctl, 0, sizeof(ledCtl));
		//���̽㿡�� OFF�� ȣ���ϴ� ��� ����Ǵ� �Լ��Դϴ�.
        if (!PyArg_ParseTuple(args, "i", &pinNum)) {
			//���̽㿡�� �Ѱ��� ����(pin��ȣ)�� pinNum������ �����մϴ�.
            return NULL;

        }

        ledPin = pinNum;

        int fd = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
		//�ش� ����̹��� �����Ͽ� GPIO��� �մϴ�.
        if (fd >= 0) {

            memcpy(&ledctl, &ledPin, sizeof(ledCtl));

            ledctl.pin = pinNum;

            ledctl.funcNum = 1;

            ioctl(fd, MY_IOC_SET, &ledctl);

            ioctl(fd, MY_IOC_GPIO_SETFUNC);

        }
		//�ش� ����̹��� ��ϵ� MY_OFF��ɾ ���� 
		//LED�� ���� ���ݴϴ�.
        ioctl(fd, MY_OFF);

}

 

static PyMethodDef keywdarg_methods[] = {
	//�Լ��� ����մϴ�.
        {"ON",(PyCFunction)On,METH_VARARGS,NULL},

            {"OFF",(PyCFunction)Off,METH_VARARGS,NULL},

            {NULL,NULL,0,NULL}

};

 

static struct PyModuleDef keywdargmodule = {

        PyModuleDef_HEAD_INIT,

            "Ex06",

                NULL,

                    -1,

                        keywdarg_methods

};

 

PyMODINIT_FUNC PyInit_Ex06(void) {

        return PyModule_Create(&keywdargmodule);

}
