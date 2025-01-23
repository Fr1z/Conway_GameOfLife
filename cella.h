#ifndef cella_H
#define cella_H

class cella
{

public:
	long adiacenti[8][2];

	long x;
	long y;
	short value = 0;  // min -1 max 3
	short vicini = 0; // max 8
	int vita = 0;	  // lunghezza della vita (serve per colore)

	/**
		Costruttori di default, errore
	*/
	cella() : x(0), y(0) {}

	cella(int cx, int cy) : x(static_cast<long>(cx)), y(static_cast<long>(cy)), adiacenti{{cx - 1, cy - 1}, {cx - 1, cy}, {cx - 1, cy + 1}, {cx, cy + 1}, {cx + 1, cy + 1}, {cx + 1, cy}, {cx + 1, cy - 1}, {cx, cy - 1}} {}

	cella(long cx, long cy) : x(cx), y(cy), adiacenti{{cx - 1, cy - 1}, {cx - 1, cy}, {cx - 1, cy + 1}, {cx, cy + 1}, {cx + 1, cy + 1}, {cx + 1, cy}, {cx + 1, cy - 1}, {cx, cy - 1}} {}

	/**
		Distruttore
	*/
	~cella()
	{
		die();
	}

	/**
		Cancella tutta le cella
	*/
	void die()
	{
		x = 0;
		y = 0;
		value = 0;
		vicini = 0;
		vita = 0;
		memset(adiacenti, 0, sizeof(adiacenti));
	}
};

#endif
