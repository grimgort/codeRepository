#ifndef CATEGORY_H_INCLUDED
#define CATEGORY_H_INCLUDED


/*
Attribut un entier � une cat�gorie. "1 << 0" : le 1 d�finit que l'etier est cod� sur 1bit ; le "0" donne la valeur d'initialisation.
*/
namespace Category
{
    enum type
    {
        none = 0,
        Scene = 1 << 0,
        PlayerAircraft = 1 << 1,
        AlliedAircraft = 1 << 2,
        EnemyAircraft = 1 << 3,
    };
}

#endif // CATEGORY_H_INCLUDED
