/***************************************************************************
                          Piece.cpp  -  description
                             -------------------
    begin                : Thu Dec 6 2001
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

#include "Piece.h"
#include "GameBoard.h"
#include <qcanvas.h>
#include <math.h>
#include <stdlib.h>

Piece::Piece(int type, QCanvasPixmapArray *pmArray, GameBoard *gameBoard)
	: QCanvasSprite(pmArray, gameBoard), m_type(type)
{
	m_myBoard = gameBoard;
	setState(PS_Pending);
}

Piece::~Piece()
{
}

void Piece::setState(enum State s)
{
	m_state = s;
	switch(m_state)
	{
	case PS_Pending:
		setZ(0.0);
		setX(m_myBoard->width()-width());
		setY(m_myBoard->height()-height());
		show();
		break;
		
	case PS_Launcher:
		setZ(20.0);
		show();
		break;
		
	case PS_Placed:
		setZ(0.0);
		show();
		break;
	
	case PS_Moving:
		setZ(20.0);
		show();
		break;

	case PS_Disappearing:
		initializeDisappearing();
		break;
			
	case PS_Dropping:
		setZ(20.0);
		initializeDropping();
		break;
		
	default:
		break;
	}
}

void Piece::setLaunchAngle(float a)
{
	setVelocity(1*sin(a*M_PI/180.0f), -1*cos(a*M_PI/180.0f));
}

bool Piece::update()
{
	switch(m_state)
	{
		case PS_Moving:
			updateMoving();
			return true;
		
		case PS_Dropping:
			return updateDropping();
			
		case PS_Disappearing:
			return updateDisappearing();
		
		default:
			break;
	}
	return false;
}

void Piece::updateMoving()
{
	float dx;
	
	advance(1);
	if(x() < 0)
	{
		setX(-x());
		setXVelocity(-xVelocity());
	}
	dx = m_myBoard->width()-width();
	if(x() >= dx)
	{
		setX(2*dx-x());
		setXVelocity(-xVelocity());
	}
}

bool Piece::updateDropping()
{
	if(x() <= -width())
		return false;
	if(x() >= m_myBoard->width())
		return false;
	if(y() >= m_myBoard->height())
		return false;
		
	advance(1);
	setYVelocity(yVelocity()+0.1);	// gravity;
	return true;
}

bool Piece::updateDisappearing()
{
	return (m_disappearingFrameCounter-- > 0);
}

void Piece::initializeDropping()
{
	setXVelocity(6.0*(float)rand()/(float)RAND_MAX-3.0);
	setYVelocity(3.0*(float)rand()/(float)RAND_MAX-2.0);
}

void Piece::initializeDisappearing()
{
	m_disappearingFrameCounter = 15;
	setFrame(1);
}