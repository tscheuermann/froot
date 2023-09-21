/***************************************************************************
                          GameWidget.cpp  -  description
                             -------------------
    begin                : Tue Dec 25 2001
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

#include "GameWidget.h"
#include "DangerBar.h"
#include "Launcher.h"
#include "GameBoard.h"
#include "GameSettings.h"
#include "Piece.h"
#include "PieceSlot.h"
#include "PieceCounter.h"
#include "PieceFactory.h"
#include "ScoreBoard.h"
#include "AimingState.h"
#include "MovingState.h"
#include "RemoveState.h"
#include "IdleState.h"
#include "GameOverState.h"
#include <qcanvas.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qapplication.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------------
GameWidget::GameWidget(QWidget *parent, const char *name )
	: QWidget(parent,name)
{
	initializeUI();

	m_gameRunning = false;
	m_settings = NULL;
	m_state = NULL;
	m_nextPiece = NULL;
	m_movingPiece = NULL;
	m_launcher = new Launcher(m_gameBoard);
	m_launcher->setPosition(QPoint(m_gameBoard->width()/2, 264));
	m_pFactory = new PieceFactory;
	m_pCounter = new PieceCounter(m_pFactory->numTypes());
	m_scoreBoard = new ScoreBoard;

	m_aimingState = new AimingState(this);
	m_movingState = new MovingState(this);
	m_removeState = new RemoveState(this);
	m_idleState = new IdleState(this);
	m_gameOverState = new GameOverState(this);

	QObject::connect(m_gameBoard, SIGNAL(timer()),
									 this, SLOT(update()));
	QObject::connect(m_gameBoard, SIGNAL(timer()),
									 m_launcher, SLOT(update()));
	QObject::connect(this, SIGNAL(piecePlaced()),
									 m_dangerBar, SLOT(step()));
	QObject::connect(this, SIGNAL(piecesRemoved(int, int)),
									 m_scoreBoard, SLOT(piecesRemoved(int, int)));
	QObject::connect(m_dangerBar, SIGNAL(overFlow()),
									 m_gameBoard, SLOT(lowerCeiling()));
	QObject::connect(m_gameBoard, SIGNAL(ceilingLowered()),
									 this, SLOT(ceilingLowered()));
	QObject::connect(m_scoreBoard, SIGNAL(newScore(int)),
									 this, SIGNAL(newScore(int)));
	QObject::connect(this, SIGNAL(newScore(int)),
									 m_score, SLOT(setNum(int)));
	QObject::connect(this, SIGNAL(pieceUsed(int)),
									 m_piece, SLOT(setNum(int)));

	setState(m_idleState);

	if(parent)
		parent->installEventFilter(this);

}
//----------------------------------------------------------------------------------------
GameWidget::~GameWidget()
{
	delete m_launcher;
	delete m_pFactory;
	delete m_pCounter;
	delete m_scoreBoard;
	delete m_aimingState;
	delete m_movingState;
	if(parent())
		parent()->removeEventFilter(this);
}
//----------------------------------------------------------------------------------------
void GameWidget::initializeUI()
{
	setFixedSize(240,320);
	setBackgroundColor(black);

	m_dangerBar = new DangerBar(this);
	m_gameBoard = new GameBoard(this);
	m_cView = new QCanvasView(m_gameBoard, this);
	m_scoreLabel = new QLabel(this);
	m_score = new QLabel(this);
	m_pieceLabel = new QLabel(this);
	m_piece = new QLabel(this);

	m_cView->setFrameStyle(QFrame::NoFrame);
	m_cView->move(0, 0);
	m_cView->resize(m_gameBoard->width(), m_gameBoard->height());
	m_cView->show();
	m_dangerBar->move(m_cView->width(), 0);
	m_dangerBar->resize(width()-m_cView->width(), m_cView->height());
	m_dangerBar->show();

	QColorGroup cg;
	cg.setColor(QColorGroup::Background, black);
	cg.setColor(QColorGroup::Foreground, white);
	m_scoreLabel->setPalette(QPalette(cg, cg, cg));
	m_scoreLabel->setAlignment(AlignLeft|AlignVCenter);
	m_scoreLabel->setText("Score:");
	m_scoreLabel->resize(m_scoreLabel->sizeHint().width(), 12);
	m_score->setPalette(QPalette(cg, cg, cg));
	m_score->setNum(9999999);
	m_score->setAlignment(AlignRight|AlignVCenter);
	m_score->resize(m_score->sizeHint().width(), 12);
	m_score->setNum(0);
	m_scoreLabel->move((width()/2-m_scoreLabel->width()-m_score->width())/2,
										 m_cView->height());
	m_score->move(m_scoreLabel->x() + m_scoreLabel->width(), m_cView->height());
	m_scoreLabel->show();
	m_score->show();

	m_pieceLabel->setPalette(QPalette(cg, cg, cg));
	m_pieceLabel->setAlignment(AlignLeft|AlignVCenter);
	m_pieceLabel->setText("Fruits Used:");
	m_pieceLabel->resize(m_pieceLabel->sizeHint().width(), 12);
	m_piece->setPalette(QPalette(cg, cg, cg));
	m_piece->setNum(9999);
	m_piece->setAlignment(AlignRight|AlignVCenter);
	m_piece->resize(m_piece->sizeHint().width(), 12);
	m_piece->setNum(0);
	m_pieceLabel->move(width()/2 + (width()/2-m_pieceLabel->width()-m_piece->width())/2,
										 m_cView->height());
	m_piece->move(m_pieceLabel->x()+m_pieceLabel->width(), m_cView->height());
	m_pieceLabel->show();
	m_piece->show();
}
//----------------------------------------------------------------------------------------
bool GameWidget::eventFilter(QObject *o, QEvent *e)
{
	if(e->type() == QEvent::Close)
	{
		if(m_gameRunning)
		{
			int res;

			res = QMessageBox::warning(this, "Abort Game",
																 "Are you sure you want<br>to abort the game?",
																 "Yes", "No");
			if(res == 0)
				abortGame();
			return true;
		}
		return false;
	}
	else
		return QWidget::eventFilter(o, e);
}
//----------------------------------------------------------------------------------------
void GameWidget::keyPressEvent(QKeyEvent *e)
{
	m_state->keyPressEvent(e);
}
//----------------------------------------------------------------------------------------
void GameWidget::keyReleaseEvent(QKeyEvent *e)
{
	m_state->keyReleaseEvent(e);
}
//----------------------------------------------------------------------------------------
void GameWidget::mousePressEvent(QMouseEvent *e)
{
	m_state->mousePressEvent(e);
}
//----------------------------------------------------------------------------------------
void GameWidget::setGameSettings(GameSettings *settings)
{
	if(m_settings)
	{
		QObject::disconnect(this, SIGNAL(piecesRemoved(int, int)),
												m_settings, SLOT(piecesRemoved(int, int)));
		QObject::disconnect(m_settings, SIGNAL(increaseBar(int)),
												m_dangerBar, SLOT(increase(int)));
		QObject::disconnect(m_settings, SIGNAL(decreaseBar(int)),
												m_dangerBar, SLOT(decrease(int)));
	}

	m_settings = settings;
	m_dangerBar->setBarSteps(m_settings->barSteps());

	QObject::connect(this, SIGNAL(piecesRemoved(int, int)),
									 m_settings, SLOT(piecesRemoved(int, int)));
	QObject::connect(m_settings, SIGNAL(increaseBar(int)),
									 m_dangerBar, SLOT(increase(int)));
	QObject::connect(m_settings, SIGNAL(decreaseBar(int)),
									 m_dangerBar, SLOT(decrease(int)));
}
//----------------------------------------------------------------------------------------
void GameWidget::startGame()
{
	m_numPiecesUsed = 0;
	m_wonGame = false;
	m_ceilingTooLow = false;
	m_pCounter->reset();
	m_dangerBar->reset();
	m_scoreBoard->reset();
	m_gameBoard->initialize();
	if(m_nextPiece)
		delete m_nextPiece;
	if(m_movingPiece)
		delete m_movingPiece;
	emit pieceUsed(m_numPiecesUsed);

	m_launcher->reset();

	// create the pieces
	for(int y=0; y<m_settings->startRows(); y++)
	{
		for(int x=0; x<m_gameBoard->arrayWidth(); x++)
		{
			(m_gameBoard->getSlot(x, y))->setPiece(m_pFactory->createRandomPiece(m_gameBoard));
			m_pCounter->addPiece(m_gameBoard->getSlot(x, y)->piece());
		}
	}

	initializeBadPieces();

	m_nextPiece = m_pFactory->createPiece(m_pCounter->getRandomRemainingType(), m_gameBoard);
	setState(m_aimingState);
	m_gameRunning = true;
}
//----------------------------------------------------------------------------------------
void GameWidget::initializeBadPieces()
{
	int i = 0;
	int x, y;
	Piece *p;

	while(i < m_settings->badPieceCount())
	{
		x = rand() % m_gameBoard->arrayWidth();
		// no bad pieces on the top row
		y = rand() % (m_settings->startRows()-1) + 1;

		// make sure not to overwrite existing bad piece
		p = m_gameBoard->getSlot(x, y)->piece();
		if(p->type() == m_pFactory->numTypes())
			continue;

		m_pCounter->removePiece(p);
		delete p;
		m_gameBoard->getSlot(x, y)->setPiece(m_pFactory->createBadPiece(m_gameBoard));
		i++;
	}
}
//----------------------------------------------------------------------------------------
void GameWidget::abortGame()
{
	m_state->reset();		// give the state object a chance to clean up

	if(m_nextPiece)
	{
		delete m_nextPiece;
		m_nextPiece = NULL;
	}
	if(m_movingPiece)
	{
		delete m_movingPiece;
		m_movingPiece = NULL;
	}
	m_gameBoard->initialize();

	setState(m_idleState);
	m_gameRunning = false;
	emit gameFinished();
}
//----------------------------------------------------------------------------------------
int GameWidget::score()
{
	return m_scoreBoard->score();
}
//----------------------------------------------------------------------------------------
void GameWidget::setState(GameState *newState)
{
	m_state = newState;
	m_state->initialize();
}
//----------------------------------------------------------------------------------------
void GameWidget::armLauncher()
{
	m_launcher->setPiece(m_nextPiece);
	m_movingPiece = m_nextPiece;			// this will be the next piece that is moving
	m_pCounter->addPiece(m_movingPiece);
	m_nextPiece = m_pFactory->createPiece(m_pCounter->getRandomRemainingType(), m_gameBoard);
}
//----------------------------------------------------------------------------------------
void GameWidget::shoot()
{
	if(m_state->type() != GameState::Aiming)
		return;

	m_launcher->launchPiece();
	setState(m_movingState);
}
//----------------------------------------------------------------------------------------
void GameWidget::ceilingLowered()
{
	int bottomRow;

	bottomRow = m_gameBoard->arrayHeight()-1 - m_gameBoard->ceilingWidth();
	// check if piece came down too far
	for(int i=0; i<m_gameBoard->arrayWidth(); i++)
	{
		if(m_gameBoard->getSlot(i, bottomRow)->piece())
		{
			// this flag will get evaluated wherever piecePlaced() is emitted
			m_ceilingTooLow = true;
			break;
		}
	}
}
//----------------------------------------------------------------------------------------
void GameWidget::update()
{
	// the state object knows what to do in the current situation
	m_state->update();
}
//----------------------------------------------------------------------------------------
