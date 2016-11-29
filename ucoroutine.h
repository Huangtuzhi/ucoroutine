#ifndef _UCOROUTINE_H
#define _UCOROUTINE_H

#include <stdio.h>
#include <ucontext.h>
#include <vector>
using namespace std;

#define DEFAULT_STACK_SIZE (1024 * 128)

enum coroutineState 
{
	IDLE,
	RUNNABLE,
	RUNNING,
	SUSPEND
};

typedef void (*Func)(void* arg);

typedef struct ucoroutine_t
{
	ucontext_t ctx;
	Func func;
	void* arg;
	enum coroutineState state;
	char stack[DEFAULT_STACK_SIZE];
}ucoroutine_t;

typedef struct schedule_t
{
	ucontext_t main;
	int running_coroutine;
	vector<ucoroutine_t> coroutines;
	schedule_t(): running_coroutine(-1) {}
} schedule_t;

static void ucoroutine_body(schedule_t* ps);

int ucoroutine_create(schedule_t &schedule, Func func, void* arg);

void ucoroutine_resume(schedule_t &schedule, int id);

void ucoroutine_yield(schedule_t &schedule);

int schedule_finished(const schedule_t& schedule);

#endif
