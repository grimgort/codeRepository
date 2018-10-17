#ifndef ARME_H_INCLUDED
#define ARME_H_INCLUDED

#include<string>

class Arme
{
public:
    Arme() ;
    Arme(std::string nom, int degat) ;
    void changer (std::string nom, int degat) ;
    void afficherEtat() const;
    int getDegat()const ; //accesseur de l'attribut m_degat (commence par get)
private:
    std::string m_nom ;
    int m_degat ;

};




#endif // ARME_H_INCLUDED
