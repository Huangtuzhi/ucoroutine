#include "ucoroutine.h"

int ucoroutine_create(schedule_t &schedule, Func func, void* arg)
{
	int id = 0;
	int coroutine_num = schedule.coroutines.size();

	for (id = 0; id < coroutine_num; id++) {
		if (schedule.coroutines[id].state == IDLE) {
			break;
		}
	}

	if (id == coroutine_num) {
		ucoroutine_t coroutine;
		schedule.coroutines.push_back(coroutine);
	}

	ucoroutine_t *t = &(schedule.coroutines[id]);

	t->state = RUNNABLE;
	t->func = func;
	t->arg = arg;

	return id;
}

void ucoroutine_body(schedule_t *ps)
{
	int id = ps->running_coroutine;

	if (id != -1) {
		ucoroutine_t *t = &(ps->coroutines[id]);
		t->func(t->arg);
		t->state = IDLE;
		ps->running_coroutine = -1;
	}
}

void ucoroutine_resume(schedule_t &schedule, int id)
{
	if (id < 0 || id > schedule.coroutines.size()) {
		return;
	}

	ucoroutine_t *t = &(schedule.coroutines[id]);

	switch(t->state) {
		case RUNNABLE:
			getcontext(&(t->ctx));
			t->ctx.uc_stack.ss_sp = t->stack;
			t->ctx.uc_stack.ss_size = DEFAULT_STACK_SIZE;
			t->ctx.uc_stack.ss_flags = 0;
			t->ctx.uc_link = &(schedule.main);
			t->state = RUNNING;

			schedule.running_coroutine = id;
			makecontext(&(t->ctx), (void(*)(void))(ucoroutine_body), 1, &schedule);

		case SUSPEND:
			swapcontext(&(schedule.main), &(t->ctx));
			break;
		default:;
	}
}

void ucoroutine_yield(schedule_t &schedule)
{
	if (schedule.running_coroutine != -1) {
		ucoroutine_t *t = &(schedule.coroutines[schedule.running_coroutine]);
		t->state = SUSPEND;
		schedule.running_coroutine = -1;
		swapcontext(&(t->ctx), &(schedule.main));
	}
}

int schedule_finished(const schedule_t &schedule)
{
	if (schedule.running_coroutine != -1) {
		return 0;
	}

	else {
		for (int i = 0; i < schedule.coroutines.size(); i++) {
			if (schedule.coroutines[i].state != IDLE) {
				return 0;
			}
		}
	}

	return 1;
}

