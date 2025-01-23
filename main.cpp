// For sleep thread
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "threads.h"
#include "GUI.h"
#include "worldContainer.h"

WorldContainer *world;
Windows *win;

static int keep_running = 0;

/**
 * TO DO
 *
	fix view near 0,0
	resizing
	speed up drawing using a variable for scaling (avoiding scale*cdim every time)
	consider to use surface for drawing
*/
void *thread_task(void *p)
{
	long id = (long)p;

	while (keep_running)
	{
		Sleep(500); // delay for a half-second
		if (world->getspace()->alive())
			world->step();
	}
	return 0;
}

void cb_edit(Fl_Widget *, void *)
{
	(world->editmode) ? world->editmode = false : world->editmode = true;
}

void cb_step(Fl_Widget *, void *)
{
	if (world->getspace()->alive())
		world->step();
}

void cb_clear(Fl_Widget *, void *)
{
	world->getspace()->clear();
	world->update();
}

void cb_simulate(Fl_Widget *, void *)
{
	if (keep_running == 0)
	{
		keep_running = 1;
		Fl_Thread thread_id;
		static int active_thrds = 1;
		fl_create_thread(thread_id, thread_task, (void *)active_thrds);
	}
	else
	{
		keep_running = 0;
	}
}

int main()
{

	long generation = 0;
	long _uw = 256;
	long _uh = 256;

	int w{800}, h{640};

	win = new Windows(w, h, "Game of Life");
	world = new WorldContainer(w - 8, h - 40, _uw, _uh);

	world->gui = win;

	win->bt_edit->callback(cb_edit, 0);
	win->bt_step->callback(cb_step, 0);
	win->bt_clear->callback(cb_clear, 0);
	win->bt_simulate->callback(cb_simulate, 0);

	win->end();
	win->show();

	return Fl::run();
}
