#ifndef GUI_H
#define GUI_H

#include <FL/Fl_Double_Window.H>
#include <string>

#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H> //per running simulation


struct Windows : Fl_Double_Window {

	Fl_Button *bt_edit;
 	Fl_Light_Button *bt_simulate;
	Fl_Button *bt_step;
	Fl_Button *bt_clear;

	Windows(int width, int height, std::string title)
		: Fl_Double_Window(0, 0, width, height, title.c_str())
		{

			//Fl::add_handler(handle);

			bt_step = new Fl_Button(width-60,height-28,58,26, "step @->");
			bt_simulate = new Fl_Light_Button(width-160,height-28,90,26, "simulate@>");
			bt_edit = new Fl_Light_Button(width-240,height-28,58,26, " edit ");
			bt_clear = new Fl_Button(10,height-28,58,26, " clear ");

			bt_step->labelcolor(FL_BLACK);
			bt_step->type(FL_NORMAL_BUTTON);
			
			bt_step->labelsize(14);
			bt_simulate->labelsize(14);
			bt_clear->labelsize(14);
			bt_edit->labelsize(14);

			color(FL_DARK3);
		}

/*

*/
/*
void draw(){

	Fl_Double_Window::draw();
		
}

*/
	

};


#endif
