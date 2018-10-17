#include<iostream>
#include <vector> //Ne pas oublier !
#include <ctime>        //permet d'initialiser les nombre aleatoire (time(0)
#include <cstdlib>      // permet d'utiliser les nombre aleatoire
using namespace std;

template<typename T, typename S>
S moyenne(T tableau[], int taille)
{
  S somme(0);                  //La somme des éléments du tableau
  for(int i(0); i<taille; ++i)
    somme += tableau[i];

  return somme/taille;
}

template<typename T, typename S, typename W>
W plusPetit(T a, S b)
{
    if (a<b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

template<typename T, typename S>
S moyenne2(vector<T>tableau, int taille)
{
  S somme(0);                  //La somme des éléments du tableau
  for(int i(0); i<taille; ++i)
    somme += tableau[i];

  return somme/taille;
}

template<typename W>
W aleatoire()
{
    W nbAleatoire ;
    nbAleatoire = rand();
    return nbAleatoire;

}

int main()
{
    srand(time(0)) ;
  int tab[5];
  //Remplissage du tableau

  cout << "Moyenne : " << moyenne<int,double>(tab,5) << endl;
  cout << "plus petit : " << plusPetit<double,double,double>(5,2.2) << endl;
  vector<int> tab2(5,3) ;
  cout << "moyenne vector : " << moyenne2<int,double>(tab2,5) << endl;
  cout << "bnaleatoire : " << aleatoire<double>() << endl;

  return 0;
}
