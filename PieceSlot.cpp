/***************************************************************************
                          PieceSlot.cpp  -  description
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

#include "PieceSlot.h"
#include "Globals.h"
#include "Piece.h"
#include "GameBoard.h"

PieceSlot::PieceSlot(GameBoard *board, int posX, int posY)
	: QObject(board)
{
	m_myBoard = board;
	m_piece = NULL;
	m_arrayPos = QPoint(posX, posY);
	m_mark = false;
	m_neighbors.setAutoDelete(false);
}

PieceSlot::~PieceSlot()
{
}
	
void PieceSlot::setPiece(Piece *p)
{
	m_piece = p;
	if(!p)
		return;
	
	QPoint sPos = screenPosition();
	p->move(sPos.x(), sPos.y());
	p->setState(Piece::PS_Placed);
}

QPoint PieceSlot::arrayPosition()
{
	return m_arrayPos;
}

QPoint PieceSlot::screenPosition()
{
	return QPoint(m_arrayPos.x()*PIECE_WIDTH + (m_arrayPos.y()%2)*PIECE_WIDTH/2,
								(m_arrayPos.y() + m_myBoard->ceilingWidth())*PIECE_HEIGHT);
}

QRect PieceSlot::boundingRect()
{
	return QRect(screenPosition(), QSize(PIECE_WIDTH, PIECE_HEIGHT));
}

float PieceSlot::squaredDist(Piece *p)
{
	float a, b;
	
	a = 4+m_arrayPos.x()*PIECE_WIDTH + (m_arrayPos.y()%2)*PIECE_WIDTH/2 - p->x();
	b = 4+(m_arrayPos.y() + m_myBoard->ceilingWidth())*PIECE_HEIGHT - p->y();
	return a*a + b*b;
}

void PieceSlot::addNeighbor(PieceSlot *p)
{
	m_neighbors.append(p);
}

QList<PieceSlot> *PieceSlot::neighborList()
{
	return &m_neighbors;
}

void PieceSlot::removeDFS(QList<PieceSlot> *l, int type)
{
	PieceSlot *s;
	
	if(isMarked())
		return;
	setMark(true);

	if(!m_piece)
		return;
	
	if(m_piece->type() != type)
		return;
	
	// I have the right piece type, so add myself to the list
	l->append(this);
	
	for(s=m_neighbors.first(); s; s=m_neighbors.next())
		s->removeDFS(l, type);
}


void PieceSlot::connectionDFS()
{
	PieceSlot *s;

	if(!m_piece)
		return;	
	if(isMarked())
		return;
	setMark(true);
	
	for(s=m_neighbors.first(); s; s=m_neighbors.next())
		s->connectionDFS();
}

void PieceSlot::updatePiecePosition()
{
	if(m_piece)
	{
		QPoint sPos = screenPosition();
		m_piece->move(sPos.x(), sPos.y());
	}
}
