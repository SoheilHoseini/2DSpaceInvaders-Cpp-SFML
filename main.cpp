//Soheil Hoseini
//Mojtaba Safari

#include<iostream>
#include <fstream>
#include <string> 
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp> //to play sounds in the game
#include <math.h>
#include <cstdlib>
#include<vector>

using namespace sf; //with this we dont need to write sf::  anymore


class Bullet
{
public:
    Sprite shape;

    Bullet(Texture *texture, Vector2f pos)
    {
        this->shape.setTexture(*texture);

        this->shape.setScale(0.09f, 0.09f);  //size of bullets

        this->shape.setPosition(pos); // setting te position of bullets at the player position
    }

    ~Bullet() {}  //destructor

};

class Player
{
public:
    Sprite shape;    
    Texture *texture;
    int HP;
    int HPmax;
    std::vector<Bullet> bullets;   //to shoot bullet

    Player(Texture *texture)    //constructor
    {
        this->HPmax = 3;//number of lifes that the player has
        this->HP = this->HPmax;

        this->texture = texture;
        this->shape.setTexture(*texture);

        this->shape.setScale(0.2f, 0.2f);  //minimizing the textures
    }

    ~Player()  {}    //destructor
};

class Enemy
{
public:
    Sprite shape;  //shape of the enemies

    int HP;
    int  HPMax;  //maximum number of lifes that the enemies have

    Enemy(Texture *texture, Vector2u windowSize)
    {
        this->HPMax = rand() % 5 + 1;   //enemies have lifes between 1 to 5 lifes
        this->HP = this->HPMax;

        this->shape.setTexture(*texture); //set the enemy texture that we send in

        this->shape.setScale(0.3f, 0.3f); //set the size of enemy spaceships

        this->shape.setPosition(rand()% (int)(windowSize.x - this->shape.getGlobalBounds().width), 0.f);
        
    }                                                            
    
    ~Enemy(){} //destructor
    
};

class EnemyBullet
{
public:
    Sprite shape;  //shape of the enemy bullet

    EnemyBullet(Texture *texture, Vector2u windowSize)
    {
        this->shape.setTexture(*texture); //set the enemy texture that we send in

        this->shape.setScale(0.1f, 0.1f); //set the size of enemy bullets

        this->shape.setPosition(rand()% (int)(windowSize.x - this->shape.getGlobalBounds().width), 0.f);
    }                                     
    
    ~EnemyBullet(){} //destructor
    
};

class Heart
{
public:
    Sprite shape;  //shape of the enemy bullet

    Heart(Texture *texture, Vector2u windowSize)
    {
        this->shape.setTexture(*texture); //set the enemy texture that we send in

        this->shape.setScale(0.3f, 0.3f); //set the size of enemy bullets

        this->shape.setPosition(rand()% (int)(windowSize.x - this->shape.getGlobalBounds().width), 0.f);
    }                                     
    
    ~Heart(){} //destructor
    
};




int main()
{

    srand(time(NULL));

    //Creat and open a window for the game. VideoMode and RenderWindow classes in SFML
    RenderWindow window(VideoMode(800, 600), "Space Invaders!", Style::Default);
    window.setFramerateLimit(60);

    //=====================================================Font=====================================================//
    Font font;
    font.loadFromFile("Fonts/Dosis-Light.ttf");


    //=====================================================Textures=================================================// 
    Texture playerTex;
    playerTex.loadFromFile("Textures/noborderjet.png");

    Texture enemyTex;
    enemyTex.loadFromFile("Textures/noborderenemy.png");

    Texture bulletTex;
    bulletTex.loadFromFile("Textures/noborderbullet.png");

    Texture enemyBulletTex;
    enemyBulletTex.loadFromFile("Textures/enemybullet.png");

    Texture heartTex;
    heartTex.loadFromFile("Textures/heart.png");
    
    //Background Pic
    Texture backGroundTex;//Create a texture to hold a graphic on the GPU
    backGroundTex.loadFromFile("Textures/space555.jpg");//Load a graphic into the texture
    Sprite spriteBackGround;//Create a sprite
    spriteBackGround.setTexture(backGroundTex);//Attach the texture to the sprite
    spriteBackGround.setPosition(0,0);//Set the spriteBackGround to cover the screen
    
    
    Sprite explosion_;
    Texture explosion_tex;
    explosion_tex.loadFromFile("Textures/boom.png");
    explosion_.setTexture(explosion_tex);
    explosion_.setScale(0.1f, 0.1f);
    explosion_.setPosition(-100.f, -100.f);
    int explosion_timer = 6;
    float timer = 0;
    std::string s;
    

    //=====================================================Texts====================================================//
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10.f, 550.f);//set the position of "Score"


    //UI init   ----> Timer
    Text timer_text;
    timer_text.setFont(font);
    timer_text.setCharacterSize(40);
    timer_text.setFillColor(Color::White);
    timer_text.setPosition(220.f, 550.f);//set the position of "Score"

    //UI init   ----> best_score
    Text best_score_text;
    best_score_text.setFont(font);
    best_score_text.setCharacterSize(20);
    best_score_text.setFillColor(Color::Green);
    best_score_text.setPosition(10.f, 10.f);//set the position of "Score"


    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(250.f, 250.f);//show the "Game Over" at the center of the screen
    gameOverText.setString("GAME OVER!");


    Text winText;
    winText.setFont(font);
    winText.setCharacterSize(60);
    winText.setFillColor(Color::Green);
    winText.setPosition(300.f, 250.f);//show the "You Win" at the center of the screen
    winText.setString("You Win!");


    Text noBulletText;
    noBulletText.setFont(font);
    noBulletText.setCharacterSize(35);
    noBulletText.setFillColor(Color::Red);
    noBulletText.setPosition(190.f, 270.f);//show the "No Missile" at the center of the screen
    noBulletText.setString("You Have No Missiles Anymore!");


    Text missileCntText; //making a text object from Text class in sfml to show some texts on the screen
    missileCntText.setFont(font); //load the font from the font folder
    missileCntText.setCharacterSize(40);
    missileCntText.setFillColor(Color::White);//color of the text
    missileCntText.setPosition(420.f, 550.f);
    

    Text hpText; //making a text object from Text class in sfml to show some texts on the screen
    hpText.setFont(font); //load the font from the font folder
    hpText.setCharacterSize(40);
    hpText.setFillColor(Color::Black);//color of the text
    hpText.setPosition(680.f, 550.f);

    Text eHpText; //making a text object to show some texts on the screen  for enemy ships
    eHpText.setFont(font); 
    eHpText.setCharacterSize(12);
    eHpText.setFillColor(Color::White);

    
    //=====================================================Inits====================================================//
    
    //Player init
    int score = 0;
    int bCnt = 500;//Bullets limit 1000
    Player player(&playerTex);
    int shootTimer = 20;   //shooting timer

    //Enemy init
    int enemySpawnTimer = 0;
    std::vector<Enemy> enemies;  //making enemy vector

    //enemy bullet init
    int enemyBulletSpawnTimer = 0;
    std::vector<EnemyBullet> enBullets;

    //heart init
    int heartSpawnTimer = 0;
    std::vector<Heart> hearts;



    //=====================================================Sounds====================================================//
    SoundBuffer backGroundBuffer;
    backGroundBuffer.loadFromFile("Sounds/background.wav");
    Sound backGroundMusic;//Sound object
    backGroundMusic.setBuffer(backGroundBuffer);//Attach the backGroundBuffer to the backGroundMusic

    
    SoundBuffer explosionBuffer;
    explosionBuffer.loadFromFile("Sounds/explosion.wav");
    Sound explosionMusic;
    explosionMusic.setBuffer(explosionBuffer);


    SoundBuffer shootdBuffer;
    shootdBuffer.loadFromFile("Sounds/shoot.wav");
    Sound shootMusic;
    shootMusic.setBuffer(shootdBuffer);


    SoundBuffer playerdamageBuffer;
    playerdamageBuffer.loadFromFile("Sounds/damageplayer.wav");
    Sound playerdamageMusic;
    playerdamageMusic.setBuffer(playerdamageBuffer);
    

    SoundBuffer heartBuffer;
    heartBuffer.loadFromFile("Sounds/heart.wav");
    Sound heartMusic;
    heartMusic.setBuffer(heartBuffer);


    SoundBuffer touchEnemyBuffer;
    touchEnemyBuffer.loadFromFile("Sounds/touchEnemy.wav");
    Sound touchEnemyMusic;
    touchEnemyMusic.setBuffer(touchEnemyBuffer);
    


    backGroundMusic.play();//play the backgriund music
                                  
    while (window.isOpen())//the game's main loop
    {
        

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)//close the window when X button is pressed
                window.close();
        }

        

        if(player.HP > 0 && score <= 150 && bCnt > 0)//if the player dies or get to 150 scores, everything will stop and we dont update the game any more
        {
            

            //Player movements
            if(Keyboard::isKeyPressed(Keyboard::W))
                player.shape.move(0.f, -10.f);
            if(Keyboard::isKeyPressed(Keyboard::A))
                player.shape.move(-10.f, 0.f);
            if(Keyboard::isKeyPressed(Keyboard::S))
                player.shape.move(0.f, 10.f);
            if(Keyboard::isKeyPressed(Keyboard::D))
                player.shape.move(10.f, 0.f);


            //keeping our spaceship within the window
            if(player.shape.getPosition().x <= 0)  //left boundery
                player.shape.setPosition(0.f, player.shape.getPosition().y);

            if(player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width)  //right boundery
                player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);

            if(player.shape.getPosition().y <= 0)  //top boundery
                player.shape.setPosition(player.shape.getPosition().x, 0.f);

            if(player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) //bottom boundery  
                player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);


            //Update Controls
            if(shootTimer < 10)//the less this timer is, the more missiles could be shot
                shootTimer++;


            //check explosion_timer
            if(explosion_timer <= 5)
                explosion_timer++;
            else
                explosion_.setPosition(-100.f, -100.f);

            //timer
            timer += 0.03;




            if(Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 10)  //shooting
            {
                shootMusic.play();
                bCnt--;
                player.bullets.push_back(Bullet(&bulletTex, player.shape.getPosition())); //at the pos of player
                shootTimer = 0;  //reset timer
            }

        
            //Bullets
            for (size_t i = 0; i < player.bullets.size(); i++)
            {
                //Move
                player.bullets[i].shape.move(0.f , -20.f);   //shooting upward

                //going out of window bounds
                if(player.bullets[i].shape.getPosition().x > window.getSize().x)
                {
                    player.bullets.erase(player.bullets.begin() + i);
                    break; //every time we remove the bullet we should break this loop to avoid crashing of the game
                }
                

                //Enemy collision
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    //when the bullets hit any enemy ship, it will be destroyed
                    if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
                    {
                        if(enemies[k].HP <= 1)//when the life of any enemy goes under 1 (by any shoot one life) it'll die
                        {

                            explosion_.setPosition(enemies[k].shape.getPosition().x, enemies[k].shape.getPosition().y);
                            explosion_timer = 0;

                            score += enemies[k].HPMax;//everytime we kill an enemy at position "k" our score will increase(the tougher the enemy the more score we get)
                            explosionMusic.play();
                            enemies.erase(enemies.begin() + k);//kill the enemy
                        }    

                        else
                            enemies[k].HP--;//Enemy at position "i" take damage 


                        player.bullets.erase(player.bullets.begin() + i);
                        break;
                    }
                
                }
            }


            //Enemy
            if(enemySpawnTimer < 55)//the greater this number, the slower enemies
                enemySpawnTimer++;

            //enemy spawn
            if (enemySpawnTimer >= 55)
            {
                enemies.push_back(Enemy(&enemyTex, window.getSize()));
                enemySpawnTimer = 0;//reset timer
            }

            for(size_t i = 0; i < enemies.size(); i++)
            {
                enemies[i].shape.move(0.f, 1.f); // setting the direction and speed of the enemy

                //clear the enemies that hit the window boundery
                if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }
                
                //clear the enemies that hit the player
                if(enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
                {
                    enemies.erase(enemies.begin() + i);
                    touchEnemyMusic.play();
                    player.HP--; //when the player hits the enemies, it gets damaged
                    break;
                }

            }



            //Enemy bullest
            if(enemyBulletSpawnTimer < 100)//the greater this number, the slower bullets
                enemyBulletSpawnTimer++;

            //enemy bullets spawn
            if (enemyBulletSpawnTimer >= 100)
            {
                enBullets.push_back(EnemyBullet(&enemyBulletTex, window.getSize()));
                enemyBulletSpawnTimer = 0;//reset timer
            }
        
            for (size_t i = 0; i < enBullets.size() ; i++)
            {
                enBullets[i].shape.move(0.f, 4.f);// setting the direction and speed of the enemy bullets

                //clear the enemy bullets that hit the player and decrease the player's life
                if(enBullets[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
                {
                    enBullets.erase(enBullets.begin() + i);
                    playerdamageMusic.play();//play a sound when hits the player
                    player.HP--;
                    break;
                }
            }
            


            //Hearts
            if(heartSpawnTimer < 300)//the greater this number, the slower bullets
                heartSpawnTimer++;

            //Hearts spawn
            if (heartSpawnTimer >= 300)
            {
                hearts.push_back(Heart(&heartTex, window.getSize()));
                heartSpawnTimer = 0;//reset timer
            }

            for (size_t i = 0; i < hearts.size() ; i++)
            {
                hearts[i].shape.move(0.f, 1.f);// setting the direction and speed of the hearts

                //clear the hearts that hit the player and increase the player's life
                if(hearts[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
                {
                    heartMusic.play();
                    hearts.erase(hearts.begin() + i);
                    player.HP++;
                    break;
                }
            }

            //UI Update ----> update the score each time we hit an enemy
            scoreText.setString("Score : " + std::to_string(score));//cast int to string
            hpText.setString("Lifes : " + std::to_string(player.HP));

            missileCntText.setString("Missiles : " + std::to_string(bCnt));
            timer_text.setString("Time : " + std::to_string((int)timer));

        }

        

        //=====================================================Drawings==================================================//

        window.clear(); //clear everything from the last frame
        window.draw(spriteBackGround);//Draw our game background
        //explosion
        window.draw(explosion_);


        //Player Drawing
        window.draw(player.shape);
        
        //Bullets Drawing
        for (size_t i = 0; i < player.bullets.size(); i++)
        {
            window.draw(player.bullets[i].shape);
        }
        
        //Enemy Drawing
        for (size_t i = 0; i < enemies.size(); i++)  //every time we run this, the enemy would be in a random position
        {
            eHpText.setString(std::to_string(enemies[i].HP) + "/" + std::to_string(enemies[i].HPMax));
            eHpText.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y - eHpText.getGlobalBounds().height);
            window.draw(eHpText); //show texts at the position of any enemy 

            window.draw(enemies[i].shape);
        }

        //Enemy Bullets Drawing
        for (size_t i = 0; i < enBullets.size(); i++)
        {
            window.draw(enBullets[i].shape);
        }
        
        //Hearts  Drawing
        for (size_t i = 0; i < hearts.size(); i++)
        {
            window.draw(hearts[i].shape);
        }


        //UI  ----> draw the texts
        
        window.draw(scoreText);
        window.draw(hpText);

        window.draw(timer_text);

        window.draw(missileCntText);

        if(player.HP <= 0)

            window.draw(gameOverText);//when the player's life goes to 0 it is Game Over

            
            std::ifstream MyReadFile("best_scores.txt");
            getline(MyReadFile, s);
            int best_score=std::stoi(s);
            getline(MyReadFile, s);
            int best_time = std::stoi(s);
            if(score > best_score)
                best_score = score;
            if(timer > best_time)
                best_time = timer;
            MyReadFile.close();

            //write
            std::ofstream MyWriteFile("best_scores.txt");
            MyWriteFile << std::to_string(best_score) + '\n' + std::to_string(best_time);
            MyWriteFile.close();
            
            //print
            best_score_text.setString("Best Score : " + std::to_string(best_score) + "\nBest Time : " + std::to_string(best_time));
            window.draw(best_score_text);

        if(score > 150)//if the player could get to 150 scores he/she wins!
            window.draw(winText);

        if(bCnt <= 0)
            window.draw(noBulletText);
        
        window.display();//shows everything we just drew
    }

    return 0;
}