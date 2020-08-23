/*!
 * \file Fichier.h
 * \author Atma
 * \date 22 Août 2020
 * \brief Contient les prototypes de la classe Fichier
 * \version 1.0
 */

 #ifndef FICHIER_H
 #define FICHIER_H

#include <stdio.h>
#include <string>

class Fichier
{
    private:
        FILE * pointeurFichier;
	    char * buffer;
        long tailleFichier;
        std::string cheminFichier;

    public:

        /*!\struct ErreurFichier
        * \brief Liste des erreurs possible de la classe Fichier
        */
        enum class ErreurFichier
        {
            OuvertureFichierLecture,
            OuvertureFichierEcriture,
            TailleFichierIncorrect,
            AllocationMemoire,
            CopieMemoire,
            CopieFichier,
        };

        Fichier(std::string cheminFichier, char mode);

        ~Fichier();

        void determinerTailleFichier();

        void initialiserBuffer();

        void lire(); 

        void sauvegarder();

        char* get_buffer();

        std::string get_cheminFichier();

        long get_tailleFichier();

        void set_tailleFichier(long tailleFichier);
};

 #endif