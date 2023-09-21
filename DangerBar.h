/***************************************************************************
                          DangerBar.h  -  description
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

#ifndef DANGERBAR_H
#define DANGERBAR_H

#include <qwidget.h>

/**
  *@author Thorsten Scheuermann
  */

class DangerBar : public QWidget
{
   Q_OBJECT

public:
	DangerBar(QWidget *parent=0, const char *name=0);
	~DangerBar();

signals:
	void overFlow();
	
public slots:
	void setBarSteps(int steps);
	void step();
	void increase(int n);
	void decrease(int n);
	void setPos(int pos);
	void reset();
	
protected:
	virtual void paintEvent(QPaintEvent *pe);
	
	int m_barSteps;
	int m_fill;
};

#endif
