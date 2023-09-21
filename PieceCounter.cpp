/***************************************************************************
                          PieceCounter.cpp  -  description
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

#include "PieceCounter.h"
#include "Piece.h"
#include <stdlib.h>

PieceCounter::PieceCounter(int numTypes)
{
	m_numTypes = numTypes;
	m_numPiecesLeft = new int[m_numTypes];
	m_typesLeft = new int[m_numTypes];
	
	reset();
}

PieceCounter::~PieceCounter()
{
	delete[] m_typesLeft;
	delete[] m_numPiecesLeft;
}

void PieceCounter::reset()
{
	int i;
	
	m_numTypesLeft = 0;
	m_totalPiecesLeft = 0;

	for(i=0; i<m_numTypes; i++)
		m_numPiecesLeft[i] = 0;
}

void PieceCounter::addPiece(Piece *p)
{
	m_numPiecesLeft[p->type()]++;
	m_totalPiecesLeft++;
	if(m_numPiecesLeft[p->type()] == 1)
		m_typesLeft[m_numTypesLeft++] = p->type();	// add the new type to the list
}

void PieceCounter::removePiece(Piece *p)
{
	if(p->type() >= m_numTypes)				// make sure it isn't a bad piece
		return;

	if(m_numPiecesLeft[p->type()] == 0)
		return;

	m_numPiecesLeft[p->type()]--;
	m_totalPiecesLeft--;
	if(m_numPiecesLeft[p->type()] == 0)
		recalcTypesLeft();

}

void PieceCounter::recalcTypesLeft()
{
	m_numTypesLeft = 0;
	for(int i=0; i<m_numTypes; i++)
	{
		if(m_numPiecesLeft[i])
			m_typesLeft[m_numTypesLeft++] = i;
	}
}

int PieceCounter::getRandomRemainingType() const
{
	return m_typesLeft[rand()%m_numTypesLeft];
}
