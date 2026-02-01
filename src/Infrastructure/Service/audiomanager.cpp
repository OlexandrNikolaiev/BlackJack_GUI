#include "audiomanager.h"
#include "../Helpers/settingshelper.h"

#include <QUrl>

AudioManager& AudioManager::instance()
{
    static AudioManager _instance;
    return _instance;
}

AudioManager::AudioManager(QObject *parent) : QObject(parent)
{
    m_musicPlayer = new QMediaPlayer(this);
    m_musicOutput = new QAudioOutput(this);
    m_musicPlayer->setAudioOutput(m_musicOutput);

    m_musicPlayer->setLoops(QMediaPlayer::Infinite);

    m_musicVolume = SettingsHelper::getValue(KEY_MUSIC_VOL, 0.5f).toFloat();
    m_sfxVolume   = SettingsHelper::getValue(KEY_SFX_VOL, 0.5f).toFloat();
    m_isMuted     = SettingsHelper::getValue(KEY_MUTED, false).toBool();

    updateOutputVolumes();
}

void AudioManager::playBackgroundMusic(const QString &path)
{
    m_musicPlayer->setSource(QUrl(path));
    m_musicPlayer->play();
}

void AudioManager::playSound(const QString &name, const QString &path)
{
    if (m_isMuted) return;

    if (!m_sfxCache.contains(name)) {
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setSource(QUrl(path));
        effect->setVolume(m_sfxVolume);
        m_sfxCache.insert(name, effect);
    }

    m_sfxCache[name]->play();
}

void AudioManager::setMusicVolume(float volume)
{
    m_musicVolume = qBound(0.0f, volume, 1.0f);
    SettingsHelper::setValue(KEY_MUSIC_VOL, m_musicVolume);
    updateOutputVolumes();
}

void AudioManager::setSfxVolume(float volume)
{
    m_sfxVolume = qBound(0.0f, volume, 1.0f);
    SettingsHelper::setValue(KEY_SFX_VOL, m_sfxVolume);

    for(auto effect : m_sfxCache) {
        effect->setVolume(m_isMuted ? 0 : m_sfxVolume);
    }
}

void AudioManager::setMuted(bool muted)
{
    m_isMuted = muted;
    SettingsHelper::setValue(KEY_MUTED, m_isMuted);
    updateOutputVolumes();
}

void AudioManager::updateOutputVolumes()
{
    m_musicOutput->setVolume(m_isMuted ? 0 : m_musicVolume);

    for(auto effect : m_sfxCache) {
        effect->setVolume(m_isMuted ? 0 : m_sfxVolume);
    }
}
