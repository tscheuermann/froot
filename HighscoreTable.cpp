/***************************************************************************
                          HighscoreTable.cpp  -  description
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

#include "HighscoreTable.h"
#include <qpe/config.h>

HighscoreTable::HighscoreTable(unsigned int maxEntries, const QString &scoreName)
	: QObject()
{
	m_maxEntries = maxEntries;
	m_scoreName = scoreName;
	m_latestEntry = -1;
}
//----------------------------------------------------------------------------------------
HighscoreTable::~HighscoreTable()
{
}
//----------------------------------------------------------------------------------------
bool HighscoreTable::isHighscore(int score)
{
	if(m_entryList.count() < m_maxEntries)
		return true;

	return (score > m_entryList.last().score);
}
//----------------------------------------------------------------------------------------
void HighscoreTable::addEntry(int score, QString &name)
{
	int pos = 0;

	if(!isHighscore(score))
		return;

	// find the right place to insert the score
	QValueList<Entry>::Iterator it = m_entryList.begin();
	struct Entry newEntry;

	newEntry.score = score;
	newEntry.name = name;

	while((score <= (*it).score) && (it != m_entryList.end()))
	{
		it++;
		pos++;
	}

	if((it == m_entryList.end()) && score <= (*it).score)
		m_entryList.append(newEntry);
	else
		m_entryList.insert(it, newEntry);

	if(m_entryList.count() > (unsigned int)m_maxEntries)
		m_entryList.remove(m_entryList.fromLast());

	m_latestEntry = pos;

	emit tableChanged();
}
//----------------------------------------------------------------------------------------
void HighscoreTable::clearLatestEntryPos()
{
	m_latestEntry = -1;
}
//----------------------------------------------------------------------------------------
int HighscoreTable::getScore(int num)
{
	if((num < 0) || ((unsigned int)num >= m_entryList.count()))
		return 0;
	return m_entryList[num].score;
}
//----------------------------------------------------------------------------------------
const QString &HighscoreTable::getName(int num)
{
	if((num < 0) || ((unsigned int)num >= m_entryList.count()))
		return QString::null;
	return m_entryList[num].name;
}
//----------------------------------------------------------------------------------------
void HighscoreTable::writeScores(Config *config)
{
	QString key;

	config->setGroup(m_scoreName);
	config->writeEntry("numEntries", (int)(m_entryList.count()));
	for(unsigned int i=0; i<m_entryList.count(); i++)
	{
		key = "Name" + QString::number(i);
		config->writeEntry(key, m_entryList[i].name);
		key = "Score" + QString::number(i);
		config->writeEntry(key, m_entryList[i].score);
	}
}
//----------------------------------------------------------------------------------------
void HighscoreTable::readScores(Config *config)
{
	QString key;
	unsigned int n;
	Entry e;

	m_entryList.clear();

	config->setGroup(m_scoreName);
	n = config->readNumEntry("numEntries", 0);
	for(unsigned int i=0; i<n; i++)
	{
		key = "Name" + QString::number(i);
		e.name = config->readEntry(key, "");
		key = "Score" + QString::number(i);
		e.score = config->readNumEntry(key, 0);
		m_entryList.append(e);
	}
	emit tableChanged();
}
//----------------------------------------------------------------------------------------
void HighscoreTable::resetScores(Config *config)
{
	QString key;

	config->setGroup(m_scoreName);
	config->writeEntry("numEntries", 0);
	m_entryList.clear();
	emit tableChanged();
}
//----------------------------------------------------------------------------------------
