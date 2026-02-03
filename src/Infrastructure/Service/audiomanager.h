#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QMap>

class AudioManager : public QObject
{
    Q_OBJECT
public:
    static AudioManager& instance();

    void playBackgroundMusic(const QString& path);
    void playSound(const QString& name, const QString& path);

    void setMusicVolume(float volume);
    void setSfxVolume(float volume);

    float getMusicVolume() const { return m_musicVolume; }
    float getSfxVolume() const { return m_sfxVolume; }

    void setMuted(bool muted);
    bool isMuted() const { return m_isMuted; }

private:
    explicit AudioManager(QObject *parent = nullptr);

    QMediaPlayer* m_musicPlayer;
    QAudioOutput* m_musicOutput;

    QMap<QString, QSoundEffect*> m_sfxCache;

    float m_musicVolume;
    float m_sfxVolume;
    bool m_isMuted;

    // settings
    const QString KEY_MUSIC_VOL = "audio/music_volume";
    const QString KEY_SFX_VOL = "audio/sfx_volume";
    const QString KEY_MUTED = "audio/muted";

    void updateOutputVolumes();
};

#endif // AUDIOMANAGER_H
