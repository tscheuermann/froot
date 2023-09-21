#include "SoundPlayer.h"
#include <qpe/sound.h>
#include <qsound.h>
#include <qpe/qpeapplication.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------
SoundPlayer::SoundPlayer(QObject *parent)
	: QObject(parent, "SoundPlayer"),
		m_placedSound(NULL),
		m_disappearSound(NULL),
		m_fallingSound(NULL),
		m_manyFallingSound(NULL),
		m_wonSound(NULL),
		m_lostSound(NULL)
{
	//m_enabled = available();
	m_enabled = false;

	if(available())
	{
		m_placedSound = new Sound("froot/test2");
		m_disappearSound = new Sound("froot/test6");
		m_fallingSound = new Sound("froot/drop");
		m_manyFallingSound = new Sound("froot/drop");
		m_wonSound = new Sound("level_up");
		m_lostSound = new Sound("ohdear");
	}
}
//----------------------------------------------------------------------------------------
SoundPlayer::~SoundPlayer()
{
	if(m_placedSound)
		delete m_placedSound;
	if(m_disappearSound)
		delete m_disappearSound;
	if(m_fallingSound)
		delete m_fallingSound;
	if(m_manyFallingSound)
		delete m_manyFallingSound;
	if(m_wonSound)
		delete m_wonSound;
	if(m_lostSound)
		delete m_lostSound;
}
//----------------------------------------------------------------------------------------
bool SoundPlayer::available()
{
	return false;				// disable sound for now
//	return QSound::available();
}
//----------------------------------------------------------------------------------------
void SoundPlayer::setEnabled(bool on)
{
	m_enabled = on && available();
}
//----------------------------------------------------------------------------------------
void SoundPlayer::piecePlaced()
{
	if(m_enabled)
	{
		// click
			printf("click\n");
		m_placedSound->play();
	}
}
//----------------------------------------------------------------------------------------
void SoundPlayer::piecesRemoved(int numDisappearing, int numDropping)
{
	if(m_enabled)
	{
		if(numDropping == 0)
		{
			printf("plop\n");
			m_disappearSound->play();
		}
		else if(numDropping < 5)
		{
			printf("drop\n");
			m_fallingSound->play();
		}
		else
		{
			printf("superdrop\n");
			m_manyFallingSound->play();
		}
	}
}
//----------------------------------------------------------------------------------------
void SoundPlayer::gameFinished(bool won)
{
	if(m_enabled)
	{
		if(won)
			m_wonSound->play();
		else
			m_lostSound->play();
	}
}
//----------------------------------------------------------------------------------------
