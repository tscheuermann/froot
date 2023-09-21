/***************************************************************************
                          HighscoreTable.h  -  description
                             -------------------
    begin                : Sat Jan 12 2002
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

#ifndef HIGHSCORETABLE_H
#define HIGHSCORETABLE_H

#include <qvaluelist.h>
#include <qstring.h>
#include <qobject.h>

/**
  *@author Thorsten Scheuermann
  */

class Config;

class HighscoreTable : public QObject
{
	Q_OBJECT

public:
	HighscoreTable(unsigned int maxEntries, const QString &scoreName);
	~HighscoreTable();

	inline int numEntries() const;
	bool isHighscore(int score);
	void addEntry(int score, QString &name);
	int getScore(int num);
	int getLatestEntryPos() const;
	void clearLatestEntryPos();
	const QString &getName(int num);
	void readScores(Config *config);
	void writeScores(Config *config);
	void resetScores(Config *config);

signals:
	void tableChanged();

protected:
	unsigned int m_maxEntries;
	QString m_scoreName;

	struct Entry
	{
		int score;
		QString name;
	};

	QValueList<struct Entry> m_entryList;
	int m_latestEntry;
};

inline int HighscoreTable::numEntries() const
{
	return m_entryList.count();
}

inline int HighscoreTable::getLatestEntryPos() const
{
	return m_latestEntry;
}

#endif
