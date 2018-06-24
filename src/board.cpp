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


void Board::clear()
{
	for(int row = 0; row < NUM_ROWS; row++)
	{
		for(int col = 0; col < NUM_COLS; col++)
		{
			if(getSquareValue(row, col) != 0)
			{
				removePiece(row, col);
			}
		}
	}
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

vector<int> Board::getPieceVector(int piece, int color)
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
		whitePiecesPos[piece - 1].push_back(row * NUM_COLS + col);
	}
	else if (piece == 0)
	{
		return 0;
	}
	else
	{
		piece *= -1;
		blackPiecesPos[piece - 1].push_back(row * NUM_COLS + col);
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
		whitePiecesPos[piece - 1].erase(remove(whitePiecesPos[piece - 1].begin(), whitePiecesPos[piece - 1].end(), row * NUM_ROWS + col), whitePiecesPos[piece - 1].end());
	}
	else
	{
		piece *= -1;
		blackPiecesPos[piece - 1].erase(remove(blackPiecesPos[piece - 1].begin(), blackPiecesPos[piece - 1].end(), row * NUM_ROWS + col), blackPiecesPos[piece - 1].end());

	}

	boardArray[row][col] = 0;


	return piece;
}	

bool Board::isValid()
{
	int whiteKingPos, blackKingPos;

	if (getPieceVector(KING, WHITE).size() != 1 || getPieceVector(KING, BLACK).size() != 1)
	{
		return false;
	}

	whiteKingPos = getPieceVector(KING, WHITE)[0];
	blackKingPos = getPieceVector(KING, BLACK)[0];

	if (blackKingPos == whiteKingPos + UP or blackKingPos == whiteKingPos + DOWN or (whiteKingPos % 8 != 0 and blackKingPos == whiteKingPos + LEFT or blackKingPos == whiteKingPos + UP_LEFT or blackKingPos == whiteKingPos + DOWN_LEFT) or (whiteKingPos % 8 == 7 and blackKingPos == whiteKingPos + RIGHT or blackKingPos == whiteKingPos + UP_RIGHT or blackKingPos == whiteKingPos + DOWN_RIGHT))
	{
		return false;
	}

	return true;
}

bool Board::isMoveLegal(int fromRow, int fromCol, int toRow, int toCol)
{
	vector<int> legalMoves;
	int toPos = toRow * NUM_ROWS + toCol;

	if ((fromRow >= NUM_ROWS || fromRow < 0) || (fromCol >= NUM_COLS || fromCol < 0))
	{
		cout << "Coordenadas de partida("<< fromRow << ", " << fromCol <<  ") inválidas\n";
		return false;
	}

	if ((toRow >= NUM_ROWS || toRow < 0) || (toCol >= NUM_COLS || toCol < 0))
	{
		cout << "Coordenadas de chegada("<< toRow << ", " << toCol <<  ") inválidas\n";
		return false;
	}

	if (boardArray[fromRow][fromCol] == 0)
	{
		cout << "Posição ("<< fromRow << ", " << fromCol <<  ") desocupada\n";
		return false;
	}

	assert(boardArray[fromRow][fromCol] >= -KING and boardArray[fromRow][fromCol] <= KING);

	legalMoves = getMovesVector(fromRow, fromCol);

	if(find(legalMoves.begin(), legalMoves.end(), toPos) != legalMoves.end())
	{
    	return true;
	} 
	else
	{
	    return false;
	}
}

vector<int> Board::getMovesVector(int row, int col)
{
	int piece, color;

	piece = getSquareValue(row, col);

	if (piece < 0)
	{
		piece *= -1;
		color = BLACK;
	}
	else
	{
		color = WHITE;
	}

	if ((row >= NUM_ROWS || row < 0) || (col >= NUM_COLS || col < 0))
	{
		cout << "Coordenadas("<< row << ", " << col <<  ") inválidas\n";
		return {};
	}

	switch(piece)
	{
		case PAWN:
			return getPawnMoves(color, row, col);
		case ROOK:
			return getRookMoves(color, row, col);
		case KNIGHT:
			return getKnightMoves(color, row, col);
		case BISHOP:
			return getBishopMoves(color, row, col);
		case QUEEN:
			return getQueenMoves(color, row, col);
		case KING:
			return getKingMoves(color, row, col);
		default:
			cout << "Peça inválida\n";
			return {};
	}
}

vector<int> Board::getPawnMoves(int color, int row, int col)
{
	vector<int> destinations;
	int origin = row * NUM_ROWS + col;

	if (color == WHITE)
	{
		if (row == 1)
		{
			destinations.push_back(origin + UP + UP);
		}

		if (row != 7 and getSquareValue(row + 1, col) == 0)
		{
			destinations.push_back(origin + UP);
		}

		if (row != 7 and col != 0 and getSquareValue(row + 1, col - 1) < 0)
		{
			if (getSquareValue(row + 1, col - 1) == -KING)
			{
				blackCheckList.push_back(origin);
			}
			else
			{
				destinations.push_back(origin + UP_LEFT);
			}
		}

		if (row != 7 and col != 7 and getSquareValue(row + 1, col + 1) < 0)
		{
			if (getSquareValue(row + 1, col + 1) == -KING)
			{
				blackCheckList.push_back(origin);
			}
			else
			{
				destinations.push_back(origin + UP_RIGHT);
			}
		}
	}

	else
	{
		if (row == 6)
		{
			destinations.push_back(origin + DOWN + DOWN);
		}

		if (row != 0 and getSquareValue(row - 1, col) == 0)
		{
			destinations.push_back(origin + DOWN);
		}

		if (row != 0 and col != 0 and getSquareValue(row - 1, col - 1) > 0)
		{
			if (getSquareValue(row - 1, col - 1) == KING)
			{
				whiteCheckList.push_back(origin);
			}
			else
			{
				destinations.push_back(origin + DOWN_LEFT);
			}
		}

		if (row != 0 and col != 7 and getSquareValue(row - 1, col + 1) > 0)
		{
			if (getSquareValue(row - 1, col + 1) == KING)
			{
				whiteCheckList.push_back(origin);
			}
			else
			{
				destinations.push_back(origin + DOWN_RIGHT);
			}
		}
	}

	return destinations;
}

vector<int> Board::getRookMoves(int color, int row, int col)
{
	vector<int> destinations;
	int origin = row * NUM_ROWS + col;

	for (int spaces = 1; row + spaces < NUM_ROWS; spaces++)
	{
		if (boardArray[row + spaces][col] == 0)
		{
			destinations.push_back(origin + spaces * UP);
		}

		else if (boardArray[row + spaces][col] * color > 0)
		{
			break;
		}

		else
		{
			if (boardArray[row + spaces][col] == KING or boardArray[row + spaces][col] == -KING)
			{
				if (color = WHITE)
				{
					blackCheckList.push_back(origin);
				}
				else
				{
					whiteCheckList.push_back(origin);
				}
			}
			else
			{
				destinations.push_back(origin + spaces * UP);
				break;
			}
		}
	}

	for (int spaces = 1; row - spaces >= 0; spaces++)
	{
		if (boardArray[row - spaces][col] == 0)
		{
			destinations.push_back(origin + spaces * DOWN);
		}

		else if (boardArray[row - spaces][col] * color > 0)
		{
			break;
		}

		else
		{
			if (boardArray[row - spaces][col] == KING or boardArray[row - spaces][col] == -KING)
			{
				if (color = WHITE)
				{
					blackCheckList.push_back(origin);
				}
				else
				{
					whiteCheckList.push_back(origin);
				}
			}
			else
			{
				destinations.push_back(origin + spaces * DOWN);
				break;
			}
		}
	}

	for (int spaces = 1; col + spaces < NUM_COLS; spaces++)
	{
		if (boardArray[row][col + spaces] == 0)
		{
			destinations.push_back(origin + spaces * RIGHT);
		}

		else if (boardArray[row][col + spaces] * color > 0)
		{
			break;
		}

		else
		{
			if (boardArray[row][col + spaces] == KING or boardArray[row][col + spaces] == -KING)
			{
				if (color = WHITE)
				{
					blackCheckList.push_back(origin);
				}
				else
				{
					whiteCheckList.push_back(origin);
				}
			}
			else
			{
				destinations.push_back(origin + spaces * RIGHT);
				break;
			}
		}
	}

	for (int spaces = 1; col - spaces >= 0; spaces++)
	{
		if (boardArray[row][col - spaces] == 0)
		{
			destinations.push_back(origin + spaces * LEFT);
		}

		else if (boardArray[row][col - spaces] * color > 0)
		{
			break;
		}

		else
		{
			if (boardArray[row][col - spaces] == KING or boardArray[row][col - spaces] == -KING)
			{
				if (color = WHITE)
				{
					blackCheckList.push_back(origin);
				}
				else
				{
					whiteCheckList.push_back(origin);
				}
			}
			else
			{
				destinations.push_back(origin + spaces * LEFT);
				break;
			}
		}
	}

	return destinations;
}

vector<int> Board::getKnightMoves(int color, int row, int col)
{
	return {};
}

vector<int> Board::getBishopMoves(int color, int row, int col)
{
	return {};
}

vector<int> Board::getQueenMoves(int color, int row, int col)
{
	return {};
}

vector<int> Board::getKingMoves(int color, int row, int col)
{
	return {};
}