/***************************************************************************
                          PieceSlot.h  -  description
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

#ifndef PIECESLOT_H
#define PIECESLOT_H

#include <qpoint.h>
#include <qrect.h>
#include <qobject.h>
#include <qlist.h>

/**
  *@author Thorsten Scheuermann
  */
class Piece;
class GameBoard;

class PieceSlot : public QObject
{
public: 
	PieceSlot(GameBoard *board, int posX, int posY);
	~PieceSlot();
	
	void setPiece(Piece *p);
	inline Piece *piece();
	QPoint arrayPosition();
	QPoint screenPosition();
	QRect boundingRect();
	void updatePiecePosition();
	float squaredDist(Piece *p);
	
	inline void setMark(bool m);
	inline bool isMarked();

	void addNeighbor(PieceSlot *p);
	QList<PieceSlot> *neighborList();
	void removeDFS(QList<PieceSlot> *l, int type);
	void connectionDFS();
	
protected:
	GameBoard *m_myBoard;
	Piece *m_piece;
	QPoint m_arrayPos;
	bool m_mark;
	QList<PieceSlot> m_neighbors;
};


inline void PieceSlot::setMark(bool m)
{
	m_mark = m;
}

inline bool PieceSlot::isMarked()
{
	return m_mark;
}

inline Piece *PieceSlot::piece()
{
	return m_piece;
}

#endif
