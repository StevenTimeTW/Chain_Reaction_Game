#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "rules.h"
#include "algorithm.h"
#include "player.h"

using namespace std;

# define Row_Num 5
# define Col_Num 6

bool Would_Lose_The_Game(Board board,Player *Opponent,int Opponent_Color){
    for(int i=0;i<Row_Num;i++){
        for(int j=0;j<Col_Num;j++){
            Board Newboard = board;
            Newboard.place_orb(i, j, Opponent);
            if(board.win_the_game(*Opponent)) return true;
        }
    }
    return false;
}

void Random_From_Avalible_Points(int *Index_Temp_X,int *Index_Temp_Y,int Avalible_Counts,int *index){
    
    int Random_Num = rand()%Avalible_Counts;
    index[0] = Index_Temp_X[Random_Num];
    index[1] = Index_Temp_Y[Random_Num];
}

void Random(Board board,int Player_Color,int *index){
    int row,col;
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == Player_Color || board.get_cell_color(row, col) == 'w') break;
    }
    index[0] = row;
    index[1] = col;
}

void ChooseBestOrb(Board board,Player *player,int Player_Color,int Opponent_Color,int *index){
    // Use index_temp to load avalible orbs, which would not give the opponent chance to win directly.
    int Index_Temp_X[30]{1000},Index_Temp_Y[30]{1000};
    
    // Count the number of avalible orbs as explained above.
    int Avalible_Counts = 0;
    
    Player *Opponent = new Player(Opponent_Color);
    
    // Use Stop to end the if/else.
    bool stop =false;
    
    // Travesal all the index
    for (int i=0;i<Row_Num;i++){
        for (int j=0;j<Col_Num;j++){
            
            // Find avaliable orbs.
            if(board.get_cell_color(i,j)==Player_Color || board.get_cell_color(i,j)=='w'){
                
                // Try to put the orb on the index to see the followings :
                
                Board NewBoard = board;
                NewBoard.place_orb(i, j, player);
                
                // 1. Check whether we could directly win the game.
                //    If so, return the index.
                if (NewBoard.win_the_game(*player)){
                    index[0] = i;
                    index[1] = j;
                    stop = true;
                    break;
                }
                
                // 2. Check after our placement, whether the opponent could possibly end the game.
                //    If not, we load the index.
                
                if (!Would_Lose_The_Game(NewBoard,Opponent,Opponent_Color)){
                    Index_Temp_X[Avalible_Counts] = i;
                    Index_Temp_Y[Avalible_Counts] = j;
                    Avalible_Counts++;
                    }
            }
        }
        if (stop) break;
    }
    
    
    // If we could not directly win the game, we find avalible orbs that would not possibly lose the game.
    if(!stop&&Avalible_Counts>0){
        Random_From_Avalible_Points(Index_Temp_X,Index_Temp_Y,Avalible_Counts,index);
    }
    
    // Lastly, we randomly choose the avalible orbs, which might lose the game.
    if(!stop){
        Random(board,Player_Color,index);
    }
    
}

// Main function
void algorithm_A(Board board, Player player, int index[]){
    
    // Use Colors to represent Player and Opponent
    int Player_Color = player.get_color();
    int Opponent_Color ='v';
    for ( int i=0;i<Row_Num;i++){
        for ( int j=0;j<Col_Num;j++){
            if(board.get_cell_color(i,j)!=Player_Color || board.get_cell_color(i,j)!='w'){
                Opponent_Color = board.get_cell_color(i,j);
                break;
                
            }
        }
    }
    // Use void function to output index.
    ChooseBestOrb(board,&player,Player_Color,Opponent_Color,index);
    
}
