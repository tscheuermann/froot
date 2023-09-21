/***************************************************************************
                          GameSettings.h  -  description
                             -------------------
    begin                : Tue Dec 18 2001
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

#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <qobject.h>

/**
  *@author Thorsten Scheuermann
  */

class GameSettings : public QObject
{
	Q_OBJECT
	
public: 
	GameSettings(int barSteps, int startRows, int numBadPieces,
							 int bonusBaseScore);
	~GameSettings();

	inline int barSteps() const;
	inline int startRows() const;
	inline int badPieceCount() const;
	inline int bonusBaseScore() const;
	
signals:
	void increaseBar(int n);
	void decreaseBar(int n);

public slots:
	void setBarSteps(int steps);
	void piecesRemoved(int numDirect, int numIndirect);
	
protected:
	int m_barSteps;				// number of steps in the danger bar
	int m_startRows;			// number of rows to start out with
	int m_badPieceCount;	// number of unremovable pieces
	int m_bonusBaseScore;	// the bonus score from which the number of used pieces is subtracted
};

inline int GameSettings::barSteps() const
{
	return m_barSteps;
}

inline int GameSettings::startRows() const
{
	return m_startRows;
}

inline int GameSettings::badPieceCount() const
{
	return m_badPieceCount;
}

inline int GameSettings::bonusBaseScore() const
{
	return m_bonusBaseScore;
}

#endif
