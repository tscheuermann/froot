TEMPLATE	= app
CONFIG		= qt warn_on debug
#CONFIG		= qt warn_on release

HEADERS		= AimingState.h GameState.h Launcher.h PieceSlot.h \
 DangerBar.h GameWidget.h LauncherTurnState.h RemoveState.h \
 FrootApp.h Globals.h MovingState.h ScoreBoard.h GameBoard.h \
 HighscoreTable.h Piece.h GameOverState.h HighscoreWidget.h \
 PieceCounter.h GameSettings.h IdleState.h PieceFactory.h \
 NameInputDialog.h SoundPlayer.h

SOURCES		= AimingState.cpp GameState.cpp Launcher.cpp PieceSlot.cpp \
 DangerBar.cpp GameWidget.cpp LauncherTurnState.cpp RemoveState.cpp \
 FrootApp.cpp Globals.cpp MovingState.cpp ScoreBoard.cpp GameBoard.cpp \
 HighscoreTable.cpp Piece.cpp main.cpp GameOverState.cpp  HighscoreWidget.cpp \
 PieceCounter.cpp GameSettings.cpp IdleState.cpp PieceFactory.cpp \
 NameInputDialog.cpp SoundPlayer.cpp

INTERFACES	 = MenuWidget.ui NameInputDialogBase.ui \
 OptionsDialog.ui AboutWidget.ui

INCLUDEPATH	+= $(QPEDIR)/include
DEPENDPATH	+= $(QPEDIR)/include
LIBS            += -lqpe
TARGET		= froot
