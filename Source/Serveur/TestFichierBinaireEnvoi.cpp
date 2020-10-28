/*!
 * \file TestFichierBinaireEnvoi.cpp
 * \author Atma
 * \date 24 Août 2020
 * \brief Programme permettant de tester la classe Fichier
 *       avec des fichiers binaires en envoi
 *       Compilation:
 *       g++ -o TestFichierBinaireEnvoi TestFichierEnvoi.cpp ../Fichier.cpp ../ServeurTCP.cpp
 * \version 1.0
 */

#include <iostream>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>

#include "ServeurTCP.h"
#include "../json.hpp"

#define CHEMIN_MUSIQUE "/mnt/d/Windows/Bureau/Envoi Fichier"

//Retourne un vecteur contenant tous les noms de fichier présent dans le dossier, à l'adresse chemin
std::vector<std::string> obtenirFichierDansDossier(std::string chemin)
{
	std::vector<std::string> nomsFichier;

	for(const auto & entry : std::filesystem::directory_iterator(chemin))
	{
		std::cout << "Fichier trouver: " << entry.path().filename() << std::endl;
		nomsFichier.push_back(entry.path());
	}

	return nomsFichier;
}

int main(void)
{
    ServeurTCP *monServeurTCP = NULL;

	std::ifstream fichierEnvoi;
	std::vector<std::string> nomsFichier;

	std::string trameTailleJSON;
	std::string trameFichier;

	std::string dumpJSON;
	nlohmann::json listeFichier;

	int tailleAlire = 5000;
	int tailleLu = 0;
	int i = 0;

	char bufferFichier[tailleAlire];

    try
    {
        monServeurTCP = new ServeurTCP();
		
		//On analyse le dossier Musique pour obtenir le nom de tous les fichiers
		std::cout << "ANALYSE DU DOSSIER..." << std::endl; 
		nomsFichier = obtenirFichierDansDossier(CHEMIN_MUSIQUE);
		
		//On ouvre le serveur et on connecte un client
		std::cout << "\nOuverture du serveur et attente du client..." << std::endl; 
		monServeurTCP->ouvrir("127.0.0.1", 55555);
		monServeurTCP->connecterUnClient();

		//Création d'une liste contenant les info des fichiers
		std::cout << "Creation de la liste de fichier..." << std::endl; 
		for(i = 0; i < nomsFichier.size(); i++)
		{
			listeFichier[i] = 
			{ 
				{"nomFichier", std::filesystem::path(nomsFichier[i]).filename()}, 
				{"tailleFichier", std::filesystem::file_size(nomsFichier[i])} 
			};
		}

		//Préparation de l'envoi de la liste
		dumpJSON = listeFichier.dump();

		//Envoi de la trame contenant la taille du JSON
		std::cout << "\nEnvoi de la liste au client..." << std::endl; 
		trameTailleJSON = std::to_string(dumpJSON.size()) + "\n";
		monServeurTCP->emettreData((void*) trameTailleJSON.c_str(), trameTailleJSON.size());

		//Envoi du JSON contenant les informations des fichiers
		while(tailleLu < dumpJSON.size())
		{
			if((tailleLu + tailleAlire) > dumpJSON.size())
			{
				tailleAlire = dumpJSON.size() - tailleLu;
			}

			//Envoi du JSON
			monServeurTCP->emettreData((void*)(dumpJSON.c_str() + tailleLu), tailleAlire);
			tailleLu += tailleAlire;
		}
		
		//Envoi des fichiers
		std::cout << "\nEnvoi des fichiers au client..." << std::endl; 
		for (i = 0; i < listeFichier.size(); i++)
		{
			//On ouvre le fichier en lecture
			fichierEnvoi.open(nomsFichier[i], std::ios::binary);

			//Afficher le contenu du header
			std::cout << "Fichier en envoi:" << std::endl;
			std::cout << "\tNom: " << listeFichier[i]["nomFichier"] << std::endl;
			std::cout << "\tTaille: " << listeFichier[i]["tailleFichier"] << std::endl;

			//Envoi du fichier petit à petit
			tailleLu = 0;
			while(tailleLu < listeFichier[i]["tailleFichier"].get<int>())
			{
				if((tailleLu + tailleAlire) > listeFichier[i]["tailleFichier"].get<int>())
				{
					tailleAlire = listeFichier[i]["tailleFichier"].get<int>() - tailleLu;
				}
				
				//On copie une partie du fichier dans le buffer
				fichierEnvoi.read(bufferFichier, tailleAlire);

				//On envoi le buffer petit à petit
				monServeurTCP->emettreData((void *)bufferFichier, tailleAlire);
				tailleLu += tailleAlire;
			}

			//On ferme le fichier en cours
			std::cout << "Envoi de " << listeFichier[i]["nomFichier"] << " effectue" << std::endl << std::endl;	
			fichierEnvoi.close();
		}

		monServeurTCP->deconnecterUnClient();
		monServeurTCP->fermer();
    }

    catch (ServeurTCP::ErreurServeurTCP x)
	{
		std::cout << "Erreur ServeurTCP: ";
		switch (x)
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