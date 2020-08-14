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
		//구조체 만큼의 메모리 영역을 확보합니다.
		//파이썬에서 ON을 호출하는 경우 실행되는 함수입니다.
        if (!PyArg_ParseTuple(args, "i", &pinNum)) {
			//파이썬에서 넘겨준 인자(pin번호)를 pinNum변수에 저장합니다.
            return NULL;

        }

        ledPin = pinNum;                 

        int fd = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
		//해당 드라이버에 접근하여 GPIO제어를 합니다.
        if (fd >= 0) {

            memcpy(&ledctl, &ledPin, sizeof(ledCtl));

            ledctl.pin = pinNum;
            ledctl.funcNum = 1;

                                                             

            ioctl(fd, MY_IOC_SET, &ledctl);

            ioctl(fd, MY_IOC_GPIO_SETFUNC);

        }
		//해당 드라이버에 등록된 MY_ON명령어를 통해 
		//LED의 불을 켜줍니다.
        ioctl(fd, MY_ON);

}

 

static PyObject* Off(PyObject* self, PyObject* args) {

        int ledPin = 0, pinNum = 0;

        ledCtl ledctl;
        memset(&ledctl, 0, sizeof(ledCtl));
		//파이썬에서 OFF를 호출하는 경우 실행되는 함수입니다.
        if (!PyArg_ParseTuple(args, "i", &pinNum)) {
			//파이썬에서 넘겨준 인자(pin번호)를 pinNum변수에 저장합니다.
            return NULL;

        }

        ledPin = pinNum;

        int fd = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
		//해당 드라이버에 접근하여 GPIO제어를 합니다.
        if (fd >= 0) {

            memcpy(&ledctl, &ledPin, sizeof(ledCtl));

            ledctl.pin = pinNum;

            ledctl.funcNum = 1;

            ioctl(fd, MY_IOC_SET, &ledctl);

            ioctl(fd, MY_IOC_GPIO_SETFUNC);

        }
		//해당 드라이버에 등록된 MY_OFF명령어를 통해 
		//LED의 불을 꺼줍니다.
        ioctl(fd, MY_OFF);

}

 

static PyMethodDef keywdarg_methods[] = {
	//함수를 등록합니다.
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
