# com-rs485 

  

Voici un programme de communication entre un ordinateur et autant de Teensy que voulu qui fonctionne avec Qt et Arduino IDE.  

La communication se fait en RS-485 avec des convertisseurs TTL/RS-485 

  

Lors de l'initialisation du système, un seul Teensy doit être alimenté. C'est à lui d'alimenter le suivant après avoir eu une adresse attribuée, de cette façon l'ordinateur laisse autant de Teensy que voulu se connecter et permet de déterminé le nombre de Teensy connecter afin d'analysé un éventuel problème. 

  

Le programme est conçu pour ajouter des Teensy qui recevront et enverront différentes informations et cela via l'attribution d'un groupe. 

Dans l'état les Teensy envoyé seulement une information de position en degrés via un capteur AS5600. 

  

Les Teensy fonctionne en 3,3v il faut donc penser aux moyens de transformer le signal. 

  

Une documentation Doxygene peut être extraite. 

  

Je en travail plus sur ce projet et je vous laisse donc mon email pour toutes question ou demande d'amélioration : raynaud.timothe@gmail.com 
