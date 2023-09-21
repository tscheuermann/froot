/***************************************************************************
                          FrootApp.cpp  -  description
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

#include "FrootApp.h"
#include "MenuWidget.h"
#include "GameWidget.h"
#include "AboutWidget.h"
#include "OptionsDialog.h"
#include "GameSettings.h"
#include "HighscoreTable.h"
#include "HighscoreWidget.h"
#include "NameInputDialog.h"
#include "SoundPlayer.h"
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qpe/config.h>
#include <qcheckbox.h>

//----------------------------------------------------------------------------------------
FrootApp::FrootApp(QWidget *parent, const char *name)
	: QWidget(parent,name)
{
//	setFixedSize(240, 320);
	showMaximized();
//	qDebug("%d, %d\n", width(), height());
	setCaption("Froot");

	QPalette myPal(palette());

	myPal.setColor(QColorGroup::Background, QColor(black));

	move(-1, 0);
	setFixedWidth(240);
	m_menuWidget = new MenuWidget(this);
	m_gameWidget = new GameWidget(this);
  m_aboutWidget = new AboutWidget(this);
	m_soundPlayer = new SoundPlayer(this);

	m_menuWidget->setPalette(myPal);
	m_aboutWidget->setPalette(myPal);

	m_menuWidget->move(0, 0);
	m_gameWidget->move(0, 0);
	m_aboutWidget->move(0, 0);

	m_menuWidget->resize(width(), height());
	m_gameWidget->resize(width(), height());
	m_aboutWidget->resize(width(), height());

	m_gameWidget->hide();
	m_aboutWidget->hide();
	m_menuWidget->show();

	// initialize the difficulty settings
	m_settings[0] = new GameSettings(12, 6, 0, 15000);
	m_settings[1] = new GameSettings(8, 7, 0, 20000);
	m_settings[2] = new GameSettings(7, 8, 5, 25000);
	m_settings[3] = new GameSettings(6, 9, 8, 30000);

	m_highscore[0] = new HighscoreTable(10, "BeginnerScores");
	m_highscore[1] = new HighscoreTable(10, "AmateurScores");
	m_highscore[2] = new HighscoreTable(10, "AdvancedScores");
	m_highscore[3] = new HighscoreTable(10, "ProScores");

	Config *config = new Config("FrootApp");
	for(int i=0; i<4; i++)
		m_highscore[i]->readScores(config);
	delete config;

	loadSettings();

	m_menuWidget->m_difficultyCombo->setCurrentItem(m_currentSettings);

	connect(m_highscore[m_currentSettings], SIGNAL(tableChanged()),
					m_menuWidget->m_highscoreWidget, SLOT(repaint()));

	setDifficulty(m_currentSettings);

	connect(m_menuWidget->m_startButton, SIGNAL(clicked()),
					this, SLOT(startGame()));

	connect(m_menuWidget->m_startButton, SIGNAL(clicked()),
					m_gameWidget, SLOT(startGame()));
	connect(m_menuWidget->m_startButton, SIGNAL(clicked()),
					m_menuWidget, SLOT(hide()));
	connect(m_menuWidget->m_startButton, SIGNAL(clicked()),
					m_gameWidget, SLOT(show()));
	connect(m_menuWidget->m_startButton, SIGNAL(clicked()),
					m_gameWidget, SLOT(setFocus()));

	connect(m_menuWidget->m_optionsButton, SIGNAL(clicked()),
					this, SLOT(showOptionsDialog()));

	connect(m_menuWidget->m_aboutButton, SIGNAL(clicked()),
					m_aboutWidget, SLOT(show()));
  connect(m_menuWidget->m_aboutButton, SIGNAL(clicked()),
					m_menuWidget, SLOT(hide()));

	connect(m_aboutWidget->m_backButton, SIGNAL(clicked()),
					m_aboutWidget, SLOT(hide()));
  connect(m_aboutWidget->m_backButton, SIGNAL(clicked()),
					this, SLOT(showMenu()));

	connect(m_menuWidget->m_difficultyCombo, SIGNAL(activated(int)),
					this, SLOT(setDifficulty(int)));
	connect(m_gameWidget, SIGNAL(gameFinished()),
					this, SLOT(showMenu()));
	connect(m_gameWidget, SIGNAL(finalScore(int)),
					this, SLOT(addHighscore(int)));

	QObject::connect(m_gameWidget, SIGNAL(piecePlacedSound()),
									 m_soundPlayer, SLOT(piecePlaced()));
	QObject::connect(m_gameWidget, SIGNAL(piecesRemovedSound(int, int)),
									 m_soundPlayer, SLOT(piecesRemoved(int, int)));
	QObject::connect(m_gameWidget, SIGNAL(gameOver(bool)),
									 m_soundPlayer, SLOT(gameFinished(bool)));
}
//----------------------------------------------------------------------------------------
FrootApp::~FrootApp()
{
	saveSettings();
}
//----------------------------------------------------------------------------------------
void FrootApp::startGame()
{
	for(int i=0; i<4; i++)
		m_highscore[i]->clearLatestEntryPos();

	m_menuWidget->hide();
	m_gameWidget->show();
	m_gameWidget->setFocus();
}
//----------------------------------------------------------------------------------------
void FrootApp::showMenu()
{
	m_gameWidget->hide();
	m_menuWidget->show();
	m_menuWidget->m_startButton->setFocus();
}
//----------------------------------------------------------------------------------------
void FrootApp::setDifficulty(int level)
{
	disconnect(m_highscore[m_currentSettings], SIGNAL(tableChanged()),
						 m_menuWidget->m_highscoreWidget, SLOT(repaint()));

	if((level < 0) || (level >= 4))
		return;
	m_currentSettings = level;
	m_gameWidget->setGameSettings(m_settings[level]);
	m_menuWidget->m_highscoreWidget->setScores(m_highscore[level]);

	connect(m_highscore[level], SIGNAL(tableChanged()),
					m_menuWidget->m_highscoreWidget, SLOT(repaint()));
}
//----------------------------------------------------------------------------------------
void FrootApp::addHighscore(int score)
{
	if(m_highscore[m_currentSettings]->isHighscore(score))
	{
		// ask for name
		m_name = NameInputDialog::getName(m_name, this);

		// add score
		m_highscore[m_currentSettings]->addEntry(score, m_name);

		Config *config = new Config("FrootApp");
		m_highscore[m_currentSettings]->writeScores(config);
		delete config;
	}
}
//----------------------------------------------------------------------------------------
void FrootApp::resetHighscores()
{
	int res;

	res = QMessageBox::warning(this, "Reset Highscores",
														 "Are you sure you want<br>to reset the high scores?",
														 "Yes", "No");
	if(res == 0)
	{
		Config *config = new Config("FrootApp");
		for(int i=0; i<4; i++)
			m_highscore[i]->resetScores(config);
		delete config;
	}
}
//----------------------------------------------------------------------------------------
void FrootApp::showOptionsDialog()
{
	OptionsDialog dialog(this, "OptionsDialog", true);

	connect(dialog.m_resetButton, SIGNAL(clicked()),
					this, SLOT(resetHighscores()));

//	QPalette myPal(dialog.palette());
//	myPal.setColor(QColorGroup::Background, QColor(black));
//	dialog.setPalette(myPal);
//	myPal.setColor(QColorGroup::Foreground, QColor(white));
//	dialog.m_soundCheck->setPalette(myPal);

	dialog.m_soundCheck->setEnabled(m_soundPlayer->available());
	//dialog.m_soundCheck->setEnabled(true);
	dialog.m_soundCheck->setChecked(m_soundPlayer->isEnabled());
	if(dialog.exec())
	{
		m_soundPlayer->setEnabled(dialog.m_soundCheck->isChecked());
		saveSettings();
	}
}
//----------------------------------------------------------------------------------------
void FrootApp::loadSettings()
{
	Config config("FrootApp");

	config.setGroup("Settings");
	m_soundPlayer->setEnabled(config.readBoolEntry("sound", false));
	m_currentSettings = config.readNumEntry("difficulty",
																					m_menuWidget->m_difficultyCombo->currentItem());
}
//----------------------------------------------------------------------------------------
void FrootApp::saveSettings()
{
	Config config("FrootApp");

	config.setGroup("Settings");
	config.writeEntry("sound", m_soundPlayer->isEnabled());
	config.writeEntry("difficulty", m_currentSettings);
}
//----------------------------------------------------------------------------------------
