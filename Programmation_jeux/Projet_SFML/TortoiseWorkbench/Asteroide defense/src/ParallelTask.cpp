#include "ParallelTask.h"

ParallelTask::ParallelTask()
    : m_thread (&ParallelTask::runTask, this)
    , m_finished (false)
{
}

void ParallelTask::execute()
{
    m_finished = false;
    m_elapsedTime.restart();
    m_thread.launch();
}

bool ParallelTask::isFinished()
{
    sf::Lock lock (m_mutex);
    return m_finished;
}

float ParallelTask::getCompletion()
{
    sf::Lock lock (m_mutex);
    return m_elapsedTime.getElapsedTime().asSeconds() / 10.f;
}

void ParallelTask::runTask()
{
    bool ended = false;

    while (!ended)
    {
        sf::Lock lock (m_mutex);

        if (m_elapsedTime.getElapsedTime().asSeconds() >= 10.f) { ended = true; }

    }

    {
        sf::Lock lock (m_mutex);
        m_finished = true;
    }
}
