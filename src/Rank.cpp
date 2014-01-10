#include "Rank.h"

// Gère le classement de la course
void ranking(std::vector<Kart*>& karts){
	// Boucle sur tous les karts
	for(std::vector<Kart*>::iterator currKart = karts.begin() ; currKart != karts.end(); ++currKart){

		// Boucle sur tous les autres karts
		for(std::vector<Kart*>::iterator otherKart = currKart+1 ; otherKart != karts.end(); ++otherKart){
			// Si otherKart est classé juste avant ou juste après currKart
			if( (*currKart)->getRank() == (*otherKart)->getRank()+1 || (*currKart)->getRank() == (*otherKart)->getRank()-1 ){
				// Si currKart a passé moins de noeuds que otherKart
				if( (*currKart)->getNbNodesPassed() < (*otherKart)->getNbNodesPassed() ){
					// Si currKart est devant otherKart
					if( (*currKart)->getRank() < (*otherKart)->getRank() ){
						(*currKart)->incrRank();
						(*otherKart)->decrRank();
					}else{

					}
				}// Si currKart a passé plus de noeuds que otherKart
				else if( (*currKart)->getNbNodesPassed() > (*otherKart)->getNbNodesPassed() ){
					// Si currKart est derriere otherKart
					if( (*currKart)->getRank() > (*otherKart)->getRank() ){
						(*currKart)->decrRank();
						(*otherKart)->incrRank();
					}else{

					}
				}// Si currKart et otherKart ont franchi le même nombre de noeuds
				else if( (*currKart)->getNbNodesPassed() == (*otherKart)->getNbNodesPassed() ){
					// Si currKart est plus proche du noeud que otherKart
					if( (*currKart)->distanceToNextNode() < (*otherKart)->distanceToNextNode() ){
						// Si currKart est derrière otherKart
						if( (*currKart)->getRank() > (*otherKart)->getRank() ){
							(*currKart)->decrRank();
							(*otherKart)->incrRank();
						}else{

						}
					}// Si currKart est plus loin du noeud que otherKart
					else if( (*currKart)->distanceToNextNode() > (*otherKart)->distanceToNextNode() ){
						// Si currKart est devant otherKart
						if( (*currKart)->getRank() < (*otherKart)->getRank() ){
							(*currKart)->incrRank();
							(*otherKart)->decrRank();
						}else{

						}
					}
				}
			}	
		}//for tous les autres karts
	}//for tous les karts
}

// Affiche le classement à la fin de la course
void getFinalRanking(std::vector<PlayerIA*>& players){
	const char* finalRankTab[8];
	for(std::vector<PlayerIA*>::iterator it = players.begin() ; it != players.end(); ++it){
		finalRankTab[(*it)->getKart().getRank()-1] = (*it)->getCharacter().getCharacterName();
	}
	std::cout << "Résultat final : " << std::endl;
	std::cout << "1 : " <<  finalRankTab[0] << std::endl;
	std::cout << "2 : " <<  finalRankTab[1] << std::endl;
	std::cout << "3 : " <<  finalRankTab[2] << std::endl;
	std::cout << "4 : " <<  finalRankTab[3] << std::endl;
	std::cout << "5 : " <<  finalRankTab[4] << std::endl;
	std::cout << "6 : " <<  finalRankTab[5] << std::endl;
	std::cout << "7 : " <<  finalRankTab[6] << std::endl;
	std::cout << "8 : " <<  finalRankTab[7] << std::endl;
 
} 


std::vector<SDL_Surface*> createRankSurfaces(TTF_Font* font){
	SDL_Color white = {255, 255, 255};
	std::vector<SDL_Surface*> rankSurfaces;
	for(int i=0; i<8; ++i){
    	SDL_Surface* newSurface = NULL;
    	rankSurfaces.push_back(newSurface);
    }
    rankSurfaces[0] = TTF_RenderText_Blended(font, "1er", white);
    rankSurfaces[1] = TTF_RenderText_Blended(font, "2ème", white);
    rankSurfaces[2] = TTF_RenderText_Blended(font, "3ème", white);
    rankSurfaces[3] = TTF_RenderText_Blended(font, "4ème", white);
    rankSurfaces[4] = TTF_RenderText_Blended(font, "5ème", white);
    rankSurfaces[5] = TTF_RenderText_Blended(font, "6ème", white);
    rankSurfaces[6] = TTF_RenderText_Blended(font, "7ème", white);
    rankSurfaces[7] = TTF_RenderText_Blended(font, "8ème", white);

    return rankSurfaces;
}

std::vector<SDL_Surface*> createCharactersSurfaces(TTF_Font* font){

}

std::vector<SDL_Rect> createPositions(){
	std::vector<SDL_Rect> positions;
	for(int i=0; i<8; ++i){
    	SDL_Rect newPosition;
    	positions.push_back(newPosition);
    }
    
    positions[0].x = 300;    positions[0].y = 100;
    positions[1].x = 300;    positions[1].y = 120;
    positions[2].x = 300;    positions[2].y = 140;
    positions[3].x = 300;    positions[3].y = 160;
    positions[4].x = 300;    positions[4].y = 180;
    positions[5].x = 300;    positions[5].y = 200;
    positions[6].x = 300;    positions[6].y = 220;
    positions[7].x = 300;    positions[7].y = 240;

    return positions;
}


void showRankSurfaces(std::vector<SDL_Surface*> surfaces, SDL_Surface* screen, std::vector<SDL_Rect> positions){
	for(int i=0; i<8; ++i){
		SDL_BlitSurface(surfaces[i], NULL, screen, &positions[i]); 
	}
	SDL_Flip(screen); 
}

