#include <Application.h>
#include <Utility.h>
#include <State.h>
#include <StateIdentifiers.h>
#include <TitleState.h>
#include <GameState.h>
#include <MenuState.h>
#include <PauseState.h>
#include <SettingsState.h>
#include <GameOverState.h>

const sf::Time Application::timePerFrame = sf::seconds(1.f/60.f);

/************************************************
CONSTRUCTEUR/DESTRUCTEUR
************************************************/
Application::Application()
    : m_window(sf::VideoMode(640, 768),"Gameplay", sf::Style::Close)
    , m_textures()  //Conteneur de texture(textureHolder)
    , m_fonts() //Conteneur de font (fontHolder)
    , m_player()    //class player
    , m_stateStack(State::Context(m_window, m_textures, m_fonts, m_player))
    , m_statisticsText()    //Class SFML Text
    , m_statisticsUpdateTime()  //Class SFML Time
    , m_statisticsNumFrames(0)
{
    m_window.setKeyRepeatEnabled(false);

// Chargement des textures de bases
    m_fonts.load(Fonts::Main, "media/Sansation.ttf");   //chargement de la fonte sous l'identifiant "Main"

    m_textures.load(Textures::TitleScreen, "media/Textures/TitleScreen.png");   //chargement de l'image de départ sous l'identifiant "TitleScreen"
    m_textures.load(Textures::ButtonNormal, "media/Textures/ButtonNormal.png");
    m_textures.load(Textures::ButtonPressed, "media/Textures/ButtonPressed.png");
    m_textures.load(Textures::ButtonSelected, "media/Textures/ButtonSelected.png");

// Caractéristique du text de m_staticsText().
    m_statisticsText.setFont(m_fonts.get(Fonts::Main));
    m_statisticsText.setPosition(5.f, 5.f);
    m_statisticsText.setCharacterSize(10u);

    registerStates();
    //Initialise la pile des stack avec la page de titre
    m_stateStack.pushState(States::Title);
}



/************************************************
METHODE
************************************************/
/*
Lance le jeux
dt = pas de temps de chaque frame. Plus dt est élevé, plus le jeux lag.
timePerFrame = nombre maximum de fram par seconde (60fps).
*/
void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero; // define time of each frame (initialisé à 0)

// RQ : If Application lag, you can take it smoother with called multi time. Look lesson on it(p24).
//      You can activate V-Sync for adapts time step with your monitory(p24)
    while (m_window.isOpen()) //On fait tourner l'application tant que la fenêtre principal est ouverte.
    {
        sf::Time dt = clock.restart();  //on remet à 0 le pas de temps
        timeSinceLastUpdate += dt;
        //??
        while (timeSinceLastUpdate > timePerFrame )
        {
            timeSinceLastUpdate -= timePerFrame ;
            //Appel la pile des événements
            processInput();
            //Met à jour l'ensemble des calcul du jeux en fonction du dt entre 2 frame
            update(timePerFrame);

            //Ferme l'application si le stack des fenêtre est vide
            if (m_stateStack.isEmpty()) m_window.close();
        }
        //Ecrit le nombre de frame maximum(performence du jeux)
        updateStatistics(dt);
        //Trace les texture
        render();
    }
}

// Permet de gérer les événements
void Application::processInput()
{
    sf::Event event;
    //On associe les événement à la fenêtre
    while (m_window.pollEvent(event))
    {
        //On applique les actions des événements du stack courant (jeux par exemple)
        m_stateStack.handleEvent(event);

        //On ferme la fenêtre si on clic sur la croix
        if (event.type == sf::Event::Closed) m_window.close();
    }
}

// Mise a jour du jeux en fonction des événements
void Application::update(sf::Time dt)
{
    m_stateStack.update(dt);
}

// applique les objets
void Application::render()
{
    m_window.clear(); // Fill screen with black color (default)
    m_stateStack.draw(); // apply player on screen
    m_window.setView(m_window.getDefaultView());
    m_window.draw(m_statisticsText); //montre les statistique du jeux dans la fenetre

    m_window.display(); // update screen
}

/*
Donne les performance du jeux
*/
void Application::updateStatistics(sf::Time dt)
{
    m_statisticsUpdateTime += dt;
    m_statisticsNumFrames += 1;

    if (m_statisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_statisticsText.setString(
            "Frames / Second = " + toString(m_statisticsNumFrames) + "\n" +
            "Time / Update = " + toString(m_statisticsUpdateTime.asMicroseconds() / m_statisticsNumFrames) + "us");

        m_statisticsUpdateTime -= sf::seconds(1.0f);
        m_statisticsNumFrames = 0;
    }
}

/*
Enregistre les différent Stack(état) possible.
*/
void Application::registerStates()
{
    m_stateStack.registerState<TitleState>(States::Title);
    m_stateStack.registerState<MenuState>(States::Menu);
    m_stateStack.registerState<PauseState>(States::Pause);
    m_stateStack.registerState<GameState>(States::Game);
    m_stateStack.registerState<SettingsState>(States::Settings);
    m_stateStack.registerState<GameOverState>(States::GameOver);
}

