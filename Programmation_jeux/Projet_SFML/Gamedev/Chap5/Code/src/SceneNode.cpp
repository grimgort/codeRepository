#include <SceneNode.h>
#include <Command.h>
#include <Foreach.hpp>

#include <algorithm>
#include <cassert>

SceneNode::SceneNode()
: m_children() //constructeur sans enfant par d�fault
, m_parent(nullptr) //constructeur sans parent par d�fault
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
    auto found = std::find_if(m_children.begin(),m_children.end(), [&] (Ptr& p) { return p.get() == &node; });
    assert (found != m_children.end()); //v�rifie si le poitneur a �t� trouv�
    Ptr result = std::move(*found); //cr�er le pointeur Ptr result comme �tant le pointeur trouv� par found
    result->m_parent = nullptr; // d�truit le pointeur du parent de l'enfant trouv�
    m_children.erase(found); // Enl�ve le poitneur de l'enfant dans le tableau parent
    return result; //pourquoi renvoyer result??
}

/*
permet de tracer le noeud et les noeuds enfants sur la fen�tre(target)
*/
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform(); //permet d'ajouter les transformation du noeud parent au transformation du noeud enfant via la surcharge de l'op�rateur *=
    drawCurrent(target,states); //trace ce noeud(neoud parent)
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

/*
Utilise les fonctions updateCurrent du parent et des noeud enfant.
Cette fonction est surcharg� dans la class fille Entity.
*/
void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt)
{
}

void SceneNode::updateChildren(sf::Time dt)
{
	FOREACH(Ptr& child, m_children)
		child->update(dt);
}

/*
Permet d'avoir la position absolu des noeud fille (par rapport � la fen�tre).
Pour cela, on ajoute les attribut de transform des noeud filles aux noeud parent.
*/
sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity; //Pr�ciser Identity n'est pas obligatoire, mais montre que l'on r�cup�re les attribut de transform.
    for (const SceneNode* node = this; node !=nullptr; node = node->m_parent) //it�re sur l'ensemble des noeud parent jusqu'� ce que m_parent renvoit un pointeur null(donc pas de parent)
    {
        transform = node->getTransform() * transform ; //l'op�rateur * est surcharg� dans la class transform, ce qui permet de retourner les atribut  des deux transform (parent et fille)
    }
    return transform;
}

/*
Renvoit le vecteur indiquant la position absolut de l'objet.
*/
sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

/*
V�rifie que la commande envoy� doit �tre appliqu� au pr�sent noeud(bonne cat�gorie) et applique l'action si c'est le cas.
*/
void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory()) //V�rifie que la commande envoy� est de la m�me categorie que celle du noeud.
    {
        command.action(*this, dt); //applique la commande
    }

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
    return Category::Scene;
}








