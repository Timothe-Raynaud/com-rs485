#include "cerveau.h"
/**
 * @brief Cerveau::Cerveau
 * Constructeur de la class Cerveau. Il instancie chaque objet et creer les cadences necessaire au fonctionnement du programme.
 */
Cerveau::Cerveau() : QWidget(){
    /**
     * Création de l'objet QPushButton quitter et affiliation du text "Fin de communication".
     */
    quitter = new QPushButton;
    quitter->setText("Fin de communication");
    /**
     * Création des objets utile a la class.
     */
    sauvegarde = new Sauvegarde;
    maFenetre = new MaFenetre(this, quitter);
    communication = new Communication();
    traitements = new Traitements(communication, sauvegarde, maFenetre);
    /**
     * Affichage des ports disponible dans le terminal.
     * Puis verification de l'etat de la connexion
     */
    communication->chercherPort();
    ioEtatConnexion();
    /**
     * Lancement de la sequence d'initialisation et attente de la fin de celle-ci.
     */
    while(!traitements->initialisation());
    /**
     * Creation d'un lien entre le signal clique sur le bouton quitter et l'action de quitter l'application.
     */
    QObject::connect(quitter, SIGNAL(clicked()), this, SLOT(quitterApplication()));
    /**
     * Création d'un QTimer qui verifie l'etat de la connexion toute les seconde.
     */
    connexion = new QTimer(this);
    QObject::connect(connexion, SIGNAL(timeout()), SLOT(ioEtatConnexion()));
    connexion->start(1000);
    /**
     * Création d'un QTimer qui envoie les informations de la fenetre vers la class traitement
     * Puis traite les informations retour.
     * L'action se répétes toutes les 100ms.
     */
    informations = new QTimer(this);
    QObject::connect(informations, SIGNAL(timeout()), SLOT(communicationInformations()));
    informations->start(100);
}
/**
 * @brief Traitements::ioEtatConnexion
 * \fn Demande a l'objet communication l'etat de la connexion et l'envoie dans l'objet maFenetre.
 */
void Cerveau::ioEtatConnexion(){
    maFenetre->etatConnexion(communication->etatConnexion());
}
/**
 * @brief Cerveau::informationsFenetreVersTraitement
 * \fn Si des informations sont a recuperer dans maFenetre, les envoie vers la class de traitement.
    Puis je traite les informations retour.
 */
void Cerveau::communicationInformations(){
    traitements->transmissions(maFenetre->recupereValeurs());
    traitements->traitementMessageRetour();
}
/**
 * @brief quitterApplication
 * \fn Permet de quitter l'aplication aprés avoir vidé la sauvegarde
 */
void Cerveau::quitterApplication(){
    traitements->arret();
    sauvegarde->arret();
    qApp->exit();
}
