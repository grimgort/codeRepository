#include <iostream>
#include <string>
#include <cstdio>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

// Foncteur qui effectue un chiffrement par decalage
class ChiffrementDecal
{

public:

    // Constructeur prenant le decalage voulu en argument
    ChiffrementDecal(int decalage) : m_decalage(decalage)
    {
    }

    char operator()(char lettre)   //La surcharge de l'op�rateur ()
    {
        int lettre_int(0);
        //On v�rifie que l'�quivalent entier ASCII de la lettre correspond � une lettre majuscule
        if ((int)lettre>=(int)'A' && (int)lettre<=(int)'Z')
        {
            m_decalage = m_decalage%26 ; //On retourne la valeur de m_decalage sur une range de 26
            lettre_int = (int)lettre + m_decalage ; //on d�quale la lettre
            if (lettre_int>(int)'Z')    //Si on d�passe Z on revien � A
            {
                lettre_int = lettre_int - (int)'Z' + (int)'A' - 1 ;
            }
            else if (lettre_int<(int)'A')  //Si on est inf�rieur � A on passe � Z
            {
                lettre_int = lettre_int - (int)'A' + (int)'Z' + 1 ;
            }
            lettre = (char)lettre_int;
            return lettre;
        }
        else //si on est pas une lettre majuscule on renvoi le caract�re d'origine
        {
            return lettre;
        }

    }


private:

    int m_decalage; // Le decalage a appliquer au texte

};


int main()
{

    // Le message a crypter
    string texte("BIENVENUE SUR LE MOOC C++ D'OCR !!");

    // Demande du decalage a l'utilisateur
    cout << "Quel decalage voulez-vous utiliser ? ";
    int decalage;
    cin >> decalage;

    // Creation du foncteur
    ChiffrementDecal foncteur(decalage);

    // Chaine de caracteres pour le message crypter
    string texte_crypte("");
    ostringstream flux;   //Un flux permettant d'�crire dans une cha�ne

    copy(texte.begin(),texte.end(),ostream_iterator<char>(flux, ", ")); // on �crit dans la chaine
    texte_crypte = flux.str(); //On r�cup�re la chaine

    transform(texte_crypte.begin(), texte_crypte.end(), texte_crypte.begin(), foncteur);

    cout << texte_crypte ;


    return 0;
}

