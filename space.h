#ifndef space_H
#define space_H

#include <iostream>
#include <iterator>
#include <list>
#include "cella.h"

class space
{

private:
    unsigned long _w;
    unsigned long _h;

public:
    long generazione = 1;
    std::list<cella *> celle;

    /**
        Costruttori di default
    */
    space() : _h(0), _w(0) {}

    space(int w, int h) : _h(255), _w(255)
    {
        _w = w;
        _h = h;
    }

    space(long w, long h) : _h(h), _w(w)
    {
        _w = w;
        _h = h;
    }
    /**
        Distruttore
    */
    ~space()
    {
        _w = _h = 0;
        clear();
    }

    /**
        Cancella tutti le celle
    */
    void clear()
    {

        // delete all cells
        std::list<cella *>::iterator it = celle.begin();
        while (it != celle.end())
        {
            delete *it;
            it++;
        }

        celle.clear();

#ifndef NDEBUG
        std::cout << "[Universe is being deleted]" << std::endl;
#endif
    }

    bool alive()
    {
        return (!celle.empty());
    }

    bool deletecella(long x, long y)
    {

        bool found = false;

        if (checkpoint(x, y) == false)
            return false;

        std::list<cella *>::iterator itc = celle.begin();
        while ((itc != celle.end()) && (found == false))
        {
            if ((**itc).x == x && (**itc).y == y)
            {
                found = true;
                delete *itc;

                break;
            }

            itc++;
        }
        if (found)
        {
            celle.remove(*itc);
            return true;
        }

        return false;
    }

    /**
     * void step in universe richiama lo step per ogni cella viva , space conosce le adiacenti
     * */
    cella *getcella(long x, long y)
    {

        if (checkpoint(x, y) == false)
            return (new cella()); // should launch an exception instead

        std::list<cella *>::iterator itc = celle.begin();
        while (itc != celle.end())
        {
            if ((**itc).x == x && (**itc).y == y)
                return (*itc); // ritorna una cella esistente

            // std::cout<<"("<<(**itc).x << ","<< (**itc).y <<")   ";
            itc++;
        }

        cella *n = new cella(x, y);
        celle.push_back(n);

        // std::cout << x << "," << y <<" (Creata una cella virtuale) \n\r";

        return n;
    }

    cella *newcella(long x, long y)
    {

        cella *n = new cella(x, y);
        n->value = -1;
        celle.push_back(n);
        // std::cout<<" Creata una cella in posizione " << x << ","<< y << std::endl;
        return n;
    }

    bool checkpoint(long x, long y)
    { // check if a point exist in space
        if ((abs(x * 2) > _w) || (abs(y * 2) > _h))
            return false;
        else
            return true;
    }

    long getNcelle()
    {
        return celle.size();
    }

    void step(cella &c)
    {

        if (c.value < 0)
        { // SE la celle è viva

            cella *adiacente;

            for (int i = 0; i < 8; i++)
            { // aggiorna il numero di vicini

                adiacente = (getcella(c.adiacenti[i][0], c.adiacenti[i][1]));

                if (adiacente->value < 0)
                { // se è viva
                    c.vicini++;
                }
                else
                { // se è una cella virtuale  (before was: else if (adiacente->value<=2) )
                    adiacente->value++;
                }

                // non fare niente se è una cella fuori dallo spazio o già candidata alla popolazione
            }
        }
    }

    void phase()
    {

        // aggiorno i vicini di ogni cella viva, creo e assegno un valore alle celle virtuali
        for (std::list<cella *>::iterator it = celle.begin(); it != celle.end(); ++it)
            step(**it);

        // std::cout << "fine fase step() - ultima cella su " << celle.size() << std::endl;

        std::list<cella *> newcelle; // lista di appoggio per celle
        cella tmp;

        for (std::list<cella *>::iterator it = celle.begin(); it != celle.end(); ++it)
        {

            tmp = **it;

            if (((tmp.value < 3) && (tmp.value != -1)) || tmp.vicini > 3)
            { // morte per insufficienza o sovrappopolamento
                delete *it;
            }
            else if (tmp.value == 3)
            { // nascita di una nuova cella
                (**it).value = -1;
                (**it).vicini = 0;
                newcelle.push_back(*it);
            }
            else if (tmp.value = -1)
            { // conservazione o dispersione di una cella VIVA
                // morte per dispersione
                if (tmp.vicini < 2)
                {
                    delete *it;
                }
                else
                {
                    // persistenza
                    (**it).vicini = 0;
                    (**it).vita++;
                    newcelle.push_back(*it);
                }
            }
            // END OF JUDGMENT
        }

        celle.clear();
        celle.swap(newcelle);

        generazione++;
    }
};

#endif
