#include "Traitements.h"
/**
 * @brief Traitements
 * Constructeur de la classe Traitements.
 */
Traitements::Traitements(Communication* com, Sauvegarde* save, MaFenetre* fenetre) : QThread(){
    communication = com;
    sauvegarde = save;
    maFenetre = fenetre;
}
/**
 * @brief Traitements::initialisation
 * @return bool
 * \fn Séquence d'initialisation qui se lance a l'allumage. Cela test si une mémoire existe deja ou si il faut distribuer des adresse.
 */
bool Traitements::initialisation(){
    /**
     * Attente que l'arduino soit bien allumé.
     */
    msleep(100);
    /**
     * Iniialisation des variable et tableaux.
     */
    adresses.clear();
    uint8_t init = 0;
    messageInit.clear();
    messageRecu.clear();
    /**
     * test si une sauvegarde existe.
     * Si c'est le cas on met fin a l'initialisation.
     * Sinon on commence celle-ci.
     */
    if(sauvegarde->estTIlVide()){
        /**
         * Ecriture du message d'initialisation et envoie de celui-ci dans le port série.
         */
        messageInit.append(debutMessageInitialisation);
        messageInit.append(init);
        messageInit = ajoutTailleChecksum(messageInit);
        communication->ecriture(messageInit);
        msleep(20);
        messageRecu = communication->readSerial();
        /**
         * Boucle qui définit tant que l'on recois un retour au message d'initialisation on continue.
         */
        while(messageRecu.size() > 2){
            /**
             * initialisation du tableaux infosSave et vérification de la taille et du checksum du message recu.
             */
            infosSave.clear();
            messageRecu = verifieChecksumlTaille(messageRecu);
            qDebug() << messageRecu;
            /**
             * Lecture du message recu definissant le groupe d'appartenance de l'arduino qui repond.
             * Vérification qu'il reste bien des adresses disponible dans ce groupe.
             * On donne en fonction de ce groupe une adresse à l'arduino.
             * Si la reponse de l'arduino ne correspond pas a un groupe on a un message d'erreur.
             */
            if((uchar)messageRecu[0] == 1){
                if(premiereAdresseTete == 150){
                    qDebug() << "plus d'adresse Tete disponible.";
                    return false;
                }
                adresses.append(premiereAdresseTete);
                infosSave.append((uint8_t)1);
                infosSave.append(premiereAdresseTete);
                sauvegarde->ajoutAdresse(infosSave);
                premiereAdresseTete++;
            } else if ((uchar)messageRecu[0] == 2){
                if(premiereAdresseGrue == 200){
                    qDebug() << "Plus d'adresse Grue disponible";
                    return false;
                }
                adresses.append(premiereAdresseGrue);
                infosSave.append((uint8_t)2);
                infosSave.append(premiereAdresseGrue);
                sauvegarde->ajoutAdresse(infosSave);
                premiereAdresseGrue++;
            } else if ((uchar)messageRecu[0] == 3){
                if(premiereAdressePlatine == 250){
                    qDebug() << "Plus d'adresse platine disponible";
                    return false;
                }
                adresses.append(premiereAdressePlatine);
                infosSave.append((uint8_t)3);
                infosSave.append(premiereAdressePlatine);
                sauvegarde->ajoutAdresse(infosSave);
                premiereAdressePlatine++;
            } else {
                qDebug() << "Problème lors de l'initialisation, le retour d'un message ne contient pas de numero de groupe valide.";
                return false;
            }
            /**
             * Ecriture et envoie de l'adresse a l'arduino.
             */
            message.clear();
            message.append(debutMessageInitialisation);
            message.append(adresses[adresses.size()-1]);
            message = ajoutTailleChecksum(message);
            communication->ecriture(message);
            msleep(1000);
            /**
             * Envoie d'un nouveau message d'initialisation pour voir si un autre arduino est présent.
             * Puis attente d'une possible réponse.
             */
            communication->ecriture(messageInit);
            msleep(1000);
            messageRecu.clear();
            messageRecu = communication->readSerial();
        }
        sauvegarde->stream();
    } else{
        /**
         * Dans le cas ou nous avons une mémoire, on récupere les adresse en memoir.
         * Puis on demande si tout le monde est present sur la ligne
         */
        qDebug() << "récuperation de la sauvegarde";
        adresses = sauvegarde->recupereAdresses();
        message.clear();
        message.append(adresses);
        message.prepend(debutMessagePresence);
        message = ajoutTailleChecksum(message);
        communication->ecriture(message);
        if(traitementMessageRetour()){
            qDebug() << "Tout le monde est present";
        } else{
            return false;
        }
        return true;
    }
    return true;
}
/**
 * @brief Traitements::transmissions
 * @param QByteArray adresses, QByteArray messages, Communication* communication
 * \fn Récupere toutes les informations pour creer le message qui sera envoyé sur le port série et l'envoie par la fonction ecriture de l'objet communication.
 */
void Traitements::transmissions(QByteArray infos){
    /**
     * Dans le cas ou nous avons des adresses de destination
     */
    if(!adresses.isEmpty()){
        /**
         * Réinitialisation des variable et tableaux
         */
        message.clear();
        /**
         * Création d'un message avec pour objectif l'ordre suivant :
         * Taille(2) | Type(1) | Adresses(non definie) | Separateur(1) | Informations(non definie) | Checksum(2)
         * Ensuite ecriture de ce message sur la ligne de communication.
         */
        message.append(debutMessageInformation);
        message.append(adresses);
        message.append(separateur);
        message.append(infos);
        message = ajoutTailleChecksum(message);
        communication->ecriture(message);
    }
}
/**
 * @brief Traitements::traitementMessageRetour
 * @return bool
 * \fn Fonction qui vérifie que tout les messages retour sont recu et sont correct.
 * et qui traite les information recu pour les envoyer vers la fenetre.
 */
bool Traitements::traitementMessageRetour(){
    /**
     * Remise à zéro de variable et tableaux.
     */
    envoieAdresses.clear();
    envoieValeurs.clear();
    messageRecu.clear();
    dernierAdresseRecu = 0;
    /**
     * Pour autant de fois qu'il y a d'adresses.
     */
    for(int i(0); i< adresses.size(); i++){
        msleep(40);                                                             //!< Attente du temps définis entre chaque message.
        messageRecu = verifieChecksumlTaille(communication->readSerial());      //!< Réception du message d'erreur et vérification de la taille et du checksum.
        /**
         * Si le message d'erreur ne commence pas par l'indication messageErreur alors le message est éroné.
         * fin de la fonction et indication de la dernière adresse recu dans le terminal
         */
        if(messageRecu[0] != debutMessageErreur){
            qDebug() << "Dernière personne ayant repondu est l'adresse : " <<dernierAdresseRecu;
            return false;
        }
        /**
         * Si l'on arrive la c'est que l'on a bien un message d'erreur correct.
         * Donc enregistrement de l'adresse actuel comme derniere adresse recu.
         * On récupère la valeurs erreur du message.
         * Puis suppression du caractére erreur, de l'adresse, du separateur et de la valeur erreur.
         */
        dernierAdresseRecu = (uint8_t)messageRecu[1];
        erreur = (uint8_t)messageRecu[2];
        messageRecu.remove(0, 4);
        /**
         * Mise en forme du tableaux envoie de valeurs avec en premier l'adresse de l'actuel message recu.
         * Puis en second la valeur de l'angle recu.
         * Cela se répète et aura donc la forme[adresse, angle, adresse, angle...]
         */
        envoieAdresses.append(dernierAdresseRecu);
        envoieValeurs.append((float)(uint8ToUInt32(messageRecu)));
    }
    /**
     * Envoie des valeurs dans l'objet ma fenetre.
     */
    maFenetre->setValeurAngle(envoieAdresses, envoieValeurs);
    return true;
}
/**
 * @brief MaFenetre::calculChecksum
 * @param QByteArray a
 * @return uint16_t result
 * \fn Fonction d'addition de tout les composants de a dans un unsigned int qui est le checksum.
 */
uint16_t Traitements::calculChecksum(const QByteArray a){
    uint16_t result(0);
    for(int i(0); i<a.length() ; i++){
        result += (uchar)a[i];
    }
    return result;
}
/**
 * @brief MaFenetre::intToByte
 * @param const uint16_t i
 * @return QByteArray a(2)
 * \fn Une fonction qui permet de ranger un unsigned int de deux octets dans deux octets d'un QByteArray.
 */
QByteArray Traitements::uint16ToByte(const uint16_t i) {
    QByteArray a;
    a.resize(2);
    a[1] = (uchar)(i & 0x00ff);
    a[0] = (uchar)((i & 0xff00) >> 8);
    return a;
}
/**
 * @brief uint8ToUInt16
 * @param uint8_t a , uint8_t b
 * @return uint16_t x
 * \fn Fonction transformant deux uint8_t en un seul uint16_t
 */
uint16_t Traitements::uint8ToUInt16(const uint8_t a, const uint8_t b){
    uint16_t x;
    x = ((uint16_t)a << 8) | b;
    return x;
}
/**
 * @brief MaFenetre::bytesToUInt
 * @param const char a, const char b
 * @return uint16_t x
 * \fn prend deux valeur char et positionne un octet aprés l'autre dans un uint16_t
 */
uint16_t Traitements::bytesToUInt(const char a, const char b){
    uint16_t x;
    x = ((uint16_t)a << 8) | b;
    return x;
}
/**
 * @brief Traitements::verifieChecksulTaille
 * @param QBytesArray a
 * @return QByteArray a ou erreur
 * \fn Fonction qui vérifie le checksum ainsi que la taille d'un message et renvoie le message sans checksum ni ttaille si c'est bon ou alors 0 si il y a erreur
 */
QByteArray Traitements::verifieChecksumlTaille(QByteArray a){
    /**
     * Création d'un QByteArray contenant seulement la valeurs 0 qui sera envoyé dans le cas d'un mauvais message.
     */
    QByteArray erreur;
    uchar zero = 0;
    erreur.append(zero);
    /**
     * Si le message est plus petit que 4 octets, alors il ne peux contenir la taille et le checksum.
     * Donc erreur.
     */
    if(a.size() < 4 ){
        return erreur;
    /**
     * Si le message est assez grand, On récupère l'information checksum des deux octet de fin que l'on supprime du message.
     * On le remet ensuite sous forme d'un uint16.
     */
    } else{
        checksum = uint8ToUInt16(a.at(a.size()-2), a.at(a.size()-1));
        a.remove(a.size()-2, 2);
        /**
         * Vérification de la concordance du checksum en le recalculant et en comparant.
         * S'il est bon on continue, sinon on retourne une erreur.
         */
        if(checksum == calculChecksum(a)){
            /**
             * Si le checksum est bon on récupère l'information taille des deux octet de debut que l'on supprime du message.
             * On le remet ensuite sous forme de uint16.
             * Puis on le compare a la taille du message pour verifier sa veracité.
             * Si c'est bon on retourne le message correct, sans taille et sans checksum.
             * Sinon on declare une erreur.
             */
            taille = uint8ToUInt16(a.at(0), a.at(1));
            a.remove(0, 2);
            if(a.size() == taille){
                return a;
            } else{
                return erreur;
            }
        } else{
            return erreur;
        }
    }
}
/**
 * @brief Traitements::arret
 * \fn Fonction d'arret qui envoie un message pour que les arduino vide leur memoir morte.
 */
void Traitements::arret(){
    message.clear();
    message.append(debutMessageEteindre);
    taille = message.size();
    message.prepend(uint16ToByte(taille));
    message.append(uint16ToByte(calculChecksum(message)));
    communication->ecriture(message);
}
/**
 * @brief Traitements::ajoutTailleChecksum
 * @param QByteArray a
 * @return QByteArray a
 * \fn Fonction d'ajout de taille et checksum a un message.
 */
QByteArray Traitements::ajoutTailleChecksum(QByteArray a){
    uint16_t taille;
    taille = a.size();
    a.prepend(uint16ToByte(taille));
    a.append(uint16ToByte(calculChecksum(a)));
    return a;
}
/**
 * @brief Traitements::uint8ToUInt32
 * @param QByteArray a
 * @return Uint32_t b
 * \fn convertit quattre uint8 en un uint32
 */
uint32_t Traitements::uint8ToUInt32(QByteArray a){
    uint32_t b = a[3] | (a[2] << 8) | (a[1] << 16) | (a[0] << 24);
    return b;
}
