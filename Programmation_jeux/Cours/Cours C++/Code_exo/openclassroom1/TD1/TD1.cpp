#include <iostream>
#include <ctime>        //permet d'initialiser les nombre aleatoire (time(0)
#include <cstdlib>      // permet d'utiliser les nombre aleatoire
#include <vector>
#include <fstream>

using namespace std ;
/*
mot()
1/ fonction qui retourne un mot du fichier dico.txt
2/ pas d'entrée
3/ retourne un mot du fichier dico.txt

*/
string mot()
{
    ifstream flux("dico.txt") ;
    string ligne("") ;
    int nbLigne(0) ;
    int nbAleatoire(0) ;
    string motMystere("") ;
    srand(time(0)) ;

//  On compte le nombre de ligne du fichier
    if (flux)
    {
         while(std::getline(flux, ligne)) {nbLigne++;} ; //On lit chaque ligne du fichier que l'on stoke dans "ligne"
    }
    else
    {
        cout << "Erreur de lecture" << endl ;
    }
    flux.close() ;  // on ferme le fichier pour revenir au début

    nbAleatoire = rand() % nbLigne ; // on récupére une ligne aleatoire
//  On lit la ligne aléatoire du fichier dico.txt
    flux.open("dico.txt") ;
    for (int i = 0 ; i <= nbAleatoire - 1 ; i++ )
    {
         getline(flux,motMystere) ;
    }

    return motMystere ;
}


/*
rentrerMot()
1/ Permet de séléctionner un mot a deviner
2/ Pas d'entrée
3/ retourne le mot rentré par l'utilisateur
*/
string rentrerMot()
{
    string motMystere("") ;

    cout << "Rentrez le mot mystere : " << endl ;
    cin >> motMystere ;
    return motMystere ;
}
/*
melangeMot()
1/ Permet de mélanger les lettres d'un mot
2/ MotMystere est le mot a mélanger
3/ Retourne le mot mélangé
*/
string melangeMot(string motMystere)
{

    srand(time(0)); //initialisatio des nombe aleatoire
    int nbLettre(0) ;
    nbLettre = motMystere.size() ;

    int nbAleatoire(0) ;


    int const cstMelange(10) ;
    string motMelange(motMystere) ;
    char charTampon('a') ;
    for (int i = 0 ; i < nbLettre * cstMelange ; ++i)
    {
        nbAleatoire = rand() % nbLettre ; //génération d'un nombre aleatoire (entre 0 et 4 : "% 5")
        charTampon = motMelange[0] ;    // Recupére la lettre qui va être effacé
        motMelange[0] = motMelange[nbAleatoire] ;   // remplace la premiére lettre par la nouvelle lettre
        motMelange[nbAleatoire] = charTampon ;  // remplace lancienne lettre par la lettre précédement effacé
    }

    return motMelange ;
}

/*
score()
1/ Permet de renvoyer le score du jeux TD1
2/ resultat est le tableau des scores. Par pas de 2, la premier nombre est 0 si mot pas trouvé ou 1 si mot trouvé. le deuxième nombre est le nombre d'essai pour trouver le mot.
3/ Retourne le score
*/
int score(vector<int>& resultat)
{
    double score (0.0);
    double maxScore (0.0) ;

    cout << "Reussit" << " || " << "Nombre d'essai" << endl ;
    for (int i = 0 ; i < resultat.size() ; i=i+2)
    {
        cout << resultat[i] << " || " << resultat[i+1] << endl; // liste le résumé des scores
        score = score + static_cast<double>(resultat[i]) / static_cast<double>(resultat[i+1]) ;
        maxScore = maxScore + resultat[i] ;
    }
    cout << "Votre score est de " << score << "/" << maxScore << endl ;

    return score ;
}


/*
deviner()
1/ Permet de gérer un mot a deviner
2/ MotMystere est le mot mélangé. resulktat est le tableau des scores
3/ Retourne l'ensemble es score des parties joués
*/
vector<int> deviner(string motMystere, vector<int>& resultat)
{
    string reponse("") ;
    string motMelange("") ;
    motMelange = melangeMot(motMystere) ;
    int const maxEssai(5);
    int numEssai (0) ;


    int dimUn (resultat.size()) ;
    do
    {
        numEssai++ ;
        cout << " Essai numero : " << numEssai << "/" << maxEssai << endl ;
        cout << "Quel est le mot ? " <<  motMelange << endl ;
        cin >> reponse ;

//  on test si le mot rentrée est juste ou faux
        if (reponse != motMystere )
        {
            cout << "FAUX ! " << endl ;
            if (numEssai == maxEssai)
            {
                cout << "vous avez depasse le nombre maximal d'essai! c'est perdu" << endl ;
                cout << "La reponse etait : " << motMystere << endl ;
            }
        }
        else
        {
            cout << "Felicitation !! Vous avez trouve. " << endl ;
            resultat[dimUn-2] = 1 ;
            resultat[dimUn-1] = numEssai  ;
        }
// on passe a l'essai suivant (max = maxEssai )
    }while (numEssai != maxEssai && reponse != motMystere ) ;
        for(int i=0;i<dimUn;i++)
    {
        cout << resultat[i] << endl;
    }


    return resultat ;
}


/*
jeuxTD1()
1/ Recupére le mot à deviner et lance une partie
2/ resultat est le tableau des scores
3/ Retourne Rien
*/
void jeuxTD1(vector<int> & resultat)
{
    string motMystere("") ;
    motMystere = mot() ;
    deviner(motMystere,  resultat) ;
}

/*
void multiJeux()
1/ Permet de faire plusieurs partie (fonction maitre du jeux TD1)
2/ Pas d'entree
3/ Retourne rien
*/
void multiJeux()
{
    char reponse('n') ;
    vector<int> resultat ;

    do
    {
        cout << "Voulez vous rejouer une partie ? (o/n)" << endl ;
        cin >> reponse ;
        if (reponse == 'o')
        {
            resultat.push_back(0) ;
            resultat.push_back(0) ;
            jeuxTD1( resultat) ;
        }
        else if (reponse == 'n')
        {
            cout << "Fin de partie" << endl ;
            score(resultat) ;
        }
        else
        {
            cout << "Repondez par 'o' ou 'n' !" << endl ;
        }
    }while(reponse != 'n') ;
}











