#include "I2c.h"
#include "Communication.h"
/*****Constante*****/
const int pinTensionSuivant = 15;
/*****Variable d'informations de situation *****/
uint8_t monGroupe = 1;                                            //Valeur representant l'appartenance a un groupe : 1 = Tete ; 2 = Grue ; 3 = Platine.
uint8_t monAdresse = 0;                                           //Adresse attribué aux present Teensy
uint8_t adressePrecedente;
uint8_t erreur;
float aimantdeg;                                                  //Contient la valeur en degrés du capteur aimant.
/*****Création des pointeurs de class*****/
I2c* i2c;                                                         //Création d'un pointeur vers I2c nommé i2cCommunication.
Communication* communication;
/*****Variable message*****/
uint16_t taille;
uint16_t checksum;
QList<uint8_t>* messageRecu;
QList<uint8_t>* adressesRecu;
QList<uint8_t>* messageEnvoi;
QList<uint8_t>* infosMeConcernant;
/*****Constante de message *****/
const uint8_t debutMessageInformation = 35;
const uint8_t separateur = 36;
const uint8_t messageInitialisation = 37;
const uint8_t debutMessageErreur = 38; 
const uint8_t messageEteindre = 39;
const uint8_t debutMessagePresence = 40;
//********************************************************************************Setup**************************************************************************************/
void setup() {
  Serial2.begin(115200, SERIAL_8O2);                              //Initialisation du port serie(pin: 9, 10) déstiné au rs-485, en vitesse 115 200 et en communication 8bits, odd parity, 2stop-bits.
  Serial2.setTimeout(5);                                          //Initialisation d'une valeur de TimeOut.
  Serial.begin(19200);                                            //Initialisation du port serie(pin: 1, 2) à destination de l'IDE arduino pour avoir les retour du programme.
  Wire.begin();                                                   //Ouverture de la communication I2C (pin: 18, 19).
  pinMode(pinTensionSuivant, OUTPUT);
  digitalWrite(pinTensionSuivant, LOW);                           //Mise sans tension du pin qui est relié aux possible prochain arduino sur la ligne.
  /*****Création des cases memoir objet *****/
  i2c = new I2c();
  communication = new Communication();
  messageRecu = new QList<uint8_t>;
  adressesRecu = new QList<uint8_t>;
  messageEnvoi = new QList<uint8_t>;
  infosMeConcernant = new QList<uint8_t>;
  /*****Récuperation de possibles données si la mises hors tension ne s'est pas faite pas la voie normal.*****/
  monAdresse = i2c->recupereAdresseEeprom();                      //Récuperation de la valeur adresse enregistré dans l'eeprom. Renvoie "0" si il n'y en a pas.
  if(monAdresse != 0 && monAdresse != 255){                                            //Si mon adresse n'est pas égal a 0 alors j'ouvre la tension pour le possible prochain teensy.
    digitalWrite(pinTensionSuivant, HIGH);
  }
}
//********************************************************************************Loop***************************************************************************************/
void loop() {
    Serial.print(monAdresse);                                                                                             //Affiche la valeur "monAdresse" dans l'IDe Arduino.
    delay(5);
    /*****Initialisation des QList message et de la taille *****/
    messageRecu->clear();
    messageEnvoi->clear();
    taille = 0;
    /*****Réception du message *****/
    if(Serial2.available() > 1){
      messageRecu->push_back(Serial2.read());                                                                           //Reception des deux octet contennt la taille du message.
      messageRecu->push_back(Serial2.read());
      taille = communication->uint8ToUInt16(messageRecu->at(0), messageRecu->at(1));                                    //mise en forme de la taille du message.
      while(Serial2.available() < (taille + 2));                                                                        //Attente d'un message contenant au moins la taille du message entier (c'est a dire la taille + checksum(2)).
      for(unsigned int i(0); i< (taille + 2); i++){                                                                     //Récuperation du nombre d'octet equivalent a la taille du message entier.
        messageRecu->push_back(Serial2.read()); 
      }     
    }
    /*****Affichage du message dans la console. *****/
    for(unsigned int i(0); i<messageRecu->size(); i++){
      Serial.print(messageRecu->at(i));
    } 
    Serial.println(); 
    if(messageRecu->size() != 0){                                                                                         //Dans le cas ou messageRecu est vide on recommence directement.
      if(communication->verificationTailleChecksum(messageRecu) == true) {                                                //Dans ou le checksum et la taille son correct(suppression de ceux ci dans le message).
  /***************************Message Information*******************/
        if(messageRecu->at(0) == debutMessageInformation) {                                                               //Dans le cas ou il s'agit d'un message d'information.
          messageRecu->pop_front();                                                                                       //Suppression de l'octet indicant que c'est un messaage information.
          infosMeConcernant->clear();                                                                                     //Initialisation du tableaux qui contiendra les information a ma destination.
          adressesRecu->clear();                                                                                          //Initialisation du tableaux adressesRecu en preparation à l'affiliation des adresses du message.
          while(messageRecu->at(0) != separateur && messageRecu->at(0) != "" ){                                           //Tant que le debut du tableau messageRecu n'est pas le separateur ou qu'il n'est pas vide :
            adressesRecu->push_back(messageRecu->at(0));                                                                    //On ajoute l'adresse au tableaux adressesRecu.
            messageRecu->pop_front();                                                                                       //Puis on l'enleve du tableaux messageRecu.
          } 
          messageRecu->pop_front();                                                                                       //Suppression du separateur ne laissant plus que les informations.
          for(int i(0); i<adressesRecu->size() ; i++){                                                                    //Pour chaque adresse contenue dans le tableau adresseRecu.
            if(adressesRecu->at(i) == monAdresse){                                                                        //Dans le cas ou mon adresse est presente.
              erreur = 1;                                                                                                 //Je prepare une valeur erreur disant que j'ai recu quelquechose (1 est une valeur transitoire).
              if(i != 0){                                                                                                 //Et si je ne suis pas la première adresse du tableaux
                adressePrecedente = adressesRecu->at(i-1);                                                                //Je récupère l'adresse precedente pour me situer dans les futur messages d'erreur.
              }
            }
             /**
              * Ensuite selon le groupe de l'adresse actuel: 
              * Je récupère les information si c'est mon adresse.
              * Je supprime le nombre d'informations correspondant aux groupe. 
              */
            if(adressesRecu->at(i) < 100 && adressesRecu->at(i) >49){                                                     //Groupe 1
              for(int i(0); i < 3 ; i++){                                                                                 //Ici 1 est le nombre d'octet qui interrésse, c'est une valeur transitoire.
                if(adressesRecu->at(i) == monAdresse){                                                                    //Si c'est mon adresse.
                  infosMeConcernant->push_back(messageRecu->at(i));                                                       //Je récupère les informations.
                }
                messageRecu->pop_front() ;                                                                                //Suppression autant de fois que necessaire.
              }
            } else if(adressesRecu->at(i) < 150 && adressesRecu->at(i) > 99){                                             //Groupe 2
              for(int i(0); i < 3 ; i++){
                if(adressesRecu->at(i) == monAdresse){ 
                  infosMeConcernant->push_back(messageRecu->at(i));
                }
                messageRecu->pop_front() ;
              }
            } else if(adressesRecu->at(i) < 200 && adressesRecu->at(i) >149){                                             //Groupe 3
              for(int i(0); i < 3 ; i++){
                if(adressesRecu->at(i) == monAdresse){ 
                  infosMeConcernant->push_back(messageRecu->at(i));
                }
                messageRecu->pop_front() ;
              }
            }
          }
          if(monAdresse == adressesRecu->at(0)){                                                                          //Si je suis l'adresse numero 1
            messageErreur();                                                                                              //Envoie d'un message d'erreur
          }
          traitementInfosMeConcernant(infosMeConcernant);
  /***************************Message Erreur************************/ 
        } else if(messageRecu->at(0) == debutMessageErreur) {                                                             //Dans le cas ou il s'agit d'un message d'erreur.
          if(messageRecu->at(1) == adressePrecedente){                                                                    //Si L'adresse du message recu correspond à adressePrecedente c'est à nous de parler.
            messageErreur();                                                                                              //Donc envoie d'un message d'erreur.
          }
  /***************************Message Initialisation****************/
        } else if(messageRecu->at(0) == messageInitialisation && (monAdresse == 0 || monAdresse == 255)) {                                       //Dans le cas ou il s'agit d'un message d'initialisation et que nous n'avons pas encore d'adresse.
          if(messageRecu->at(1) == 0){                                                                                    //Si c'est une demande de présence, je répond.
              delay(10);                                                                                                  //Petit delay pour ne pas surcharger la ligne.
              messageEnvoi->push_back(monGroupe);                                                                         //Debut création du message avec ajout de mon adresse.
              taille = messageEnvoi->size();                                                                              //récuperation de la taille du message.
              messageEnvoi->push_front((uint8_t)(taille & 0x00ff));                                                       //Séparation de la taille du message avec ajout d'une partie en avant du message.
              messageEnvoi->push_front((uint8_t)((taille & 0xff00) >> 8));                                                //Ajout de la seconde partie en avant du message.
              checksum = communication->calculChecksum(messageEnvoi);                                                     //Calcul du checksum.
              messageEnvoi->push_back((uint8_t)((checksum & 0xff00) >> 8));                                               //Séparation du checksum avec ajout d'uune partie à la fin du message.
              messageEnvoi->push_back((uint8_t)(checksum & 0x00ff));                                                      //Ajout de la seconde partie à la fin du message.
              for(int i(0); i<messageEnvoi->size(); i++){                                                                 //Pour autant de fois que la longueur du message.
                Serial2.write(messageEnvoi->at(i));                                                                       //On envoie les information une par une.
              }
              Serial2.flush();                                                                                            //On vide le buffer d'envoi.
          } else{                                                                                                         //Sinon on m'envoi une adresse.
              digitalWrite(pinTensionSuivant, HIGH);                                                                      //Ouverture du canal pour les prochains.
              monAdresse = messageRecu->at(1);                                                                            //Attribution de l'adresse recu.
              i2c->ecritureDataEeprom(0,monAdresse);                                                                      //Enregistrement de l'adresse dans l'eeprom.
          }
  /***************************Message Presence**********************/
        } else if(messageRecu->at(0) == debutMessagePresence) {                                                           //Dans le cas ou il s'agit d'un message de confirmation de presence.
          messageRecu->pop_front();                                                                                       //Suppression du charactere signalant que c'est un message de présence.
          erreur = 1;                                                                                                     //Erreur est egale a 1 pour aucun probléme à signaler
          if(messageRecu->at(0) == monAdresse){                                                                           //Si je suis le premier à devoir parler.
            messageErreur();                                                                                              //Envoie d'un message d'erreur.
          } else{                                                                                                         //Si je ne suis pas le premier.
            for(int i(0); i<messageRecu->size() ; i++){                                                                   //Pour autant de fois la taille du message.
              if(adressesRecu->at(i) == monAdresse){                                                                      //Si je trouve mon adresse.
                adressePrecedente = adressesRecu->at(i-1);                                                                //Je définie l'adresse precedente pour les futur messages d'erreur.
              }
            }
          }
  /***************************Message Eteindre**********************/
        } else if(messageRecu->at(0) == messageEteindre) {                                                                //Dans le cas ou il s'agit d'un message de sequence de fin de communication.
          i2c->ecritureDataEeprom(0,0);                                                                                   //Suppression de mon adresse dans l'eeprom.       
          monAdresse = 0;                                                                                                 //Remise a zero de mon adresse.
        }
      } else if(messageRecu->size() != 0) {                                                                               //Dans le cas ou le checksum et taille ne sont pas correct et que le message n'est pas vide.
        
      }
  }
}
/*********************************************************************************Fonction externe*****************************************************************************/
/**
 * Création du message d'erreur.
 * Il contient une variable erreur qui indique un type d'erreur.
 * Ainsi que les information du capteur.
 */
void messageErreur(){
      delay(35);
      if(!i2c->aimantPresence()){                                                                                       //Si la presence de l'aimant est compromise.
        erreur = 2;                                                                                                     //Valeur erreur 2 pour indiquer que l'aimant est mal placé.
        aimantdeg = 0;                                                                                                  //On entre la valeur zero a l'angle.
      } else{                                                                                                           //Sinon
        aimantdeg = i2c->lireAngle();                                                                                   //On recupère la valuer de l'angle
      }
      messageEnvoi->push_back(debutMessageErreur);                                                                      //On créer le message en debutant par l'indication messageerreur
      messageEnvoi->push_back(monAdresse);                                                                              //On ajoute l'adresse
      messageEnvoi->push_back(separateur);                                                                              //Puis le separateur
      messageEnvoi->push_back(erreur);                                                                                  //La valeur erreur.
      communication->floatToUInt8(aimantdeg, messageEnvoi);                                                             //Ajout de la valeur de l'angle dans 4 cases uint8 
      taille = messageEnvoi->size();                                                                                    //On recupere la taille
      messageEnvoi->push_front((uint8_t)(taille & 0x00ff));                                                             //Ajout de la taille en debut de message sous deux uint8
      messageEnvoi->push_front((uint8_t)((taille & 0xff00) >> 8));
      checksum = communication->calculChecksum(messageEnvoi);                                                           //Calcul du checksum
      messageEnvoi->push_back((uint8_t)((checksum & 0xff00) >> 8));                                                     //Ajout du checksum sous deux uint8
      messageEnvoi->push_back((uint8_t)(checksum & 0x00ff));
      for(int i(0); i<messageEnvoi->size(); i++){                                                                       //Pour chaque case du qlist
        Serial2.write(messageEnvoi->at(i));                                                                             //On envoie l'information
      }
      Serial2.flush();                                                                                                  //Puis on vide le buffer d'envoi.
}
/**
 * Fonction qui permet de traité les informations qui sont recu.
 */
void traitementInfosMeConcernant(QList<uint8_t>* infos){
  /**
   * Temporairement vide.
   */
}
