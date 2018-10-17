#include<iostream>
#include<fstream>
#include<cmath>

using namespace std ;

void partie1()
{

    cout << "Salut beau goss" << endl << "Comment vas tu?" << endl ;
    cout << "Afficher guillemet : \"" << endl << "Afficher slash : //" << endl ; /* Count permet d'afficher une instruction
    sur la console. "<<" doit borner le texte. "endl" fait un retour � la ligne */

    /* D�claration de variable */
    /* Convention d'�criture des variables : la premi�re lettre est une minuscule, un nouveau mot commence par une majuscule,
    l'ensemble du nom est coll� (ex: un_Mot = faux, unMot = bon ) */
    int unEntier (10) ;
    unsigned int unEntierNegatif (-10) ;
    char unChar ('c') ;    // RQ : encadrer un char avec des '. Ne d�finit qu'une lettre.
    bool unBool (true) ;
    double unDouble (-0.555) ;
    string unString ("c'est un string") ;   // RQ : encadrer un string avec des "
    int plusieursEntier1 (1), plusieursEntier2 (2), plusieursEntier3 (3) ;  // plusieurs variable s�par� par une ","
    double unDoubleNonDefinit ;

    /* Affichage ds variables dans la console */
    cout << " L'entier = " << unEntier << " Le char = " << unChar << " Un booleen = " << unBool << endl <<
         " Un double = " << unDouble << " Un string = " << unString ;

    /* Definission d'une r�f�rence. */
    /* Permet de donner 2 etiquette � une m�me variable */
    int& referenceDeUnEntier(unEntier) ; // 10 peut �tre appel� par "unEntier" ou "referenceDeUnEntier"

    /* Entr�e d'une variable dans la console */
    string entreDeBeau ("Repondez!") ;  // Obligation d'initialisser la variable pour la r�atribuer apr�s
    cout << "Miroire, Miroire ! Qui est le plus beau ?" << endl  ;
    cin >> entreDeBeau ;    // "cin" demande une entr�e
    cin.ignore() ;  // permet de lib�rer les entrees ; donc r�utiliser la variable "entreDeBeau" comme entrees par la suite. A mettre toujours apr�s cin
    cout << "Le plus beau est " << entreDeBeau  ;
    cout << "Dite votre pr�nom et nom et non votre nom" << endl ;
    getline(cin, entreDeBeau) ;
    cout << "Le plus beau est " << entreDeBeau  ;

    /* Modification d'une variable */
    int a1(5), a2(6) ;
    cout << "a1 vaut : " << a1 << " et a2 vaut : " << a2 << endl;
    cout << "Affectation !" << endl;
    a1 = a2 ;
    cout << "a1 vaut : " << a1 << " et a2 vaut : " << a2 << endl;

    /* Op�ration */
    double resultat(0) ;
    a1 = 5 ;
    a2 = 2 ;
    resultat = a1 + a2 ;    //addition
    cout << "addition : " << resultat << endl ;
    resultat = a1 - a2 ;    //soustration
    cout << "soustraction : " << resultat << endl ;
    resultat = a1 / a2 ;    //division. Attention : une division de nombre entier renvoit la partie enti�re de la division. Ici 5/2 renvoit 2 au lieu de 2,5.
    cout << "division : " << resultat << endl ;
    resultat = a1 % a2 ;    //modulo = reste de la division. En C/C++ le modulo ne peut �tre fait qu'avec des entier et renvoi un entier (donc arrondis).
    cout << "modulo : " << resultat << endl ;

    /* D�claration d'une constante */
    double const cst(5.5) ; // le mot "const" sp�cifie que c'est une constante non modifiable.

    /* Exercice 1 */
    double op1(0.0), op2(0.0) ;
    cout << "Rentrer les deux nombres que vous voulez rentrer avec un espace entre" << endl ;
    cin >> op1 >> op2 ;
    double const resultat2(op1 + op2) ;
    cout << "Le r�sultat est : " << resultat2 ;

    cout << " donner deux nombres entier s�par� par un espace" << endl ;
    int intOp1(0),intOp2(0) ;
    cin >> intOp1 >> intOp2 ;
    int const intResultat(intOp1 / intOp2), modResultat(intOp1 % intOp2) ;
    cout << " Le quotient de : " << intOp1 << " / " << intOp2 << " = " << intResultat << endl ;
    cout << " Le modulo est : " << modResultat << endl ;

    /* Incr�mentation */
    int increment(0);
    cout << "Incr�mantation m�thode" << endl ;
    increment = increment + 1 ;
    cout << increment << endl ;
    increment = 0 ;
    increment++ ;
    cout << increment << endl ;
    increment = 0 ;
    ++increment ;
    cout << increment << endl ;

    /* D�cr�mentation */
    increment = 1 ;
    increment-- ;
    cout << increment << endl ;
    --increment ;
    cout << increment << endl ;

    /* Op�ration racourci */
    double nombre1 (1.1) ;
    cout << nombre1 << endl ;
    nombre1 += 5.2 ;
    cout << nombre1 << endl ;
    nombre1 /= 2.0 ;
    cout << nombre1 << endl ;
    nombre1 *= 2.0 ;
    cout << nombre1 << endl ;
    nombre1 -= 3.3 ;

    /* Fonction math�matique de <cmath> */
    cout << "Fonction math��atiqe" << endl ;
    cout << nombre1 << endl ;
    nombre1 = sqrt(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = sin(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = cos(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = exp(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = tan(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = fabs(nombre1) ;   //valeur absolu
    cout << nombre1 << endl ;
    nombre1 = log(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = log10(nombre1) ;
    cout << nombre1 << endl ;
    nombre1 = floor(nombre1) ;  //arrondi inf�rieur
    cout << nombre1 << endl ;
    nombre1= ceil(nombre1) ;    //arrondi sup�rieur
    cout << nombre1 << endl ;
    nombre1 = pow(5,2) ;    //Puissance
    cout << nombre1 << endl ;

    /* Boucle conditionel */
    intOp1 = 1 ;
    if (intOp1 == 1)
    {
        intOp1 = 2 ;
    }
    else if (intOp1 > 5)
    {
        intOp1 = 3 ;
    }
    else if (intOp1 < 5)
    {
        intOp1 = 4 ;
    }
    else if (intOp1 >= 5)
    {
        intOp1 = 3 ;
    }
    else if (intOp1 <= 5)
    {
        intOp1 = 6 ;
    }
    else if (intOp1 != 5)
    {
        intOp1 = 9 ;
    }
    else
    {
        intOp1 = 0 ;
    }


    switch (intOp1) //prend la valeur de la variable entre parenth�se est aplique la condition des case. si intOp1 =0 on ira dans case 0:
    {
    case 0: // teste seulement une �galit� avec des entiers
        intOp1 = 5 ;
    case 1:
        intOp1 = 3 ;
    default:
        intOp1 = 0 ;
    }

    bool adulte(true);
    if (adulte == true)
    {
        cout << "Vous etes un adulte !" << endl;
    }
    /* pareil que : */
    if (adulte) // la veur test� par d�fault sera "true"
    {
        cout << "Vous etes un adulte !" << endl;
    }

    int nbEnfants(0) ;
    if (adulte && nbEnfants >= 1)   // "&&" = et
    {
        adulte = false ;
    }
    else if (nbEnfants == 1 || nbEnfants == 2) // "||" = ou
    {
        adulte = true ;
    }
    else if (!adulte) // "!" = NON. traduction : si adulte == false
    {
        adulte = false ;
    }

    int nbEnfants1(-3) ; // Nombre n�gatif pour pouvoir entrer dans la boucle
    while (nbEnfants1 < 0)
    {
        cout << "Combien d'enfants avez-vous ?" << endl ;
        ++nbEnfants1 ;
    }
    cout << "Merci d'avoir indique un nombre d'enfants correct. Vous en avez " << nbEnfants << endl ;

    /* identique � la boucle while mais teste la condition apr�s avoir pass� une fois la boucle */

    do
    {
        cout << "Combien d'enfants avez-vous ?" << endl ;
        ++nbEnfants1 ;
    }while (nbEnfants1 < 0) ;

    int compteur(0);

    for (compteur = 0 ; compteur < 10 ; compteur++) // for (initialisation ; condition ; incrementation)
    {
        cout << compteur << endl;
    }

    for (compteur = 0 ; compteur < 10 ; compteur += 2) // for (initialis� � 0 ; tant que compteur est inf�rieur � 10 ; par pas de 2)
    {
        cout << compteur << endl;
    }

    for (int compteur(0) ; compteur < 10 ; compteur++) // variable d�clar� dans le for. Sa d�claration existe uniquement durant le "for". c'est la forme la plus utilis�
    {
        cout << compteur << endl;
    }
}



void ecriture1() // �crire dans un fichier
{
    ofstream monFlux("test.txt") ;  // ouvre le fichier

    if (monFlux)    //v�rifie l'existance du fichier
    {
        monFlux << "Je suis beau" << endl ; //�crie dans le fichier d�finit par son nom. ex :"monflux"
    }
    else
    {
        cout << "Fichier non trouve" ;  //renvoie une erreur si il y a un probl�me sur le fichier
    }


}

void ecriture2() // pareil que pr�c�dent mais avec un emplacement de fichier d�finit par un string
{
    string fichierTest("test.txt") ;
    ofstream monFlux(fichierTest.c_str()) ;

    if (monFlux)
    {
        monFlux << "Je suis beau" << endl ;
    }
    else
    {
        cout << "Fichier non trouve" << endl ;
    }

//
}

void ecriture3() // pareil que pr�c�dent mais avec une boucle et �criture en fin de fichier
{
    string fichierTest("test.txt") ;
    ofstream monFlux(fichierTest.c_str(), ios::app) ; //"ios::app" permet de rajouter des ligne en fin de fichier

    if (monFlux)
    {
        for (int i(0); i<5 ;++i)
        {
            monFlux << "Je suis beau" << endl ;
        }

    }
    else
    {
        cout << "Fichier non trouve" << endl ;
    }

//
}

void ecriture4() // pareil que pr�c�dent mais avec diff�rent type de variable �crite
{
    string fichierTest("test.txt") ;
    ofstream monFlux(fichierTest.c_str(), ios::app) ; //"ios::app" permet de rajouter des ligne en fin de fichier

    if (monFlux)
    {
        monFlux << "Je suis beau" << endl ;
        monFlux << 44.254 << endl ;
        monFlux << 50 << endl ;


    }
    else
    {
        cout << "Fichier non trouve" << endl ;
    }

//
}


void lecture1() // Lecture de fichier()
{
    string ligne("") ;
    ifstream monFlux("test.txt") ;

    if (monFlux)
    {
        while(getline(monFlux,ligne)) //lire la ligne du fichier monFlux et l'�crire dans la variable "ligne"
        {
            cout << ligne << endl ;
        }

    }
    else
    {
       cout <<  "Erreur de lecture" << endl ;
    }
/*
  RQ :  getline() = lit la ligne enti�re
        >> = lit mot par mot
        get() = lit par caract�re

        string mot;
        monFlux >> mot;          //On lit un mot depuis le fichier
        monFlux.ignore();        //On change de mode
        string ligne;
        getline(monFlux, ligne); //On lit une ligne compl�te
*/
}


void astuceFichier1()
{
    ofstream flux ; //sp�cifie que l'on va �crire dans un fichier encore inconnu
    flux.open("test.txt",ios::app) ; //ouvre le fichier "inconnu"

    flux.close() ; // ferme le fichier
}

void positionFichier()
{
    ofstream flux("test.txt",ios::app) ;

    if (flux)
    {
        int position (flux.tellp()) ; // r�cup�re la position du curseur
        cout << "position du curseur = " << position << endl ;
        flux.seekp(5,ios::cur) ; // on avance le curseur de 5 de sa position actuel
        position =flux.tellp() ;
        cout << "position du curseur = " << position << endl ;
    }
    else
    {
        cout << "erreur" << endl ;
    }
/*
RQ : ios::cur = position courante du curseur
        ios::beg = d�but du fichier
        ios::end = fin du fichier
        .teelp() = r�cup�re la position du curseur pour ofstream
        .seekp() = d�place le curseur pour ofstream
        .tellg() = pareil pour ifstream
        .seekg() = pareil  pour ifstream
*/
}

void tailleFichier() //donne la taille d'un fichier
{
    ifstream flux("test.txt",ios::app) ;
    if (flux)
    {
        flux.seekg(0,ios::end) ;
        int taille(flux.tellg());
        cout << "La taille du fichier est de " << taille << " octet" << endl ; // un caract�re = 1 octet
        double tailleKo(taille/1024.) ;
        cout << "ou " << tailleKo << "ko" << endl ;
    }
}























