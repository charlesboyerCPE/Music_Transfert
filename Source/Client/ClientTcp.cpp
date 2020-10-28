/*!
 * \file ClientTcp.cpp
 * \author Charles BOYER
 * \date 15 Septembre 2019
 * \brief Contient la d�claration des m�thodes permettant la
 *		  communication, cot� client, via le protocole TCP
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
#include "ClientTcp.h"


/*!
 *  \brief Constructeur par d�faut de la classe ClientTcp
 * 	Permet l'ouverture du socket de communication
 */
ClientTcp::ClientTcp()
{
	socketDialogue = -1;
}

/*!
 * \brief Destructeur de la classe ClientTcp
 *        Permet l'arr�t du socket de communication
 */
ClientTcp::~ClientTcp()
{
	clore();
}

/*!
 * \fn void ClientTcp::connecter(std::string adresse, uint16_t port) 
 * \brief Etablit la connexion avec le serveur d'adresse "adresse" sur le port "port".
 * \param [in] adresse l'adresse IP du serveur.
 * \param [in] port le port de communication.
 */
void ClientTcp::connecter(std::string adresse, uint16_t port) 
{
	struct sockaddr_in adresseServeur;
	int ctrl;
	
	socketDialogue = socket(AF_INET , SOCK_STREAM , 0);
	if (socketDialogue == -1) throw ErreurClientTcp::Ouverture;
	
	//Mettre a zero l'adresse du serveur
	bzero((char *) &adresseServeur, sizeof(struct sockaddr_in));
	
	//Initialiser l'adresse du serveur
	adresseServeur.sin_family = AF_INET;
	adresseServeur.sin_port = htons(port); 
	
	ctrl = inet_aton (adresse.c_str(), &adresseServeur.sin_addr);
	if (ctrl == 0) throw ErreurClientTcp::AdresseMalEcrite;

	ctrl = connect (socketDialogue, (struct sockaddr *) &adresseServeur , sizeof(struct sockaddr_in));
	if (ctrl == -1) throw ErreurClientTcp::Connexion;
}

/*!
 * \fn void ClientTcp::clore() 
 * \brief Se d�connecte du serveur.
 */
void ClientTcp::clore() 
{
	if (socketDialogue != -1)
	{	
		close(socketDialogue);
		socketDialogue = -1;
	}
}

/*!
 * \fn void ClientTcp::emettreData(void* data, uint32_t nbOctets) 
 * \brief Emmet au serveur connect� les "nbOctets" premier octets du tableau "data".
 * \param [in] data le tableau o� sont stocker les donn�es re�us.
 * \param [in] nbOctets le nombre d'octets stocker dans le tableau data.
 */
ssize_t ClientTcp::emettreData(void* data, uint32_t nbOctets) 
{
	int nbEmit;

	if(socketDialogue == -1) throw ErreurClientTcp::NonOuvert;
	
	nbEmit = send(socketDialogue, data, nbOctets, 0);
	if (nbEmit != nbOctets) throw ErreurClientTcp::ErreurEnvoi; 
	else
	{
		return nbEmit;
	}
	
}

/*!
 * \fn void ClientTcp::recevoirData(void* data, uint32_t nbOctets) 
 * \brief Receptionne depuis le serveur "nbOctets" octets et stocke ces octets dans le tableau "data".
 * \param [in] data le tableau servant � stocker les donn�es re�us.
 * \param [in] nbOctets le nombre d'octets � stocker dans le tableau data.
 */
ssize_t ClientTcp::recevoirData(void* data, uint32_t nbOctets) 
{
	int nbRecu;
	
	if(socketDialogue == -1) throw ErreurClientTcp::NonOuvert;
	
	nbRecu = recv (socketDialogue, data, nbOctets , MSG_WAITALL); 

	if(nbRecu != nbOctets) throw ErreurClientTcp::ErreurReception;
	else
	{
		return nbRecu;
	}
	
}

