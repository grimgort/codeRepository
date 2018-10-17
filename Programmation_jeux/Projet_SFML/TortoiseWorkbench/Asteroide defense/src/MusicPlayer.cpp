#include "MusicPlayer.h"


MusicPlayer::MusicPlayer()
    : m_music()
    , m_filenames()
    , m_volume (100.f)
{
    m_filenames[Music::MenuTheme] = "media/Music/MenuTheme.ogg";
    m_filenames[Music::MissionTheme] = "media/Music/MissionTheme.ogg";
}

void MusicPlayer::play (Music::ID theme)
{
    std::string filename = m_filenames[Music::MenuTheme];

    if (!m_music.openFromFile (filename))
    { throw std::runtime_error ("Music " + filename + " could not be loaded."); }

    m_music.setVolume (m_volume);
    m_music.setLoop (true);
    m_music.play();
}

void MusicPlayer::stop()
{
    m_music.stop();
}

void MusicPlayer::setVolume (float volume)
{
    m_volume = volume;
}

void MusicPlayer::setPaused (bool paused)
{
    if (paused)
    { m_music.pause(); }
    else
    { m_music.play(); }
}
