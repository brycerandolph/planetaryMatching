/*********************************************************************
* Assignment: Project 3 - GUI Move and Match Game                    *
* Author:                                                            *
* Date: Fall 2017                                                    *
* File: CGame.cpp                                                    *
*                                                                    *
* Description: This file contains the                                *
*********************************************************************/

#include "CGame.h"
#include <time.h>
#include <fstream>

using namespace std;

static CBitmap images [NUMIMAGES];
static CBitmap masks [2];
static int topScores [5] = {0, 0, 0, 0, 0};
CGame * savedGame = NULL;

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

CGame::CGameSquare::CGameSquare ()
{
	what = BORDER;
	whichSymbol = 0;
	matched = false;
	value = 0;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::CGameSquare::Fill ()
{
	int random = rand() % 7 + 1;
	whichSymbol = random;
	what = SYMBOL;

}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::CGameSquare::Display (CDC * deviceContextP)
{
	if (what == EMPTY)
		return;
	deviceContextP->Rectangle (where);
	CDC memDC;
	memDC.CreateCompatibleDC(deviceContextP);
	memDC.SelectObject(&images[whichSymbol]);
	int res = deviceContextP->TransparentBlt (where.left+1, where.top+1, where.Width()-2,where.Height()-2,
					&memDC, 0, 0, 100, 100, SRCCOPY); 
	DeleteDC (memDC);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will swap all values in a square        *
*	except for the where rectangles.                                 *
*********************************************************************/

void CGame::CGameSquare::Swap (CGame::CGameSquare & other, CFrameWnd * windowP)
{
	CGameSquare temp = *this;

	what = other.what;
	other.what = temp.what;

	windowP->InvalidateRect (where);
	windowP->InvalidateRect (other.where);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::CGameSquare::SetWhere (CRect whereRect)
{
	where = whereRect;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

CGame::CGame ()
{
	srand (unsigned (time (NULL)));
	numRows = numCols = 1;
	grid = new CGameSquare * [numRows+2];
	for (int r = 0; r < numRows+2; r++)
		grid[r] = new CGameSquare [numCols+2];
	savedGame = new CGame (*this);
	Init (DEFAULT_ROWS, DEFAULT_COLS);

	int res = bgImage.LoadBitmap(CString("BACKGROUND_BMP"));
	res = images[0].LoadBitmap(CString("ME_BMP"));//0
	res = images[1].LoadBitmap(CString("VENUS_BMP"));//1
	res = images[2].LoadBitmap(CString("CHEESE_BMP"));//2
	res = images[3].LoadBitmap(CString("NEPTUNE_BMP"));//3
	res = images[4].LoadBitmap(CString("MARS_BMP"));//4
	res = images[5].LoadBitmap(CString("EARTH_BMP"));//5
	res = images[6].LoadBitmap(CString("MOON_BMP"));//6
	res = images[7].LoadBitmap(CString("PINKY_BMP"));//7
	srand (unsigned (time (NULL)));
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

CGame::~CGame ()
{
	for (int r = 0; r < numRows + 2; r++)
		delete[] grid[r];
	delete[] grid;
	if (this != savedGame  && savedGame != NULL)
		delete savedGame;
	savedGame = NULL;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

CGame::CGame (const CGame & other)
{
	numRows = other.numRows;
	numCols = other.numCols;
	myRow = other.myRow;
	myCol = other.myCol;
	movesLeft = other.movesLeft;
	score = other.score;

	grid = new CGameSquare *[numRows + 2];
	for (int r = 0; r <= numRows + 1; r++)
	{
		grid[r] = new CGameSquare[numCols + 2];

		for (int c = 0; c <= numCols + 1; c++)
			grid[r][c] = other.grid[r][c];
	}

}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

CGame & CGame::operator = (const CGame & other)
{
	if (this == &other)
		return *this;

	for (int r = 0; r < numRows + 2; r++)
		delete[] grid[r];
	delete[] grid;

	numRows = other.numRows;
	numCols = other.numCols;
	myRow = other.myRow;
	myCol = other.myCol;
	movesLeft = other.movesLeft;
	score = other.score;

	grid = new CGameSquare *[numRows + 2];
	for (int r = 0; r <= numRows + 1; r++)
	{
		grid[r] = new CGameSquare[numCols + 2];

		for (int c = 0; c <= numCols + 1; c++)
			grid[r][c] = other.grid[r][c];
	}
	
	//if (this == &other)
		//return *this;
	//return *this;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::Init (int R, int C)
{
	for (int r = 0; r < numRows + 2; r++)
		delete[] grid[r];
	delete[] grid;
	numRows = R;
	numCols = C;
	grid = new CGameSquare *[numRows + 2];
	for (int r = 0; r < numRows + 2; r++)
		grid[r] = new CGameSquare[numCols + 2];
	FillIn  ();
	PlaceMe ();

	state = 0;
	score = 0;
	movesLeft = DEFAULT_MOVES;
	elapsedTime = 0;
	finished = 0;
	thisRect = CRect (0,0,0,0);
	*savedGame = *this;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::Instructions (CFrameWnd * windowP)
{
	CString message = "\t\t     Welcome to Milky Way Matching\n\n";
	message += "The goal of this game is to accumulate points by creating tile matches. A match is defined as 3 or more adjacent tiles of the same planet in a row or column.\n\nUse 'w', 'a', 's', and 'd' keys to move the Rocket Ship to create matches and collect points.\n\nWhen the Rocket Ship moves, it swaps its space with that of an adjacent tile. Once you run out of moves, the game is over.\n\nScoring: Each tile in a match is worth 1 point.\n\nHow many points can you accumulate in 100 moves? \n";
	CString title = "Instructions";
	windowP->MessageBox (message, title);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::Display (CFrameWnd * windowP)
{
	CPaintDC dc (windowP);
	CRect rect;
	windowP->GetClientRect (&rect);
	if (rect != thisRect)
		SetUp (rect);

	CDC memDC;
	int res = memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bgImage);
	dc.TransparentBlt (0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 1800, 880, SRCCOPY); 
	DeleteDC (memDC);
	
	dc.SetBkMode(TRANSPARENT);	
	CFont font;
	CString fontName = "Verdana";
	font.CreatePointFont(rect.Width() / 5, fontName, &dc);
	CFont* def_font = dc.SelectObject(&font);
	COLORREF def_color = dc.SetTextColor (RGB (255, 255, 255));	
	GameDisplay (&dc);
	DataDisplay (&dc);
	dc.SetTextColor (RGB (0, 0, 0));
	ButtonDisplay (&dc);
	dc.SelectObject(def_font);
	dc.SetTextColor(def_color);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::SetUp (CRect rect)
{
	thisRect = rect;

	int sixthW = rect.Width() / 6;
	int fourthH = rect.Height() / 4;
	int twelfthW = rect.Width() / 12;
	int twelfthH = rect.Height() / 12;

	gameRect = CRect (sixthW, fourthH, rect.Width()-sixthW, rect.Height()-twelfthH);
	
	dataRect = CRect (0, 5 * twelfthH, sixthW, rect.Height()/2);
	scoreRect = CRect (dataRect.left, dataRect.bottom, dataRect.right, dataRect.bottom + twelfthH);
	
	movesRect = CRect(dataRect.left, gameRect.top, dataRect.right, scoreRect.top);

	titleRect = CRect(sixthW,twelfthH, rect.Width()-sixthW, gameRect.top);
	
	newButton = CRect (dataRect.left+twelfthW/3, 8 * twelfthH, dataRect.right-twelfthW/3, 9 * twelfthH);
	restartButton = CRect(dataRect.left + twelfthW / 3, 10 * twelfthH, dataRect.right - twelfthW / 3, 11 * twelfthH);

	int sqHeight = gameRect.Height() / numRows;
	int sqWidth = gameRect.Width() / numCols;
	int top = gameRect.top + (gameRect.Height() - sqHeight * numRows) / 2;
	int left = gameRect.left + (gameRect.Width() - sqWidth * numCols) / 2;
	for (int r = 1; r <= numRows; r++)
		for (int c = 1; c <= numCols; c++)
			grid[r][c].SetWhere (CRect(left + (c-1)*sqWidth, top + (r-1)*sqHeight, left + (c)*sqWidth, top + (r)*sqHeight));
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::GameDisplay (CDC * deviceContextP)
{
	deviceContextP->Rectangle(gameRect);
	for (int r = 1; r <= numRows; r++)
		for (int c = 1; c <= numCols; c++)
			grid [r][c].Display (deviceContextP);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::DataDisplay (CDC * deviceContextP)
{
	deviceContextP->DrawText (CString ("Your Score:"), dataRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	char buffer [50];
	sprintf_s (buffer, "%d", score);
	deviceContextP->DrawText(CString (buffer), scoreRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	deviceContextP->DrawText(CString("Moves Left:"), movesRect, DT_CENTER);
	char buffer2[50];
	sprintf_s(buffer2, "%d", movesLeft);
	deviceContextP->DrawText(CString(buffer2), movesRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);


	deviceContextP->DrawText(CString("Milky Way Matching"), titleRect, DT_CENTER);

}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::ButtonDisplay (CDC * deviceContextP)
{
	CPoint corner = CPoint (newButton.Width() / 5, newButton.Height() / 5);
	deviceContextP->RoundRect (newButton, corner);
	deviceContextP->DrawText(CString ("New"), newButton, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	deviceContextP->RoundRect(restartButton, corner);
	deviceContextP->DrawText(CString("Restart"), restartButton, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	//deviceContextP->RoundRect(restartButton, corner);
	//deviceContextP->DrawText(CString("Milky Way Matching"), titleRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::Click (int y, int x, CFrameWnd * windowP)
{
	if (newButton.PtInRect(CPoint (x, y)))
	{
		Init (numRows, numCols);
		windowP->SetTimer (1, 1000, NULL);
		windowP->Invalidate(true);
	}
	if (restartButton.PtInRect(CPoint(x, y)))
	{
		
		state = 0;
		score = 0;
		movesLeft = DEFAULT_MOVES;
		elapsedTime = 0;
		finished = 0;
		thisRect = CRect(0, 0, 0, 0);
		*this = *savedGame;
		
		//windowP->InvalidateRect(gameRect);
		windowP->SetTimer(1, 1000, NULL);
		windowP->Invalidate(true);

	}
}
/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/
/*void CGame::Restart()
{
	*this = *savedGame;
}
*/
/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::Move (char direction, CFrameWnd * windowP)
{
	if (finished)
		return;
	int row = myRow;
	int col = myCol;
	switch (direction)
	{
		case 'a': // moving left
			if (col != 1)
			{
				swap(grid[row][col].whichSymbol, grid[row][col-1].whichSymbol);
				swap(grid[row][col].what, grid[row][col-1].what);
				col--;
				windowP->InvalidateRect(gameRect);
				windowP->InvalidateRect(movesRect);
			}
			
			break;
			
		case 'w': // moving up
			if (row != 1)
			{
				swap(grid[row][col].whichSymbol, grid[row-1][col].whichSymbol);
				swap(grid[row][col].what, grid[row-1][col].what);
				row--;
				windowP->InvalidateRect(gameRect);
				windowP->InvalidateRect(movesRect);
			}

			break;

		case 'd': // moving right
			if (col != numCols)
			{
				swap(grid[row][col].whichSymbol, grid[row][col+1].whichSymbol);
				swap(grid[row][col].what, grid[row][col+1].what);
				col++;
				windowP->InvalidateRect(gameRect);
				windowP->InvalidateRect(movesRect);
			}

			break;

		case 's': // moving down
			if (row != numRows)
			{
				swap(grid[row][col].whichSymbol, grid[row+1][col].whichSymbol);
				swap(grid[row][col].what, grid[row+1][col].what);
				windowP->InvalidateRect(gameRect);
				windowP->InvalidateRect(movesRect);
				row++;
			}

			break;
	}
	if (row != myRow || col  != myCol)
	{
		/*if (grid[row][col].value > 0)
		{
			score += grid[row][col].value;
			grid[row][col].value = 0;
			windowP->InvalidateRect(scoreRect);
		}
		*/
		grid[myRow][myCol].Swap (grid[row][col], windowP);
		myRow = row;
		myCol = col;
		movesLeft--;
		if (movesLeft < 1)
			finished = true;
		if (state == 0)
		{
			state = 1;
			TimerEvaluateAndUpdate (windowP);
		}
	}
	
	//windowP->Invalidate(true);
	//windowP->InvalidateRect(gameRect);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::KeyDown (char key, CFrameWnd * windowP)
{	
	// This function is a place holder for additional key input
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::Message (CFrameWnd * windowP)
{
	char buffer [30];
	sprintf_s (buffer, "Your score is: %d", score);
	CString message = "You ran out of moves!\nThanks for playing Milky Way Matching!\n" + CString(buffer);
	CString title = "Game Over";
	windowP->KillTimer(1);
	windowP->MessageBox (message, title);
	Init(numRows, numCols);
	windowP->SetTimer(1, 1000, NULL);
	windowP->Invalidate(true);
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::UpdateTime (CFrameWnd * windowP)
{
	elapsedTime++;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::FillIn()
{
	for (int r = 1; r <= numRows; r++)
		for (int c = 1; c <= numCols; c++)
			grid[r][c].Fill ();
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::PlaceMe ()
{
	myRow = 1 + rand() % numRows;
	myCol = 1 + rand() % numCols;

	grid [myRow][myCol].what = ME;
	grid [myRow][myCol].whichSymbol = 0;
	grid [myRow][myCol].value = 0;
 	grid [myRow][myCol].matched = false;
}
/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

bool CGame::FindMatches (CFrameWnd * windowP)
{
	int matches=0;
	for (int r = 1; r <= numRows; r++)
	{
		for (int c = 1; c <= numCols; c++)
		{
			if (grid[r][c].matched == false && c > 1 && c < numCols && grid[r][c - 1].whichSymbol == grid[r][c].whichSymbol && grid[r][c + 1].whichSymbol == grid[r][c].whichSymbol)
			{
				grid[r][c].matched = true;
				grid[r][c - 1].matched = true;
				grid[r][c + 1].matched = true;
				matches = matches + 3;
				int movesright = 2;

				while (c + movesright<numCols && grid[r][c + movesright].matched == false && grid[r][c + movesright].whichSymbol == grid[r][c].whichSymbol)
				{
					grid[r][c + movesright].matched = true;
					movesright++;
					matches++;
				}
			}
			if (grid[r][c].matched == false && r > 1 && r < numRows && grid[r - 1][c].whichSymbol == grid[r][c].whichSymbol && grid[r + 1][c].whichSymbol == grid[r][c].whichSymbol)
			{
				grid[r][c].matched = true;
				grid[r - 1][c].matched = true;
				grid[r + 1][c].matched = true;
				matches = matches + 3;
				int movesdown = 2;

				while (r + movesdown < numRows && grid[r + movesdown][c].matched == false && grid[r + movesdown][c].whichSymbol == grid[r][c].whichSymbol)
				{
					grid[r + movesdown][c].matched = true;
					movesdown++;
					matches++;
				}
			}
			
		}
	}
	windowP->Invalidate(true);
	if (matches > 0)
	{
		score = score + matches-1;
		return true;
	}
	else
		return false;

}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

int CGame:: RemoveMatches (CFrameWnd * windowP)
{	
	for (int r = 1; r <= numRows; r++)
	{
		for (int c = 1; c <= numCols; c++)
		{
			if (grid[r][c].matched == true)
			{
				grid[r][c].what = EMPTY;
				grid[r][c].matched = false;
			}
		}
	}
	return 0;
}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

void CGame::UpdateGrid (int row, int col, CFrameWnd * windowP)
{
	for (int r = 1; r <= numRows; r++)
	{
		for (int c = 1; c <= numCols; c++)
		{
			if (grid[r][c].what==EMPTY)
			{
				grid[r][c].what = SYMBOL;
				grid[r][c].Fill();
				
			}
		}
	}



}

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

int CGame::TimerEvaluateAndUpdate (CFrameWnd * windowP)
{
	int matchesFound = 0;
	static int row, col;
	switch (state)
	{
	case 1:
		matchesFound = FindMatches (windowP);
		if (matchesFound == 0)
		{
			state = 0;
			if (finished)
				Message (windowP);
			return 0;
		}
		score += matchesFound;
		windowP->InvalidateRect (scoreRect);
		state = 2;
		windowP->SetTimer(2, 100, NULL);
		break;
	case 2:
		state = 3;
		break;
	case 3:
		RemoveMatches (windowP);
		state = 4;
		break;
	case 4: // what is this doing?
		UpdateGrid (row, col, windowP);
		state = 5;
		break;
	case 5:
		matchesFound = FindMatches (windowP);
		if (matchesFound == 0)
		{
			state = 0;
			windowP->KillTimer(2);
			if (finished)
				Message (windowP);
			return 0;
		}
		score += matchesFound;
		windowP->InvalidateRect (scoreRect);
		state = 2;
		break;
	}
	return 0;
}
