/***************************************************************************
                          GameOverState.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2002
    copyright            : (C) 2002 by Thorsten Scheuermann
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

#include "GameOverState.h"
#include "GameWidget.h"
#include "GameBoard.h"
#include "ScoreBoard.h"
#include <qcanvas.h>
#include <qevent.h>
#include <qtimer.h>

//----------------------------------------------------------------------------------------
GameOverState::GameOverState(GameWidget *gl)
	: GameState(gl),
		m_bigFont("helvetica", 24, QFont::Bold),
	  m_smallFont("helvetica", 12),
		m_bigMetrics(m_bigFont),
		m_smallMetrics(m_smallFont)
{
	for(int i=0; i<5; i++)
	{
		m_mainText[i] = NULL;
		m_bonusText[i] = NULL;
	}

	m_timer = new QTimer(this);
	QObject::connect(m_timer, SIGNAL(timeout()),
									 this, SLOT(stopGame()));
}
//----------------------------------------------------------------------------------------
GameOverState::~GameOverState()
{
	int i;

	if(m_mainText[0])
		for(i=0; i<5; i++)
			delete m_mainText[4];
	if(m_bonusText[0])
		for(i=0; i<5; i++)
			delete m_bonusText[4];
}
//----------------------------------------------------------------------------------------
enum GameState::State GameOverState::type()
{
	return GameState::GameOver;
}
//----------------------------------------------------------------------------------------
void GameOverState::initialize()
{
	static int xOffset[4] = {-1, 1, 0, 0};
	static int yOffset[4] = {0, 0, -1, 1};
	QString bonusString;
	int i;

	if(m_myLogic->m_wonGame)
	{
		m_mainText[0] = new QCanvasText("You Won!", m_bigFont, m_myLogic->m_gameBoard);
		m_mainText[0]->setColor(Qt::white);
		m_mainText[0]->setX((m_myLogic->width() - m_bigMetrics.width("You Won!"))/2);
		m_mainText[0]->setY(50.0);
		m_mainText[0]->setZ(100.0);
		m_mainText[0]->show();

		// calculate bonus
		bonusString = "Bonus: " +
			QString::number(m_myLogic->m_scoreBoard->addWinBonus(m_myLogic->m_numPiecesUsed,
																													 m_myLogic->m_settings));
		m_bonusText[0] = new QCanvasText(bonusString, m_smallFont, m_myLogic->m_gameBoard);
		m_bonusText[0]->setColor(Qt::white);
		m_bonusText[0]->setX((240 - m_smallMetrics.width(bonusString))/2);
		m_bonusText[0]->setY(100.0);
		m_bonusText[0]->setZ(100.0);
		m_bonusText[0]->show();

		// add black seam around bonus
		for(i=1; i<5; i++)
		{
			m_bonusText[i] = new QCanvasText(m_bonusText[0]->text(), m_smallFont,
																			 m_myLogic->m_gameBoard);
			m_bonusText[i]->setColor(Qt::black);
			m_bonusText[i]->setX(m_bonusText[0]->x() + xOffset[i-1]);
			m_bonusText[i]->setY(m_bonusText[0]->y() + yOffset[i-1]);
			m_bonusText[i]->setZ(99.0);
			m_bonusText[i]->show();
		}
	}
	else
	{
		m_mainText[0] = new QCanvasText("Game Over", m_bigFont, m_myLogic->m_gameBoard);
		m_mainText[0]->setColor(Qt::white);
		m_mainText[0]->setX((m_myLogic->width() - m_bigMetrics.width("Game Over"))/2);
		m_mainText[0]->setY(50.0);
		m_mainText[0]->setZ(100.0);
		m_mainText[0]->show();
	}

	// add black seam around title
	for(i=1; i<5; i++)
	{
		m_mainText[i] = new QCanvasText(m_mainText[0]->text(), m_bigFont, m_myLogic->m_gameBoard);
		m_mainText[i]->setColor(Qt::black);
		m_mainText[i]->setX(m_mainText[0]->x() + xOffset[i-1]);
		m_mainText[i]->setY(m_mainText[0]->y() + yOffset[i-1]);
		m_mainText[i]->setZ(99.0);
		m_mainText[i]->show();
	}

	// grab mouse input so that clicks on child widgets are registered
	m_myLogic->grabMouse();

	m_timer->start(5000, true);

	m_myLogic->m_gameRunning = false;
	
	emit m_myLogic->gameOver(m_myLogic->m_wonGame);
}
//----------------------------------------------------------------------------------------
void GameOverState::keyPressEvent(QKeyEvent *)
{
	stopGame();
}
//----------------------------------------------------------------------------------------
void GameOverState::mousePressEvent(QMouseEvent *)
{
	stopGame();
}
//----------------------------------------------------------------------------------------
void GameOverState::stopGame()
{
	int i;

	m_timer->stop();
	m_myLogic->releaseMouse();

	// reset everything and get back to the main menu
	emit m_myLogic->finalScore(m_myLogic->score());

	if(m_mainText[0])
	{
		for(i=0; i<5; i++)
		{
			delete m_mainText[i];
			m_mainText[i] = NULL;
		}
	}
	if(m_bonusText[0])
	{
		for(i=0; i<5; i++)
		{
			delete m_bonusText[i];
			m_bonusText[i] = NULL;
		}
	}
	m_myLogic->setState(m_myLogic->m_idleState);
	emit m_myLogic->gameFinished();
}
//----------------------------------------------------------------------------------------
