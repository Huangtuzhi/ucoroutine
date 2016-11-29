#include "ucoroutine.h"
#include <stdio.h>

void func1(void* args)
{
	puts("This is func1");
}

void func2(void* args)
{
	puts("This is func2");
}

void func3(void* args)
{
	puts("This is func3");
}

int main()
{
	schedule_t s;
	
	int id1 = ucoroutine_create(s, func1, &s);
	int id2 = ucoroutine_create(s, func2, &s);
	int id3 = ucoroutine_create(s, func3, &s);

	while (!schedule_finished(s)) {
		ucoroutine_resume(s, id1);
		ucoroutine_resume(s, id2);
		ucoroutine_resume(s, id3);
	}

	puts("main over.");
	return 0;
}
