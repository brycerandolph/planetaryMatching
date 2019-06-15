/*********************************************************************
* Assignment: Project 3 - GUI Move and Match Game                    *
* Author:                                                            *
* Date: Fall 2017                                                    *
* File: CGame.h                                                      *
*                                                                    *
* Description: This file contains the                                *
*********************************************************************/

#ifndef GAME_H
#define GAME_H

#include <afxwin.h>
#include <string>

using namespace std;

#define NUMIMAGES 15
#define DEFAULT_TIME 120
#define DEFAULT_MOVES 100
#define DEFAULT_ROWS 5
#define DEFAULT_COLS 9

/**********************************************************************
* Type: sType                                                         *
*                                                                     *
* Description: The sType enumerated type is used to identify the type *
*              of the information stored in a CGameSquare object.     *
**********************************************************************/

enum sTypes {BORDER, ME, SYMBOL, EMPTY};

/*********************************************************************
* Class: CGame                                                       *
*                                                                    *
* Description: This class is designed to                             *
*********************************************************************/

class CGame
{
	public:
		CGame ();
		CGame (const CGame & other);
		~CGame ();
		CGame & operator = (const CGame & other);
		void Init (int R, int C);
		void Instructions (CFrameWnd * windowP);
		void Display (CFrameWnd * windowP);
		void Click (int x, int y, CFrameWnd * windowP);
		void Move (char dir, CFrameWnd * windowP);
		void KeyDown (char key, CFrameWnd * windowP);
		void UpdateTime (CFrameWnd * windowP);		
		int TimerEvaluateAndUpdate (CFrameWnd * windowP);
		void Message (CFrameWnd * windowP);

	private:
		//void Temporary ();
		struct CGameSquare
		{
			CGameSquare ();
			void Fill ();
			void Display (CDC * deviceContextP);
			void Swap (CGameSquare & other, CFrameWnd * windowP);
			void SetWhere (CRect whereRect);
			sTypes what;
			int whichSymbol;
			bool matched;
			int value;
			CRect where;
		};
		void SetUp (CRect window);
		void GameDisplay (CDC * deviceContextP);
		void DataDisplay (CDC * deviceContextP);
		void ButtonDisplay (CDC * deviceContextP);
		void FillIn ();
		void PlaceMe (); 
		bool FindMatches (CFrameWnd * windowP);
		int RemoveMatches (CFrameWnd * windowP);
		void UpdateGrid (int row, int col, CFrameWnd * windowP);

		CGameSquare ** grid;
		int numRows, numCols;
		int myRow, myCol;
		int movesLeft;
		int score;
		int state;
		bool finished;
		int elapsedTime;

		CRect thisRect;
		CRect gameRect;
		CRect dataRect;
		CRect scoreRect;
		CRect newButton;
		CRect restartButton;
		CRect titleRect;
		CRect movesRect;
		CBitmap bgImage;
};

#endif
