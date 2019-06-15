/*********************************************************************
* Assignment: Project 3 - GUI Move and Match Game                    *
* Author:                                                            *
* Date: Fall 2017                                                    *
* File: CGameWin.h                                                   *
*                                                                    *
* Description: This file contains the                                *
*********************************************************************/

#include <afxwin.h>
#include "CGame.h"

/*********************************************************************
* Class: CGameWin                                                    *
*                                                                    *
* Description: This class is designed to                             *
*********************************************************************/

class CGameWin : public CFrameWnd
{
	public:
		CGameWin ();
		afx_msg void OnPaint ();
		afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
		afx_msg void OnKeyDown (UINT achar, UINT repeat, UINT flags);
		afx_msg void OnTimer (UINT nIDEvent);
	private:
		CGame myCGame;
		bool firstTime;
		DECLARE_MESSAGE_MAP ()
};
