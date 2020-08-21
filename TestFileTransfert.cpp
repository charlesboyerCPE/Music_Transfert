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
#include <cstring>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "ServeurTCP.h"


int main(void)
{
	FILE *pointeurMusique;
	long tailleMusique;
	char *buffer;
	size_t resultat;

    ServeurTCP *monServeurTCP = NULL;
	
    try
    {
		//On ouvre la musique
		pointeurMusique = fopen("Musique/ADAMAS.mp3", "r");
		if (pointeurMusique == NULL)
		{
			fputs("ERREUR FICHIER", stderr);
			exit (1);
		}

		//On obtient la taille du fichier
		fseek(pointeurMusique, 0, SEEK_END);
		tailleMusique = ftell(pointeurMusique);
		rewind (pointeurMusique);

		//On alloue la mémoire nécessaire pour contenir la musique
		buffer = (char*) malloc (sizeof(char*)*tailleMusique);
		if (buffer == NULL)
		{
			fputs("ERREUR MEMOIRE", stderr);
			exit (2);
		}

		//On copie la musique dans la mémoire
		resultat = fread(buffer, 1, tailleMusique, pointeurMusique);
		if (resultat != tailleMusique)
		{
			fputs("ERREUR LECTURE", stderr);
			exit (3);
		}

		//On ouvre le serveur TCP
        monServeurTCP = new ServeurTCP();
        monServeurTCP->ouvrir("127.0.0.1", 55555);
        monServeurTCP->connecterUnClient();

		//On envoi le contenu de buffer
		monServeurTCP->emettreData(buffer, sizeof(buffer));

		//On ferme le serveur 
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

	//On ferme le fichier et on libère la mémoire
	fclose(pointeurMusique);
	free(buffer);
    delete monServeurTCP;

    return 0;
}