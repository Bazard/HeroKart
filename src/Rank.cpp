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
void getFinalRanking(std::vector<PlayerIA*>& players, std::vector<SDL_Surface*> textSurfaces, TTF_Font* font){
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

	SDL_Color black = {0, 0, 0};
	//SDL_Surface* texte = NULL;
    SDL_Rect position;

    textSurfaces[1] = TTF_RenderText_Blended(font, "POUET", black);
    //SDL_Surface* ecran = SDL_GetVideoSurface();
    //SDL_Surface* fond = NULL;
    //fond = IMG_Load("../textures/CCTex.jpg");

    SDL_FillRect(textSurfaces[0], NULL, SDL_MapRGB(textSurfaces[0]->format, 255, 255, 255));
    position.x = 0;
    position.y = 0;
    //SDL_BlitSurface(fond, NULL, textSurfaces[0], &position); 
    position.x = 60;
    position.y = 60;
    SDL_BlitSurface(textSurfaces[1], NULL, textSurfaces[0], &position); 
    //SDL_Flip(textSurfaces[0]);
    //SDL_FreeSurface(texte);
    
} 
