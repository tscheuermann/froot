#ifndef _SOUNDPLAYER_H
#define _SOUNDPLAYER_H

#include <qobject.h>

class Sound;

class SoundPlayer : public QObject
{
	Q_OBJECT

public:
	SoundPlayer(QObject *parent = 0);
	virtual ~SoundPlayer();

	inline bool isEnabled();
	static bool available();

public slots:
	void setEnabled(bool on);
	void piecePlaced();
	void piecesRemoved(int numDisappearing, int numDropping);
	void gameFinished(bool won);

protected:
	bool m_enabled;
	Sound *m_placedSound;
	Sound *m_disappearSound;
	Sound *m_fallingSound;
	Sound *m_manyFallingSound;
	Sound *m_wonSound;
	Sound *m_lostSound;
};

inline bool SoundPlayer::isEnabled()
{
	return m_enabled;
}

#endif
