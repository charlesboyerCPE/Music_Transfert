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

void Fichier::obtenirTailleFichier()
{
	fseek(pointeurFichier, 0, SEEK_END);
	tailleFichier = ftell(pointeurFichier);
	rewind (pointeurFichier);
}

void Fichier::initialiserBuffer()
{
	buffer = (char*) malloc (tailleFichier+1);
	if (buffer == NULL) throw ErreurFichier::AllocationMemoire;
}

void Fichier::copierDansMemoire()
{
	size_t resultat;

	resultat = fread(buffer, sizeof(char), tailleFichier, pointeurFichier);
	if (resultat != tailleFichier) throw ErreurFichier::CopieMemoire;
}

void Fichier::copierDansFichier()
{
	size_t resultat;

	resultat = fwrite(buffer, sizeof(char), sizeof(buffer), pointeurFichier);
	if (resultat != sizeof(buffer)) throw ErreurFichier::CopieFichier;
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