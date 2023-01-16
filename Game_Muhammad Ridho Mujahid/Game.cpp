#include "Game.h"

//Fungsi
void Game::initWorld()
{
    if (!this->worldBackgroundTex.loadFromFile("Textures/latar.jpg"))
    {
        std::cout << "ERROR::GAME::Gagal menampilkan latar" << "\n";
    }

    this->worldBackground.scale(2.f, 2.f);

    this->worldBackground.setTexture(this->worldBackgroundTex);
}

void Game::initVariables()
{
    this->window = nullptr;

    //Game Logic
    this->endGame = false;
    this->points = 0;
    this->nyawa = 10;
    this->enemySpawnTimerMax = 1.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 10;
}

void Game::initTextures()
{
    this->textures["BULLET"] = new sf::Texture();
    this->textures["BULLET"]->loadFromFile("Textures/bullet.png");
}


void Game::initWindow()
{
    this->videoMode.height = 1080;
    this->videoMode.width = 1920;

    this->window = new sf::RenderWindow(this->videoMode, "Game Ridho", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(30);
}

void Game::initFonts()
{
    if (this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
    {
        std::cout << "ERROR::GAME::INITFONTS::Gagal menampilkan font!" << "\n";
    }
}

void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(50);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");

}

//Init Music
/*sf::Music music;
if (!music.openFromFile("Texture/Lost Saga BGM - Wild West.wav"))
    return -1; // error
music.play();
*/

void Game::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(sf::Color::Cyan);
    //this->enemy.setOutlineColor(sf::Color::Green);
    //this->enemy.setOutlineThickness(1.f);
}

void Game::initPlayer()
{
    this->player = new Player();
}

//Constructors / Destructors
Game::Game()
{
    this->initWorld();
    this->initTextures();
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
    this->initPlayer();
}

Game::~Game()
{
    delete this->window;
    delete this->player;

    //delete textures
    for (auto& i : this->textures)
    {
        delete i.second;
    }

    //Delete bullets
    for (auto* i : this->bullets)
    {
        delete i;
    }
}

//Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

//Fungsi
void Game::spawnEnemy()
{
    /*
        @return void

        Spawn musuh dan set tipe, warna dan posisi secara random
        - set tipe secara random
        - set posisi random
        - set warna random
        - tambah musuh ke vector
    */

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    //Tipe musuh random
    int type = rand() % 5;

    switch (type)
    {
    case 0:
        this->enemy.setSize(sf::Vector2f(50.f, 50.f));
        this->enemy.setFillColor(sf::Color::Yellow);
        break;
    case 1:
        this->enemy.setSize(sf::Vector2f(70.f, 70.f));
        this->enemy.setFillColor(sf::Color::Red);
        break;
    case 2:
        this->enemy.setSize(sf::Vector2f(80.f, 80.f));
        this->enemy.setFillColor(sf::Color::Magenta);
        break;
    case 3:
        this->enemy.setSize(sf::Vector2f(100.f, 100.f));
        this->enemy.setFillColor(sf::Color::Blue);
        break;
    case 4:
        this->enemy.setSize(sf::Vector2f(120.f, 120.f));
        this->enemy.setFillColor(sf::Color::Cyan);
        break;
    default:
        this->enemy.setFillColor(sf::Color::Green);
        this->enemy.setSize(sf::Vector2f(150.f, 150.f));
        break;
    }

    //Spawn musuh
    this->enemies.push_back(this->enemy);

}

void Game::pollEvents()
{
    //Event Polling
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::updateInput()
{

    //Gerak player
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->player->move(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        this->player->move(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->player->move(1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        this->player->move(1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->player->move(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        this->player->move(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->player->move(0.f, 1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        this->player->move(0.f, 1.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->player->canAttack())
    {
        this->bullets.push_back(
            new Bullet(
                this->textures["BULLET"],
                this->player->getPos().x,
                this->player->getPos().y,
                0.f,
                -1.f,
                20.f
            )
        );
    }

}

void Game::updateBullets()
{
    unsigned counter = 0;
    for (auto* bullet : this->bullets)
    {
        bullet->update();

        //Bullet culling (top of screen)
        if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
        {
            //Delete bullet
            delete this->bullets.at(counter);
            this->bullets.erase(this->bullets.begin() + counter);
            --counter;

            std::cout << this->bullets.size() << "\n";
        }
         
        ++counter;
    }
}


void Game::updateText()
{
    std::stringstream ss;

    ss << "Points: " << this->points << "\n"
        << "Nyawa: " << this->nyawa;

    this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
    /*
        update spawn musuh
        Gerakan musuh ke bawah.
        Hapus musuh ketika melewati window
    */


    //Update waktu musuh spawn
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimerMax >= this->enemySpawnTimerMax)
        {
            //Spawn musuh dan reset timer
            this->spawnEnemy();
            this->enemySpawnTimer = 3.f;
        }
        else
            this->enemySpawnTimer += 5.f;
    }

    //Gerak dan update musuh
    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;

        this->enemies[i].move(0.f, 5.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->nyawa -= 1;
            std::cout << "Nyawa: " << this->nyawa << "\n";
        }
    }

}

void Game::update()
{
    this->pollEvents();

    if (this->endGame == false)
    {
        this->updateText();
        this->updateEnemies();
    }

    //input
    this->updateInput();

    this->player->update();

    this->updateBullets();

    //End game condition
    if (this->nyawa <= 0)
        this->endGame = true;

}

void Game::renderWorld()
{
    this->window->draw(this->worldBackground);
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    //Rendering all the enemies
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::renderPlayer(sf::RenderTarget& target)
{
    //render player
    this->player->render(*this->window);
}

void Game::render()
{
    /*
        @return void

        - clear old frame
        - render objek
        - display frame di window

        Renders game objek
    */

    this->window->clear();

    //Draw latar
    this->renderWorld();

    //Draw Game objek
    this->renderEnemies(*this->window);

    for (auto* bullet : this->bullets)
    {
        bullet->render(this->window);
    }

    this->renderText(*this->window);

    this->renderPlayer(*this->window);

    this->window->display();
}