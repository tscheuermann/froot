/***************************************************************************
                          RemoveState.cpp  -  description
                             -------------------
    begin                : Mon Dec 24 2001
    copyright            : (C) 2001 by Thorsten Scheuermann
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

#include "RemoveState.h"
#include "GameWidget.h"
#include "GameBoard.h"
#include "PieceSlot.h"
#include "PieceCounter.h"
#include "Piece.h"

//----------------------------------------------------------------------------------------
RemoveState::RemoveState(GameWidget *gl)
	: LauncherTurnState(gl)
{
}
//----------------------------------------------------------------------------------------
RemoveState::~RemoveState()
{
}
//----------------------------------------------------------------------------------------
enum GameState::State RemoveState::type()
{
	return Remove;
}
//----------------------------------------------------------------------------------------
void RemoveState::reset()
{
	Piece *p;

	for(p=m_disappearingPieces.first(); p; p=m_disappearingPieces.next())
		delete p;
	for(p=m_droppingPieces.first(); p; p=m_droppingPieces.next())
		delete p;
	m_disappearingPieces.clear();
	m_droppingPieces.clear();
}
//----------------------------------------------------------------------------------------
void RemoveState::initialize()
{
	if(!removePieces(m_myLogic->m_lastPos))
	{
		// check for game over condition
		if(m_myLogic->m_lastPos.y()+m_myLogic->m_gameBoard->ceilingWidth() >=
			 m_myLogic->m_gameBoard->arrayHeight()-1)
		{
			m_myLogic->m_wonGame = false;	// lost game because pieces came down too far
			m_myLogic->setState(m_myLogic->m_gameOverState);
			return;
			// piecePlaced() is not emitted here to prevent pushing a piece that is
			// already out of bounds too far down.
		}
		else
		{
			emit m_myLogic->piecePlacedSound();
			emit m_myLogic->piecePlaced();
			// the ceiling might have come down after that...
			if(m_myLogic->m_ceilingTooLow)
			{
				m_myLogic->m_wonGame = false;
				m_myLogic->setState(m_myLogic->m_gameOverState);
			}
			else
				m_myLogic->setState(m_myLogic->m_aimingState);
		}
	}
	else
	{
		emit m_myLogic->piecesRemovedSound(m_numDisappearingPieces,
																			 m_numDroppingPieces);
	}
}
//----------------------------------------------------------------------------------------
void RemoveState::update()
{
	Piece *p;

	// update disappearing pieces
	for(p=m_disappearingPieces.first(); p; )
	{
		if(!p->update())
		{
			// finished disappearing, remove piece
			m_disappearingPieces.take();
			delete p;
			p = m_disappearingPieces.current();
		}
		else
			p = m_disappearingPieces.next();
	}

	// update dropping pieces
	for(p=m_droppingPieces.first(); p; )
	{
		if(!p->update())
		{
			// finished dropping, remove piece
			m_droppingPieces.take();
			delete p;
			p = m_droppingPieces.current();
		}
		else
			p = m_droppingPieces.next();
	}

	if((m_disappearingPieces.count() == 0) && (m_droppingPieces.count() == 0))
	{
		// both lists are empty, go on with game
	 	emit m_myLogic->piecesRemoved(m_numDisappearingPieces,
 																	m_numDroppingPieces);

		if(m_myLogic->m_pCounter->totalPiecesLeft() == 0)
		{
			m_myLogic->m_wonGame = true;			// won the game
			m_myLogic->setState(m_myLogic->m_gameOverState);
		}
		else if(m_myLogic->m_ceilingTooLow)
		{
			// lost the game because the ceiling has come down too far
			m_myLogic->m_wonGame = false;
			m_myLogic->setState(m_myLogic->m_gameOverState);
		}
		else
			m_myLogic->setState(m_myLogic->m_aimingState);
	}
}
//----------------------------------------------------------------------------------------
bool RemoveState::removePieces(QPoint &startingPoint)
{
	GameBoard *gb;
	QList<PieceSlot> matchingSlots;
	PieceSlot *ps;
	Piece *p;
	int x, y;

	gb = m_myLogic->m_gameBoard;
	gb->clearMarks();

 	ps = gb->getSlot(startingPoint);
 	// depth-first search for pieces of the same color
 	ps->removeDFS(&matchingSlots, ps->piece()->type());

 	if(matchingSlots.count() >= 3)
 	{
	 	for(ps=matchingSlots.first(); ps; ps=matchingSlots.next())
 		{
 			p = ps->piece();
 			p->setState(Piece::PS_Disappearing);
 			m_disappearingPieces.append(p);
 			ps->setPiece(NULL);
 			m_myLogic->m_pCounter->removePiece(p);
 		}
 	}
	else
		return false;

 	gb->clearMarks();

 	// get a list of now unattached pieces that will fall off
 	for(x=0; x<gb->arrayWidth(); x++)
 		gb->getSlot(x, 0)->connectionDFS();

 	// all the unmarked pieces are unattached after the DFS run
 	for(y=0; y<gb->arrayHeight(); y++)
 	{
	 	for(x=0; x<gb->arrayWidth(); x++)
	 	{
 			if(!gb->getSlot(x, y)->isMarked())
 			{
 				p = gb->getSlot(x, y)->piece();
 				if(!p)			// slot might not be connected because it's empty
 					continue;
 				gb->getSlot(x, y)->setPiece(NULL);
 				p->setState(Piece::PS_Dropping);
 				m_droppingPieces.append(p);
 				m_myLogic->m_pCounter->removePiece(p);
 			}
 		}
 	}

 	m_numDisappearingPieces = m_disappearingPieces.count();
 	m_numDroppingPieces = m_droppingPieces.count();
 	return true;
}
//----------------------------------------------------------------------------------------
