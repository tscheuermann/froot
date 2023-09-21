/***************************************************************************
                          PieceFactory.cpp  -  description
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

#include "PieceFactory.h"
#include "Piece.h"
#include <qcanvas.h>
#include <stdlib.h>
#include <qpe/qpeapplication.h>

PieceFactory::PieceFactory()
{
	m_pmArray = new QCanvasPixmapArray[8];
	m_pmArray[0].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece1%1.png", 2);
	m_pmArray[1].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece2%1.png", 2);
	m_pmArray[2].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece3%1.png", 2);
	m_pmArray[3].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece4%1.png", 2);
	m_pmArray[4].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece5%1.png", 2);
	m_pmArray[5].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece6%1.png", 2);
	m_pmArray[6].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece7%1.png", 2);
	m_pmArray[7].readPixmaps(QPEApplication::qpeDir()+"pics/froot/piece8.png");
	m_numArrays = 7;
}

PieceFactory::~PieceFactory()
{
	delete[] m_pmArray;
}

Piece *PieceFactory::createPiece(int type, GameBoard *theBoard)
{
	return new Piece(type, &m_pmArray[type%m_numArrays], theBoard);
}

Piece *PieceFactory::createBadPiece(GameBoard *theBoard)
{
	return new Piece(7, &m_pmArray[7], theBoard);
}

Piece *PieceFactory::createRandomPiece(GameBoard *theBoard)
{
	int t = rand() % m_numArrays;
	return new Piece(t, &m_pmArray[t], theBoard);
}

int PieceFactory::numTypes() const
{
	return m_numArrays;
}
