/***************************************************************************
                          GameWidget.h  -  description
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

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <qwidget.h>

/**
  *@author Thorsten Scheuermann
  */

class GameBoard;
class GameSettings;
class GameState;
class Piece;
class PieceFactory;
class PieceCounter;
class Launcher;
class DangerBar;
class ScoreBoard;
class QLabel;
class QCanvas;
class QCanvasView;

class GameWidget : public QWidget
{
   Q_OBJECT

public:
	GameWidget(QWidget *parent=0, const char *name=0);
	virtual ~GameWidget();

	QCanvas *gameBoard();
	Launcher *launcher();
	int score();
	void setGameSettings(GameSettings *settings);
	virtual bool eventFilter(QObject *o, QEvent *e);

signals:
	void newScore(int s);
	void pieceUsed(int numPiecesUsed);
	void piecePlaced();
	void piecesRemoved(int numDirect, int numIndirect);
	void piecePlacedSound();
	void piecesRemovedSound(int numDirect, int numIndirect);
	void finalScore(int s);
	void gameOver(bool won);
	void gameFinished();

public slots:
	void startGame();
	void abortGame();
	void shoot();
	void ceilingLowered();

protected slots:
	void update();

protected:
	friend class LauncherTurnState;
	friend class AimingState;
	friend class MovingState;
	friend class RemoveState;
	friend class GameOverState;

	void initializeUI();
	void initializeBadPieces();
	void setState(GameState *newState);
	void armLauncher();

	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent *e);
	virtual void mousePressEvent(QMouseEvent *e);

	// UI stuff
	QCanvasView *m_cView;
	QLabel *m_scoreLabel;
	QLabel *m_score;
	QLabel *m_pieceLabel;
	QLabel *m_piece;

	// game logic stuff
	bool m_gameRunning;					// indicates whether a game is currently in progress
	GameState *m_state;					// current state of the game
	int m_numPiecesUsed;
	bool m_wonGame;							// indicates if player has won at end of game
	bool m_ceilingTooLow;				// flag that indicated that the ceiling moved a piece
															// out of bounds
	Piece *m_nextPiece;
	Piece *m_movingPiece;				// the piece that has been launched
	QPoint m_lastPos;						// the array position of the last placed piece

	GameBoard *m_gameBoard;
	GameSettings *m_settings;
	DangerBar *m_dangerBar;
	Launcher *m_launcher;
	PieceFactory *m_pFactory;
	PieceCounter *m_pCounter;
	ScoreBoard *m_scoreBoard;

	GameState *m_idleState;			// the possible game states
	GameState *m_aimingState;
	GameState *m_movingState;
	GameState *m_removeState;
	GameState *m_gameOverState;
};

#endif
