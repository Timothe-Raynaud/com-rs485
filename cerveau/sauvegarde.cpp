#include "sauvegarde.h"
/**
 * @brief Sauvegarde
 * Constructeur de l'objet Sauvegarde qui ouvre la communication avec le fichier externe save.xml
 */
Sauvegarde::Sauvegarde(){
    doc = new QDomDocument("save");
    file = new QFile("save.xml");
    if(!file->open(QIODevice::ReadWrite)){
        qDebug() << "erreur d'accés au fichier xml save.xml";
    } else{
        root = doc->createElement("Root");
        doc->appendChild(root);
    }
}
/**
 * @brief ajoutAdresse
 * @param QByteArray a
 * \fn ajoute sans le fichier xml une nouvelle adresse ainsi que le groupe d'appartenance.
 */
void Sauvegarde::ajoutAdresse(QByteArray a){
    QDomElement teensy = doc->createElement("Teensy");
    teensy.setAttribute("Groupe", QString::number(a[0]));
    teensy.appendChild(doc->createTextNode(QString::number(a[1])));
    root.appendChild(teensy);
}
/**
 * @brief recupereAdresses
 * @return QByteArray adresses
 * \fn Fonction qui renvoie dans un tableaux les Adresses présente dans le fichier save.
 */
QByteArray Sauvegarde::recupereAdresses(){
    adresses.clear();
    doc->clear();
    doc->setContent(file);
    QDomElement racine = doc->documentElement();
    QDomElement child = racine.firstChild().toElement();
    while(!child.isNull()){
        if(child.tagName() == "Teensy"){
            QString a = child.text();
            int b = a.toInt();
            adresses.append((uint8_t)b);
            qDebug() << "test" << b;
        }
        child = child.nextSibling().toElement();
    }
    qDebug() << adresses;
    return adresses;
}
/**
 * @brief estTIlVide
 * @return  bool
 * \fn Vérifie si la fichier save.xml est vide ou non.
 */
bool Sauvegarde::estTIlVide(){
    if(file->size() == 0){
        return true;
    } else{
        return false;
    }
}
/**
 * @brief arret
 * \fn fonction qui vide le fichier save.xml
 */
void Sauvegarde::arret(){
    file->resize(0);
    file->close();
}
/**
 * @brief stream
 * \fn Envoie des informations aux document save.xml une fois récolté.
 */
void Sauvegarde::stream(){
    QTextStream stream(file);
    stream << doc->toString();
}
