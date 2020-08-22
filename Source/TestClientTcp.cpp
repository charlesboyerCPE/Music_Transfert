/*!
 * \file TestClientTcp.cpp
 * \author Charles BOYER
 * \date 15 Septembre 2019
 * \brief Contient le programme de test permettant la
 *		  communication, coté client, via le protocole TCP
 * \version 1.0
 */

#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>

#include "ClientTcp.h"

int main (void)
{
	FILE *pointeurFichier;
	char *buffer;

	ClientTcp *monClient = NULL;
	
	try
	{
		//On créer le fichier
		pointeurFichier = fopen("testClient.txt", "ww");
		if (pointeurFichier == NULL)
		{
			fputs("ERREUR FICHIER", stderr);
			exit (1);
		}

		//On alloue la mémoire nécessaire pour contenir la musique
		buffer = (char*) malloc (sizeof(char*));
		if (buffer == NULL)
		{
			fputs("ERREUR MEMOIRE", stderr);
			exit (2);
		}

		//On instancie le client TCP
		monClient = new ClientTcp();
		monClient->connecter("127.0.0.1", 55555);
		
		//On receptionne le contenu du fichier que le serveur a envoyer
		monClient->recevoirData(buffer, sizeof(buffer)+1);
		monClient->clore();

		//On stock le contenu de buffer dans le fichier que nous avons ouvert
		fwrite(buffer, sizeof(char), sizeof(buffer), pointeurFichier);

		//On affiche le contenu du fichier
		std::cout << buffer << std::endl;
	}

	catch(ClientTcp::ErreurClientTcp y)
	{
		std::cout << "Erreur ClientTcp: ";
		switch (y)
		{
			case ClientTcp::ErreurClientTcp::Ouverture :
				std::cerr << "Création du socket" << std::endl;
				break;
			case ClientTcp::ErreurClientTcp::AdresseMalEcrite :
				std::cerr << "Adresse IPv4 mal ecrite" << std::endl;
				break;
			case ClientTcp::ErreurClientTcp::Connexion :
				std::cerr << "Connexion impossible" << std::endl;
				break;
			case ClientTcp::ErreurClientTcp::ErreurEnvoi :
				std::cerr << "Emission des donnees impossible" << std::endl;
				monClient->clore();
				break;
			case ClientTcp::ErreurClientTcp::ErreurReception :
				std::cerr << "Reception des donnees impossible" << std::endl;
				monClient->clore();
				break;
			case ClientTcp::ErreurClientTcp::NonOuvert :
				std::cerr << "Socket non Ouvert" << std::endl;
				break;
		}
	}
		
	delete monClient;
	fclose(pointeurFichier);
	return 0;
}
