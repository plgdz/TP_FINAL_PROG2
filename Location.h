#pragma once
#include <string>
#include <iomanip>
#include "type.h"
#include "utils.h"
#include "cvm 21.h"
#include "Clients.h"
#include "Livres.h"

Date_s Location(int client_id, int livre_id);
float Retour(int client_id, int livre_position, Date_s retour);