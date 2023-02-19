#pragma once
#include <fstream>
#include "flux_io.h"

using namespace std;

// Vous devez remplir cette section
#define MATRICULE "2169046"
#define NOM "AGUDZE"
#define PRENOM "PAUL"

//Constantes
static const int MAX_CHAR = 125;

extern const string FILENAME_CLIENT;
extern const string FILENAME_LIVRE;

// Définissez vos structures
struct Date_s {
    int jour, mois, annee,
        heure, minute, seconde;
};

struct Livre_s
{
    char Titre[MAX_CHAR];
    char Auteur[MAX_CHAR];
    bool Loue;
};

struct Adresse_s {
    size_t NumCivil;
    char Rue[MAX_CHAR], Ville[MAX_CHAR],
        CodePostal[7];
};

struct Pret_s {
    int NumLivre = -1;
    Date_s DateLocation, DateRetourPrevu;
};

struct Client_s
{
    char Nom[MAX_CHAR], Prenom[MAX_CHAR],
        Telephone[11];
    Adresse_s Adresse;
    size_t NombrePretActif;
    Pret_s Prets[3];
};