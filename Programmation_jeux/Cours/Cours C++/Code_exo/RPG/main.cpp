#include <iostream>
#include "Personage.h"
#include "Action.h"

using namespace std;

int main()
{
/*
    Personnage David("David");
    Personnage Goliath("épée Trachante", 20,"Goliath");

    Goliath.attaquer(David) ;
    David.boirePotionDeVie(20);
    Goliath.attaquer(David);
    David.attaquer(Goliath);

    Goliath.changerArme("Casse Tous",40);
    Goliath.attaquer(David);
    Personnage Clone_Goliath(Goliath); //Copie du personnage Goliath

    David.afficherEtat();
    Goliath.afficherEtat();
    Clone_Goliath.afficherEtat();
*/

    Personnage *listePersonnage[5];

    listePersonnage[0] =  new Personnage("David") ;
    listePersonnage[1] =  new Personnage("Goliath") ;
    listePersonnage[2] =  new Personnage("Touriste") ;

    int actionChoisi ;
    int cibleChoisi ;
    int equipementChoisi ;
    do
    {


    cout << "***********************" << endl ;
    cout << "Tour de David :" << endl;
    do
    {
        cout << "Choisissez votre action" << endl ;
        cout << "1=attaquer / 2=lancer boule de feu / 3=equipement / "  << endl ;
        cin >> actionChoisi ;
        switch (actionChoisi)
        {
        case 1:

            do
            {
                cout << "Choisissez votre cible!" << endl ;
                for (int i=0 ; i<3 ; i++)
                {
                    cout << i << " = "<< listePersonnage[i]->getNom() << endl;
                }
                cin >> cibleChoisi ;
                switch (cibleChoisi)
                {
                case 0:
                    listePersonnage[0]->attaquer(*listePersonnage[0]);
                    break;
                case 1:
                    listePersonnage[0]->attaquer(*listePersonnage[1]);
                    break;
                case 2:
                    listePersonnage[0]->attaquer(*listePersonnage[2]);
                    break;
                }
            }while (cibleChoisi < 0 || cibleChoisi > 2 );
            break;
        case 2:
            do
            {
                cout << "Choisissez votre cible!" << endl ;
                for (int i=0 ; i<3 ; i++)
                {
                    cout << i << " = "<< listePersonnage[i]->getNom() << endl;
                }
                cin >> cibleChoisi ;
                switch (cibleChoisi)
                {
                case 0:
                    listePersonnage[0]->bouleDeFeu(*listePersonnage[0]);
                    break;
                case 1:
                    listePersonnage[0]->bouleDeFeu(*listePersonnage[1]);
                    break;
                case 2:
                    listePersonnage[0]->bouleDeFeu(*listePersonnage[2]);
                    break;
                }
            }while (cibleChoisi < 0 || cibleChoisi > 2 );

            break;
        case 3:
            do
            {
                cout << "Choisissez l'equipement à utiliser" << endl ;
                cout << "1 = Potion de vie" << endl ;
                cout << "2 = Potion de mana" << endl ;
                cin >> equipementChoisi ;
                switch (equipementChoisi)
                {
                case 1:
                    listePersonnage[0]->boirePotionDeVie(20);
                    break;
                case 2:
                    listePersonnage[0]->boirePotionDeMana(20);
                    break;
                default:
                    cout << " Mauvais choix!" << endl ;
                    break;
                }
            }while (equipementChoisi < 1 || equipementChoisi > 2 );



            break;
        default:

            cout << "Mauvais choix. Recommencez" << endl ;
            break;
        }
    }while (actionChoisi < 0 || actionChoisi > 3);

    for (int i=0 ; i<3 ; i++)
    {
        listePersonnage[i]->afficherEtat();
    }

    cout << "***********************" << endl ;
    cout << "Tour de Goliath :" << endl;
    do
    {
        cout << "Choisissez votre action" << endl ;
        cout << "1=attaquer / 2=lancer boule de feu / 3=equipement / "  << endl ;
        cin >> actionChoisi ;
        switch (actionChoisi)
        {
        case 1:

            do
            {
                cout << "Choisissez votre cible!" << endl ;
                for (int i=0 ; i<3 ; i++)
                {
                    cout << i << " = "<< listePersonnage[i]->getNom() << endl;
                }
                cin >> cibleChoisi ;
                switch (cibleChoisi)
                {
                case 0:
                    listePersonnage[1]->attaquer(*listePersonnage[0]);
                    break;
                case 1:
                    listePersonnage[1]->attaquer(*listePersonnage[1]);
                    break;
                case 2:
                    listePersonnage[1]->attaquer(*listePersonnage[2]);
                    break;
                }
            }while (cibleChoisi < 0 || cibleChoisi > 2 );
            break;
        case 2:
            do
            {
                cout << "Choisissez votre cible!" << endl ;
                for (int i=0 ; i<3 ; i++)
                {
                    cout << i << " = "<< listePersonnage[i]->getNom() << endl;
                }
                cin >> cibleChoisi ;
                switch (cibleChoisi)
                {
                case 0:
                    listePersonnage[1]->bouleDeFeu(*listePersonnage[0]);
                    break;
                case 1:
                    listePersonnage[1]->bouleDeFeu(*listePersonnage[1]);
                    break;
                case 2:
                    listePersonnage[1]->bouleDeFeu(*listePersonnage[2]);
                    break;
                }
            }while (cibleChoisi < 0 || cibleChoisi > 2 );

            break;
        case 3:
            do
            {
                cout << "Choisissez l'equipement à utiliser" << endl ;
                cout << "1 = Potion de vie" << endl ;
                cout << "2 = Potion de mana" << endl ;
                cin >> equipementChoisi ;
                switch (equipementChoisi)
                {
                case 1:
                    listePersonnage[1]->boirePotionDeVie(20);
                    break;
                case 2:
                    listePersonnage[1]->boirePotionDeMana(20);
                    break;
                default:
                    cout << " Mauvais choix!" << endl ;
                    break;
                }
            }while (equipementChoisi < 1 || equipementChoisi > 2 );



            break;
        default:

            cout << "Mauvais choix. Recommencez" << endl ;
            break;
        }
    }while (actionChoisi < 0 || actionChoisi > 3);

    for (int i=0 ; i<3 ; i++)
    {
        listePersonnage[i]->afficherEtat();
    }

        cout << "***********************" << endl ;
    cout << "Tour de Tourist :" << endl;
    do
    {
        cout << "Choisissez votre action" << endl ;
        cout << "1=attaquer / 2=lancer boule de feu / 3=equipement / "  << endl ;
        cin >> actionChoisi ;
        switch (actionChoisi)
        {
        case 1:

            do
            {
                cout << "Choisissez votre cible!" << endl ;
                for (int i=0 ; i<3 ; i++)
                {
                    cout << i << " = "<< listePersonnage[i]->getNom() << endl;
                }
                cin >> cibleChoisi ;
                switch (cibleChoisi)
                {
                case 0:
                    listePersonnage[2]->attaquer(*listePersonnage[0]);
                    break;
                case 1:
                    listePersonnage[2]->attaquer(*listePersonnage[1]);
                    break;
                case 2:
                    listePersonnage[2]->attaquer(*listePersonnage[2]);
                    break;
                }
            }while (cibleChoisi < 0 || cibleChoisi > 2 );
            break;
        case 2:
            do
            {
                cout << "Choisissez votre cible!" << endl ;
                for (int i=0 ; i<3 ; i++)
                {
                    cout << i << " = "<< listePersonnage[i]->getNom() << endl;
                }
                cin >> cibleChoisi ;
                switch (cibleChoisi)
                {
                case 0:
                    listePersonnage[2]->bouleDeFeu(*listePersonnage[0]);
                    break;
                case 1:
                    listePersonnage[2]->bouleDeFeu(*listePersonnage[1]);
                    break;
                case 2:
                    listePersonnage[2]->bouleDeFeu(*listePersonnage[2]);
                    break;
                }
            }while (cibleChoisi < 0 || cibleChoisi > 2 );

            break;
        case 3:
            do
            {
                cout << "Choisissez l'equipement à utiliser" << endl ;
                cout << "1 = Potion de vie" << endl ;
                cout << "2 = Potion de mana" << endl ;
                cin >> equipementChoisi ;
                switch (equipementChoisi)
                {
                case 1:
                    listePersonnage[2]->boirePotionDeVie(20);
                    break;
                case 2:
                    listePersonnage[2]->boirePotionDeMana(20);
                    break;
                default:
                    cout << " Mauvais choix!" << endl ;
                    break;
                }
            }while (equipementChoisi < 1 || equipementChoisi > 2 );



            break;
        default:

            cout << "Mauvais choix. Recommencez" << endl ;
            break;
        }
    }while (actionChoisi < 0 || actionChoisi > 3);

    for (int i=0 ; i<3 ; i++)
    {
        listePersonnage[i]->afficherEtat();
    }

    }while (listePersonnage[0]->estVivant()) ;

            cout << "Vous vous êtes suicidé! Bien joué!  " ;
    return 0;
}
