#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    // TODO: implement print_board here
    for (auto& row : board) {
	    for (auto val : row) {
	    	cout << val << " ";
	    }
	    cout << endl;
    }
    write_board_csv(board, first);
}

// TODO: use algorithms to spawn properly
void spawn_tile(vector<vector<int>>& board) {
    vector<pair<int, int>> empty_cells;	
    for (int r=0;r<4;r++) {
        for (int c=0;c<4;c++) {
            if (board[r][c]==0) {
		    empty_cells.push_back({r, c});
	    }
	}
    }

    if (empty_cells.empty()) return;

    static std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> dist(0, (int)empty_cells.size() - 1);
    auto [r, c] = empty_cells[dist(rng)];

    std::uniform_int_distribution<int> two_or_four(1, 10);
    board[r][c] = (two_or_four(rng) == 1) ? 4 : 2;
    // TODO: Feed this into chat GPT and have it correct the function for you
    // with proper prompting
}


// TODO: Compress a row, remove zeroes, and then pad with zeroes at the end
std::vector<int> compress_row(const std::vector<int>& row) {
	std::vector<int> compressed;
	std::copy_if(row.begin(), row.end(), back_inserter(compressed), [](int x) {return x != 0;});
	compressed.resize(4, 0);
	return compressed;
}
// TODO: Merge a row (assumes the row is already compressed)
std::vector<int> merge_row(std::vector<int> row) {
	for (size_t i = 0; i < row.size() - 1; i++) {
		if (row[i] != 0 && row[i] == row[i + 1]) {
			row[i] *= 2;
			row[i + 1] = 0;
			i++;
		}
	}
	row = compress_row(row);
	return row;
}
// TODO: use copy_if and iterators
bool move_left(vector<vector<int>>& board){
	bool moved = false;
	for (auto& row : board) {
		std::vector<int> orig = row;
		row = compress_row(row);
		row = merge_row(row);
		if (row != orig) {
			moved = true;
		}
	}
	return moved;
}
// TODO: use reverse iterators
bool move_right(vector<vector<int>>& board){
	bool moved = false;
	for (auto& row : board) {
		std::vector<int> orig = row;
		std::reverse(row.begin(), row.end());
		row = compress_row(row);
		row = merge_row(row);
		std::reverse(row.begin(), row.end());
		if (row != orig) {
			moved = true;
		}
	}
	return moved;
}
// TODO: use column traversal
bool move_up(vector<vector<int>>& board){
	bool moved = false;
	for (int c = 0; c < 4; ++c) {
		std::vector<int> col;
		for (auto& row : board) {
			col.push_back(row[c]);
		}
		auto orig = col;
		col = compress_row(col);
		col = merge_row(col);

		for (int r = 0; r < 4; ++r) {
			board[r][c] = col[r];
		}
		if (col != orig) {
			moved = true;
		}
	}
	return moved;
}
// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board){
	bool moved = false;
	for (int c = 0; c < 4; ++c) {
		std::vector<int> col;
		for (int r = 0; r < 4; ++r) {
			col.push_back(board[r][c]);
		}
		std::vector<int> orig = col;
		std::reverse(col.begin(), col.end());
		col = compress_row(col);
		col = merge_row(col);
		std::reverse(col.begin(), col.end());
		for (int r = 0; r < 4; ++r) {
			board[r][c] = col[r];
		}
		if (col != orig) {
			moved = true;
		}
	}
	return moved;
}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: get the history and print the board and continue
        	if (!history.empty()) {
			board = history.top();
			history.pop();
		}
		continue;
	}

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            	history.push(prev);
		spawn_tile(board);
        }
    }
    return 0;
}


