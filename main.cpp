
#include "GUI.h"
#include "worldContainer.h"

WorldContainer *world;
Windows* win;
/**
 * TO DO
 * 
	draw  pos text by widget of a gui
	fix pos near 0,0
	button simulate 
	slider speed (timer based)
	resize 
	speed up drawing using a variable for scaling (avoiding scale*cdim every time)
	consider to use surface for drawing
*/

void cb_edit(Fl_Widget *, void *)
		{
			(world->editmode) ? world->editmode=false : world->editmode=true;	
		}

void cb_step(Fl_Widget *, void *)
		{
			if (world->getspace()->alive())
				world->step();
			////std::cout << "hey callback" << std::endl;
		}

void cb_clear(Fl_Widget *, void *)
		{
			world->getspace()->clear();
			world->update();
		}

void cb_simulate(Fl_Widget *, void *)
		{
			//should use a a timer or scheduled event for this |* UNCOMPLETED *|
			world->step();
		}

	int GUIhandler(int event){

			switch(event){
				case FL_KEYDOWN:  //if event is key down
					switch(Fl::event_key()) {
						case FL_Escape: exit (1); break;  	// Esc key
					}
					return 1;
					break;
				case FL_KEYUP:  //if event is key down
					switch(Fl::event_key()) {
						case FL_Control_L:
							//if simulation running..
							if(world->editmode) 
							{ 
								world->editmode=false;
								win->bt_simulate->value(0);
							}else{ 
								world->editmode=true; 
								win->bt_simulate -> value(1);
							}
							
							break;
					}
					return 1;
					break;
			
			}
		
			return 0;

	}


int main() {

	long generation = 0;
	long _uw = 256;
	long _uh = 256;

	int w{800}, h{640};

	win = new Windows(w,h,"Fl test");
	world = new WorldContainer(w-8,h-40,_uw,_uh);
	
	world->gui=win;

	win->bt_edit->callback(cb_edit,0);
	win->bt_step->callback(cb_step,0);
	win->bt_clear->callback(cb_clear,0);

	win->bt_simulate->callback(cb_simulate,0);
	win->resizable(win);
  	win->end();
  	win->show();
	return Fl::run();

}

	
