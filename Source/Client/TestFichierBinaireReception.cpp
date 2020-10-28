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
#include "../json.hpp"

#define CHEMIN_RECEPTION "Musique/"

int main(void)
{
    ClientTcp *monClient = NULL;

	std::ofstream fichierReception;

	nlohmann::json listeFichier;

	int tailleLu = 0;
	int tailleAlire = 5000;
	int nombreFichier = 0;
	int tailleJSON = 0;
	int i = 0;

	char bufferFichier[tailleAlire];
	
	char octetRecu;
	char octetJSON;

	std::string trameTailleJSON;
	std::string trameJSON;
    try
    {
        monClient = new ClientTcp();

		std::cout << "Connexion au serveur..." << std::endl; 
        monClient->connecter("127.0.0.1", 55555);

		//Reception de la taille du JSON
		std::cout << "\nReception de la liste des fichiers..." << std::endl; 
		while(octetJSON != '\n')
		{
			monClient->recevoirData(&octetJSON, 1);	
			if (octetJSON != '\n')
			{
				trameTailleJSON += octetJSON;
			}
		}
		tailleJSON = std::stoi(trameTailleJSON);

		//Reception de la trame contenant le JSON sous forme de chaine de char
		while(tailleLu < tailleJSON)
		{
			if((tailleLu + tailleAlire) > tailleJSON)
			{
				tailleAlire = tailleJSON - tailleLu;
			}
			
			monClient->recevoirData((void *)(bufferFichier + tailleLu), tailleAlire);
			tailleLu += tailleAlire;
			trameJSON += bufferFichier;
		}

		//Sérialization du JSON
		listeFichier = nlohmann::json::parse(trameJSON);

		//Reception des fichiers
		std::cout << "\nReception des fichiers..." << std::endl; 
		for (i = 0; i < listeFichier.size(); i++)
		{
			//Affichage du fichier a recevoir
			std::cout << "Fichier en attente:" << std::endl;
			std::cout << "\tNom: " << listeFichier[i]["nomFichier"] << std::endl;
			std::cout << "\tTaille: " << listeFichier[i]["tailleFichier"] << std::endl;

			//On créer le fichier avec les informations du header
			fichierReception.open(listeFichier[i]["nomFichier"], std::ios::binary);
			
			//On recoit le fichier par paquet d'octets
			tailleLu = 0;
			while(tailleLu < listeFichier[i]["tailleFichier"].get<int>())
			{
				if((tailleLu + tailleAlire) > listeFichier[i]["tailleFichier"].get<int>())
				{
					tailleAlire = listeFichier[i]["tailleFichier"].get<int>() - tailleLu;
				}

				monClient->recevoirData((void *)bufferFichier, tailleAlire);
				fichierReception.write(bufferFichier, tailleAlire);
				tailleLu += tailleAlire;
			}

			//On ferme le fichier en cours
			std::cout << "Sauvegarde de " << listeFichier[i]["nomFichier"] << " effectue" << std::endl << std::endl;
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