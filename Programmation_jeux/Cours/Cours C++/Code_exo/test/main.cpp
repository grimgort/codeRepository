#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

class Convertir
{
public:
    char operator()(char lettre)  //La surcharge de l'opérateur ()
    {
        int lettre_int(0);
        int m_decalage(0);

        if ((int)lettre>=(int)'A' && (int)lettre<=(int)'Z')
        {
            m_decalage = 5 ;
            lettre_int = (int)lettre + m_decalage ;
            if (lettre_int>(int)'Z')
            {
                lettre_int = lettre_int - (int)'Z' + (int)'A' - 1 ;
            }
            else if (lettre_int<(int)'A')
            {
                lettre_int = lettre_int - (int)'A' + (int)'Z' + 1 ;
            }
            lettre = (char)lettre_int;
            return lettre;
        }
        else
        {
            return lettre;
        }
    }
};

int main()
{
    string chaine("Salut les zeros !");
    transform(chaine.begin(), chaine.end(), chaine.begin(), Convertir());
    cout << chaine << endl;
    return 0;
}
