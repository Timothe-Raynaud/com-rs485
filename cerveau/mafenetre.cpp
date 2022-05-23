#include "mafenetre.h"
#include "Traitements.h"
/**
 * @brief MaFenetre
 * @param QWidget* a
 * Constructeur de la class MaFenetre.
 * Création de tout le rendu visuel.
 */
MaFenetre::MaFenetre(QWidget* a, QPushButton* quitter){
    /**
     * Création de tout les widgets liée à la communication de l'arduino.
     * Organisation du Layout horizontal qui comprend un bouton checkable ainsi que trois QSpinBox chacun nommé respectivement selon la couleur qu'il represente.
     */
    boutonArduino = new QPushButton;
    textRouge1 = new QLabel;
    numRouge1 = new QSpinBox;
    textBleu1 = new QLabel;
    numBleu1 = new QSpinBox;
    textVert1 = new QLabel;
    numVert1 = new QSpinBox;
    arduinoLayout = new QHBoxLayout;

    boutonArduino->setText("Arduino  - ");
    boutonArduino->setCheckable(true);
    textRouge1->setText("Rouge : ");
    textBleu1->setText("Bleu : ");
    textVert1->setText("Vert : ");
    numRouge1->setMaximum(255);
    numBleu1->setMaximum(255);
    numVert1->setMaximum(255);

    arduinoLayout->addWidget(boutonArduino);
    arduinoLayout->addWidget(textRouge1);
    arduinoLayout->addWidget(numRouge1);
    arduinoLayout->addWidget(textBleu1);
    arduinoLayout->addWidget(numBleu1);
    arduinoLayout->addWidget(textVert1);
    arduinoLayout->addWidget(numVert1);
    /**
     * Création de tout les widgets liée à la communication du teensy.
     * Organisation du Layout horizontal qui comprend un bouton checkable ainsi que trois QSpinBox chacun nommé respectivement selon la couleur qu'il represente.
     */
    boutonTeensy = new QPushButton;
    textRouge2 = new QLabel;
    numRouge2 = new QSpinBox;
    textBleu2 = new QLabel;
    numBleu2 = new QSpinBox;
    textVert2 = new QLabel;
    numVert2 = new QSpinBox;
    teensyLayout = new QHBoxLayout;

    boutonTeensy->setText("Teensy    - ");
    boutonTeensy->setCheckable(true);
    textRouge2->setText("Rouge : ");
    textBleu2->setText("Bleu : ");
    textVert2->setText("Vert : ");
    numRouge2->setMaximum(255);
    numBleu2->setMaximum(255);
    numVert2->setMaximum(255);

    teensyLayout->addWidget(boutonTeensy);
    teensyLayout->addWidget(textRouge2);
    teensyLayout->addWidget(numRouge2);
    teensyLayout->addWidget(textBleu2);
    teensyLayout->addWidget(numBleu2);
    teensyLayout->addWidget(textVert2);
    teensyLayout->addWidget(numVert2);
    /**
     * Création de tout les widgets liée à l'affichage des valeurs des angles.
     */
    affichageDesValeursAngles = new QLabel;
    valeursAngles = new QHBoxLayout;

    affichageDesValeursAngles->setText("Valeurs des angles teensy :");

    valeursAngles->addWidget(affichageDesValeursAngles);
    premierPassage = true;
    /**
     * Création de l'objet QLabel donnant état de la connexion physique de la communication.
     */
    afficheEtat = new QLabel;
    /**
     * Mise en page de chaque layout horizontal dans un layout principal vertical.
     */
    mainLayout = new QVBoxLayout(a);
    mainLayout->addLayout(arduinoLayout);
    mainLayout->addLayout(teensyLayout);
    mainLayout->addLayout(valeursAngles);
    mainLayout->addWidget(afficheEtat);
    mainLayout->addWidget(quitter);
}
/**
 * @brief MaFenetre::etatConnexion
 * @param bool a
 * \fn Affiche si conncté si a est true, non connecté si a est false.
 */
void MaFenetre::etatConnexion(bool a){
    if(a)
        afficheEtat->setText("Connecté");
    else
        afficheEtat->setText("Pas de connexion");
}
/**
 * @brief MaFenetre::recupereValeurs
 * \fn Fonction qui récupère les informations demandé (selon les boutons qui sont selectionné).
 * @return QByteArray message.
 */
QByteArray MaFenetre::recupereValeurs(){
    message.clear();
    uchar concatene[] = {(uchar)numRouge1->value(), (uchar)numVert1->value(), (uchar)numBleu1->value(), (uchar)numRouge2->value(), (uchar)numVert2->value(), (uchar)numBleu2->value()};
    for(int i(0); i<6 ; i++){
        message.append(concatene[i]);
    }
    return message;
}
/**
 * @brief MaFenetre::setValeurAngle
 * @param QByteArray adresses, QVector<float> valeurs
 * \fn Fonction qui utilise les adresses recu ainsi que les valeurs recu pour creer le nombre d'affichage necessaire à la presentation de ces données.
 */
void MaFenetre::setValeurAngle(QByteArray adresses, QVector<float> valeurs){
    /**
     * Pour autant d'adresses que l'on à recu.
     */
    for(int i(0); i < adresses.size(); i++){
        textAffichageValeurs.push_back(new QLabel());
        affichageValeurs.push_back(new QLabel());
        /**
         * On créer un QLabel ainsi q'un QLcdNumber et on y attribut les valeurs correspondantes.
         */
        textAffichageValeurs[i]->setText(QString::number(adresses[i]));
        affichageValeurs[i]->setText(QString::number(valeurs[i]));
        /**
         * Ajout des widget creer précédement au layout correspondant.
         * Le but n'est de le faire qu'une fois alors verifie si c'est le premier passage.
         * Puis declare premier passage false.
         */
        if(premierPassage){
            valeursAngles->addWidget(textAffichageValeurs[i]);
            valeursAngles->addWidget(affichageValeurs[i]);
        }
    }
    premierPassage = false;
}
