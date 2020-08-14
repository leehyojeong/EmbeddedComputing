#include <Python.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void my_timer_callback();
void* my_thread();

PyObject* cb_func = NULL;//���̽㿡�� ���ڷ� �Ѿ�� callback�Լ��� ���� PyObject����
int count = 0;//���ʵڿ� callback�Լ��� ���������� ���� count����

static PyObject* py_add_callback(PyObject* self, PyObject* args, PyObject* kwargs) {
	//�ݹ��Լ��� ȣ���ϴ� �Լ�
	pthread_t p_thread;//�����带 �����ϱ� ���� ����
	int p_id = 0;
	static char* kwlist[] = { "count", "cb", NULL };//count���� cb���� ���ڷ� �޴´�

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iO", kwlist, &count, &cb_func)) {
		//���̽㿡�� ���ڷ� �ѱ� ���� Ű���� �������� �޴´�
		//(count���� ��, cb_func���� ���̽��� �ݹ��Լ��� ����ȴ�)
		return NULL;
	}

	if (cb_func != NULL && !PyCallable_Check(cb_func)) {
		//���� callback�Լ��� ����� ������ ���� �ʾҴٸ� ����
		PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
		return NULL;
	}

	p_id = pthread_create(&p_thread, NULL, my_thread, NULL);
	//my_thread�Լ��� �����ϴ� �����带 ����
	if (p_id < 0) {//�����尡 ����� �������� �ʾҴٸ� ����
		perror("thread create error : ");
		exit(0);
	}
	pthread_detach(p_thread);//�����带 ���ο��� �и��Ѵ�.

	Py_RETURN_NONE;
}

static PyMethodDef keywdarg_methods[] = {
	//���̽㿡�� add_callback�� ȣ���� ��� C����� py_add_callback�� ȣ���Ѵ�
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
	return PyModule_Create(&keywdargmodule);//PyModule�� �����Ѵ�
}

void* my_thread() {//�����尡 ������ �Լ�
	struct sigaction sa;//signal����ü�� ����
	struct itimeval my_timer;//Ÿ�̸Ӹ� �����Ѵ�

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = my_timer_callback;

	//SIGALRM�ñ׳��� �����ϸ� sa�� ��ϵ� my_timer_callback�Լ��� ȣ���ϰڴٴ� �ǹ�
	sigaction(SIGALRM, &sa, NULL);

	my_timer.it_value.tv_sec = count;//Ÿ�̸��� �ð��� count��ŭ �����Ѵ�.
	my_timer.it_value.tv_usec = 0;
	my_timer.it_interval.tv_sec = 0;
	my_timer.it_interval.tv_usec = 0;

	//���� �ð��� ������� �ϸ� count��ŭ�� �ð��� ������ SIGALRM�� �����ش�
	setitimer(ITIMER_REAL, &my_timer, NULL);
	printf("Start Timer\n");
}

void my_timer_callback() {
	PyObject* result;
	PyGILState_STATE gstate;

	gstate = PyGILState_Enure();

	//���̽��� �ݹ��Լ��� ȣ���Ѵ�.(cb_func���� ���̽��� �ݹ��Լ��� ����Ǿ��ִ�)
	result = PyObject_CallFunction(cb_func, "");

	PyGILState_Release(gstate);
}