/***************************************************************************
                          FrootApp.h  -  description
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

#ifndef FROOTAPP_H
#define FROOTAPP_H

#include <qwidget.h>

/**
  *@author Thorsten Scheuermann
  */

class MenuWidget;
class GameWidget;
class AboutWidget;
class GameSettings;
class HighscoreTable;
class SoundPlayer;

class FrootApp : public QWidget
{
   Q_OBJECT

public:
	FrootApp(QWidget *parent=0, const char *name=0);
	~FrootApp();

public slots:
	void startGame();
	void showMenu();
	void setDifficulty(int level);
	void addHighscore(int score);
	void showOptionsDialog();
	void resetHighscores();

protected:
	void loadSettings();
	void saveSettings();

	MenuWidget *m_menuWidget;
	GameWidget *m_gameWidget;
	AboutWidget *m_aboutWidget;
	SoundPlayer *m_soundPlayer;

	int m_currentSettings;
	GameSettings *m_settings[4];
	HighscoreTable *m_highscore[4];
	QString m_name;			// the last name entered for a highscore
};

#endif
