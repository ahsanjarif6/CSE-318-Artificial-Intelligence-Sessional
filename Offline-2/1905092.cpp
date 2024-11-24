#include<bits/stdc++.h>
#define INF 100000000
#define TOTAL_BINS 14
#define MANCALA0 6
#define MANCALA1 13
#define INITIAL_STONES 4
#define W1 10
#define W2 15
#define W3 20
#define W4 25
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

class Mancala{
public:
    vector< int > bins;
    int turn;
    bool gameOver;
    int additionalMove;
    int capture;

    Mancala(){
        bins = vector< int > (TOTAL_BINS , INITIAL_STONES);
        bins[MANCALA0] = bins[MANCALA1] = turn = additionalMove = capture = 0;
        gameOver = false;
    }

    int heuristic1(){
        return bins[MANCALA1]-bins[MANCALA0];
    }

    int heuristic2(){
        int sum = 0;
        for(int i = MANCALA0 + 1 ; i < MANCALA1 ; i ++ ){
            sum += bins[i];
        }
        for(int i = 0 ; i < MANCALA0 ; i ++ ){
            sum -= bins[i];
        }
        return W1*heuristic1() + W2*sum;
    }

    int heuristic3(int addMoves){
        return heuristic2()+W3*addMoves;
    }

    int heuristic4(int addMoves , int capt){
        return heuristic3(addMoves) + W4*capt;
    }

    int evalHeuristic(int heuristicNo , int turn , int moves ,int capt){
        switch(heuristicNo){
            case 1:
                return heuristic1();
            case 2:
                return heuristic2();
            case 3:
                return heuristic3(moves);
            case 4:
                return heuristic4(moves , capt);
        }
        return heuristic1();
    }


    void printGame(){
        for(int i = 1 ; i <= 6 ; i ++ ) cout << "\t" << i;
        cout << '\t' << '\n';
        for(int i = 1 ; i <= 7 ; i ++ ) cout << "---\t";
        cout << "\n\t";
        for(int i = MANCALA1 - 1 ; i > MANCALA0 ; i -- ){
            cout << bins[i] << "\t";
        }
        cout<<"\t"<<"-->Player2\n";
        cout << bins[MANCALA1];
        for(int i = 1 ; i <= 7 ; i ++ ) cout << "\t";
        cout << bins[MANCALA0] << '\n' << '\t';
        for(int i = 0 ; i < MANCALA0 ; i ++ ){
            cout << bins[i] << '\t';
        }
        cout<<"\t"<<"-->Player1\n";
        for(int i = 1 ; i <= 7 ; i ++ ) cout << "---\t";
        cout << '\n';
        for(int i = 6 ; i >= 1 ; i -- ) cout << "\t" << i ;
        cout << '\t' << '\n' << '\n';
    }

    void distribute(int avoid , int input  , int& val){
        bins[input] = 0;
        for(int i = 1 ; i <= val ; i ++ ){
            int j = (input + i) % TOTAL_BINS;
            if(j == avoid){
                val ++ ;
            }
            else{
                bins[j] ++ ;
            }
        }
    }

    void chooseBin(int input){
        int val = bins[input];
        if(!val){
            cout << "no stones in bin " << input << " , discarding input\n";
            return;
        }   
        if(!turn){
            distribute(MANCALA1 , input , val);
            int index = (input + val) % TOTAL_BINS;
            if(bins[index] == 1){
                if(index >= 0 && index <= 5){
                    if(bins[12 - index] > 0){
                        capture = bins[index] + bins[12 - index];
                        bins[MANCALA0] += capture;
                        bins[index] = bins[12 - index] = 0;
                    }
                    else{
                        capture = 0;
                    }
                }
            }

            if(index != MANCALA0){
                turn ^= 1;
                additionalMove = 0;
            }
            else additionalMove ++ ;
        }
        else{
            distribute(MANCALA0 , input , val);
            int index = (input + val) % TOTAL_BINS;
            if(bins[index] == 1){
                if(index >= 7 && index <= 12){
                    if(bins[12 - index] > 0){
                        capture = bins[index] + bins[12 - index];
                        bins[MANCALA1] += capture;
                        bins[index] = bins[12 - index] = 0;
                    }
                    else{
                        capture = 0;
                    }
                }
            }

            if(index != MANCALA1){
                turn ^= 1;
                additionalMove = 0;
            }
            else additionalMove ++ ;
        }
    }

    bool rowEmpty(){
        int sum0 = 0;
        int sum1 = 0;
        for(int i = 0 ; i < 6 ; i ++ ) sum0 += bins[i];
        for(int i = 7 ; i < 13 ; i ++ ) sum1 += bins[i];
        if(sum0 == 0){
            bins[13] += sum1;
            for(int i = 7 ; i < 13 ; i ++ ) bins[i] = 0;
            return true;
        }
        else if(sum1 == 0){
            bins[6] += sum0;
            for(int i = 0 ; i < 6 ; i ++ ) bins[i] = 0 ;
            return true;
        }
        return false;
    }

    int getWinner(){
        cout << "Player1 : " << bins[MANCALA0] << '\n';
        cout << "Player2 : " << bins[MANCALA1] << '\n';
        if(bins[MANCALA0] > bins[MANCALA1]){
            cout << "Player 1 is winner\n";
            return 1;
        }
        else if(bins[MANCALA0] < bins[MANCALA1]){
            cout << "Player 2 is winner\n";
            return 2;
        }
        else{
            cout << "Draw\n";
            return 3;
        }
    }
    
    pair<int , int> minimaxAlgo(int depth , int turn , int alpha , int beta , int addMoves , int capt , int heuristicNo){
        if(!depth){
            return make_pair(evalHeuristic(heuristicNo , turn , addMoves , capt) , 1);
        }
        if(gameOver){
            return make_pair(evalHeuristic(heuristicNo , turn , addMoves , capt) , 1);
        }
        int maxEva , minEva ,index = -1;
        vector< int > temp;
        if(turn){
            maxEva = -INF;
            for(int i = MANCALA0 + 1 ; i < MANCALA1 ; i ++ ) temp.push_back(i);
            shuffle(temp.begin(),temp.end(),rng);
            for(int i = 0 ; i < temp.size() ; i ++ ){
                int j = temp[i];
                if(!bins[j]) continue;
                auto b = bins;
                int bTurn = this->turn;
                bool bGameOver = this->gameOver;
                int bAdditionalMove = this->additionalMove;
                int bCapture = this->capture;
                chooseBin(j);
                int val = minimaxAlgo(depth - 1 , this->turn , alpha , beta , this->additionalMove , this->capture , heuristicNo).first;
                if(val >= maxEva){
                    maxEva = val;
                    index = j;
                }
                bins = b;
                this->turn = bTurn;
                this->gameOver = bGameOver;
                this->additionalMove = bAdditionalMove;
                this->capture = bCapture;
                alpha = max(alpha , maxEva);
                if(beta <= alpha){
                    break;
                }
            }
            return make_pair(maxEva , index);
        }
        else{
            minEva = INF;
            for(int i = 0 ; i < MANCALA0 ; i ++ ) temp.push_back(i);
            shuffle(temp.begin() , temp.end() , rng);
            for(int i = 0 ; i < temp.size() ; i ++ ){
                int j = temp[i];
                if(!bins[j]){
                    continue;
                }
                auto b = bins;
                int bTurn = this->turn;
                bool bGameOver = this->gameOver;
                int bAdditionalMove = this->additionalMove;
                int bCapture = this->capture;
                chooseBin(j);
                int val = minimaxAlgo(depth - 1 , this->turn , alpha , beta , this->additionalMove , this->capture , heuristicNo).first;
                if(val <= minEva){
                    minEva = val;
                    index = j;
                }
                bins = b;
                this->turn = bTurn;
                this->gameOver = bGameOver;
                this->additionalMove = bAdditionalMove;
                this->capture = bCapture;
                beta = min(beta , minEva);
                if(beta <= alpha){
                    break;
                }
            }
            return make_pair(minEva , index);
        }
    }
};

int gamePlay(int choice , int heu1 , int heu2 , int depth1 , int depth2){
    Mancala* mancala = new Mancala();
    mancala->printGame();
    int input;
    while(!mancala->gameOver){
        cout << "Turn : " << mancala->turn + 1 << '\n';
        if(!mancala->turn){
            if(choice == 1){
                int heu = heu1;
                int index = mancala->minimaxAlgo(depth1 , mancala->turn , -INF , INF , 0 , 0 , heu).second;
                cout << "Bin : " << 7 - (index + 1) << '\n';
                mancala->chooseBin(index);
                mancala->printGame();
                mancala->gameOver = mancala->rowEmpty();
            }
            else{
                cin >> input;
                cout << "Bin : " << input << '\n';
                input = 7 - input;
                mancala->chooseBin(input - 1);
                mancala->printGame();
                mancala->gameOver = mancala->rowEmpty();
            }
        }
        else{
            int heu = heu2;
            int index = mancala->minimaxAlgo(depth2 , mancala->turn , -INF , INF , 0 , 0 , heu).second;
            cout << "Bin : " << 13 - index << '\n';
            mancala->chooseBin(index);
            mancala->printGame();
            mancala->gameOver = mancala->rowEmpty();
        }
    }
    mancala->getWinner();
    mancala->printGame();
    cout << "GAME OVER\n";
    return 0;
}

int main(){
    int heu1 = 1;
    int heu2 = 4;
    int depth1 = 5;
    int depth2 = 6;
    int choice;
    cin >> choice;
    gamePlay(choice , heu1 , heu2 , depth1 , depth2);
    return 0;
}
