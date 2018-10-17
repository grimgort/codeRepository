#include<iostream>
#include<fstream>
#include<cmath>

using namespace std ;

void partie1()
{

    cout << "Salut beau goss" << endl << "Comment vas tu?" << endl ;
    cout << "Afficher guillemet : \"" << endl << "Afficher slash : //" << endl ; /* Count permet d'afficher une instruction
    sur la console. "<<" doit borner le texte. "endl" fait un retour à la ligne */

    /* Déclaration de variable */
    /* Convention d'écriture des variables : la premiére lettre est une minuscule, un nouveau mot commence par une majuscule,
    l'ensemble du nom est collé (ex: un_Mot = faux, unMot = bon ) */
    int unEntier (10) ;
    unsigned int unEntierNegatif (-10) ;
    char unChar ('c') ;    // RQ : encadrer un char avec des '. Ne définit qu'une lettre.
    bool unBool (true) ;
    double unDouble (-0.555) ;
    string unString ("c'est un string") ;   // RQ : encadrer un string avec des "
    int plusieursEntier1 (1), plusieursEntier2 (2), plusieursEntier3 (3) ;  // plusieurs variable séparé par une ","
    double unDoubleNonDefinit ;

    /* Affichage ds variables dans la console */
    cout << " L'entier = " << unEntier << " Le char = " << unChar << " Un booleen = " << unBool << endl <<
         " Un double = " << unDouble << " Un string = " << unString ;

    /* Definission d'une référence. */
    /* Permet de donner 2 etiquette à une même variable */
    int& referenceDeUnEntier(unEntier) ; // 10 peut être appelé par "unEntier" ou "referenceDeUnEntier"

    /* Entrée d'une variable dans la console */
    string entreDeBeau ("Repondez!") ;  // Obligation d'initialisser la variable pour la réatribuer aprés
    cout << "Miroire, Miroire ! Qui est le plus beau ?" << endl  ;
    cin >> entreDeBeau ;    // "cin" demande une entrée
    cin.ignore() ;  // permet de libérer les entrees ; donc réutiliser la variable "entreDeBeau" comme entrees par la suite. A mettre toujours aprés cin
    cout << "Le plus beau est " << entreDeBeau  ;
    cout << "Dite votre prénom et nom et non votre nom" << endl ;
    getline(cin, entreDeBeau) ;
    cout << "Le plus beau est " << entreDeBeau  ;

    /* Modification d'une variable */
    int a1(5), a2(6) ;
    cout << "a1 vaut : " << a1 << " et a2 vaut : " << a2 << endl;
    cout << "Affectation !" << endl;
    a1 = a2 ;
    cout << "a1 vaut : " << a1 << " et a2 vaut : " << a2 << endl;

    /* Opération */
    double resultat(0) ;
    a1 = 5 ;
    a2 = 2 ;
    resultat = a1 + a2 ;    //addition
    cout << "addition : " << resultat << endl ;
    resultat = a1 - a2 ;    //soustration
    cout << "soustraction : " << resultat << endl ;
    resultat = a1 / a2 ;    //division. Attention : une division de nombre entier renvoit la partie entiére de la division. Ici 5/2 renvoit 2 au lieu de 2,5.
    cout << "division : " << resultat << endl ;
    resultat = a1 % a2 ;    //modulo = reste de la division. En C/C++ le modulo ne peut âtre fait qu'avec des entier et renvoi un entier (donc arrondis).
    cout << "modulo : " << resultat << endl ;

    /* Déclaration d'une constante */
    double const cst(5.5) ; // le mot "const" spécifie que c'est une constante non modifiable.

    /* Exercice 1 */
    double op1(0.0), op2(0.0) ;
    cout << "Rentrer les deux nombres que vous voulez rentrer avec un espace entre" << endl ;
    cin >> op1 >> op2 ;
    double const resultat2(op1 + op2) ;
    cout << "Le résultat est : " << resultat2 ;

    cout << " donner deux nombres entier séparé par un espace" << endl ;
    int intOp1(0),intOp2(0) ;
    cin >> intOp1 >> intOp2 ;
    int const intResultat(intOp1 / intOp2), modResultat(intOp1 % intOp2) ;
    cout << " Le quotient de : " << intOp1 << " / " << intOp2 << " = " << intResultat << endl ;
    cout << " Le modulo est : " << modResultat << endl ;

    /* Incrémentation */
    int increment(0);
    cout << "Incrémantation méthode" << endl ;
    increment = increment + 1 ;
    cout << increment << endl ;
    increment = 0 ;
    increment++ ;
    cout << increment << endl ;
    increment = 0 ;
    ++increment ;
    cout << increment << endl ;

    /* Décrémentation */
    increment = 1 ;
    increment-- ;
    cout << increment << endl ;
    --increment ;
    cout << increment << endl ;

    /* Opération racourci */
    double nombre1 (1.1) ;
    cout << nombre1 << endl ;
    nombre1 += 5.2 ;
    cout << nombre1 << endl ;
    nombre1 /= 2.0 ;
    cout << nombre1 << endl ;
    nombre1 *= 2.0 ;
    cout << nombre1 << endl ;
    nombre1 -= 3.3 ;

    /* Fonction mathématique de <cmath> */
    cout << "Fonction mathéùatiqe" << endl ;
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
    nombre1 = floor(nombre1) ;  //arrondi inférieur
    cout << nombre1 << endl ;
    nombre1= ceil(nombre1) ;    //arrondi supérieur
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


    switch (intOp1) //prend la valeur de la variable entre parenthèse est aplique la condition des case. si intOp1 =0 on ira dans case 0:
    {
    case 0: // teste seulement une égalité avec des entiers
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
    if (adulte) // la veur testé par défault sera "true"
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

    int nbEnfants1(-3) ; // Nombre négatif pour pouvoir entrer dans la boucle
    while (nbEnfants1 < 0)
    {
        cout << "Combien d'enfants avez-vous ?" << endl ;
        ++nbEnfants1 ;
    }
    cout << "Merci d'avoir indique un nombre d'enfants correct. Vous en avez " << nbEnfants << endl ;

    /* identique à la boucle while mais teste la condition aprés avoir passé une fois la boucle */

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

    for (compteur = 0 ; compteur < 10 ; compteur += 2) // for (initialisé à 0 ; tant que compteur est inférieur à 10 ; par pas de 2)
    {
        cout << compteur << endl;
    }

    for (int compteur(0) ; compteur < 10 ; compteur++) // variable déclaré dans le for. Sa déclaration existe uniquement durant le "for". c'est la forme la plus utilisé
    {
        cout << compteur << endl;
    }
}



void ecriture1() // écrire dans un fichier
{
    ofstream monFlux("test.txt") ;  // ouvre le fichier

    if (monFlux)    //vérifie l'existance du fichier
    {
        monFlux << "Je suis beau" << endl ; //écrie dans le fichier définit par son nom. ex :"monflux"
    }
    else
    {
        cout << "Fichier non trouve" ;  //renvoie une erreur si il y a un problème sur le fichier
    }


}

void ecriture2() // pareil que précédent mais avec un emplacement de fichier définit par un string
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

void ecriture3() // pareil que précédent mais avec une boucle et écriture en fin de fichier
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

void ecriture4() // pareil que précédent mais avec différent type de variable écrite
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
        while(getline(monFlux,ligne)) //lire la ligne du fichier monFlux et l'écrire dans la variable "ligne"
        {
            cout << ligne << endl ;
        }

    }
    else
    {
       cout <<  "Erreur de lecture" << endl ;
    }
/*
  RQ :  getline() = lit la ligne entiére
        >> = lit mot par mot
        get() = lit par caractére

        string mot;
        monFlux >> mot;          //On lit un mot depuis le fichier
        monFlux.ignore();        //On change de mode
        string ligne;
        getline(monFlux, ligne); //On lit une ligne complète
*/
}


void astuceFichier1()
{
    ofstream flux ; //spécifie que l'on va écrire dans un fichier encore inconnu
    flux.open("test.txt",ios::app) ; //ouvre le fichier "inconnu"

    flux.close() ; // ferme le fichier
}

void positionFichier()
{
    ofstream flux("test.txt",ios::app) ;

    if (flux)
    {
        int position (flux.tellp()) ; // récupére la position du curseur
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
        ios::beg = début du fichier
        ios::end = fin du fichier
        .teelp() = récupére la position du curseur pour ofstream
        .seekp() = déplace le curseur pour ofstream
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
        cout << "La taille du fichier est de " << taille << " octet" << endl ; // un caractére = 1 octet
        double tailleKo(taille/1024.) ;
        cout << "ou " << tailleKo << "ko" << endl ;
    }
}























