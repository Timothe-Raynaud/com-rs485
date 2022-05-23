#ifndef CERVEAU_H
#define CERVEAU_H
#include <QWidget>
#include <QPushButton>
#include "mafenetre.h"
#include "Traitements.h"
#include "communication.h"
#include <sauvegarde.h>

class Cerveau : public QWidget{
    Q_OBJECT
public:
    /**
     * @brief Cerveau::Cerveau
     * Constructeur de la class Cerveau. Il instancie chaque objet et creer les cadence necessaire au fonctionnement du programme.
     */
    Cerveau();
private:
    QPushButton *quitter;               //!< Création d'une case mémoir pour QPushButton

    MaFenetre* maFenetre;               //!< Création case mémoir pour maFenetre
    Traitements* traitements;           //!< Création case mémoir pour traitements
    Communication* communication;       //!< Création case mémoir pour communication
    Sauvegarde* sauvegarde;             //!< Création case mémoir pour sauvegarde

    QTimer* connexion;                  //!< Création case mémoir pour QTimer (dédié à l'etat de la connexion)
    QTimer* informations;               //!< Création case mémoir pour QTimer (dédié à l'echange d'informations)

private slots:
    /**
     * @brief Traitements::ioEtatConnexion
     * \fn Demande a l'objet communication l'etat de la connexion et l'envoie dans l'objet maFenetre.
     */
    void ioEtatConnexion();
    /**
     * @brief Cerveau::informationsFenetreVersTraitement
     * \fn Si des informations sont a recuperer dans maFenetre, les envoie vers la class de traitement.
        Puis je traite les informations retour.
     */
    void communicationInformations();
    /**
     * @brief quitterApplication
     * \fn Permet de quitter l'aplication aprés avoir vidé la sauvegarde
     */
    void quitterApplication();
};

#endif // CERVEAU_H
