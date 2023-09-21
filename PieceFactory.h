/***************************************************************************
                          PieceFactory.h  -  description
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

#ifndef PIECEFACTORY_H
#define PIECEFACTORY_H


/**
  *@author Thorsten Scheuermann
  */

class GameBoard;
class Piece;
class QCanvasPixmapArray;

class PieceFactory
{
public: 
	PieceFactory();
	~PieceFactory();
	
	Piece *createPiece(int type, GameBoard *theBoard);
	Piece *createBadPiece(GameBoard *theBoard);
	Piece *createRandomPiece(GameBoard *theBoard);
	int numTypes() const;
	
protected:
	QCanvasPixmapArray *m_pmArray;
	int m_numArrays;
};

#endif
