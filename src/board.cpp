#include "includes/board.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

Board::Board()
{
	for(int row = 0; row < NUM_ROWS; row++)
	{
		for(int col = 0; col < NUM_COLS; col++)
		{
			boardArray[row][col] = 0;
		}
	}

	player = WHITE;
}

int Board::getSquareValue(int row, int col)
{
	if ((row >= NUM_ROWS && row < 0) || (col >= NUM_COLS && col < 0))
	{
		cout << "Coordenadas inválidas\n";
		return ERROR;
	}

	assert(boardArray[row][col] >= -KING && boardArray[row][col] <= KING);

	return boardArray[row][col];
}

int Board::getPlayer()
{
	assert(player == WHITE || player == BLACK);
	return player;
}

vector<pos> Board::getPieceVector(int piece, int color)
{
	assert(piece >= -KING && piece <= KING);
	assert(color == BLACK || color == WHITE);
	if (color == BLACK)
	{
		return blackPiecesPos[piece - 1];
	}
	else
	{
		return whitePiecesPos[piece - 1];
	}

}

int Board::setPlayer(int newPlayer)
{
	if (newPlayer != WHITE && newPlayer != BLACK)
	{
		cout << "Valor " << newPlayer << " inválido\n";
		return ERROR;
	}

	player = newPlayer;
	return 0;
}

void Board::populate()
{
	for(int row = 0; row < NUM_ROWS; row++)
	{
		for(int col = 0; col < NUM_COLS; col++)
		{
			putPiece(START_ARRAY[row][col], row, col);
		}
	}
}

int Board::putPiece(int piece, int row, int col)
{
	if ((row >= NUM_ROWS || row < 0) || (col >= NUM_COLS || col < 0))
	{
		cout << "Coordenadas("<< row << ", " << col <<  ") inválidas\n";
		return ERROR;
	}

	if (boardArray[row][col] != 0)
	{
		cout << "Posição ("<< row << ", " << col <<  ") ocupada\n";
		return ERROR;
	}

	if (piece < -KING || piece > KING)
	{
		cout << "Peça " << piece << " inválida\n";
		return ERROR;
	}

	boardArray[row][col] = piece;
	if (piece > 0)
	{
		whitePiecesPos[piece - 1].push_back(make_pair(row, col));
	}
	else if (piece == 0)
	{
		return 0;
	}
	else
	{
		piece *= -1;
		blackPiecesPos[piece - 1].push_back(make_pair(row, col));
	}

	return 0;
}

int Board::removePiece(int row, int col)
{
	int piece;

	if ((row >= NUM_ROWS || row < 0) || (col >= NUM_COLS || col < 0))
	{
		cout << "Coordenadas("<< row << ", " << col <<  ") inválidas\n";
		return ERROR;
	}

	if (boardArray[row][col] == 0)
	{
		cout << "Posição ("<< row << ", " << col <<  ") desocupada\n";
		return ERROR;
	}

	piece = boardArray[row][col];

	assert(piece >= -KING && piece <= KING);

	if (piece > 0)
	{
		whitePiecesPos[piece - 1].erase(remove(whitePiecesPos[piece - 1].begin(), whitePiecesPos[piece - 1].end(), make_pair(row, col)), whitePiecesPos[piece - 1].end());
	}
	else
	{
		piece *= -1;
		blackPiecesPos[piece - 1].erase(remove(blackPiecesPos[piece - 1].begin(), blackPiecesPos[piece - 1].end(), make_pair(row, col)), blackPiecesPos[piece - 1].end());

	}

	boardArray[row][col] = 0;


	return piece;
}

bool Board::isValid()
{
	if (getPieceVector(KING, WHITE).size() != 1 || getPieceVector(KING, BLACK).size() != 1)
	{
		return false;
	}

	return true;
}