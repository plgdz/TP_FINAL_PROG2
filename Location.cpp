// Ajouter vos include
#include "Location.h"



Date_s Location(int client_id, int livre_id) {
	// Si pas de prêt -> retourne date vide
	// Sinon retourne la date de retour
	// Récupère le client
	static const Date_s vide = {};
	Client_s client = {};
	Livre_s livre = {};

	//Verif si le livre existe
	if (LivreEquals(livre, RechercherLivre(livre_id)))
		return vide;
	else 
	{
		livre = RechercherLivre(livre_id);
		//verif si le livre est loue
		if (livre.Loue == true)
			return vide;
		else
			livre.Loue = true;
	}

	 //verif si le client existe
	if (ClientEquals(client, RechercherDossierClient(client_id)))
		return vide;
	else {
		client = RechercherDossierClient(client_id);
		//verif si le client peut louer
		for (size_t i = 0; i < 3; i++)
		{
			if (client.NombrePretActif == 3)
				return vide;
			else
			{
				client.Prets[client.NombrePretActif].NumLivre = livre_id;
				client.Prets[client.NombrePretActif].DateLocation = Aujourdhui();
				client.Prets[client.NombrePretActif].DateRetourPrevu = AjouterJours(Aujourdhui(), 15);
				client.NombrePretActif++;
				break;
			}		
		}
	}
	 //MAJ 
	 MiseAJourClient(client_id, client);
	 MiseAJourLivre(livre_id, livre);

	return AjouterJours(Aujourdhui(), 15);
}

float Retour(int client_id, int livre_position, Date_s retour) {
	// livre_position représente la position du livre à retourner dans le tableau du client
	static const float penalite = 5, TVQ = 0.09975, TPS = 0.05;
	float frais;
	int nbJourRetard;

	Client_s client = RechercherDossierClient(client_id);
	Livre_s livre = RechercherLivre(livre_position);

	nbJourRetard = 0;
	frais = 0;
	for (size_t i = 0; i < 3; i++)
	{
		if (livre_position == client.Prets[i].NumLivre)
		{
			nbJourRetard = NombreJours(client.Prets[i].DateRetourPrevu, retour);

			livre.Loue = false;
			
			if (nbJourRetard > 0)
			{
				frais = penalite * nbJourRetard;
				frais *= 1 + (TVQ + TPS);
			}

			for (size_t x = i; x < client.NombrePretActif; x++)
				client.Prets[x] = client.Prets[x + 1];

			client.NombrePretActif--;

			for (size_t i = client.NombrePretActif; i < 3; i++)
				client.Prets[i] = {};		
		}
	}
	MiseAJourClient(client_id, client);
	MiseAJourLivre(livre_position, livre);
	return frais;
}