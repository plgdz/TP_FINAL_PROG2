#pragma once
#include <string>
#include "type.h"
#include "utils.h"
#include "cvm 21.h"
#include <iomanip>

void NouveauClient();
Client_s RechercherDossierClient(int id);
void MiseAJourClient(int id, Client_s client);
void AfficherDossierClient(int id);
void ListeDesClientsEnRetard();