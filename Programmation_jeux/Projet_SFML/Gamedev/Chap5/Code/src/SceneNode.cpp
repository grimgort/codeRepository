#include <SceneNode.h>
#include <Command.h>
#include <Foreach.hpp>

#include <algorithm>
#include <cassert>

SceneNode::SceneNode()
: m_children() //constructeur sans enfant par défault
, m_parent(nullptr) //constructeur sans parent par défault
{
}

/*
attachChild : Permet de définir les noeuds enfants et parents
Ptr child = pointeur d'un sceneNode
*/
void SceneNode::attachChild(Ptr child)
{
    child->m_parent = this; //définit l'attribut m_parent de l'enfant à ce sceneNode
    m_children.push_back(std::move(child)); //déplace le pointeur du noeud enfant dans le tableau m_children de ce noeud.
}
/*
detacheChild : Permet de détacher un noeud enfant du noeud parent appelant cette fonction
*/
SceneNode::Ptr SceneNode::detacheChild(const SceneNode& node)
{
    /*
    find_if : cherche dans le tableau m_children, [&] signifient que l'on récupére la référence des variables
                ,(Ptr& p) définit la variable p comme la référence d'un Ptr(pointeur); cette variable est l'itérateur de la recherche
                ,{ return p.get() == &node; } renvoie vrai(booléen) si le p obtenue correspond à la référence du pointeur node
    */
    auto found = std::find_if(m_children.begin(),m_children.end(), [&] (Ptr& p) { return p.get() == &node; });
    assert (found != m_children.end()); //vérifie si le poitneur a été trouvé
    Ptr result = std::move(*found); //créer le pointeur Ptr result comme étant le pointeur trouvé par found
    result->m_parent = nullptr; // détruit le pointeur du parent de l'enfant trouvé
    m_children.erase(found); // Enléve le poitneur de l'enfant dans le tableau parent
    return result; //pourquoi renvoyer result??
}

/*
permet de tracer le noeud et les noeuds enfants sur la fenêtre(target)
*/
void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform(); //permet d'ajouter les transformation du noeud parent au transformation du noeud enfant via la surcharge de l'opérateur *=
    drawCurrent(target,states); //trace ce noeud(neoud parent)
    drawChildren(target, states); // trace les noeud enfants.
}

/*
la méthode drawCurrent est surchargé sur l'ensemble des classe fille.
Par exemple, pour les Aircrafts, la texture est tracé dans cette classe qui hérite de SceneNode.
C'est donc pour cela que drawCurrent est vide ici.
*/
void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
}

/*
trace les neoud enfant avec les transformation/déplacement du neoeud parent
*/
void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    // itére sur les pointeur des noeud enfant et utilise la fonction draw de ces noeud
	FOREACH(const Ptr& child, m_children)
		child->draw(target, states);
}

/*
Utilise les fonctions updateCurrent du parent et des noeud enfant.
Cette fonction est surchargé dans la class fille Entity.
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
Permet d'avoir la position absolu des noeud fille (par rapport à la fenêtre).
Pour cela, on ajoute les attribut de transform des noeud filles aux noeud parent.
*/
sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity; //Préciser Identity n'est pas obligatoire, mais montre que l'on récupére les attribut de transform.
    for (const SceneNode* node = this; node !=nullptr; node = node->m_parent) //itére sur l'ensemble des noeud parent jusqu'à ce que m_parent renvoit un pointeur null(donc pas de parent)
    {
        transform = node->getTransform() * transform ; //l'opérateur * est surchargé dans la class transform, ce qui permet de retourner les atribut  des deux transform (parent et fille)
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
Vérifie que la commande envoyé doit être appliqué au présent noeud(bonne catégorie) et applique l'action si c'est le cas.
*/
void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory()) //Vérifie que la commande envoyé est de la même categorie que celle du noeud.
    {
        command.action(*this, dt); //applique la commande
    }

    //Applique onCommend sur tous les noeuds enfant.
	FOREACH(Ptr& child, m_children)
		child->onCommand(command, dt);
}

/*
Renvoit à la catégorie du noeud.
Par défault, si aucune classe fille ne surcharge cette fonction, le noeud renvoit a la catégore scéne qui correspond
à l'entier 0.
*/
unsigned int SceneNode::getCategory() const
{
    return Category::Scene;
}








