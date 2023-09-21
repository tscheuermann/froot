/***************************************************************************
                          DangerBar.cpp  -  description
                             -------------------
    begin                : Tue Dec 18 2001
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

#include "DangerBar.h"
#include <qpainter.h>

DangerBar::DangerBar(QWidget *parent, const char *name )
	: QWidget(parent,name)
{
	m_barSteps = 5;
	m_fill = 0;
	setBackgroundColor(QColor(255,0,0));
}

DangerBar::~DangerBar()
{
}

void DangerBar::setBarSteps(int steps)
{
	m_barSteps = steps;
}

void DangerBar::step()
{
	increase(1);
}

void DangerBar::setPos(int pos)
{
	m_fill = pos;
	if(m_fill < 0)
		m_fill = 0;
	else if(m_fill > m_barSteps)
	{
		m_fill = 0;
		emit overFlow();
	}
	update();
}

void DangerBar::increase(int n)
{
	setPos(m_fill+n);
}

void DangerBar::decrease(int n)
{
	setPos(m_fill-n);
}

void DangerBar::reset()
{
	setPos(0);
}
	
void DangerBar::paintEvent(QPaintEvent *pe)
{
	QPainter p(this);
	
	p.setPen(yellow);
	p.setBrush(yellow);
	p.drawRect(0, height()-m_fill*height()/m_barSteps, width(), height());
}
