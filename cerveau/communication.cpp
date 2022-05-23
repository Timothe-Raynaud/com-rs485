#include "communication.h"
/**
 * @brief Communication::Communication
 * Constructeur de la class Communication.
 */
Communication::Communication(){
    /**
     * Création de l'objet QSerialPort.
     */
    arduino = new QSerialPort();
}
/**
 * @brief Communication::~Communication
 * Destructeur de la class Communication.
 */
Communication::~Communication(){
    if(arduino->isOpen()){
        arduino->close();
    }
}
/**
 * @brief Communication::etatConnexion
 * @return bool
 * \fn Fonction qui test la connexion physique des machine qui communique et renvoie un booléen pour decrire l'etat.
 */
bool Communication::etatConnexion(){
    /**
     * Initialisation des variable serialBuffer ainsi que arduino_is_available
     */
    serialBuffer = "";
    arduino_is_available = false;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            if((serialPortInfo.productIdentifier() == arduino_uno_product_id) && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_uno_port_name = serialPortInfo.portName();
                arduino_is_available = true;
            }
        }
    }
    /**
     * Initialisation des parametre de communication si le port arduino est valable. Envoie true c'est le cas, sinon false.
     */
    if(arduino_is_available){
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::OddParity);
        arduino->setStopBits(QSerialPort::TwoStop);
        return true;
    } else {
        return false;
    }
}
/**
 * @brief readSerial
 * @return QByteArray
 * \fn Fonction qui lit ce qui se trouve dans le buffer de la ligne de communication.
 */
QByteArray Communication::readSerial(){
    /**
     * Initialisation de serialDataRead.
     */
    serialDataRead.clear();
    /**
     * Attent 10ms que des données soit disponible pour lire ces données et les mettre dans serialDataRead.
     */
    if(arduino->waitForReadyRead(10)){
        serialDataRead.append(arduino->readAll());
    }
    qDebug() << "Serial Data Read : " <<  serialDataRead;
    return serialDataRead;
}
/**
 * @brief Dialog::allumage
 * @param QByteArray const a
 * \fn Envoie sur la ligne série arduino le QByteArray a.
 */
void Communication::ecriture(QByteArray const a){
    /**
     * Initialisation de bw
     */
    quint64 bw = 0;
    /**
     * Si le port serie existe et que l'on peu ecrire dedans alors on ecrit la valeur de a et l'on attend que cette valeur soit completement ecrite.
     */
    if(arduino !=NULL && arduino->isWritable()){
        bw = arduino->write(a);
        arduino->flush();
        qDebug() << bw <<"bytes written | Value sent:" << a;
    }
    //qDebug() << "passage fonction ecriture" << (arduino !=NULL) << arduino->isWritable();
}
/**
 * @brief arduino
 * \fn Fonction qui affiche dans le terminal les informations des ports valide.
 */
void Communication::chercherPort(){
    qDebug() << "Number of ports : " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << serialPortInfo.description();
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier();
        qDebug() << "Vendor ID:" << serialPortInfo.vendorIdentifier() ;
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier();
        qDebug() << "product ID: " << serialPortInfo.productIdentifier() << "\n";
    }
}
