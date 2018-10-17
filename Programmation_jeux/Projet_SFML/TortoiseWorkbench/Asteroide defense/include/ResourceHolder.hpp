#ifndef RESOURCEHOLDER_HPP_INCLUDED
#define RESOURCEHOLDER_HPP_INCLUDED

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
/*
    Permet de stocker les textures ou les son
    Can stock resource with "loadFromFile"(texture or sound buffer) but not music or shader.
*/
// Resource = Correspond aux classes d�finit dans ressourceIdentifier.h
// Identifier = d�finit l'identifiant de la ressource. Il se r�f�re donc � un conteneur (enum) d�finit dans un
//              namespace.
// filename = Chemin d'acc�e du fichier � charger (image, son, font)
template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    void load (Identifier id,
               const std::string& filename); //charge une texture depuis son identifiant(Identifier) et son image (filename)

    template<typename Parameter>
    void load (Identifier id, const std::string& filename,
               const Parameter& secondParam);

    Resource& get (Identifier id); //get resource

    const Resource& get (Identifier id)
    const; //get resource without modification

private:
    void insertResource (Identifier id,
                         std::unique_ptr<Resource> resource);

private:
    std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};


/* "*.inl" is use and included in and of file "*.hpp" because a template is used. "*.inl" is a extension of "*.hpp"*/
#include <ResourceHolder.inl>
#endif // RESOURCEHOLDER_HPP_INCLUDED
