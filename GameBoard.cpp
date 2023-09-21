/***************************************************************************
                          GameBoard.cpp  -  description
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

#include "GameBoard.h"
#include "Globals.h"
#include "Piece.h"
#include "PieceSlot.h"
#include <qpe/qpeapplication.h>

//----------------------------------------------------------------------------------------
GameBoard::GameBoard(QObject *parent, const char *name)
	: QCanvas(parent, name)
{
	resize(232,272);
	setBackgroundColor(black);
	setDoubleBuffering(true);
	setUpdatePeriod(10);

	m_tileMap = new QPixmap(QPEApplication::qpeDir()+"pics/froot/bgtiles.png");
	setTiles(*m_tileMap, 15, 17, 16, 16);

	m_arrayWidth = 14;				// put these in globals
	m_arrayHeight = 16;
	// one extra row for pieces that moved out of bounds
	m_numSlots = m_arrayWidth*(m_arrayHeight);

	m_pieceArray = new PieceSlot*[m_arrayWidth * m_arrayHeight];
	// create array where the pieces will reside
	for(int y=0; y<m_arrayHeight; y++)
		for(int x=0; x<m_arrayWidth; x++)
			m_pieceArray[y*m_arrayWidth+x] = new PieceSlot(this, x, y);

	initializeNeighbors();

	// initialize label for next piece
	QFont f("helvetica");
	QFontMetrics fm(f);
	m_nextLabel = new QCanvasText("Next:", f, this);
	m_nextLabel->setColor(white);
	m_nextLabel->setZ(0.0);
	m_nextLabel->setX(width()-18-fm.width(m_nextLabel->text()));
	m_nextLabel->setY(height()-(16+fm.height())/2);
	m_nextLabel->show();
}
//----------------------------------------------------------------------------------------
GameBoard::~GameBoard()
{
	delete[] m_pieceArray;
	delete m_nextLabel;
}
//----------------------------------------------------------------------------------------
int GameBoard::ceilingWidth()
{
	return m_ceilingWidth;
}
//----------------------------------------------------------------------------------------
PieceSlot *GameBoard::getSlot(QPoint &pt)
{
	return getSlot(pt.x(), pt.y());
}
//----------------------------------------------------------------------------------------
PieceSlot *GameBoard::getSlot(int x, int y)
{
	if(!isValidArrayPos(x, y))
		return NULL;
	return m_pieceArray[y*m_arrayWidth+x];
}
//----------------------------------------------------------------------------------------
void GameBoard::initialize()
{
	int x, y;
	int numSlots;
	
	numSlots = m_arrayWidth*m_arrayHeight;
	
	m_ceilingWidth = 0;
	
	// initialize the background tiles
	for(y=0; y<m_arrayHeight-1; y++)
	{
		for(x=0; x<m_arrayWidth+1; x++)
			setTile(x, y, 1);
	}
	for(y=m_arrayHeight-1; y<17; y++)
	{
		for(x=0; x<15; x++)
			setTile(x, y, 2);
	}
	
	// remove all remaining pieces
	for(int i=0; i<numSlots; i++)
	{
		if(m_pieceArray[i]->piece())
			delete m_pieceArray[i]->piece();
			
		m_pieceArray[i]->setPiece(NULL);
	}
}
//----------------------------------------------------------------------------------------
void GameBoard::update()
{
	emit timer();	
	QCanvas::update();
}
//----------------------------------------------------------------------------------------
void GameBoard::clearMarks()
{
 	for(int i=0; i<m_numSlots; i++)
 		m_pieceArray[i]->setMark(false);
}
//----------------------------------------------------------------------------------------
void GameBoard::getOverlappingSlots(Piece *p, QList<PieceSlot> *slotList)
{
	int xs, ys, xe, ye;
	int x, y;
	
	ys = (int)p->y()/PIECE_HEIGHT - m_ceilingWidth;
	if(ys >= m_arrayHeight)
		return;	// no overlapping slots
	else if(ys < 0)
		ys = 0;
	xs = (int)p->x()/PIECE_WIDTH;
	ye = ys+2;
	ye = (ye > m_arrayHeight) ? m_arrayHeight : ye;
	xe = xs+2;
	xe = (xe > m_arrayWidth) ? m_arrayWidth : xe;
	
	for(y=ys; y<ye; y++)
	{
		for(x=xs; x<xe; x++)
		{
			if(p->boundingRect().intersects(m_pieceArray[y*m_arrayWidth+x]->boundingRect()))
				slotList->append(m_pieceArray[y*m_arrayWidth+x]);
		}
	}
}
//----------------------------------------------------------------------------------------
void GameBoard::initializeNeighbors()
{
	int x, y;
	
	for(y=0; y<m_arrayHeight; y++)
	{
		for(x=0; x<m_arrayWidth; x++)
		{
			// left and right neighbors
			if(isValidArrayPos(x-1, y))
				m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[y*m_arrayWidth+x-1]);
			if(isValidArrayPos(x+1, y))
				m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[y*m_arrayWidth+x+1]);

			// 2 of the bottom and top neighbors
			if(isValidArrayPos(x, y-1))
				m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[(y-1)*m_arrayWidth+x]);
			if(isValidArrayPos(x, y+1))
				m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[(y+1)*m_arrayWidth+x]);
				
			// the remaining ones depend on the row offset
			if(y%2)
			{
				if(isValidArrayPos(x+1, y-1))
					m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[(y-1)*m_arrayWidth+x+1]);
				if(isValidArrayPos(x+1, y+1))
					m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[(y+1)*m_arrayWidth+x+1]);
			}
			else
			{
				if(isValidArrayPos(x-1, y-1))
					m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[(y-1)*m_arrayWidth+x-1]);
				if(isValidArrayPos(x-1, y+1))
					m_pieceArray[y*m_arrayWidth+x]->addNeighbor(m_pieceArray[(y+1)*m_arrayWidth+x-1]);
			}
		}
	}
}
//----------------------------------------------------------------------------------------
bool GameBoard::isValidArrayPos(int x, int y)
{
	if((x < 0) || (x >= m_arrayWidth))
		return false;
	if((y < 0) || (y >= m_arrayHeight))
		return false;
	return true;
}
//----------------------------------------------------------------------------------------
void GameBoard::lowerCeiling()
{
	int i;
	int numSlots;
	QPoint lowestPos;
		
	// update the background tiles
	for(i=0; i<15; i++)
		setTile(i, m_ceilingWidth, 0);
	
	m_ceilingWidth++;
	
	emit ceilingLowered();
	
	numSlots = m_arrayWidth*m_arrayHeight;	
	for(i=0; i<numSlots; i++)
	{
		m_pieceArray[i]->updatePiecePosition();
		if(m_pieceArray[i]->piece())
			lowestPos = m_pieceArray[i]->arrayPosition();
	}
}
//----------------------------------------------------------------------------------------
