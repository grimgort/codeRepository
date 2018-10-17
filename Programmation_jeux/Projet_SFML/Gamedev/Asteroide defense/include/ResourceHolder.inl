/*
Suite de RessourceHolder, il d�finit l'ensemble des m�thodes de la classe
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
    unique_ptr<Resource> est un pointeur intelligent (g�re automatiquement le destructeur) vers la classe Resource.
    Ce pointeur est unique et ne peut donc pas �tre copier. Cela est utile pour stocker une classe non copiable.
    Le <Resource> est le nom de la classe point� par le pointeur. "resource" est le nom du pointeur.
    "(new Resource())" est l'action sur le pointeur. "new Resource()" permet de crer la classe cibl� par le pointeur.
    Cela n'est pas la seul action possible. par exemple, std::move(Ressource()) permettrai de d�placer le pointeur de la classe
    vers un autre idetifiant (nom) de pointeur. Cependant, le "new" est obligatoire pour g�n�rer la classe cibl�.

    */
    std::unique_ptr<Resource> resource(new Resource());
    /*resource is load and we verify that return true, else that return error.*/
    if (!resource->loadFromFile(filename, secondParam)) //utilise la m�thode loadFromFile de la classe point� par resource
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    /*resource is inserted with "id" key and
    return boolean value in "inserted". if inserted is false, error is returned with assert (debug mode only) */
    insertResource(id, std::move(resource));    //Permet de stocker le pointeur de la classe resource dans l'attribut m_resourceMap
}
/*
::get = Permet de r�cup�r� l'adresse du pointeur de la classe ressource gr�ce � son identifiant associ�
        par insertResource (id). Dans les fait, il r�cup�re le second argument du tableau m_resourceMap
        associ� � l'id donn� par l'utilisateur. Il permet donc pas la suite de manipuler la classe resource.
Resource& = retourne l'adresse(r�f�rence) du pointeur de la classe Resource.

*/
template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = m_resourceMap.find(id);    //cherche dans le tableau (map) l'id et stock la ligne dans "found".
    /*if "id" is not included in m_textureMap error is returned with assert*/
    assert(found != m_resourceMap.end());   //retourne une erreur si pas trouv�.

    /*Return texture associated with id.*/
    return *found->second;  //Renvoit l'adresse du pointeur stocker dans le second argument de found.
}
/*
D�clinaison de la m�thode get mais en emp�chant la modification de la classe point�.
*/
template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource,Identifier>::get(Identifier id) const
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

/*
Permet d'ins�rer le pointeur de la classe "resource" et sont identifiant dans un tableau "map"
afin de pouvoir r�cup�rer le pointeur de la classe en cherchant son identifiant.
Ainsi, l'utilisateur peut manipuler la classe "resource" � n'importe quel moment du code.
*/
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    // Insert and check success
    auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}



