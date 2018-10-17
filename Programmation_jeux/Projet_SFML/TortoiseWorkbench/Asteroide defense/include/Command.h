#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <Category.h>

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

/*
    Permet de crer l'action de la commande gr�ce � une fonction template.
*/
struct Command
{
    Command();
    std::function<void (SceneNode&, sf::Time) > action;
    unsigned int category;
};

/*
    Pas tous compris. a �claircir. c'est une fonction lambda qui est associ� � l'op�rateur () de aircrafMmover.
    Dans l'id�e, la fonction template est associ� � un sceneNode(aircraft ou autre) et on met en argument
    un foncteur d'une nouvelle classe (ex:AircraftMouver) qui lui va passer en argument la classe a y associer(ex:Aircraft).
    Le foncteur doit renvoyer une fonction de la classe associ�. Lors de l'apelle de la classe Command dans le sceneNode,
    le sceneNode va automatiquement �tre associ� � la transformation de l'attribut modifi�.

*/
template <typename GameObject, typename Function>
std::function<void (SceneNode&, sf::Time) > derivedAction (
    Function fn)
{
    //Renvoit cette fonction ([=])
    return [ = ] (SceneNode & node, sf::Time dt)
    {
        //V�rifie en mode debug que le pointeur du noeud existe
        assert (dynamic_cast<GameObject*> (&node) != nullptr);
        //Applique la fonction fn en copiant le pointeur de l'objet avec le m�me type(static_cast)
        fn (static_cast<GameObject&> (node), dt);
    };
}

#endif // COMMAND_H_INCLUDED
