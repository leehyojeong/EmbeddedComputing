#include <Python.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void my_timer_callback();
void* my_thread();

PyObject* cb_func = NULL;//파이썬에서 인자로 넘어온 callback함수를 받을 PyObject변수
int count = 0;//몇초뒤에 callback함수를 실행할지에 대한 count변수

static PyObject* py_add_callback(PyObject* self, PyObject* args, PyObject* kwargs) {
	//콜백함수를 호출하는 함수
	pthread_t p_thread;//스레드를 생성하기 위한 변수
	int p_id = 0;
	static char* kwlist[] = { "count", "cb", NULL };//count값과 cb값을 인자로 받는다

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iO", kwlist, &count, &cb_func)) {
		//파이썬에서 인자로 넘긴 값을 키워드 형식으로 받는다
		//(count에는 초, cb_func에는 파이썬의 콜백함수가 저장된다)
		return NULL;
	}

	if (cb_func != NULL && !PyCallable_Check(cb_func)) {
		//만약 callback함수가 제대로 설정이 되지 않았다면 에러
		PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
		return NULL;
	}

	p_id = pthread_create(&p_thread, NULL, my_thread, NULL);
	//my_thread함수를 실행하는 스레드를 생성
	if (p_id < 0) {//스레드가 제대로 생성되지 않았다면 오류
		perror("thread create error : ");
		exit(0);
	}
	pthread_detach(p_thread);//스레드를 메인에서 분리한다.

	Py_RETURN_NONE;
}

static PyMethodDef keywdarg_methods[] = {
	//파이썬에서 add_callback을 호출한 경우 C모듈의 py_add_callback을 호출한다
   {"add_callback", (PyCFunction)py_add_callback, METH_VARARGS | METH_KEYWORDS, NULL},
   {NULL, NULL, 0, NULL}
};

static struct PyModuleDef keywdargmodule = {
   PyModuleDef_HEAD_INIT,
   "Timer",
   NULL,
   -1,
   keywdarg_methods
};

PyMODINIT_FUNC PyInit_Timer(void) {
	return PyModule_Create(&keywdargmodule);//PyModule을 선언한다
}

void* my_thread() {//스레드가 실행할 함수
	struct sigaction sa;//signal구조체를 선언
	struct itimeval my_timer;//타이머를 선언한다

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = my_timer_callback;

	//SIGALRM시그널이 도착하면 sa에 등록된 my_timer_callback함수를 호출하겠다는 의미
	sigaction(SIGALRM, &sa, NULL);

	my_timer.it_value.tv_sec = count;//타이머의 시간을 count만큼 설정한다.
	my_timer.it_value.tv_usec = 0;
	my_timer.it_interval.tv_sec = 0;
	my_timer.it_interval.tv_usec = 0;

	//실제 시간을 기반으로 하며 count만큼의 시간이 지난뒤 SIGALRM을 보내준다
	setitimer(ITIMER_REAL, &my_timer, NULL);
	printf("Start Timer\n");
}

void my_timer_callback() {
	PyObject* result;
	PyGILState_STATE gstate;

	gstate = PyGILState_Enure();

	//파이썬의 콜백함수를 호출한다.(cb_func에는 파이썬의 콜백함수가 연결되어있다)
	result = PyObject_CallFunction(cb_func, "");

	PyGILState_Release(gstate);
}