/***************************************************************************
                          RemoveState.h  -  description
                             -------------------
    begin                : Mon Dec 24 2001
    copyright            : (C) 2001 by Thorsten Scheuermann
 ***************************************************************************/

 /*
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the �Software�), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef REMOVESTATE_H
#define REMOVESTATE_H

#include "LauncherTurnState.h"
#include <qlist.h>
#include <qpoint.h>

/**
  *@author Thorsten Scheuermann
  */

class Piece;

class RemoveState : public LauncherTurnState
{
public: 
	RemoveState(GameWidget *gl);
	~RemoveState();
	
	virtual enum State type();
	virtual void initialize();
	virtual void update();
	virtual void reset();

protected:
	bool removePieces(QPoint &startingPoint);
	
	QList<Piece> m_droppingPieces;
	QList<Piece> m_disappearingPieces;
	int m_numDisappearingPieces;
	int m_numDroppingPieces;
};

#endif
