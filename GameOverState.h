/***************************************************************************
                          GameOverState.h  -  description
                             -------------------
    begin                : Fri Jan 11 2002
    copyright            : (C) 2002 by Thorsten Scheuermann
 ***************************************************************************/

 /*
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the �Software�), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"
#include <qfont.h>
#include <qfontmetrics.h>

/**
  *@author Thorsten Scheuermann
  */

class QCanvasText;
class QTimer;

class GameOverState : public GameState
{
	Q_OBJECT

public:
	GameOverState(GameWidget *gl);
	virtual ~GameOverState();

	virtual enum State type();
	virtual void initialize();
	virtual void keyPressEvent(QKeyEvent *e);

protected slots:
	void stopGame();

protected:
	virtual void mousePressEvent(QMouseEvent *e);

	QFont m_bigFont;
	QFont m_smallFont;
	QFontMetrics m_bigMetrics;
	QFontMetrics m_smallMetrics;
	QCanvasText *m_mainText[5];
	QCanvasText *m_bonusText[5];
	QTimer *m_timer;
};

#endif
