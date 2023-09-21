/***************************************************************************
                          MovingState.cpp  -  description
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

#include "MovingState.h"
#include "Globals.h"
#include "GameWidget.h"
#include "GameBoard.h"
#include "PieceSlot.h"
#include "Piece.h"
#include <stdio.h>

//----------------------------------------------------------------------------------------
MovingState::MovingState(GameWidget *gl)
	: LauncherTurnState(gl)
{
}
//----------------------------------------------------------------------------------------
MovingState::~MovingState()
{
}
//----------------------------------------------------------------------------------------
enum GameState::State MovingState::type()
{
	return Moving;
}
//----------------------------------------------------------------------------------------
void MovingState::update()
{
	// update position of the piece
  for(int i=0; i<3; i++)
  {
    // advance one pixel per iteration
	  m_myLogic->m_movingPiece->update();
	  if(checkCollision())
      return;
	}
}
//----------------------------------------------------------------------------------------
bool MovingState::checkCollision()
{
	QList<PieceSlot> overlappingSlots;
	bool collision = false;
	PieceSlot *closestSlot = NULL;
	PieceSlot *sl;
	Piece *p;
	float minDist = 1000.0;
	float collisionDist;
	float dist;

	p = m_myLogic->m_movingPiece;
	collisionDist = 0.8 * PIECE_WIDTH*PIECE_WIDTH;
	m_myLogic->m_gameBoard->getOverlappingSlots(m_myLogic->m_movingPiece,
																							&overlappingSlots);

	for(sl=overlappingSlots.first(); sl; sl=overlappingSlots.next())
	{
		// check if an overlapped slot is occupied and close enough to flying piece
		if(sl->piece())
		{
			if(sl->squaredDist(p) <= collisionDist)
				collision = true;
		}
		else
		{
			// if not it might be a candidate for putting the piece if there is a collision
			dist = sl->squaredDist(p);
			if(dist < minDist)
			{
				minDist = dist;
				closestSlot = sl;
			}
		}
	}
	// need to handle hitting the top row without collision with piece
	if(p->y() < m_myLogic->m_gameBoard->ceilingWidth()*PIECE_HEIGHT)
		collision = true;

	if(collision)
	{
    if(!closestSlot)
    {
      fprintf(stderr, "Error: Collison, but cannot find a free slot!\n");
    }
		emit m_myLogic->pieceUsed(++(m_myLogic->m_numPiecesUsed));
		closestSlot->setPiece(p);
		m_myLogic->m_movingPiece = NULL;

		m_myLogic->m_lastPos = closestSlot->arrayPosition();
		m_myLogic->setState(m_myLogic->m_removeState);
	}
  
  return collision;
}
//----------------------------------------------------------------------------------------
