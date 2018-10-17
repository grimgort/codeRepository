#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <Category.h>

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

/*
    Permet de crer l'action de la commande grâce à une fonction template.
*/
struct Command
{
    Command();
    std::function<void (SceneNode&, sf::Time) > action;
    unsigned int category;
};

/*
    Pas tous compris. a éclaircir. c'est une fonction lambda qui est associé à l'opérateur () de aircrafMmover.
    Dans l'idée, la fonction template est associé à un sceneNode(aircraft ou autre) et on met en argument
    un foncteur d'une nouvelle classe (ex:AircraftMouver) qui lui va passer en argument la classe a y associer(ex:Aircraft).
    Le foncteur doit renvoyer une fonction de la classe associé. Lors de l'apelle de la classe Command dans le sceneNode,
    le sceneNode va automatiquement être associé à la transformation de l'attribut modifié.

*/
template <typename GameObject, typename Function>
std::function<void (SceneNode&, sf::Time) > derivedAction (
    Function fn)
{
    //Renvoit cette fonction ([=])
    return [ = ] (SceneNode & node, sf::Time dt)
    {
        //Vérifie en mode debug que le pointeur du noeud existe
        assert (dynamic_cast<GameObject*> (&node) != nullptr);
        //Applique la fonction fn en copiant le pointeur de l'objet avec le même type(static_cast)
        fn (static_cast<GameObject&> (node), dt);
    };
}

#endif // COMMAND_H_INCLUDED
