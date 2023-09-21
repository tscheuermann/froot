/***************************************************************************
                          NameInputDialog.cpp  -  description
                             -------------------
    begin                : Sun Jan 13 2002
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

#include "NameInputDialog.h"
#include <qlineedit.h>

//----------------------------------------------------------------------------------------
NameInputDialog::NameInputDialog(QWidget *parent = 0, const char *name = 0)
	: NameInputDialogBase(parent, name, true)
{
}
//----------------------------------------------------------------------------------------
NameInputDialog::~NameInputDialog()
{
}
//----------------------------------------------------------------------------------------
QString NameInputDialog::getName(QString &defaultName, QWidget *parent = 0)
{
	NameInputDialog theDialog(parent, "nameInput");
	theDialog.m_nameEdit->setText(defaultName);
	theDialog.m_nameEdit->setSelection(0, defaultName.length());
	theDialog.m_nameEdit->setFocus();
	theDialog.exec();
	return theDialog.m_nameEdit->text();
}
//----------------------------------------------------------------------------------------
