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

Fichier::Fichier(std::string cheminFichier, int mode)
{
	this->buffer = {NULL};
	this->pointeurFichier = NULL;

	switch (mode)
	{
		case 1:
			pointeurFichier = fopen(cheminFichier.c_str(), "r");
			if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierLecture;
			break;
		
		case 2:
			pointeurFichier = fopen(cheminFichier.c_str(), "w");
			if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierEcriture;
			break;
	}
}

Fichier::~Fichier()
{
	fclose(pointeurFichier);
}

void Fichier::obtenirTailleFichier()
{
	fseek(pointeurFichier, 0, SEEK_END);
	tailleFichier = ftell(pointeurFichier);
	rewind (pointeurFichier);
}

void Fichier::initialiserBuffer()
{
	buffer = (char*) malloc (sizeof(char*));
	if (buffer == NULL) throw ErreurFichier::AllocationMemoire;
}

void Fichier::copierDansMemoire()
{
	size_t resultat;

	resultat = fread(buffer, 1, tailleFichier, pointeurFichier);
	if (resultat != tailleFichier) throw ErreurFichier::CopieMemoire;
}

void Fichier::copierDansFichier()
{
	size_t resultat;

	resultat = fwrite(buffer, sizeof(char), strlen(buffer), pointeurFichier);
	if (resultat != strlen(buffer)) throw ErreurFichier::CopieFichier;
}

char* Fichier::get_buffer()
{
	return buffer;
}
