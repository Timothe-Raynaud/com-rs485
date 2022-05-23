#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>

class Communication{
public:
    /**
     * @brief Communication::Communication
     * Constructeur de la class Communication.
     */
    Communication();
    /**
     * @brief Communication::~Communication
     * Destructeur de la class Communication.
     */
    ~Communication();
    /**
     * @brief Communication::etatConnexion
     * @return bool
     * \fn Fonction qui test la connexion physique des machine qui communique et renvoie un booléen pour decrire l'etat.
     */
    bool etatConnexion();
    /**
     * @brief allumage
     * \fn Fonction qui envoie un message (true lorsque le bouton est activé ou false lorsqu'il est desactive) à l'arduino.
     */
    void ecriture(QByteArray const a);
    /**
     * @brief readSerial
     * @return QByteArray
     * \fn Fonction qui lit ce qui se trouve dans le buffer de la ligne de communication.
     */
    QByteArray readSerial();
    /**
     * @brief arduino
     * \fn Fonction qui affiche dans le terminal les informations des ports valide.
     */
    void chercherPort();

private:
    QSerialPort *arduino;                                   //!< Port Serie nommé arduino.
    static const quint16 arduino_uno_vendor_id = 4292;      //!< définition du VID arduino.
    static const quint16 arduino_uno_product_id = 60000;    //!< définition du PID arduino.
    bool arduino_is_available;                              //!< Booléen définissans l'etat de connection physique
    QString arduino_uno_port_name;                          //!< QString contenant le nom du port série utilisé.
    QByteArray serialDataRead;                              //!< Variable de récéption des donées de l'Arduino.
    QByteArray serialDataWrite;                             //!< Variable pour ecriture des données vers l'Arduino.
    QString serialBuffer;                                   //!< Buffer pour conversions des données de l'Arduino.
    QStringList tabResult;                                  //!< Tableaux contenants les partie du message recu.


private slots:
    /**
     * @brief readAllMessage
     * @param QByteArray contenant un fragment de message.
     * \fn Fonction slots qui recois une ligne de message et la verifie puis traite l'information.
     */
    void readAllMessag(QByteArray const a);
};

#endif // COMMUNICATION_H
