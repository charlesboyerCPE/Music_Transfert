/*!
 * \file ServeurTCP.h
 * \author Atma
 * \date 21 Août 2020
 * \file ServeurTCP.cpp
 * \brief Contient la déclaration des méthodes permettant
 *        la communication, coté serveur, via le protocole TCP
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
#include <iostream>
#include "ServeurTCP.h"

/*!
 * \brief Constructeur par défaut de la classe ServeurTCP
 * 	Permet l'ouverture du socket de communication
 */
ServeurTCP::ServeurTCP()
{
	socketAttenteClient = -1;
	socketDialogueClient = -1;
}

//Destructeur par défaut de la classe ServeurTCP
/*!
 * \brief Destructeur de la classe ServeurTCP
 * 	Permet la fermeture du socket de communication
 */
ServeurTCP::~ServeurTCP()
{
	socketAttenteClient = -1;
	socketDialogueClient = -1;
	fermer();
}

/*!
 * \fn void ServeurTCP::ouvrir(std::string adresse, uint16_t port, int32_t nbPlaces) 
 * \brief Ouvre le serveur TCP sur l'interface aillant pour adresse "adresse" avec comme port "port". 
 *        Le nombre de place pour la file d'attente est "nbPlaces".
 * \param [in] adresse l'adresse IP du client.
 * \param [in] port le port de communication.
 * \param [in] nbPlaces le nombre de place pour la file d'attente.
 */
void ServeurTCP::ouvrir(std::string adresse, uint16_t port, int32_t nbPlaces) 
{
	struct sockaddr_in adresseServeur;
	int ctrl;

	socketAttenteClient = socket(AF_INET, SOCK_STREAM, 0);
	if (socketAttenteClient == -1) throw ErreurServeurTCP::CreationInterface;
	
	bzero((char*)&adresseServeur, sizeof(struct sockaddr_in));

	adresseServeur.sin_family = AF_INET;
	adresseServeur.sin_port = htons(port); //On passe le numero de port en reseau

	ctrl = inet_aton(adresse.c_str(), &adresseServeur.sin_addr); //On passe l'adresse en r�seau
	if (ctrl == 0) throw ErreurServeurTCP::AdresseMalEcrite;

	ctrl = bind(socketAttenteClient, (struct sockaddr*) &adresseServeur, sizeof(struct sockaddr_in));
	if (ctrl == -1) throw ErreurServeurTCP::AdresseDejaUtilise;

	ctrl = listen(socketAttenteClient, nbPlaces);
	if (ctrl == -1) throw ErreurServeurTCP::OuvertureServeur;
}

/*!
 * \fn void ServeurTCP::fermer() 
 * \brief Ferme le serveur TCP. Coupe si necessaire la communication en cours avec un client.
 */
void ServeurTCP::fermer() 
{
	close(socketAttenteClient);
	deconnecterUnClient();
}

/*!
 * \fn void ServeurTCP::recevoirData(void* data, uint32_t tailleData) 
 * \brief Recoit des données du client connecté. Receptionne "tailleData" octets. 
 *        Les octets recus sont stockés dans le tableau "data".
 * \param [in] data le tableau où sont stockés les données.
 * \param [in] tailleData la taille des données reçu.
 */
ssize_t ServeurTCP::recevoirData(void* data, uint32_t tailleData) 
{
	int nbEnvoi;

	nbEnvoi = recv(socketDialogueClient, data, tailleData, MSG_WAITALL);
	if (nbEnvoi != tailleData) throw ErreurServeurTCP::EchecReception;	
	else
	{
		return nbEnvoi;
	}
}

/*!
 * \fn ServeurTCP::emettreData(void* data, uint32_t nbOctets) 
 * \brief Emet au client connecté les "nbOctets" octets du tableau "data".
 * \param [in] data le tableau où sont stockés les données.
 * \param [in] nbOctets les octets du tableau data.
 */
ssize_t ServeurTCP::emettreData(void* data, uint32_t nbOctets) 
{
	int nbEmit;

	nbEmit = send(socketDialogueClient, data, nbOctets, 0);
	if (nbEmit != nbOctets) throw ErreurServeurTCP::EchecEnvoi;
	else
	{
		return nbEmit;
	}
}

/*!
 * \fn ServeurTCP::connecterUnClient() 
 * \brief Accepte un client. Méthode bloquante.
 */
void ServeurTCP::connecterUnClient() 
{
	socklen_t tailleAdresseClient;
	struct sockaddr_in adresseClient;

	tailleAdresseClient = sizeof(struct sockaddr_in);
	bzero((char*)& adresseClient, sizeof(struct sockaddr_in));

	socketDialogueClient = accept(socketAttenteClient, (struct sockaddr*) & adresseClient, &tailleAdresseClient);
	if (socketDialogueClient == -1) throw ErreurServeurTCP::EchecAttenteClient;

}

/*!
 * \fn ServeurTCP::deconnecterUnClient() 
 * \brief D�connecte le client connect�.
 */
void ServeurTCP::deconnecterUnClient() 
{
	close(socketDialogueClient);
}

