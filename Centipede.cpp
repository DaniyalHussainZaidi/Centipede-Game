#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

int gameGrid[30][30] = {};

const int x = 0;
const int y = 1;
const int exists = 2;
const int hits = 3;



void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite);
void movePlayer(Sprite& playerSprite, float player[], int mushrooms[][4]);

void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite);
void moveBullet(float bullet[], Clock& bulletClock, float player[], int mushrooms[][4], float centipede[][2], int& centipedeLength, Texture& mushroomTexture, Texture& mushroomTextureHit1, Texture& mushroomTextureHit2,int& score);

void mushroomGeneration(int mushrooms[][4], float player[]);
void drawMushrooms(RenderWindow& window, int mushrooms[][4], Texture& mushroomTexture, Texture& mushroomTextureHit1, Texture& mushroomTextureHit2);

void initializeCentipede(float centipede[][2], Sprite centipedeSprites[], const Texture& centipedeTexture);
void moveCentipede(float centipede[][2], Sprite centipedeSprites[], float speed,int mushrooms[][4]);
void drawCentipede(RenderWindow& window, Sprite centipedeSprites[],float centipede[][2]);

int main()
{
    srand(time(0));

    // Window Graphics Initialization
    // Setting up the graphical window with a specific resolution, title, and style.

    RenderWindow window(VideoMode(resolutionX, resolutionY), "Centipede", Style::Close | Style::Titlebar);
    window.setSize(Vector2u(640, 640));
    window.setPosition(Vector2i(100, 0));

    // Music and Music Files Initialization
    // Loading and playing background music with volume adjustment.

    Music bgMusic;
    bgMusic.openFromFile("Music/g.ogg");
    bgMusic.play();
    bgMusic.setVolume(50);
	
    // Background Initialization
    // Loading and configuring the background texture and sprite.

    Texture backgroundTexture;
    Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/op.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.65));
	
    // Font and Score Text Initialization
    // Loading a font, setting up text properties, and positioning the score display.

	Font font;
	font.loadFromFile("Textures/times new roman.ttf");  
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(35);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(resolutionX - 180, 20);
	
    // Player Initialization
    // Setting initial parameters for the player character, including position and texture.

    float player[2] = {};
    player[x] = (30 / 2) * boxPixelsX;
    player[y] = (30 * 3 / 4) * boxPixelsY;
    Texture playerTexture;
    Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Bullet Initialization
    // Setting up the bullet object with initial parameters, including position and texture.

    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false;
    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Mushrooms Initialization
    // Generating and configuring mushrooms, including textures and hit variations.

    const int numMushrooms = 50;
    int mushrooms[numMushrooms][4];
    Texture mushroomTexture;
    mushroomTexture.loadFromFile("Textures/mushroom.png");
    Texture mushroomTextureHit1;
    mushroomTextureHit1.loadFromFile("Textures/mushroom1.png"); 
    Texture mushroomTextureHit2;
    mushroomTextureHit2.loadFromFile("Textures/mushroom2.png"); 
    mushroomTexture.setRepeated(true); 
    mushroomTexture.setSmooth(true); 

    mushroomGeneration(mushrooms,player);
    
    // Centipede Initialization
    // Setting up the centipede with initial parameters, including segments, speed, and textures.

    const int sizeCentipede = 12;
    float centipede[sizeCentipede][2];
    float centipedeSpeed = 0.5;
	int centipedeLength = 12;      
    Texture centipedeTexture;
    centipedeTexture.loadFromFile("Textures/c_body_left_walk.png"); 
	Sprite centipedeSprites[sizeCentipede];
	
    initializeCentipede(centipede, centipedeSprites, centipedeTexture);

	// Initial Score
    // Initializing the score variable to keep track of the player's progress.

    int Score=0;

    while (window.isOpen()) {

        movePlayer(playerSprite, player, mushrooms);
        window.draw(backgroundSprite);

  		moveCentipede(centipede, centipedeSprites, centipedeSpeed, mushrooms);
		drawCentipede(window, centipedeSprites,centipede);

        drawMushrooms(window, mushrooms, mushroomTexture, mushroomTextureHit1, mushroomTextureHit2);
        drawPlayer(window, player, playerSprite);
        drawBullet(window, bullet, bulletSprite); // 

		moveBullet(bullet, bulletClock, player,mushrooms, centipede, centipedeLength,mushroomTexture, mushroomTextureHit1, mushroomTextureHit2,Score);
		
		scoreText.setString("Score: " + to_string(Score));
        window.draw(scoreText);
        
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                return 0;
            }
        }
        window.display();
        window.clear();
    }
}

// Draw Player Function
// Draws the player sprite on the game window at the current player position.

void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite) {
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}

// Move Player Function
// Handles player movement based on keyboard input, performs collision checks with mushrooms, and updates the player's position.

void movePlayer(Sprite& playerSprite, float player[], int mushrooms[][4]) {

    float X_movement = 0.0;
    float Y_movement = 0.0;

    if (player[x] < 1.0) {
        player[x] = 1.0;
    } else if (player[x] > (29.0 * boxPixelsX) - 1.0) {
        player[x] = 29.0 * boxPixelsX - 1.0;
    }

    if (player[y] < (22.0 * boxPixelsY + 1.0)) {
        player[y] = 22.0 * boxPixelsY + 1.0;
    } else if (player[y] > (29.0 * boxPixelsY - 1.0)) {
        player[y] = 29.0 * boxPixelsY - 1.0;
    }

    if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
        X_movement = -0.4;
    } 
    else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
        X_movement = 0.4;
    } 
    else if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
        Y_movement = -0.4;
    } 
    else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
        Y_movement = 0.4;
    }

    for (int i = 0; i < 50; ++i) {
        if (mushrooms[i][exists] && X_movement != 0.0 &&
            player[x] + X_movement < mushrooms[i][x] + boxPixelsX && player[x] + X_movement + boxPixelsX > mushrooms[i][x] &&
            player[y] < mushrooms[i][y] + boxPixelsY && player[y] + boxPixelsY > mushrooms[i][y]) {
            
            X_movement = 0.0; 
            break;
        }
    }

    
    for (int i = 0; i < 50; ++i) {
        if (mushrooms[i][exists] && Y_movement != 0.0 &&
            player[x] < mushrooms[i][x] + boxPixelsX && player[x] + boxPixelsX > mushrooms[i][x] &&
            player[y] + Y_movement < mushrooms[i][y] + boxPixelsY && player[y] + Y_movement + boxPixelsY > mushrooms[i][y]) {
            
            Y_movement = 0.0; 
            break;
        }
    }

   
    player[x] += X_movement;
    player[y] += Y_movement;
}

// Draw Bullet Function
// Draws the bullet sprite on the game window when it exists.

void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite) {
    if (bullet[exists]) { 
        bulletSprite.setPosition(bullet[x], bullet[y]);
        window.draw(bulletSprite);
    }
}

// Move Bullet Function
// Handles bullet movement, collision checks with mushrooms and centipede segments, and updates the score.

void moveBullet(float bullet[], Clock& bulletClock, float player[], int mushrooms[][4], float centipede[][2], int& centipedeLength, Texture& mushroomTexture, Texture& mushroomTextureHit1, Texture& mushroomTextureHit2, int& score) {
    static int count = 0;
    const float collisionThreshold = 5.0f;

    if (Keyboard::isKeyPressed(Keyboard::Space) && count == 0) {
        bullet[x] = player[x];
        bullet[y] = player[y] - boxPixelsY;
        bullet[exists] = true; 
        count = 1;
        bulletClock.restart();
    }

    if (count > 0) {
        if (bulletClock.getElapsedTime().asMilliseconds() < 2.5)
            return;

        bulletClock.restart();
        bullet[y] -= 10;
        ++count;

        if (bullet[y] < -32) {
            count = 0;
            bullet[exists] = false; 
        }

        for (int i = 0; i < 50; ++i) {
            if (mushrooms[i][exists] && bullet[x] < mushrooms[i][x] + boxPixelsX && bullet[x] + boxPixelsX > mushrooms[i][x] &&
                bullet[y] < mushrooms[i][y] + boxPixelsY && bullet[y] + boxPixelsY > mushrooms[i][y]) {

                mushrooms[i][hits]++;  

                if (mushrooms[i][hits] == 1) {
                    mushroomTextureHit1.loadFromFile("Textures/mushroom1.png");
                } 
				else if (mushrooms[i][hits] == 2) {
                    mushroomTextureHit2.loadFromFile("Textures/mushroom2.png");
                    mushrooms[i][exists] = false;
                    score += 1;
                }


                count = 0;
                bullet[exists] = false; 
                bullet[y] = player[y] - boxPixelsY;
                return;  
            }
        }


        for (int j = 0; j < centipedeLength; ++j) {
            if (bullet[x] > centipede[j][x] - collisionThreshold && bullet[x] < centipede[j][x] + boxPixelsX + collisionThreshold &&
                bullet[y] > centipede[j][y] - collisionThreshold && bullet[y] < centipede[j][y] + boxPixelsY + collisionThreshold) {


                for (int k = j; k < centipedeLength - 1; ++k) {
                    centipede[k][x] = centipede[k + 1][x];
                    centipede[k][y] = centipede[k + 1][y];
                }
                --centipedeLength;
                count = 0;
                bullet[exists] = false; 


                score += 10;

                return;
            }
        }


    }
}


// Mushroom Generation Function
// Generates mushrooms in the game world, avoiding the player's initial position.

void mushroomGeneration(int mushrooms[][4], float player[]) {
    int mushroomsGenerated = 0;

    for (int row = 0; row < 50; ++row) {
        int mushroomsInRow = rand() % 2 + 1; 

        for (int i = 0; i < mushroomsInRow && mushroomsGenerated < 50; ++i) {

            do {
                mushrooms[mushroomsGenerated][x] = rand() % (29 * boxPixelsX);
                mushrooms[mushroomsGenerated][y] = row * boxPixelsY;
            } while (abs(mushrooms[mushroomsGenerated][x] - player[x]) < 3 * boxPixelsX &&
                     abs(mushrooms[mushroomsGenerated][y] - player[y]) < 3 * boxPixelsY);

            mushrooms[mushroomsGenerated][exists] = true;
            mushrooms[mushroomsGenerated][hits] = 0;  
            ++mushroomsGenerated;
        }
    }
}



// Draw Mushrooms Function
// Draws existing mushrooms on the game window, using different textures based on the number of hits.

void drawMushrooms(RenderWindow& window, int mushrooms[][4], Texture& mushroomTexture, Texture& mushroomTextureHit1, Texture& mushroomTextureHit2) {
    for (int i = 0; i < 50; ++i) {
        if (mushrooms[i][exists]) {  
            Sprite mushroomSprite;
            if (mushrooms[i][hits] == 0) {
                mushroomSprite.setTexture(mushroomTexture);
            } else if (mushrooms[i][hits] == 1) {
                mushroomSprite.setTexture(mushroomTextureHit1);
            } else if (mushrooms[i][hits] == 2) {
                mushroomSprite.setTexture(mushroomTextureHit2);
            }
            
            mushroomSprite.setPosition(mushrooms[i][x], mushrooms[i][y]);
            window.draw(mushroomSprite);
        }
    }
}


// Initialize Centipede Function
// Initializes the centipede array and corresponding sprites with initial positions and textures.

void initializeCentipede(float centipede[][2], Sprite centipedeSprites[], const Texture& centipedeTexture) {
    for (int i = 0; i < 12; ++i) {
        centipede[i][x] = (30 - 12 + i) * boxPixelsX;
        centipede[i][y] = 0;
    }
    for (int i = 0; i < 12; ++i) {
        centipedeSprites[i].setTexture(centipedeTexture);
        centipedeSprites[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
        centipedeSprites[i].setPosition(centipede[i][x], centipede[i][y]);
    }
}

// Move Centipede Function
// Handles the movement of the centipede, updating its position based on the defined rules and avoiding collisions with mushrooms.

void moveCentipede(float centipede[][2], Sprite centipedeSprites[], float speed,int mushrooms[][4]) {
    for (int i = 0; i < 12; ++i) {
        if (centipede[i][0] < 0 || centipede[i][0] > resolutionX - boxPixelsX) {
            centipede[i][1] += boxPixelsX;
        }
        if (static_cast<int>(centipede[i][1]) / boxPixelsX % 2 == 0) {
            centipede[i][0] += speed;  
        } else {
            centipede[i][0] -= speed;  
        }
    }
}

// Draw Centipede Function
// Draws the centipede sprites on the game window at their current positions.

void drawCentipede(RenderWindow& window, Sprite centipedeSprites[], float centipede[][2]) {
    for (int i = 0; i < 12; ++i) {
        centipedeSprites[i].setPosition(centipede[i][x], centipede[i][y]);
        window.draw(centipedeSprites[i]);
    }}




