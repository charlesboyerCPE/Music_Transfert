/*!
 * \file TestFichierBinaireReception.cpp
 * \author Atma
 * \date 22 Août 2020
 * \brief Programme permettant de tester la classe Fichier
        avec des fichiers binaires en reception
        Compilation:
        g++ -o TestFichierBinaireReception TestFichierEnvoi.cpp ../Fichier.cpp ../ClientTcp.cpp
 * \version 1.0
 */

#include <iostream>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>

#include "ClientTcp.h"
#include "../HeaderFichier.h"
#include "../Header.h"

#define CHEMIN_RECEPTION "MusiqueRecu/"

int main(void)
{
    ClientTcp *monClient = NULL;
	HeaderFichier headerFichier;
	Header header;

	std::ofstream fichierReception;
	
	int tailleLu = 0;
	int tailleAlire = 5000;
	int tailleFichier = 0;
	int nombreFichier = 0;

	char bufferFichier[tailleAlire];
	

    try
    {
        monClient = new ClientTcp();

        monClient->connecter("127.0.0.1", 55555);
		
		//On recoit le nombre de fichier que le serveur va envoyer
		monClient->recevoirData(&header, sizeof(header));
		std::cout << "Nombre de fichier a recevoir: " << header.nombreFichier << std::endl;

		for (int i = 0; i < header.nombreFichier; i++)
		{

			//On recoit le header du fichier
			monClient->recevoirData(&headerFichier, sizeof(headerFichier));
			std::cout << "Fichier recu:" << std::endl;
			std::cout << "\tNom: " << headerFichier.nomFichier << std::endl;
			std::cout << "\tTaille du nom: " << headerFichier.tailleNomFichier << std::endl;
			std::cout << "\tTaille: " << headerFichier.tailleFichier << std::endl;

			//On créer le fichier avec les informations du header
			fichierReception.open(headerFichier.nomFichier, std::ios::binary);
			tailleFichier = headerFichier.tailleFichier;
			
			//On recoit le fichier par paquet d'octets
			while(tailleLu < tailleFichier)
			{
				if((tailleLu + tailleAlire) > tailleFichier)
				{
					tailleAlire = tailleFichier - tailleLu;
				}

				monClient->recevoirData(bufferFichier, tailleAlire);
				fichierReception.write(bufferFichier, tailleAlire);
				tailleLu += tailleAlire;	
			}

			//On ferme le fichier en cours
			std::cout << "\nSauvegarde de " << headerFichier.nomFichier << std::endl;
			fichierReception.close();
		}

		monClient->clore();
    }

    catch(ClientTcp::ErreurClientTcp x)
    {
        std::cout << "Erreur ClientTcp: ";
		switch (x)
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

    return 0;
}