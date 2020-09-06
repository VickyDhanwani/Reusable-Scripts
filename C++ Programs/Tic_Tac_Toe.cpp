/*

Given an array moves where each element is another array of size 2 corresponding to the row and column of the grid where 
they mark their respective character in the order in which A and B play.

Return the winner of the game if it exists (A or B), in case the game ends in a draw return "Draw", 
if there are still movements to play return "Pending".

You can assume that moves is valid (It follows the rules of Tic-Tac-Toe), the grid is initially empty and A will play first.

*/

class Solution {
public:
    string tictactoe(vector<vector<int>>& moves) {
        char board[3][3];
        int i, j;
        char player = 'A';
        for(i = 0; i < 3; i++) {
            for(j = 0; j < 3; j++) {
                board[i][j] = '-';
            }
        }
        int n = moves.size();
        int valid = 1;
        for(i = 0; i < n; i++) {
            if(player == 'A') {
                board[moves[i][0]][moves[i][1]] = 'X';
                player = 'B';
            }
            else if(player == 'B') {
                board[moves[i][0]][moves[i][1]] = 'O';
                player = 'A';
            }
        }
        if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == 'X') {
            return "A";   
        }
        if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == 'O') {
            return "B";
        }
        if(board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[0][2] == 'X') {
         return "A";   
        }
        if(board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[0][2] == 'O') {
            return "B";
        }
        for(i = 0; i < 3; i++) {
            for(j = 0; j < 3; j++) {
                cout<<board[i][j]<<" ";
            }
            cout<<endl;
            if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][2] == 'X') {
                return "A";
            }
            if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][2] == 'O') {
                return "B";
            }
            
            if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] == 'X') {
                return "A";
            }
            if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] == 'O') {
                return "B";
            }
                
        }
        
        
        for(i = 0; i < 3; i++) {
            for(j = 0; j < 3; j++) {
                if(board[i][j] == '-') {
                    valid = 0;
                    break;
                }
            }
        }
        if(valid == 1) {
            return "Draw";
        }
        return "Pending";
    }   
   
};
