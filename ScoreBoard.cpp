/***************************************************************************
                          ScoreBoard.cpp  -  description
                             -------------------
    begin                : Sun Dec 23 2001
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

#include "ScoreBoard.h"
#include "GameSettings.h"

ScoreBoard::ScoreBoard()
	: QObject()
{
	reset();
}

ScoreBoard::~ScoreBoard()
{
}

void ScoreBoard::reset()
{
	m_score = 0;
	emit newScore(m_score);
}

void ScoreBoard::piecesRemoved(int numDirect, int numIndirect)
{
	m_score += 1+numDirect*numDirect;
	m_score += 10*numIndirect*numIndirect;
	emit newScore(m_score);
}

int ScoreBoard::addWinBonus(int numPiecesUsed, GameSettings *settings)
{
	int bonus;
	
	bonus = settings->bonusBaseScore() - 100*numPiecesUsed;
	bonus = (bonus < 0) ? 0 : bonus;
	bonus += 1000;			// win bonus
	m_score += bonus;
	emit newScore(m_score);
	return bonus;
}
