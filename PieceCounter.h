/***************************************************************************
                          PieceCounter.h  -  description
                             -------------------
    begin                : Mon Dec 17 2001
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

#ifndef PIECECOUNTER_H
#define PIECECOUNTER_H


/**
  *@author Thorsten Scheuermann
  */

class Piece;

class PieceCounter
{
public: 
	PieceCounter(int numTypes);
	~PieceCounter();
	
	void reset();
	void addPiece(Piece *p);
	void removePiece(Piece *p);
	inline int totalPiecesLeft() const;
	inline int piecesLeft(int type) const;
	int getRandomRemainingType() const;
	
protected:
	void recalcTypesLeft();
	
	int m_numTypes;					// the number of different piece types there is
	int m_numTypesLeft;			// the number of piece types that are left
	int *m_numPiecesLeft;		// the number of pieces left for each type
	int *m_typesLeft;				// the types of which pieces are left
	int m_totalPiecesLeft;	// the total number of pieces left
};

inline int PieceCounter::totalPiecesLeft() const
{
	return m_totalPiecesLeft;
}

inline int PieceCounter::piecesLeft(int type) const
{
	return m_numPiecesLeft[type];
}

#endif
