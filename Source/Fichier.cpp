/*!
 * \file Fichier.cpp
 * \author Atma
 * \date 22 Août 2020
 * \brief Contient la déclaration de la classe Fichier
 * \version 1.0
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "Fichier.h"

Fichier::Fichier(std::string cheminFichier, char mode)
{
	buffer = {NULL};
	this->pointeurFichier = NULL;
	this->cheminFichier = cheminFichier;
	tailleFichier = 0;

	if (mode == 'r')
	{
		pointeurFichier = fopen(cheminFichier.c_str(), "r");
		if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierLecture;
	} 
	else if (mode == 'w')
	{
		pointeurFichier = fopen(cheminFichier.c_str(), "w");
		if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierEcriture;
	}
}

Fichier::~Fichier()
{
	fclose(pointeurFichier);
	free(buffer);
}

void Fichier::determinerTailleFichier()
{
	fseek(pointeurFichier, 0, SEEK_END);
	tailleFichier = ftell(pointeurFichier);
	rewind (pointeurFichier);
}

void Fichier::initialiserBuffer()
{
	buffer = (char*) malloc (sizeof(char) * tailleFichier);
	if (buffer == NULL) throw ErreurFichier::AllocationMemoire;
}

void Fichier::lire()
{
	size_t resultat;

	resultat = fread(buffer, 1, tailleFichier, pointeurFichier);
	if (resultat != tailleFichier) throw ErreurFichier::CopieMemoire;
}

void Fichier::sauvegarder()
{
	size_t resultat;

	resultat = fwrite(buffer, sizeof(char), strlen(buffer), pointeurFichier);
	if (resultat != strlen(buffer)) throw ErreurFichier::CopieFichier;
}

char* Fichier::get_buffer()
{
	return buffer;
}

std::string Fichier::get_cheminFichier()
{
	return cheminFichier;
}

long Fichier::get_tailleFichier()
{
	return tailleFichier;
}

void Fichier::set_tailleFichier(long tailleFichier)
{
	this->tailleFichier = tailleFichier;
	std::cout << "Fichier::set_tailleFichier() - Nouvelle taille de fichier: " << this->tailleFichier << std::endl;
}