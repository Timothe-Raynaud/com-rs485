#include "Communication.h"
#include "Arduino.h"
/**
 * Constructeur de la class Communication.
 */
Communication::Communication(){
}
/**
 * Fonction qui calcul le checksum, verifie sa veracité et fait de même pour la taille ensuite.
 * Elle return un booleen selon la justesse ou non des deux paamètre.
 */
bool Communication::verificationTailleChecksum(QList<uint8_t>* messageRecu){
  if(messageRecu->size() > 4){                                                                                    //On vérifie que le message est assez grand.
    checksum = uint8ToUInt16(messageRecu->at(messageRecu->size()-2), messageRecu->at(messageRecu->size()-1));     //Recuperation des valeurs checksum du message et mise sous la bonne forme(uint16_t).
    messageRecu->pop_back();                                                                                      //Suppression d'une valeur checksum.
    messageRecu->pop_back();                                                                                      //Suppression de la seconde valeur checksum afin de comparer
    if(checksum == calculChecksum(messageRecu)){                                                                  //Dans le cas ou le checksum est correct on continue avec la taille.
      taille = uint8ToUInt16(messageRecu->at(0), messageRecu->at(1));                                             //Récupèration des valeurs taille du message et mise sous la bonne forme(uint16_t).
      messageRecu->pop_front();                                                                                   //Suppression d'une valeur taille.
      messageRecu->pop_front();                                                                                   //Suppression de la seconde valeur taille afin de comparer.
      if(messageRecu->size() == taille){                                                                          //Dans le cas ou la taille est bonne :
        return true;                                                                                                //On retourne true.
      } else{                                                                                                     //Si la taille est mauvaise:
        Serial.println("Erreur taille !");                                                                          //On signal une erreur taille sur le moniteur.
        return false;                                                                                               //Onretourne false.
      }
    } else{                                                                                                       //Si le checksum est mauvais:
      Serial.println("Erreur checksum !");                                                                          //On signal une erreur checksum sur le moniteur.
      return false;                                                                                                 //On retourne false.
    }
  } else{                                                                                                         //Si le message est trop petit.
    Serial.println("Erreur verificationTailleChecksum, message trop petit !");                                      //On signal une erreur de taille message sur le moniteur.
    return false;                                                                                                    //On retourne false.
  }
}
/**
 * Fonction qui calcul le checksum en additionnant chaque partie du message.
 * Return un uint16_t avec le résultat.
 */
uint16_t Communication::calculChecksum(QList<uint8_t>* a){
    uint16_t result = 0;                                                                                        //Création d'une variable uint16_t nommé result.
    for(unsigned int i = 0; i < a->size() ; i++){                                                                        //Ouverture d'une boucle for qui se répete autant de fois qu'il y a de case dans le tableau.
        result += (uint16_t)a->at(i);                                                                           //Ajout par addition aux precedent resultat de la nouvels case visé.
    }
    return result;                                                                                              //Retourne le resultat.
}
/**
 * Conversion 2 uint8_t en un uint16
 */
uint16_t Communication::uint8ToUInt16(const uint8_t a, const uint8_t b){
    uint16_t x;                                                                                                 //Creation d'un uint16_t qui contiendra le resultat.
    x = ((uint16_t)a << 8) | b;                                                                                 //Met b et a dans une meme variable avec la structure de bit a|b.
    return x;
}
/**
 * Conversion float vers 4 uint8
 */
void Communication::floatToUInt8(const float b, QList<uint8_t>* a) {
  uint32_t c = (uint32_t)b;
  a->push_back((c & 0xff000000) >> 24);
  a->push_back((c & 0x00ff0000) >> 16);
  a->push_back((c & 0x0000ff00) >> 8);
  a->push_back(c & 0x000000ff);
}
