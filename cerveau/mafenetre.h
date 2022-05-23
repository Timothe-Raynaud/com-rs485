#ifndef MAFENETRE_H
#define MAFENETRE_H
#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>

class MaFenetre  {
public:
    /**
     * @brief MaFenetre
     * @param QWidget* a
     * Constructeur de la class MaFenetre.
     * Création de tout le rendu visuel.
     */
    MaFenetre(QWidget* a, QPushButton* quitter);
    /**
     * @brief MaFenetre::etatConnexion
     * @param bool a
     * \fn Affiche si conncté si a est true, non connecté si a est false.
     */
    void etatConnexion(bool a);
    /**
     * @brief MaFenetre::recupereValeurs
     * \fn Fonction qui récupère les informations demandé (selon les boutons qui sont selectionné).
     * @return QByteArray message.
     */
    QByteArray recupereValeurs();
    /**
     * @brief MaFenetre::setValeurAngle
     * @param QByteArray* valeurs
     * \fn Fonction qui recois les adresse ainsi que la valeurs de l'angle correspondant.
     */
    void setValeurAngle(QByteArray adresses, QVector<float> valeurs);
private:
    QPushButton* boutonArduino;     //!< Création de la case mémoire QPushButton nommé boutonArduino
    QLabel* textRouge1;             //!< Création de la case mémoire QLabel nommé textRouge1
    QSpinBox* numRouge1;            //!< Création de la case mémoire QSpinBox nommé numRouge1
    QLabel* textBleu1;              //!< Création de la case mémoire QLabel nommé textBleu1
    QSpinBox* numBleu1;             //!< Création de la case mémoire QSpinBox nommé numBleu1
    QLabel* textVert1;              //!< Création de la case mémoire QLabel nommé textVert1
    QSpinBox* numVert1;             //!< Création de la case mémoire QSpinBox nommé numVert1
    QHBoxLayout* arduinoLayout;     //!< Création de la case mémoire QHBoxLayout nommé arduinoLayout

    QPushButton* boutonTeensy;      //!< Création de la case mémoire QPushButton nommé boutonTeensy
    QLabel* textRouge2;             //!< Création de la case mémoire QLabel nommé textRouge2
    QSpinBox* numRouge2;            //!< Création de la case mémoire QSpinBox nommé numRouge2
    QLabel* textBleu2;              //!< Création de la case mémoire QLabel nommé textbleu2
    QSpinBox* numBleu2;             //!< Création de la case mémoire QSpinBox nommé numBleu2
    QLabel* textVert2;              //!< Création de la case mémoire QLabel nommé textVert2
    QSpinBox* numVert2;             //!< Création de la case mémoire QSpinBox nommé numVert2
    QHBoxLayout* teensyLayout;      //!< Création de la case mémoire QHBoxLayout nommé teensyLayout

    bool premierPassage;
    QList<QLabel*> textAffichageValeurs = {};
    QList<QLabel*> affichageValeurs = {};
    QLabel* affichageDesValeursAngles;
    QHBoxLayout* valeursAngles;

    QLabel* afficheEtat;            //!< Création de la case mémoire QLabel nommé afficheEtat

    QVBoxLayout* mainLayout;        //!< Création de la case mémoire QVBoxLayout nommé mainLayout

    QByteArray message;             //!< Création du QByteArray nommé message
    QByteArray adresses;            //!< Création du QByteArray nommé adresses

};

#endif // MAFENETRE_H
