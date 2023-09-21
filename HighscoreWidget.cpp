/***************************************************************************
                          HighscoreWidget.cpp  -  description
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

#include "HighscoreWidget.h"
#include "HighscoreTable.h"
#include <qpainter.h>

//----------------------------------------------------------------------------------------
HighscoreWidget::HighscoreWidget(QWidget *parent, const char *name)
	: QWidget(parent,name)
{
	m_scoreTable = NULL;
}
//----------------------------------------------------------------------------------------
HighscoreWidget::~HighscoreWidget()
{
}
//----------------------------------------------------------------------------------------
void HighscoreWidget::setScores(HighscoreTable *scoreTable)
{
	m_scoreTable = scoreTable;
	repaint();
}
//----------------------------------------------------------------------------------------
void HighscoreWidget::paintEvent(QPaintEvent *)
{
	QPainter paint(this);
	int i;
	int y;
	int dy = fontMetrics().height();
	int indexWidth = fontMetrics().width("99.");
	int scoreWidth = fontMetrics().width("999999");
	QString tString;
	
	paint.setPen(white);
	for(i=0, y=0; i<10; i++, y+=dy)
	{
		if(i == m_scoreTable->getLatestEntryPos())
			paint.setPen(green);
		else
			paint.setPen(white);

		// print number (right-aligned)
		tString = QString::number(i+1) + '.';
		paint.drawText(0, y, indexWidth, dy, AlignRight|AlignVCenter, tString);
	}

	if(!m_scoreTable)
		return;

	for(i=0, y=0; i<m_scoreTable->numEntries(); i++, y+=dy)
	{
		if(i == m_scoreTable->getLatestEntryPos())
			paint.setPen(green);
		else
			paint.setPen(white);

		// print name (left-aligned)
		paint.drawText(indexWidth+5, y, width()-indexWidth-scoreWidth, dy,
									 AlignLeft|AlignVCenter, m_scoreTable->getName(i));
		// print score (right-aligned)
		tString = QString::number(m_scoreTable->getScore(i));
		paint.drawText(width()-scoreWidth, y, scoreWidth, dy,
									 AlignRight|AlignVCenter, tString);
	}
}
//----------------------------------------------------------------------------------------
