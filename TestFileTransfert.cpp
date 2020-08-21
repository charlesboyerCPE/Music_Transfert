/*!
 * \file TestFileTransfert.cpp
 * \author Atma
 * \date 21 Août 2020
 * \brief Contient le programme permettant de tester 
 *        le transfert d'un fichier simple
 * \version 1.0
 */


#include <string>
#include <cstdint>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include "ServeurTCP.h"


int main(void)
{
    ServeurTCP *monServeurTCP = NULL;
	std::string monTexte;

    try
    {
		std::ifstream monFichier("test.txt");
        monServeurTCP = new ServeurTCP();

        monServeurTCP->ouvrir("127.0.0.1", 55555);
        monServeurTCP->connecterUnClient();

		while(getline(monFichier, monTexte))
		{
			monServeurTCP->emettreData(&monTexte, sizeof(monTexte));
		}

        monServeurTCP->deconnecterUnClient();
        monServeurTCP->fermer();
    }

    catch (ServeurTCP::ErreurServeurTCP y)
	{
		std::cout << "Erreur ServeurTCP: ";
		switch (y)
		{
			case ServeurTCP::ErreurServeurTCP::CreationInterface :
				std::cerr << "Création du socket" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::AdresseMalEcrite :
				std::cerr << "Adresse IPv4 mal ecrite" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::AdresseDejaUtilise :
				std::cerr << "Adresse IPv4 deja utilise" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::OuvertureServeur :
				std::cerr << "Ouverture du serveur TCP impossible" << std::endl;
				monServeurTCP->fermer();
				break;

			case ServeurTCP::ErreurServeurTCP::EchecReception :
				std::cerr << "Reception des donnees impossible" << std::endl;
				monServeurTCP->fermer();
				break;

			case ServeurTCP::ErreurServeurTCP::EchecEnvoi :
				std::cerr << "Envoie des donnees impossible" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::EchecAttenteClient :
				std::cerr << "Attente client" << std::endl;
				break;

		}
	}

    delete monServeurTCP;

    return 0;
}