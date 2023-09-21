/***************************************************************************
                          LauncherTurnState.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2002
    copyright            : (C) 2002 by Thorsten Scheuermann
 ***************************************************************************/

 /*
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the “Software”), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "LauncherTurnState.h"
#include "GameWidget.h"
#include "Launcher.h"
#include <qevent.h>

//----------------------------------------------------------------------------------------
LauncherTurnState::LauncherTurnState(GameWidget *gl)
	: GameState(gl)
{
}
//----------------------------------------------------------------------------------------
LauncherTurnState::~LauncherTurnState()
{
}
//----------------------------------------------------------------------------------------
void LauncherTurnState::keyPressEvent(QKeyEvent *e)
{
	if(e->isAutoRepeat())
		return;
		
	switch(e->key())
	{
	case Qt::Key_Left:
		m_myLogic->m_launcher->turnLeft(true);
		break;

	case Qt::Key_Right:
		m_myLogic->m_launcher->turnRight(true);
		break;
		
	default:
		GameState::keyPressEvent(e);
	}
}
//----------------------------------------------------------------------------------------
void LauncherTurnState::keyReleaseEvent(QKeyEvent *e)
{
	if(e->isAutoRepeat())
		return;
		
	switch(e->key())
	{
	case Qt::Key_Left:
		m_myLogic->m_launcher->turnLeft(false);
		break;
		
	case Qt::Key_Right:
		m_myLogic->m_launcher->turnRight(false);
		break;
		
	default:
		e->ignore();
	}
}
//----------------------------------------------------------------------------------------
