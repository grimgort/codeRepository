#include <SceneNode.h>
#include <Command.h>
#include <Foreach.hpp>
#include <Utility.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

SceneNode::SceneNode (Category::Type category)
    : m_children() //constructeur sans enfant par défault
    , m_parent (
          nullptr) //constructeur sans parent par défault
    , m_nodeCategory (category)
      //    , m_positionCollision(-9999)
{
}

/*
    attachChild : Permet de définir les noeuds enfants et parents
    Ptr child = pointeur d'un sceneNode
*/
void SceneNode::attachChild (Ptr child)
{
    child->m_parent =
        this; //définit l'attribut m_parent de l'enfant à ce sceneNode
    m_children.push_back (std::move (
                              child)); //déplace le pointeur du noeud enfant dans le tableau m_children de ce noeud.
}
/*
    detacheChild : Permet de détacher un noeud enfant du noeud parent appelant cette fonction
*/
SceneNode::Ptr SceneNode::detacheChild (
    const SceneNode& node)
{
    /*
        find_if : cherche dans le tableau m_children
        , [&] signifient que l'on récupére la référence des variables
        ,(Ptr& p) définit la variable p comme la référence d'un
        Ptr(pointeur); cette variable est l'itérateur de la recherche
        ,{ return p.get() == &node; } renvoie vrai(booléen)
        si le p obtenue correspond à la référence du pointeur node
    */
    auto found = std::find_if (m_children.begin(),
                               m_children.end(), [&] (Ptr & p)
    {
        return p.get() == &node;
    });
    assert (found !=
            m_children.end()); //vérifie si le poitneur a été trouvé
    Ptr result = std::move (
                     *found); //créer le pointeur Ptr result comme étant le pointeur trouvé par found
    result->m_parent =
        nullptr; // détruit le pointeur du parent de l'enfant trouvé
    m_children.erase (
        found); // Enléve le poitneur de l'enfant dans le tableau parent
    return result; //renvoit le pointeur pour l'utiliser autre part
}

/*
    Utilise les fonctions updateCurrent du parent et des noeud enfant.
    Cette fonction est surchargé dans la class fille Entity.
*/
void SceneNode::update (sf::Time dt,
                        CommandQueue& commands)
{
    updateCurrent (dt, commands);
    updateChildren (dt, commands);
}

void SceneNode::updateCurrent (sf::Time dt,
                               CommandQueue& commands)
{
}

void SceneNode::updateChildren (sf::Time dt,
                                CommandQueue& commands)
{
    FOREACH (Ptr & child, m_children)
    child->update (dt,  commands);
}

/*
    permet de tracer le noeud et les noeuds enfants sur la fenêtre(target)
*/
void SceneNode::draw (sf::RenderTarget& target
                      , sf::RenderStates states) const
{
    /*
        permet d'ajouter les transformation du noeud parent au
        transformation du noeud enfant via la surcharge de l'opérateur *=
    */
    states.transform *= getTransform();
    /* trace ce noeud(neoud parent) */
    drawCurrent (target, states);
    /* trace les noeud enfants. */
    drawChildren (target, states);
}

/*
    la méthode drawCurrent est surchargé sur l'ensemble des classe fille.
    Par exemple, pour les Aircrafts, la texture est tracé dans cette
    classe qui hérite de SceneNode.
    C'est donc pour cela que drawCurrent est vide ici.
*/
void SceneNode::drawCurrent (sf::RenderTarget&
                             , sf::RenderStates) const
{
}

/*
    trace les neoud enfant avec les transformation/déplacement du neoeud parent
*/
void SceneNode::drawChildren (sf::RenderTarget &target
                              , sf::RenderStates states) const
{
    // itére sur les pointeur des noeud enfant et utilise la fonction draw de ces noeud
    FOREACH (const Ptr & child, m_children)
    child->draw (target, states);
}

void SceneNode::drawBoundingRect (sf::RenderTarget &target
                                  , sf::RenderStates) const
{
    sf::FloatRect rect = getBoundingRect();
    sf::RectangleShape shape;

    shape.setPosition (sf::Vector2f (rect.left, rect.top));
    shape.setSize (sf::Vector2f (rect.width, rect.height));
    shape.setFillColor (sf::Color::Transparent);
    shape.setOutlineColor (sf::Color::Green);
    shape.setOutlineThickness (1.f);

    target.draw (shape);
}

/*
    Renvoit le vecteur indiquant la position absolut de l'objet.
*/
sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

/*
    Permet d'avoir la position absolu des noeud fille (par rapport à la fenêtre).
    Pour cela, on ajoute les attribut de transform des noeud filles aux noeud parent.
*/
sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform =
        sf::Transform::Identity; //Préciser Identity n'est pas obligatoire, mais montre que l'on récupére les attribut de transform.

    for (const SceneNode* node = this;
            node != nullptr;
            node = node->m_parent) //itére sur l'ensemble des noeud parent jusqu'à ce que m_parent renvoit un pointeur null(donc pas de parent)
    { transform = node->getTransform() * transform ; } //l'opérateur * est surchargé dans la class transform, ce qui permet de retourner les atribut  des deux transform (parent et fille)

    return transform;
}

//sf::Vector2f SceneNode::getWorldBounding() const
//{
//    sf::Vector2f worldBounding;
//
//    for (const SceneNode* node = this;
//            node != nullptr;
//            node = node->m_parent) //itére sur l'ensemble des noeud parent jusqu'à ce que m_parent renvoit un pointeur null(donc pas de parent)
//    {
//        if(node->getCategory()==Category::Base)
//        {
//            worldBounding = node->getWorldPosition() ;
//        }
//    }
//
//    return worldBounding;
//}

/*
    Vérifie que la commande envoyé doit être appliqué au présent noeud(bonne catégorie) et applique l'action si c'est le cas.
*/
void SceneNode::onCommand (const Command& command,
                           sf::Time dt)
{
    if (command.category &
            getCategory()) //Vérifie que la commande envoyé est de la même categorie que celle du noeud.
    { command.action (*this, dt); } //applique la commande

    //Applique onCommend sur tous les noeuds enfant.
    FOREACH (Ptr & child, m_children)
    child->onCommand (command, dt);
}

/*
    Renvoit à la catégorie du noeud.
    Par défault, si aucune classe fille ne surcharge cette fonction, le noeud renvoit a la catégore scéne qui correspond
    à l'entier 0.
*/
unsigned int SceneNode::getCategory() const
{
    return m_nodeCategory;
}

void SceneNode::checkNodePosition (
    const std::vector<sf::FloatRect>
    &virtualRectCollision
    , std::multimap<int, SceneNode*>
    &collisionListeToTest
    , sf::Int32 nbCutX
    , sf::Int32 nbCutY)
{
    FOREACH (Ptr & child, m_children)
    child->checkNodePosition (virtualRectCollision
                              , collisionListeToTest
                              , nbCutX
                              , nbCutY);
}

int SceneNode::getPositionCollision() const
{
    return -9999;
}

void SceneNode::checkSceneCollision (
    std::multimap<int, SceneNode*>&
    collisionListeToTest
    , std::set<Pair>& collisionPairs
    , sf::Int32 nbCutX
    , sf::Int32 nbCutY)
{
    // Test sur le nombre de découpe de la grille de collision (nombre de op)
    for (int i = 1; i <= nbCutX * nbCutY; ++i)
    {
        std::pair <std::multimap<int, SceneNode*>::iterator
        , std::multimap<int, SceneNode*>::iterator> ret;
        ret = collisionListeToTest.equal_range (i);

        for (std::multimap<int, SceneNode*>::iterator it =
                    ret.first; it != ret.second; ++it)
        {
            for (std::multimap<int, SceneNode*>::iterator it2
                    = it; it2 != ret.second; ++it2)
            {
                //                std::cout << "it  " << it->second << "  " <<  it->first << std::endl;
                //                std::cout << "it2  " << it2->second << "  " <<  it2->first << std::endl;
                if ( (matchesCategories (*it->second,
                                         *it2->second, Category::PlayerAircraft,
                                         Category::EnemyAircraft))
                        || matchesCategories (*it->second, *it2->second,
                                              Category::PlayerAircraft, Category::Pickup)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::PlayerAircraft,
                                              Category::EnemyProjectile)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::EnemyAircraft,
                                              Category::AlliedProjectile)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::Base,
                                              Category::EnemyProjectile)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::EnemyAircraft,
                                              Category::Base)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::AlliedProjectile,
                                              Category::Base)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::Asteroide,
                                              Category::Base)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::Asteroide,
                                              Category::EnemyAircraft)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::Asteroide,
                                              Category::PlayerAircraft)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::Asteroide,
                                              Category::AlliedProjectile)
                        || matchesCategories (*it->second, *it2->second,
                                              Category::Asteroide,
                                              Category::EnemyProjectile)
                   )
                {
                    if (it->second != it2->second
                            && collision (*it->second, *it2->second)
                            && !it->second->isDestroyed()
                            && !it2->second->isDestroyed())
                    {
                        //                                                std::cout << "it  " << it->second << "  " <<
                        //                                                          it->first << std::endl;
                        //                                                std::cout << "it2  " << it2->second << "  " <<
                        //                                                          it2->first << std::endl;
                        collisionPairs.insert (std::minmax (it->second,
                                                            it2->second));
                    }
                }
            }
        }
    }
}

void SceneNode::removeWrecks()
{
    auto wreckfielBegin = std::remove_if (
                              m_children.begin(), m_children.end(),
                              std::mem_fn (&SceneNode::isMarkedForRemoval));
    m_children.erase (wreckfielBegin,
                      m_children.end());
    std::for_each (m_children.begin(),
                   m_children.end(),
                   std::mem_fn (&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
    return false;
}

bool collision (const SceneNode& lhs,
                const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects (
               rhs.getBoundingRect());
}

float distance (const SceneNode& lhs,
                const SceneNode& rhs)
{
    return length (lhs.getWorldPosition() -
                   rhs.getWorldPosition());
}

//Permet de retourner vrai si les deux noeuds correspondent à la catégorie 1 et 2.
bool SceneNode::matchesCategories (
    SceneNode& node1, SceneNode& node2,
    Category::Type type1, Category::Type type2)
{
    unsigned int category1 = node1.getCategory();
    unsigned int category2 = node2.getCategory();

    if (type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

