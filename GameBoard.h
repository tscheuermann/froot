/***************************************************************************
                          GameBoard.h  -  description
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

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <qcanvas.h>

/**
  *@author Thorsten Scheuermann
  */

class Piece;
class PieceSlot;

class GameBoard : public QCanvas
{
Q_OBJECT

public: 
	GameBoard(QObject *parent=0, const char *name=0);
	~GameBoard();
	
	int ceilingWidth();
	inline int arrayWidth();
  inline int arrayHeight();
	PieceSlot *getSlot(int x, int y);
	PieceSlot *getSlot(QPoint &pt);
	void clearMarks();
	void getOverlappingSlots(Piece *p, QList<PieceSlot> *slotList);
	
signals:
	void timer();
	void ceilingLowered();
		
public slots:
	void initialize();
	void lowerCeiling();
	
protected:
	virtual void update();
	void initializeNeighbors();
	bool isValidArrayPos(int x, int y);
	
	PieceSlot **m_pieceArray;
	QPixmap *m_tileMap;
	QCanvasText *m_nextLabel;
	
	int m_arrayWidth;
	int m_arrayHeight;
	int m_numSlots;
	
	int m_ceilingWidth;
};

inline int GameBoard::arrayWidth()
{
	return m_arrayWidth;
}

inline int GameBoard::arrayHeight()
{
	return m_arrayHeight;
}

#endif
