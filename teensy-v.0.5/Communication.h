#ifndef COMMUNICATION_h
#define COMMUNICATION_h
/**
 * Liste des Include et define necessaire aux fonctionnement de la class.
 */
#include "Arduino.h"                    //Include arduino pour l'integration dans le code arduino.
#include <QList.h>                      //Permet de creer des tableaux a taille variable necessaire pour la communication.
/**
 * Class qui contient les fonctions liée à la communication en rs-485
 */
class Communication {
  public:
    Communication();
    /**
     * Fonction qui calcul le checksum, verifie sa veracité et fait de même pour la taille ensuite.
     * Elle return un booleen selon la justesse ou non des deux paamètre.
     */
    bool verificationTailleChecksum(QList<uint8_t>* messageRecu);
    /**
     * Fonction qui calcul le checksum en additionnant chaque partie du message.
     * Return un uint16_t avec le résultat.
     */
    uint16_t calculChecksum(QList<uint8_t>* a);
    /**
     * Conversion 2 uint8_t en un uint16
     */
    uint16_t uint8ToUInt16(const uint8_t a, const uint8_t b);
    /**
     * Conversion float vers 4 uint8
     */
    void floatToUInt8(const float b, QList<uint8_t>* a); 
  private:
    uint16_t checksum;                  //Creation variable checksum.
    uint16_t taille;                    //Création variable taille.
};
#endif
