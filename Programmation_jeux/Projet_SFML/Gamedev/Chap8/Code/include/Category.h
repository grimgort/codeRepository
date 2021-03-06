#ifndef CATEGORY_H_INCLUDED
#define CATEGORY_H_INCLUDED


/*
Attribut un entier � une cat�gorie. "1 << 0" : le 1 d�finit que l'etier est cod� sur 1bit ; le "0" donne la valeur d'initialisation.
*/
namespace Category
{
    enum Type
    {
        None = 0,
        SceneAirLayer = 1 << 0,
        PlayerAircraft = 1 << 1,
        AlliedAircraft = 1 << 2,
        EnemyAircraft = 1 << 3,
        Pickup = 1 << 4,
        AlliedProjectile = 1 << 5,
        EnemyProjectile = 1 << 6,
        ParticleSystem = 1 << 7,

        Aircraft = PlayerAircraft | AlliedAircraft | EnemyAircraft,
        Projectile = AlliedProjectile | EnemyProjectile,
    };
}

#endif // CATEGORY_H_INCLUDED
