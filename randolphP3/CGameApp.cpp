/*********************************************************************
* Assignment: Project 3 - GUI Move and Match Game                    *
* Author:                                                            *
* Date: Fall 2017                                                    *
* File: CGameApp.cpp                                                 *
*                                                                    *
* Description: This file contains the                                *
*********************************************************************/

#include <afxwin.h>
#include "CGameApp.h"

/*********************************************************************
* Function:                                                          *
*                                                                    *
* Parameters:                                                        *
* Return value:                                                      *
* Description: This function will                                    *
*********************************************************************/

BOOL CGameApp::InitInstance ()
{
		m_pMainWnd = new CGameWin();
		m_pMainWnd->ShowWindow (m_nCmdShow);
		m_pMainWnd->UpdateWindow ();

		return TRUE;
}

CGameApp CGameApp;
