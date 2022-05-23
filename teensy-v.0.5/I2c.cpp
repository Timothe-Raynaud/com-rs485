#include "I2c.h"
#include "Arduino.h"
/**
 * Constructeur de la classe I2c.
 */
I2c::I2c(){
}
//****************************************************************************Fonction Liè à l'Eeprom****************************************************************************************/
/**
 * Fonction qui renvoie l'adresse en memoir de l'eeprom ou 0 si aucune adresse est présente
 */
uint8_t I2c::recupereAdresseEeprom(){
  if((messageEeprom = lectureDataEeprom(0)) != eepromVide){           //Dans le cas ou la case memoir 0(contenant l'adresse) n'est pas vide:
    return messageEeprom;                                             //On retourne la valeur de messageEeprom qui contient l'adresse.
  } else{                                                             //Sinon on retourne la valeur de eepromVide(0)    
    return eepromVide;
  }
}
/**
 * Lecture dans la mémoire de l'eeprom en fonction de l'adresse demandé.
 */
uint8_t I2c::lectureDataEeprom(unsigned int eaddress){
  uint8_t result;                                                     //Création d'une variable destiné a recuperer le resultat de la fonction.
  Wire.beginTransmission(eeprom_address);                             //Ouvre la transmission I2C vers l'adresse eeprom_address.
  /*Donne la position du pointeur vers la case memoir visé*/
  Wire.write((int)(eaddress >> 8));
  Wire.write((int)(eaddress & 0xFF));//
  Wire.endTransmission();                                             //Fin de transmission d'information.
  Wire.requestFrom(eeprom_address,1);                                 //Demande de recuperation l'octet visé.
  result = Wire.read();                                               //Lecture de l'information et attribution a la variable result.
  return result;
}
/**
 * Ecriture dans la mémoire de l'eeprom le premier parametre est l'adresse le second la data a écrire.
 */
void I2c::ecritureDataEeprom(unsigned int eaddress, uint8_t data){
  Wire.beginTransmission(eeprom_address);                             //Ouvre la transmission I2C vers l'adresse eeprom_address.
  /*Donne la position du pointeur vers la case memoir visé*/
  Wire.write((int)(eaddress >> 8));
  Wire.write((int)(eaddress & 0xFF));//
  Wire.write(data);                                                   //Envoie de l'information a écrire à la case memoir souhaité
  Wire.endTransmission();                                             //Ferme la transmission.
}
/**********************************************************************Fonction liée aux capteur d'aimant**************************************************************************************/
/** 
* Fonction qui renvoie true si l'aimant est bien placé ou false si il l'est mal.
*/ 
bool I2c::aimantPresence(){   
    aimantStatus = 0;                                                 //reset reading 
    Wire.beginTransmission(capteurAimant_address);                    //Connection aux capteur. 
    Wire.write(0x0B);                                                 //figure 21 de la doc - demande de status. 
    Wire.endTransmission();                                           //Fin de transmission. 
    Wire.requestFrom(capteurAimant_address, 1);                       //Requete d'un octet aux capteur. 
    while(Wire.available() == 0);                                     //Attente d'informations. 
    aimantStatus = Wire.read();                                       //Lecture des datas. 
    if((aimantStatus & 32) != 32){                                     //Si l'aimant n'est pas à la bonne distance - 32: MD = 1. 
      return false;                                                     //Renvoie false
    } else{                                                           //Sinon
      return true;                                                      //Renvoie true
    }
}   
/** 
* Fonction qui demande l'envoie des informations position de l'aimant et les transforme en un angle de 360 degrées. 
*/ 
float I2c::lireAngle(){  
/***********7:0 - bits**********/ 
  Wire.beginTransmission(capteurAimant_address);                      //Connection aux capteur. 
  Wire.write(0x0D);                                                   //Figure 21 de la doc - Demande angle (7:0). 
  Wire.endTransmission();                                             //Fin de transmission.  
  Wire.requestFrom(capteurAimant_address, 1);                         //Requete d'un octet aux capteur. 
  while(Wire.available() == 0);                                       //Attente d'informations. 
  octetBas = Wire.read();                                             //Lecture des datas. 
/**********11:8 - 4 bits *******/
  Wire.beginTransmission(capteurAimant_address); 
  Wire.write(0x0C);                                                   //figure 21 de la doc - Demande angle (11:8) 
  Wire.endTransmission();  
  Wire.requestFrom(capteurAimant_address, 1); 
  while(Wire.available() == 0);   
  octetHaut = Wire.read(); 
/*********Mise en forme*********/
  octetHaut = octetHaut << 8;                                         //Decalage des 4 bits de octetHaut pour former une information a 12 bits. 
  angleBrut = octetHaut | octetBas;                                   //Mise en forme des 4 bits du haut et des 8 bits du bas pour former un seul variable. 
  /** 
   * 12 bit = 4096 resultats different.  
   * 360° est divisé en 4096 parts egales. 
   * 360/4096 = 0.087890625 
   * On multipli donc le resultat par 0.087890625. 
   */ 
  degAngle = angleBrut * 0.087890625;  
  return degAngle; 
} 
