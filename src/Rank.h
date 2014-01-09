#pragma once
#include <vector>
#include "Kart.h"
#include "PlayerIA.h"

void ranking(std::vector<Kart*>& karts); // Gère le classement de la course
void getFinalRanking(std::vector<PlayerIA*>& players); // Affiche le classement à la fin de la course
