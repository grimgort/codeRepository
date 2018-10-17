#include <iostream>
#include "Zfraction.h"

using namespace std;

int main()
{

    Zfraction a(1,1);      //Déclare une fraction valant 4/5
    Zfraction b(54,5);        //Déclare une fraction valant 2/1 (ce qui vaut 2)
    Zfraction c,d;         //Déclare deux fractions valant 0

    c = a+b;               //Calcule 4/5 + 2/1 = 14/5
    d = a*b;
    c=a/b;

    cout << a << " + " << b << " = " << c << endl;

    cout << a << " * " << b << " = " << d << endl;
    b.numerateur();

    if(a > b)
        cout << "a est plus grand que b." << endl;
    else if(a==b)
        cout << "a est egal a b." << endl;
    else
        cout << "a est plus petit que b." << endl;
    cout << b.reel() << endl;


    return 0;
}
