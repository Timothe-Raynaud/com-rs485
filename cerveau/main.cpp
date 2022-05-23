#include <QApplication>
#include "cerveau.h"
#include "Traitements.h"

/**
 * @file main.cpp
 * @brief Programme principal ...
 * @details Crée et affiche la fenêtre principale de l'application ...
 * @param argc
 * @param argv[]
 * @return int
 */
int main(int argc, char *argv[]){
    QApplication app(argc, argv);


    Cerveau monProgramme;
    monProgramme.show();


    return app.exec();
}
