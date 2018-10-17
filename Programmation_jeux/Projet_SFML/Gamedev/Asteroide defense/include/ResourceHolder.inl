/*
Suite de RessourceHolder, il définit l'ensemble des méthodes de la classe
*/

/************************************************
METHODE
************************************************/


template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    std::unique_ptr<Resource> resource(new Resource());
    /*resource is load and we verify that return true, else that return error.*/
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    /*resource is inserted with "id" key and
    return boolean value in "inserted". if inserted is false, error is returned with assert (debug mode only) */
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename,const Parameter& secondParam)
{
    /*
    unique_ptr<Resource> est un pointeur intelligent (gére automatiquement le destructeur) vers la classe Resource.
    Ce pointeur est unique et ne peut donc pas être copier. Cela est utile pour stocker une classe non copiable.
    Le <Resource> est le nom de la classe pointé par le pointeur. "resource" est le nom du pointeur.
    "(new Resource())" est l'action sur le pointeur. "new Resource()" permet de crer la classe ciblé par le pointeur.
    Cela n'est pas la seul action possible. par exemple, std::move(Ressource()) permettrai de déplacer le pointeur de la classe
    vers un autre idetifiant (nom) de pointeur. Cependant, le "new" est obligatoire pour générer la classe ciblé.

    */
    std::unique_ptr<Resource> resource(new Resource());
    /*resource is load and we verify that return true, else that return error.*/
    if (!resource->loadFromFile(filename, secondParam)) //utilise la méthode loadFromFile de la classe pointé par resource
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    /*resource is inserted with "id" key and
    return boolean value in "inserted". if inserted is false, error is returned with assert (debug mode only) */
    insertResource(id, std::move(resource));    //Permet de stocker le pointeur de la classe resource dans l'attribut m_resourceMap
}
/*
::get = Permet de récupéré l'adresse du pointeur de la classe ressource grâce à son identifiant associé
        par insertResource (id). Dans les fait, il récupére le second argument du tableau m_resourceMap
        associé à l'id donné par l'utilisateur. Il permet donc pas la suite de manipuler la classe resource.
Resource& = retourne l'adresse(référence) du pointeur de la classe Resource.

*/
template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = m_resourceMap.find(id);    //cherche dans le tableau (map) l'id et stock la ligne dans "found".
    /*if "id" is not included in m_textureMap error is returned with assert*/
    assert(found != m_resourceMap.end());   //retourne une erreur si pas trouvé.

    /*Return texture associated with id.*/
    return *found->second;  //Renvoit l'adresse du pointeur stocker dans le second argument de found.
}
/*
Déclinaison de la méthode get mais en empéchant la modification de la classe pointé.
*/
template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource,Identifier>::get(Identifier id) const
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

/*
Permet d'insérer le pointeur de la classe "resource" et sont identifiant dans un tableau "map"
afin de pouvoir récupérer le pointeur de la classe en cherchant son identifiant.
Ainsi, l'utilisateur peut manipuler la classe "resource" à n'importe quel moment du code.
*/
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    // Insert and check success
    auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}



