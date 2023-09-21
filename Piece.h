/***************************************************************************
                          Piece.h  -  description
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

#ifndef PIECE_H
#define PIECE_H

#include <qcanvas.h>
/**
  *@author Thorsten Scheuermann
  */

class PieceFactory;
class GameBoard;

class Piece : public QCanvasSprite
{
public: 
	virtual ~Piece();

	enum State {PS_Pending, PS_Launcher, PS_Moving, PS_Placed, PS_Disappearing,
							PS_Dropping};
	
	int type() const;
	void setState(enum State s);
	void setLaunchAngle(float a);
	bool update();
		
protected:
	friend class PieceFactory;
	
	Piece(int type, QCanvasPixmapArray *pmArray, GameBoard *gameBoard);
	void updateMoving();
	bool updateDropping();
	bool updateDisappearing();
	void initializeDropping();
	void initializeDisappearing();
	
	int m_type;
	enum State m_state;
	GameBoard *m_myBoard;
	int m_disappearingFrameCounter;
};

inline int Piece::type() const
{
	return m_type;
}

#endif
