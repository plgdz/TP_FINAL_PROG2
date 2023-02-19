#pragma once
#include <string>
#include "type.h"
#include "utils.h"
#include "cvm 21.h"
#include <iomanip>

void NouveauLivre();
Livre_s RechercherLivre(int id);
void MiseAJourLivre(int id, Livre_s livre);
void AfficherLivre(int id);
void ListeDesLivresPretes();