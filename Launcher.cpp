/***************************************************************************
                          Launcher.cpp  -  description
                             -------------------
    begin                : Wed Dec 5 2001
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

#include "Launcher.h"
#include "GameBoard.h"
#include "Piece.h"
#include <math.h>
#include <qpe/qpeapplication.h>

const float Launcher::ACCEL = 0.05;
const float Launcher::MAX_SPEED = 2.0;

//----------------------------------------------------------------------------------------
Launcher::Launcher(GameBoard *gameBoard, const char *name)
	: QObject(gameBoard, name), m_pos(0, 0)
{
	m_turnDir = Stop;
	m_speed = 0;
	m_angle = 0;
	m_base = new QCanvasSprite(new QCanvasPixmapArray(QPEApplication::qpeDir()+"pics/froot/launcherbase.png"), gameBoard);
	m_turret[0] = new QCanvasSprite(new QCanvasPixmapArray(QPEApplication::qpeDir()+"pics/froot/launcher1.png"), gameBoard);
	m_turret[1] = new QCanvasSprite(new QCanvasPixmapArray(QPEApplication::qpeDir()+"pics/froot/launcher2.png"), gameBoard);
	m_turret[2] = new QCanvasSprite(new QCanvasPixmapArray(QPEApplication::qpeDir()+"pics/froot/launcher3.png"), gameBoard);
	m_turret[3] = new QCanvasSprite(new QCanvasPixmapArray(QPEApplication::qpeDir()+"pics/froot/launcher4.png"), gameBoard);

	m_angle = 0.0f;
	m_turretRadius[0] = 18.0f;
	m_turretRadius[1] = 24.0f;
	m_turretRadius[2] = 33.0f;
	m_turretRadius[3] = 44.0f;
	updateTurretPos();

	m_base->setZ(10.0);
	m_turret[0]->setZ(11.0);
	m_turret[1]->setZ(12.0);
	m_turret[2]->setZ(13.0);
	m_turret[3]->setZ(14.0);

	m_base->show();
	m_turret[0]->show();
	m_turret[1]->show();
	m_turret[2]->show();
	m_turret[3]->show();
}
//----------------------------------------------------------------------------------------
Launcher::~Launcher()
{
}
//----------------------------------------------------------------------------------------
void Launcher::reset()
{
	m_turnDir = Stop;
	m_speed = 0;
	m_angle = 0;
	updateTurretPos();
}
//----------------------------------------------------------------------------------------
void Launcher::turnLeft(bool on)
{
	if(on)
		m_turnDir = Left;
	else if(m_turnDir == Left)
		m_turnDir = Stop;

	m_speed = 0;
}
//----------------------------------------------------------------------------------------
void Launcher::turnRight(bool on)
{
	if(on)
		m_turnDir = Right;
	else if(m_turnDir == Right)
		m_turnDir = Stop;

	m_speed = 0;
}
//----------------------------------------------------------------------------------------
void Launcher::update()
{
	m_speed += ACCEL;
	if(m_speed > MAX_SPEED)
		m_speed = MAX_SPEED;

	switch(m_turnDir)
	{
	case Left:
		m_angle -= m_speed;
		if(m_angle < -80.0f)
			m_angle = -80.0f;
		updateTurretPos();
		break;

	case Right:
		m_angle += m_speed;
		if(m_angle > 80.0f)
			m_angle = 80.0f;
		updateTurretPos();
		break;

	default:
		break;
	}
}
//----------------------------------------------------------------------------------------
void Launcher::setPosition(QPoint s)
{
	m_pos = s;
	m_base->setX(m_pos.x()-m_base->boundingRect().width()/2);
	m_base->setY(m_pos.y()-m_base->boundingRect().height()/2);
	updateTurretPos();
}
//----------------------------------------------------------------------------------------
void Launcher::updateTurretPos()
{
	float dx, dy;
	QPoint turretPos;

	dx = sin(m_angle*M_PI/180.0f);
	dy = -cos(m_angle*M_PI/180.0f);
	for(int i=0; i<4; i++)
	{
		turretPos = m_pos + QPoint((int)(m_turretRadius[i]*dx),
															 (int)(m_turretRadius[i]*dy));
		m_turret[i]->setX(turretPos.x()-m_turret[i]->boundingRect().width()/2);
		m_turret[i]->setY(turretPos.y()-m_turret[i]->boundingRect().height()/2);
	}
}
//----------------------------------------------------------------------------------------
void Launcher::setPiece(Piece *p)
{
	m_loadedPiece = p;
	if(m_loadedPiece)
	{
		m_loadedPiece->setState(Piece::PS_Launcher);
		m_loadedPiece->move(m_pos.x()-m_loadedPiece->boundingRect().width()/2,
												m_pos.y()-m_loadedPiece->boundingRect().height()/2);
	}
}
//----------------------------------------------------------------------------------------
void Launcher::launchPiece()
{
	if(!m_loadedPiece)
		return;

	m_loadedPiece->setState(Piece::PS_Moving);
	m_loadedPiece->setLaunchAngle(m_angle);
	m_loadedPiece = NULL;
}
//----------------------------------------------------------------------------------------
