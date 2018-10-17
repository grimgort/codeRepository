#include <math.h>

/*
X1 = numero de pixel en x � t+1
Fx0 = Force en x � t
Vx0 = vitesse en x � t
masse = masse de l'objet
Vx1 = vitesse en x � t+1
masse = masse de l'objet
*/
int X1(double Vx0, int X0,double masse,double Fx0, double deltaT)
{
    double Vx1 = Vx0 + Fx0/masse*deltaT ;
    return X0 + floor(deltaT * Vx1) ;
}

/*
Z1 = numero de pixel en z � t+1
Fz0 = Force en z � t
Vz1 = vitesse en z � t+1
Vz0 = vitesse en z � t
masse = masse de l'objet
*/
int Z1(double Vz0, int Z0,double masse,double Fz0, double deltaT)
{
    double Vz1 = Vz0 + Fz0/masse*deltaT ;
    return Z0 + floor(deltaT * Vz1) ;
}











