#include <iostream>
#include "duree.h"


using namespace std;




int main()
{
    Duree duree1(1,2,3), duree2(4,5,6), duree3(1,2,3);


    if (duree1==duree3)
    {
        std::cout << "Les duree sont egal" << endl;
    }
    else
    {
        std::cout << "les duree sont differente" << endl;
    }

    if (duree1!=duree2)
        std::cout << "Les duree sont different" << endl;
    else
        std::cout << "Les durre sont egal" << endl;

    if (duree1<duree2)
        std::cout << " duree1 inferieur a duree 2" << endl;
    else
        std::cout << " duree2 inferieur a duree 1" << endl;

    if (duree1>duree2)
        std::cout << " duree1 superieur a duree 2" << endl;
    else
        std::cout << " duree2 superieur a duree 1" << endl;

    if (duree1<=duree2)
        std::cout << " duree1 inferieur ou egal a duree 2" << endl;
    else
        std::cout << " duree2 inferieur ou egal a duree 1" << endl;

    if (duree1>=duree2)
        std::cout << " duree1 superieur ou egal a duree 2" << endl;
    else
        std::cout << " duree2 superieur ou egal a duree 1" << endl;

    if (duree1>=duree3)
        std::cout << " duree1 superieur ou egal a duree 3" << endl;
    else
    {
        std::cout << " duree3 superieur ou egal a duree 1" << endl;
    }

    cout << duree3 << endl;
    duree3+=duree1 ;
    cout << duree3 << endl;

    duree3= duree1+duree2+duree3 ;
    cout << duree3 << endl;

    duree3-=duree1 ;
    cout << duree1 << endl;
    cout << duree3 << endl;

    duree3= duree1-duree2-duree3 ;
    cout << duree3 << endl;

    cout << duree1 << " et " << duree2 << endl;




    return 0;
}
