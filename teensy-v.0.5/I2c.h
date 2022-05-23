#ifndef I2C_h
#define I2C_h
/**
 * Liste des Include et define necessaire aux fonctionnement de la class.
 */
#include "Arduino.h"                    //Include arduino pour l'integration dans le code arduino.
#include <Wire.h>                       //Include nécessaire a la communication I2C.
#define eeprom_address 0x50             //Adresse Eeprom.
#define capteurAimant_address 0x36      //Addresse du capteur aimant.
/**
 * Class I2C qui contient les fonction liée aux module qui communique en protocole I2C.
 */
class I2c { 
  public:
    I2c();
    /*Fonction eeprom*/
    uint8_t recupereAdresseEeprom();                                    //Fonction qui renvoie l'adresse en memoir de l'eeprom ou 0 si aucune adresse est présente.
    uint8_t lectureDataEeprom(unsigned int eaddress);                   //Lecture dans la mémoire de l'eeprom en fonction de l'adresse demandé.
    void ecritureDataEeprom(unsigned int eaddress, uint8_t data);       //Ecriture dans la mémoire de l'eeprom le premier parametre est l'adresse le second la data à écrire.
    /*Fonction capteur aimant (encodeur AS5600)*/
    bool aimantPresence();                                              //Fonction qui renvoie true si l'aimant est bien placé ou false si il l'est mal.
    float lireAngle();                                                  //Fonction qui demande l'envoie des informations position de l'aimant et les transforme en un angle de 360 degrées. 
  private:
    /*Variable eeprom*/
    uint8_t messageEeprom = 0;                                          //Variable déstiné a recevoir l'adresse contenu dans l'eeprom.
    uint8_t eepromVide = 0;                                             //Variable de verification pour la contenance de l'eeprom. Retourné dans la fonction recupèreAdresseEeprom() si l'eeprom est vide.
    /*Variable capteur aimant (encodeur AS5600)*/
    uint16_t aimantStatus = 0;                                          //Valeur du status : MD, ML, MH 
    uint8_t octetBas;                                                   //Angle 7:0 
    word octetHaut;                                                     //Angle 7:0 et 11:8 
    uint16_t angleBrut;                                                 //OctetHaut | octetBas 
    float degAngle;                                                     //Angle en degrees (360/4096 * [valeur entre 0-4095]) 
};

#endif
