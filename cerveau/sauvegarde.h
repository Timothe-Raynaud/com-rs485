#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include <QtXml>
#include <QFile>
#include <QDomDocument>

class Sauvegarde{
public:
    /**
     * @brief Sauvegarde
     * Constructeur de l'objet Sauvegarde qui ouvre la communication avec le fichier externe save.xml
     */
    Sauvegarde();
    /**
     * @brief ajoutAdresse
     * @param QByteArray a
     * \fn ajoute sans le fichier xml une nouvelle adresse ainsi que le groupe d'appartenance.
     */
    void ajoutAdresse(QByteArray a);
    /**
     * @brief recupereAdresses
     * @return QByteArray adresses
     * \fn Fonction qui renvoie dans un tableaux les Adresses présente dans le fichier save.
     */
    QByteArray recupereAdresses();
    /**
     * @brief estTIlVide
     * @return  bool
     * \fn Vérifie si la fichier save.xml est vide ou non.
     */
    bool estTIlVide();
    /**
     * @brief arret
     * \fn fonction qui vide le fichier save.xml
     */
    void arret();
    /**
     * @brief stream
     * \fn Envoie des informations aux document save.xml une fois récolté.
     */
    void stream();
private:
    QFile* file;                //!< Création du pointeur objet QFile file
    QDomDocument* doc;          //!< Création du pointeur objet QDomDocument doc
    QDomElement root;           //!< Création du QDomElement root
    QByteArray adresses;        //!< QByteArray dédié aux adresses contenu dans le fichier save.
};

#endif // SAUVEGARDE_H
