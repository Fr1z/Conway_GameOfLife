#ifndef WCONTAINER_H
#define WCONTAINER_H

#include "utils.h"
#include <math.h>
#include <vector>

#include <FL/Fl_Widget.H>
#include <FL/names.h>
#include "space.h" //game universe

class WorldContainer : Fl_Widget
{
	long space_width, space_height;
	long x_min, y_min, x_max, y_max;
	double cx, cy; // current view start virtual position (0,0)real
	int _h, _w;	   // this can be resized in future

	std::string vpos;						 // for virtual position label
	Point drag_start = (*new Point(-1, -1)); // start drag point
	Point pos = Point(3, 6);				 // GUI position
	bool editing = false;

	space *Universe;

public:
	Windows *gui;
	bool editmode = false;
	double scale;
	int c_dim = 15; // dimension of base cella (without outline)

	~WorldContainer()
	{
		c_dim = 0;
		scale = 0;

		delete Universe;
	}

	void update()
	{
		redraw();
	}

	void step()
	{
		Universe->phase();
		update();
	}

	WorldContainer(int width, int height, long spw, long sph) : _w(width), _h(height), space_width(spw), space_height(sph), Fl_Widget(3, 6, width, height, "worldContainer")
	{

		// Creating game universe
		Universe = new space(space_width, space_height);

		// set limits to virtual space
		x_min = -floor((space_width - 1) / 2);
		x_max = floor(space_width / 2);
		y_min = -floor((space_height - 1) / 2);
		y_max = floor(space_height / 2);

		scale = 1;	   // this is an example
		cx = cy = -20; // should be different, like default view of 30 cell width so 15*30*scale = c_dim*scale*ncell

		// creating some life
		Universe->newcella(0, 0);
		Universe->newcella(1, 0);
		Universe->newcella(2, 0);
	}

	VPoint get_virtual_pos()
	{

		int real_x = Fl::event_x();
		int real_y = Fl::event_y();

		long vx, vy;

		// round
		// ottengo le coordinate virtuali partendo da quelle reali
		vx = cx + (round(real_x / (scale * c_dim)));
		vy = cy + (round(real_y / (scale * c_dim)));

		// sottraggo mezza cella per fix del quadrato
		return VPoint(vx, vy);
	}

	Point get_real_pos(long lx, long ly)
	{

		double rs = scale * c_dim;

		int real_x = floor((lx - cx) * rs);
		int real_y = floor((ly - cy) * rs);

		return Point(real_x, real_y);
	}

	bool newcella()
	{
		if (Universe->checkpoint(get_virtual_pos().x, get_virtual_pos().y) == true)
		{
			cella *a_cella = (*Universe).getcella(get_virtual_pos().x, get_virtual_pos().y);
			a_cella->value = -1;
			return true;
		}
		else
		{
			return false;
		}
	}

	space *getspace()
	{
		return Universe;
	}

	int handle(int event)
	{

		// variable for zooming
		Point rpos = get_real_pos(get_virtual_pos().x, get_virtual_pos().y);
		double kw, kh;

		switch (event)
		{

		case FL_KEYDOWN: // if event is key down
			switch (Fl::event_key())
			{
			case FL_Left:
				cx += -1;
				break; // left arrow
			case FL_Right:
				cx += 1;
				break; // left arrow
			case FL_Up:
				cy += -1;
				break; // up arrow
			case FL_Down:
				cy += 1;
				break; // down arrow
			}
			this->redraw();
			return 1;

		case FL_KEYUP: // if event is key down
			switch (Fl::event_key())
			{
			case FL_Control_L:
				// if simulation running..
				if (editmode)
				{
					editmode = false;
					gui->bt_edit->value(0);
				}
				else
				{
					editmode = true;
					gui->bt_edit->value(1);
				}
				// Spacebar 4 simulate?
			}
			return 1;

		case FL_PUSH:

			std::cout << (_w / ((scale + 0.05) * c_dim)) << std::endl;
			if (editmode)
			{

				// call newcell
				if (Fl::event_button() == FL_LEFT_MOUSE)
				{
					newcella(); // add a cella in current position
					editing = true;
				}
				else if (Fl::event_button() == FL_RIGHT_MOUSE)
				{
					// call deletecell
					if (Universe->deletecella(get_virtual_pos().x, get_virtual_pos().y))
						std::cout << "cella cancellata" << std::endl;
					editing = true;
				}
			}

			// std::cout << "ULife: " << Universe->getNcelle() << std::endl;
			return 1;
		case FL_DRAG:
			if (editing)
			{
				if (Fl::event_button() == FL_LEFT_MOUSE)
				{
					newcella(); // add a cella in current position
				}
				else if (Fl::event_button() == FL_RIGHT_MOUSE)
				{
					// call deletecell
					if (Universe->deletecella(get_virtual_pos().x, get_virtual_pos().y))
						std::cout << "cella cancellata" << std::endl;
				}
			}
			else
			{
				if (drag_start.x == -1)
				{ // if drag start
					drag_start.x = Fl::event_x();
					drag_start.y = Fl::event_y();
				}
				else
				{
					if ((cx >= x_min) && (cy >= y_min) && ((cx + (_w / (scale * c_dim))) <= x_max) && ((cy + (_h / (scale * c_dim))) <= y_max))
					{						  // limit shift out
						kw = (scale * c_dim); // kw used as temp variable
						cx += ((drag_start.x / kw) - (Fl::event_x() / kw));
						cy += ((drag_start.y / kw) - (Fl::event_y() / kw));
					}
					else
					{

						if (cx < x_min)
							cx = x_min;
						if (cy < y_min)
							cy = y_min;
						if (cx + (_w / (scale * c_dim)) > x_max)
							cx = x_max - ceil(_w / (scale * c_dim));
						if (cy + (_h / (scale * c_dim)) > y_max)
							cy = y_max - ceil(_h / (scale * c_dim));
					}
					// Update drag points
					drag_start.x = Fl::event_x();
					drag_start.y = Fl::event_y();
				}
			}
			this->redraw();
			return 1;
		case FL_RELEASE:
			editing = false;
			drag_start.x = drag_start.y = -1; // resetting the drag
			return 1;
		case FL_MOVE:

			vpos.clear(); // clear string vpos

			vpos.append("X: ");
			vpos.append(std::to_string(get_virtual_pos().x));
			vpos.append(" Y: ");
			vpos.append(std::to_string(get_virtual_pos().y));
			vpos.append(" 	Life: ");
			vpos.append(std::to_string(Universe->getNcelle()));
			// update vpos 4 draw
			this->damage(1);
			return 1;

		case FL_MOUSEWHEEL:
			// std::cout << "diff cx: " << cx << ", " << cy << "	" ;

			kw = ((double)(rpos.x)) / _w; // rapporto posizioni
			kh = ((double)(rpos.y)) / _h;

			if (Fl::event_dy() == 1)
			{
				// ZOOM OUT
				if (((cx + (_w / (scale * c_dim))) < x_max) && ((cy + (_h / (scale * c_dim))) < y_max))
				{ // limit zoom-out of universe
					// new visual relative to zoom out point
					cx = (get_virtual_pos().x - round((_w / ((scale - 0.09) * c_dim)) * kw) - round(2 * (_w / c_dim) * (1 / (scale - 0.09) - (1 / scale))) * (0.5 - kw));
					cy = (get_virtual_pos().y - round((_h / ((scale - 0.09) * c_dim)) * kh) - round(2 * (_h / c_dim) * (1 / (scale - 0.09) - (1 / scale))) * (0.5 - kh));

					scale -= 0.1;
				}
				if (scale < 0.1)
					scale = 0.1;
			}
			else
			{
				// ZOOM IN

				// keep fieldview ratio
				cx = (get_virtual_pos().x - round((_w / ((scale + 0.1) * c_dim)) * kw));
				cy = (get_virtual_pos().y - round((_h / ((scale + 0.1) * c_dim)) * kh));

				scale += 0.1;

				if (scale > 2.8)
					scale = 2.8;
			}
			this->redraw();

			return 1;

		case FL_ENTER:
			return 1;
		case FL_LEAVE:
			return 1;
		case FL_SHORTCUT:
			// let to gui handler?
			break;
		default:
			// Usually  FL_NO_EVENT
			return 0;
		}
		return 0;
	}

	/**
	 * DRAWING FUNCTIONS
	 * */

	bool is_on_viewport(cella &c)
	{
		double rs = scale * c_dim;

		if ((c.x > cx) && (c.y > cy) && (c.x < cx + (_w * rs)) && (c.y < cy + (_h * rs)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void draw_cella(cella &c)
	{
		Fl_Boxtype type = FL_FLAT_BOX;
		// get position of cell
		Point rpos = get_real_pos(c.x, c.y);

		int irs = floor((scale * c_dim) / 2); // center cell drawing at point
		rpos.x -= irs;
		rpos.y -= irs;

		Point nextpos = get_real_pos(c.x + 1, c.y + 1);
		nextpos.x -= irs;
		nextpos.y -= irs; // get right and botton cell real position

		// calculating width s
		int wd = (nextpos.x - rpos.x);
		int hd = (nextpos.y - rpos.y);

		if (c.vita <= 3)
			if (scale > 0.4)
				fl_draw_box(type, rpos.x, rpos.y, wd - 1, hd - 1, FL_LIGHT2);
			else // when zoom is far stop drawing grid
				fl_draw_box(type, rpos.x, rpos.y, wd, hd, FL_LIGHT2);

		else
			fl_draw_box(type, rpos.x, rpos.y, wd - 1, hd - 1, FL_BLUE);
	}

	void draw()
	{

		fl_draw_box(FL_FLAT_BOX, pos.x, pos.y, _w, _h, FL_BLACK);

		fl_line_style(FL_SOLID, 1);
		fl_color(FL_WHITE);
		fl_line(pos.x, pos.y, pos.x, pos.y + _h);			// ^< |
		fl_line(pos.x, pos.y, pos.x + _w, pos.y);			// ^< -
		fl_line(pos.x + _w, pos.y, pos.x + _w, pos.y + _h); // >v |
		fl_line(pos.x, pos.y + _h, pos.x + _w, pos.y + _h); // >v -

		// draw the universe
		std::list<cella *>::iterator itc = Universe->celle.begin();
		while (itc != Universe->celle.end())
		{

			if ((**itc).value == -1)	   // if a live cell
				if (is_on_viewport(**itc)) // is on viewport?
					draw_cella(**itc);	   // draw it

			itc++;
		}

		if (damage() & (FL_DAMAGE_ALL | 1))
		{
			// if settings draw position
			fl_color(FL_WHITE);
			fl_draw(vpos.c_str(), 9, pos.y + _h - 10);
		}
	}
};
#endif
