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

SceneNode::SceneNode(Category::Type category)
    : m_children() //constructeur sans enfant par d�fault
    , m_parent(nullptr) //constructeur sans parent par d�fault
    , m_nodeCategory(category)
{
}

/*
attachChild : Permet de d�finir les noeuds enfants et parents
Ptr child = pointeur d'un sceneNode
*/
void SceneNode::attachChild(Ptr child)
{
    child->m_parent = this; //d�finit l'attribut m_parent de l'enfant � ce sceneNode
    m_children.push_back(std::move(child)); //d�place le pointeur du noeud enfant dans le tableau m_children de ce noeud.
}
/*
detacheChild : Permet de d�tacher un noeud enfant du noeud parent appelant cette fonction
*/
SceneNode::Ptr SceneNode::detacheChild(const SceneNode& node)
{
    /*
    find_if : cherche dans le tableau m_children, [&] signifient que l'on r�cup�re la r�f�rence des variables
                ,(Ptr& p) d�finit la variable p comme la r�f�rence d'un Ptr(pointeur); cette variable est l'it�rateur de la recherche
                ,{ return p.get() == &node; } renvoie vrai(bool�en) si le p obtenue correspond � la r�f�rence du pointeur node
    */
    auto found = std::find_if(m_children.begin(),m_children.end(), [&] (Ptr& p)
    {
        return p.get() == &node;
    });
    assert (found != m_children.end()); //v�rifie si le poitneur a �t� trouv�

    Ptr result = std::move(*found); //cr�er le pointeur Ptr result comme �tant le pointeur trouv� par found
    result->m_parent = nullptr; // d�truit le pointeur du parent de l'enfant trouv�
    m_children.erase(found); // Enl�ve le poitneur de l'enfant dans le tableau parent
    return result; //pourquoi renvoyer result??
}

/*
Utilise les fonctions updateCurrent du parent et des noeud enfant.
Cette fonction est surcharg� dans la class fille Entity.
*/
void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    FOREACH(Ptr& child, m_children)
    child->update(dt,  commands);
}

/*
permet de tracer le noeud et les noeuds enfants sur la fen�tre(target)
*/
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform(); //permet d'ajouter les transformation du noeud parent au transformation du noeud enfant via la surcharge de l'op�rateur *=

    drawCurrent(target, states); //trace ce noeud(neoud parent)
    drawChildren(target, states); // trace les noeud enfants.
}

/*
la m�thode drawCurrent est surcharg� sur l'ensemble des classe fille.
Par exemple, pour les Aircrafts, la texture est trac� dans cette classe qui h�rite de SceneNode.
C'est donc pour cela que drawCurrent est vide ici.
*/
void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
}

/*
trace les neoud enfant avec les transformation/d�placement du neoeud parent
*/
void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    // it�re sur les pointeur des noeud enfant et utilise la fonction draw de ces noeud
    FOREACH(const Ptr& child, m_children)
    child->draw(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}

/*
Renvoit le vecteur indiquant la position absolut de l'objet.
*/
sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

/*
Permet d'avoir la position absolu des noeud fille (par rapport � la fen�tre).
Pour cela, on ajoute les attribut de transform des noeud filles aux noeud parent.
*/
sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity; //Pr�ciser Identity n'est pas obligatoire, mais montre que l'on r�cup�re les attribut de transform.

    for (const SceneNode* node = this; node !=nullptr; node = node->m_parent) //it�re sur l'ensemble des noeud parent jusqu'� ce que m_parent renvoit un pointeur null(donc pas de parent)
        transform = node->getTransform() * transform ; //l'op�rateur * est surcharg� dans la class transform, ce qui permet de retourner les atribut  des deux transform (parent et fille)

    return transform;
}



/*
V�rifie que la commande envoy� doit �tre appliqu� au pr�sent noeud(bonne cat�gorie) et applique l'action si c'est le cas.
*/
void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory()) //V�rifie que la commande envoy� est de la m�me categorie que celle du noeud.
        command.action(*this, dt); //applique la commande

    //Applique onCommend sur tous les noeuds enfant.
    FOREACH(Ptr& child, m_children)
    child->onCommand(command, dt);
}

/*
Renvoit � la cat�gorie du noeud.
Par d�fault, si aucune classe fille ne surcharge cette fonction, le noeud renvoit a la cat�gore sc�ne qui correspond
� l'entier 0.
*/
unsigned int SceneNode::getCategory() const
{
    return m_nodeCategory;
}

void SceneNode::checkScenePosition(SceneNode& sceneGraph, const std::vector<sf::FloatRect>& virtualRectCollision)
{
    checkNodePosition(sceneGraph, virtualRectCollision);

    FOREACH(Ptr& child, sceneGraph.m_children)
    checkScenePosition(*child, virtualRectCollision);
}

void SceneNode::checkNodePosition(SceneNode& node, const std::vector<sf::FloatRect>& virtualRectCollision)
{
  int op = 0;
  for (auto it = virtualRectCollision.cbegin(); it != virtualRectCollision.cend(); ++it)
  {
    op = op + 1;
    if (node.getBoundingRect().intersects(*it))
        {
            m_positionCollision = op;
            break;
        }
    else
        m_positionCollision = -9999;
  }

    FOREACH(Ptr& child, node.m_children)
    checkNodePosition(*child, virtualRectCollision);
}

int SceneNode::getPositionCollision() const
{
    return m_positionCollision;
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
    checkNodeCollision(sceneGraph, collisionPairs);

    FOREACH(Ptr& child, sceneGraph.m_children)
    checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if ((matchesCategories(*this, node, Category::PlayerAircraft, Category::EnemyAircraft))
            || matchesCategories(*this, node, Category::PlayerAircraft, Category::Pickup)
            || matchesCategories(*this, node, Category::PlayerAircraft, Category::EnemyProjectile)
            || matchesCategories(*this, node, Category::EnemyAircraft, Category::AlliedProjectile)
            || m_positionCollision == node.getPositionCollision()
            & !(m_positionCollision = -9999))
    {
        if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
            collisionPairs.insert(std::minmax(this, &node));
    }

    FOREACH(Ptr& child, m_children)
    child->checkNodeCollision(node, collisionPairs);
}

///*
//Assigne un noeud � une case du world pour faire les collisions;
//*/
//void SceneNode::attachNodeToCase(SceneNode& node, std::multimap<sf::Int32, SceneNode*>& tableauPosition, const sf::FloatRect& worldRect)
//{
//    sf::Int32 nbCutY = 10; //Nombre de d�coupe en Y du monde
//    sf::Int32 nbCutX = 3; //Nombre de d�coupe en X du monde
//    std::vector<float> leftRect; //conteneur des position gauche des rectangle d�coup� du world
//    std::vector<float> topRect; //conteneur des position top des rectangle d�coup� du world
//    std::vector<sf::FloatRect> FinalRect;
//    float initialWidth = ceil(worldRect.width / nbCutX); //Width des rectangles initial
//    float initialHeight = ceil(worldRect.height / nbCutY); //Height des rectangles initial
//
//
//    leftRect.push_back(0.f);
//    for (int i = 0; i < nbCutX; i++)
//    {
//        leftRect.push_back(ceil(leftRect[i] +  worldRect.left +  worldRect.width / nbCutX));
//    }
//
//    topRect.push_back(0.f);
//    for (int y = 0; y < nbCutY; y++)
//    {
//        topRect.push_back(ceil(topRect[y] +  worldRect.top + worldRect.height / nbCutY));
//    }
//
//    sf::Int32 key(0);
//    for (int i = 0; i < nbCutX; i++)
//    {
//        for (int y = 0; y < nbCutY; y++)
//        {
//            key = key + 1;
//            sf::FloatRect RectTemp(leftRect[i], topRect[y], initialWidth, initialHeight);
//            FinalRect.push_back(RectTemp);
//            if (node.getBoundingRect().intersects(RectTemp))
//            {
//                tableauPosition.insert (std::pair<sf::Int32, SceneNode*>(key, &node));
//                return;
//            }
//        }
//    }
//}




void SceneNode::removeWrecks()
{
    auto wreckfielBegin = std::remove_if(m_children.begin(), m_children.end(),  std::mem_fn(&SceneNode::isMarkedForRemoval));
    m_children.erase(wreckfielBegin, m_children.end());

    std::for_each(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::removeWrecks));
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

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

//Permet de retourner vrai si les deux noeuds correspondent � la cat�gorie 1 et 2.
bool SceneNode::matchesCategories(SceneNode& node1, SceneNode& node2, Category::Type type1, Category::Type type2)
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

