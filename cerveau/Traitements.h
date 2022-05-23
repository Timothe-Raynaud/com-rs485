#ifndef TRAITEMENTS_H
#define TRAITEMENTS_H
#include "communication.h"
#include "mafenetre.h"
#include "sauvegarde.h"
#include <QThread>

class Traitements : public QThread{
public:
    /**
     * @brief Traitements
     * Constructeur de la classe Traitements
     */
    Traitements(Communication* com, Sauvegarde* save, MaFenetre* fenetre);
    /**
     * @brief Traitements::initialisation
     * @return bool
     * \fn Séquence d'initialisation qui se lance a l'allumage. Cela test si une mémoire existe deja ou si il faut distribuer des adresse.
     */
    bool initialisation();
    /**
     * @brief Traitements::recupereInformationAEnvoyer
     * @param QByteArray adresses, QByteArray messages, Communication* communication
     * \fn Récupere toutes les informations pour creer le message qui sera envoyé sur le port série et l'envoie par la fonction ecriture de l'objet communication.
     */
    void transmissions(QByteArray messages);
    /**
     * @brief Traitements::traitementMessageRetour
     * @return bool
     * \fn Fonction qui vérifie que tout les messages retour sont recu et sont correct.
     * et qui traite les information recu pour les envoyer vers la fenetre.
     */
    bool traitementMessageRetour();
    /**
     * @brief MaFenetre::calculChecksum
     * @param a (QByteArray)
     * @return result (Unsigned int de 2 octets)
     * \fn fonction D'addition de tout les composants de a dans un unsigned int qui est le checksum.
     */
    uint16_t calculChecksum(const QByteArray a);
    /**
     * @brief MaFenetre::intToByte
     * @param i (unsigned int de deux octets)
     * @return a (QByteArray de taille 2)
     * \fn Une fonction qui permet de ranger un unsigned int de deux octets dans deux octets d'un QByteArray.
     */
    QByteArray uint16ToByte(const uint16_t i);
    /**
     * @brief uint8ToUInt16
     * @param uint8_t a , uint8_t b
     * @return uint16_t x
     * \fn Fonction transformant deux uint8_t en un seul uint16_t
     */
    uint16_t uint8ToUInt16(const uint8_t a, const uint8_t b);
    /**
     * @brief MaFenetre::bytesToUInt
     * @param const char a, const char b
     * @return uint16_t x
     * \fn prend deux valeur char et positionne un octet aprés l'autre dans un uint16_t
     */
    uint16_t bytesToUInt(const char a, const char b);
    /**
     * @brief Traitements::verifieChecksulTaille
     * @param QBytesArray a
     * @return QByteArray a ou erreur
     * \fn Fonction qui vérifie le checksum ainsi que la taille d'un message et renvoie le message sans checksum ni ttaille si c'est bon ou alors 0 si il y a erreur
     */
    QByteArray verifieChecksumlTaille(QByteArray a);
    /**
     * @brief Traitements::arret
     * \fn Fonction d'arret qui envoie un message pour que les arduino vide leur memoir morte.
     */
    void arret();
    /**
     * @brief Traitements::ajoutTailleChecksum
     * @param QByteArray a
     * @return QByteArray a
     * \fn Fonction d'ajout de taille et checksum a un message.
     */
    QByteArray ajoutTailleChecksum(QByteArray a);
    /**
     * @brief Traitements::uint8ToUInt32
     * @param QByteArray a
     * @return Uint32_t b
     * \fn convertit quattre uint8 en un uint32
     */
    uint32_t uint8ToUInt32(QByteArray a);

private:

    Sauvegarde *sauvegarde;                         //!< Création case mémoire d'objet sauvegarde
    Communication* communication;                   //!< Création case mémoire d'objet communication
    MaFenetre* maFenetre;                           //!< Création case mémoire d'objet MaFenetre
    char separateur = 36;                           //!< Définie le caractère de separation adresse/informations dans le message.
    char debutMessageErreur = 38;                   //!< Caractére inséré avant les adresse pour reconnaitre les messages d'erreur.
    char debutMessageInitialisation = 37;           //!< Caractère specifiant le role d'initialisation du message
    char debutMessageInformation = 35;              //!< Caractère specifiant le role d'echange d'information du message
    char debutMessageEteindre = 39;                 //!< Caractère specifiant l'arret des machines dans le message.
    char debutMessagePresence = 40;                 //!< Caractère Demandant une reponse de présence.
    QByteArray messageInit;                         //!< Tableaux pour le message d'initialisation
    QByteArray infosSave;                           //!< Tableaux comprenant adresse et groupe a destination du fichier de sauvegarde.
    QByteArray message;                             //!< Contiens le message qui sera envoyé dans le port serie
    QByteArray messageRecu;                         //!< Contiens le dernier message recu.
    QByteArray adresses;                            //!< Toutes les adresses concerné par la séquence.
    QByteArray envoieAdresses;                      //!< Contient les adresses a envoyé vers la fenetre.
    QVector<float> envoieValeurs;                   //!< Contient les valeurs des angles correspondantes.
    uint16_t taille;                                //!< Contient la taille du message qui est envoyé
    uint16_t checksum;                              //!< Contient le checksum du message traité.
    uint8_t erreur;                                 //!< Contient l'information "erreur" du message d'erreur.
    uint8_t dernierAdresseRecu;                     //!< Dernière adresse recu pour definir qui a un problème en cas d'erreur.
    uint8_t premiereAdresseTete = 50;               //!< Définie la première adresse disponible pour le groupe Tete (allant au maximum a 99)
    uint8_t premiereAdresseGrue = 100;              //!< Définie la première adresse disponible pour le groupe Grue (allant au maximum a 149)
    uint8_t premiereAdressePlatine = 150;           //!< Définie la première adresse disponible pour le groupe Platine (allant au maximum a 199)
};

#endif // TRAITEMENTS_H
