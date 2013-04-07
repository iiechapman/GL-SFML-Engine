#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <cmath>

#include "TextureHandler.h"
#include "GameTypes.h"
#include "Sprite.h"
#include "KeyCodes.h"
#include "Input.h"

using namespace std;

/*
 * Basic OpenGL 2d mario engine
 * Evan Chapman 2013
 */

//Test Objects
vector<Sprite*> sprite;
Sprite* marioSprite;
Sprite* testSprite;
Sprite* blockSprite;
Sprite* mario;
Sprite* hammer;

//Blank Variables
vector2d_t tempVec;
vector2d_t secondVec;

//Use blank canvas for animation
vector<int> blankAnimation;

//Texture Variables
TextureHandler* textureLoader;
GLuint totalTextures;
GLuint bgTexID;
GLuint blockTexID[4];
GLuint woodTexID;
GLuint grassTexID;
GLuint sandTexID;
GLuint hammerTexID;
GLuint selectedBlockID[3];
int blockSelection = 0;


string windowTitle = "Super Mario Open - By Evan Chapman";
string title = "Super Mario Open - By Evan Chapman";
bool running;
bool falling;
bool jumping;
bool stopped;
bool didCollide;
bool allowJump = false;
bool jumpLetGo = true;

dimensions_t    position;
dimensions_t    deltaPosition;
dimensions_t    size;

//Window constants
const int WINDOWWIDTH     = 1024;
const int WINDOWHEIGHT    = 768;
const int WINDOWPOSX      = 300;
const int WINDOWPOSY      = 300;
const float ASPECTRATIO = WINDOWWIDTH / WINDOWHEIGHT;

//Game Constants
const int   MAX_FPS         =  60;

bool        PLAYMUSIC       = true;
bool        PLAYSOUND       = true;
bool        DEPTH_ENABLED   = false;
bool        FULLSCREEN      = false;

//Timing Variables
float timeChunk = .001;

//Camera variables
const float turnAngle = 100;
const float turnAmount = 10;
float CAMERAOFFSET = 0.0f;
float CAMERAPOSX = 0.0f;
float lastCameraPos = 0.0f;

//Mouse Variables
dimensions_t MOUSEPOS;

//tile variables
dimensions_t tilePos;
dimensions_t tileSize;

//animation vars
float marioAnimTime     = 0.0f;
float animDelayTotal    = 1.00f;
float animTick          = 0.068f;
float animDelta         = 0.0f;
float animLastTime      = 0.0f;

//time vars
unsigned long long   delta   = 0.002f;
//This is a test

float   lastTime          =  0.0f;
float   curTime           =  0.0f;
float   deltaTime         =  0.0f;
float   timeOfLastDraw    =  0.0f;
long double   deltaDraw         =  0.0f;

//game constants
const float BACKGROUNDWIDTH     =   1024.0f;
const float BACKGROUNDHEIGHT    =   768.0f;
const float BACKGROUNDX         =   0.0f;
const float BACKGROUNDY         =   768.0f;
const float BACKGROUNDSCROLL    =   .1f;

const int DRAWROWS            = 12;
const int DRAWCOLUMNS         = 16;

const int DRAWWIDTH           =   WINDOWWIDTH/DRAWCOLUMNS;
const int DRAWHEIGHT          =   WINDOWHEIGHT/DRAWROWS;

const float SCROLLRIGHT         =   500;
const float SCROLLLEFT          =   300;

const float MARIOX          = 400;
const float MARIOY          = 200;
const float MARIOWIDTH      = DRAWWIDTH;
const float MARIOHEIGHT     = DRAWHEIGHT;
const float MARIOANIMDELAY  = .15;
const dimensions_t resetPosition {MARIOX, MARIOY,0};

const float MARIOFRICTIONX  = 1000.0f;

const float MARIOACCEL      = 3000.0f;
const float MARIOMAXVELX    = 400.0f;
const float MARIOMAXVELY    = 9999.0f;
const float MARIORUNDIFF    = 200.0f;

const float MARIOJUMP       =  1000.0f;
const float MARIOGRAV       = -4000.0f;
const float MARIOMAXGRAV    = -2000.0f;

const float MARIOHIGHJUMPGRAV       = MARIOGRAV + MARIOJUMP * 1.5;
const float MARIOHIGHJUMPMAXGRAV    = MARIOMAXGRAV;

const float MARIODEADGRAV           = MARIOGRAV + MARIOJUMP * 2.5;
const float MARIODEADMAXGRAV        = MARIOMAXGRAV + MARIOJUMP/4;

const float BLOCKX          = DRAWWIDTH;
const float BLOCKY          = 2 * DRAWHEIGHT;
const float BLOCKWIDTH      = DRAWWIDTH;
const float BLOCKHEIGHT     = DRAWHEIGHT;
const float BLOCKANIMDELAY  = .10;

const float TILEX          = DRAWWIDTH * -1;
const float TILEY          = DRAWHEIGHT;
const float TILEWIDTH      = DRAWWIDTH;
const float TILEHEIGHT     = DRAWHEIGHT;
const float TILEANIMDELAY  = .10;

direction_t marioDirection;
range_t     marioAnim;

GLuint displayList;

int     FPS;
int     framesPerSecond;
int     totalFramesRendered;

range_t leftAnim;
range_t jumpright;
range_t jumpleft;
range_t rightAnim;
range_t stoppedright;
range_t stoppedleft;


sf::Music titleSong;

sf::Sound jumpSound;
sf::Sound startGameSound;
sf::Sound skidSound;
sf::Sound hurtSound;
sf::Sound blockSound;
sf::Sound destroySound;

sf::SoundBuffer hurtBuffer;
sf::SoundBuffer startBuffer;
sf::SoundBuffer jumpBuffer;
sf::SoundBuffer skidBuffer;
sf::SoundBuffer blockBuffer;
sf::SoundBuffer destroyBuffer;

//Game state functions
void Init();
void Display();
void PassiveMouseFunc(int x, int y);
void MouseFunc(int button, int state, int x, int y);
void Reshape( int w,int h );
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x ,int y );
void SpecialKeyUp( int key,int x ,int y );
void SpriteState();
void PlaySound(sf::Sound& sound);
void PlaySong(sf::Music& song);
void SetScene();
void ControlCamera();
void CameraOffset();
void ResetCamera();
void Animate();
void CheckInput();
void Update();
void CheckCollisions();
void CalculateFPS();
void Timer();
void Idle();
void LoadTextures();
void LoadObjects();
void CalculateDrawTime();
void Quit();
void Console();

int main ( int argc,char ** argv ) {
    //Create opengl context
    cout << "Initializing Opengl...\n";
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition( WINDOWPOSX, WINDOWPOSY );
    glutInitWindowSize( WINDOWWIDTH, WINDOWHEIGHT );
    char buffer[100];
    sprintf(buffer, "%dx%d:32" , WINDOWWIDTH , WINDOWHEIGHT);
    glutGameModeString(buffer);
    
    if ( !FULLSCREEN ) {
        glutCreateWindow( windowTitle.c_str() );
    } else {
        glutCreateWindow( windowTitle.c_str() );
        glutFullScreen();
    }
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutIdleFunc( Idle );
    glutKeyboardFunc( Keyboard );
    glutKeyboardUpFunc( KeyboardUp );
    glutSpecialFunc( SpecialKey );
    glutSpecialUpFunc( SpecialKeyUp );
    glutPassiveMotionFunc( PassiveMouseFunc );
    glutMouseFunc( MouseFunc );
    glutJoystickFunc(Joystick,1);
    //Created opengl Context
    Init();
    LoadTextures();
    PlaySound(startGameSound);
    LoadObjects();
    displayList = glGenLists( 1 );
    glutMainLoop();
    Quit();

	return EXIT_SUCCESS;
}

void Idle() {
    CalculateFPS();
    CheckInput();
    Update();
    Animate();
    CheckCollisions();
    SetScene();
    ControlCamera();
    CameraOffset();
    glutPostRedisplay();
    Console();
}

void Init(){
    // Load a song to play
    if ( !titleSong.openFromFile( "overworld.ogg" ) ) {
        cout << "Failed to load song...\n";
    }
    
    if ( !hurtBuffer.loadFromFile("hurt.wav")) {
        cout << "Failed to load sound ... \n";
    }   
    
    if (!startBuffer.loadFromFile("start.wav")) {
        cout << "Failed to load sound... \n";
    }
    
    if (!jumpBuffer.loadFromFile("jump.wav")) {
        cout << "Could not load sound...\n";
    }
    
    if (!skidBuffer.loadFromFile("skid.wav")) {
        cout << "Could not load file \n";
    }
    
    if (!blockBuffer.loadFromFile("blockhit.wav")) {
        cout << "Could not load file \n";
    }
    
    if (!destroyBuffer.loadFromFile("destroy.wav")) {
        cout << "Could not load file \n";
    }
    
    //Load jump sound
    jumpSound.setBuffer(jumpBuffer);
    startGameSound.setBuffer(startBuffer);
    skidSound.setBuffer(skidBuffer);
    blockSound.setBuffer(blockBuffer);
    destroySound.setBuffer(destroyBuffer);

    
    //Play music
    titleSong.setVolume(50.0f);
    PlaySong(titleSong);
    
}

void LoadTextures() {
    //Load Textures
    textureLoader = new TextureHandler();
    bgTexID = textureLoader->Load("background2.png");
    blockTexID[0]   = textureLoader->Load("block1.png");
    blockTexID[1]   = textureLoader->Load("block2.png");
    blockTexID[2]   = textureLoader->Load("block3.png");
    blockTexID[3]   = textureLoader->Load("block4.png");
    woodTexID       = textureLoader->Load("woodblock.png");
    sandTexID       = textureLoader->Load("sandblock.png");
    grassTexID      = textureLoader->Load("grassblock.png");
    hammerTexID     = textureLoader->Load("hammer.png");
    
    selectedBlockID[0] = woodTexID;
    selectedBlockID[1] = sandTexID;
    selectedBlockID[2] = grassTexID;
    
    
}

void Display() {
    timeOfLastDraw = glutGet( GLUT_ELAPSED_TIME ) * timeChunk;
    glClear( GL_COLOR_BUFFER_BIT );
    glCallList( displayList );
    glutSwapBuffers();
    totalFramesRendered++;
    CalculateDrawTime();
}

void Reshape(int w, int h) {
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0, GLdouble (w), 0, GLdouble(h));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void SetScene() {
    glNewList( displayList, GL_COMPILE );
    
    //set line drawing width for axes
    glLineWidth( 1 );
    glColor3f( 0.1f, 0.2f, 1.0f );
    
    //start textured drawing
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //Start Object Drawing
    for ( auto i = sprite.begin() ; i != sprite.end() ; ++i ) {
        (*i)->Draw();
    }
    
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    if ( DEPTH_ENABLED ) {
        //Set line width for axes
        glLineWidth( 4 );
        glBegin( GL_LINES );
    
        //X Coordinate line
        glColor3f( 1.0f, 0.0f, 0.0f );
        glVertex3f( 0, 0, 0);
        glVertex3f( 0.8, 0, 0 );
        
        //Y Coordinate line
        glColor3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.8, 0.0 );
        
        //Z Coordinate line
        glColor3f( 0.0f, 0.0f, 1.0f);
        glVertex3f( 0, 0, 0 );
        glVertex3f( 0, 0, 0.8 );
        
        glEnd();
    }
    glDisable(GL_LIGHTING);	
    glEndList();
}

void CalculateFPS() {
    curTime = glutGet( GLUT_ELAPSED_TIME ) * .001;
    deltaTime = curTime - lastTime;

    if ( deltaTime >= 1.0f ) {
        lastTime = curTime;
        FPS = totalFramesRendered;
        totalFramesRendered = 0;
        char buffer[100];
        sprintf(buffer, "Super Mario Open - By Evan Chapman - FPS: %d MX: %f MY: %f",
                FPS,MOUSEPOS.x, MOUSEPOS.y);
        windowTitle = buffer;
        glutSetWindowTitle(windowTitle.c_str());
    }
}

void CalculateDrawTime() {
    deltaDraw = glutGet( GLUT_ELAPSED_TIME ) * timeChunk - timeOfLastDraw;
}

void Update() {
    for (auto i = sprite.begin() ; i != sprite.end() ; ++i ){
        (*i)->Update(deltaDraw);
    }
}

void Animate(){
    for (auto i = sprite.begin() ; i != sprite.end() ; i++ ){
        (*i)->Animate(deltaDraw);
    }
}

void Quit() {
    cout << "Quitting..." << endl;
    cout << sprite.size()  << " objects to delete " << endl;
    for (unsigned long i = sprite.size() - 1 ; i > 0 ; i-- ) {
        delete sprite[i];
        sprite.pop_back();
    }
}

void CheckInput() {
    glutForceJoystickFunc();
    
    //if escaped
    if ( pressed.escape ) {
        if ( FULLSCREEN ) {
            glutLeaveGameMode();
        }
        hurtSound.setBuffer(hurtBuffer);
        PlaySound(hurtSound);
        titleSong.stop();
        Quit();
        cout << "Exit \n";
        exit(0);
    }
    
    //Pause time with space
    timeChunk = pressed.space ? 0 : .001;
    
    
    //Check Wasd
    
    if (pressed.d && !held.d) {
        blockSelection++;
        if (blockSelection > 2) {
            blockSelection = 0;
        }
        hammer->SetFrame(selectedBlockID[blockSelection], 0, 0);
    }
    
    if (pressed.a && !held.a) {
        blockSelection--;
        if (blockSelection < 0) {
            blockSelection = 2;
        }
        hammer->SetFrame(selectedBlockID[blockSelection], 0, 0);
    }
    
    //Reset Mario at will
    if (pressed.r) {
        ResetCamera();
        mario->SetSavePosition(resetPosition);
        mario->Reset();
        titleSong.openFromFile("overworld.ogg");
        PlaySong(titleSong);
    }
    
    if (pressed.m) {
        glutReshapeWindow(WINDOWWIDTH, WINDOWHEIGHT );
        glutPositionWindow(WINDOWPOSX, WINDOWPOSY );
        FULLSCREEN = false;
    }
    
    if (pressed.f) {
        FULLSCREEN = true;
        glutFullScreen();
        glLoadIdentity();
        gluOrtho2D(.2,1, 0, 1);
    }
    
    //press left or right
    if ( (pressed.kleft || pressed.jleft) && !mario->isDead ) {
        tempVec.x = -MARIOACCEL;
        mario->direction = leftd;
        mario->isAnimated = true;
        mario->isStopping = false;
        
    } else if((pressed.kright || pressed.jright) && !mario->isDead) {
        tempVec.x = MARIOACCEL;
        mario->direction = rightd;
        mario->isAnimated = true;
        mario->isStopping = false;
    } else {
        tempVec.x = 0.0f;
        mario->isStopping = true;
    }
    
    //Press run button
    if (pressed.shift || held.jbutton1 || pressed.z) {
        secondVec.x = MARIOMAXVELX + MARIORUNDIFF;
        secondVec.y = MARIOMAXVELY;
        mario->SetMaxVelocity(secondVec);
        secondVec.x = -MARIOMAXVELX - MARIORUNDIFF;
        secondVec.y = -MARIOMAXVELY;
        mario->SetMinVelocity(secondVec);
    } else {
        secondVec.x = MARIOMAXVELX;
        secondVec.y = MARIOMAXVELY;
        mario->SetMaxVelocity(secondVec);
        secondVec.x = -MARIOMAXVELX;
        secondVec.y = -MARIOMAXVELY;
        mario->SetMinVelocity(secondVec);
    }
    
    //Reset marios ability to jump
    if ((mario->topCollision)  && !pressed.x && !held.x && !pressed.cntrl) {
            allowJump = true;
    }
    
    //Press Jump button make mario jump
    if ( (pressed.x || pressed.jbutton2)
        && jumpLetGo
        && !mario->isFalling
        ) {
        jumpLetGo = false;
        PlaySound(jumpSound);
        mario->Jump();
    }
    
    //change jump height based on length of jump button hold
    vector2d_t  tempGravity;
    if ( pressed.x || held.jbutton2) {
        if (mario->isJumping || mario->isFalling){
            
        }
        tempGravity.y = MARIOHIGHJUMPMAXGRAV;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = MARIOHIGHJUMPGRAV + abs(mario->GetVelocity().x/4);
        mario->SetGravity(tempGravity);
    } else {
        tempGravity.y = MARIOMAXGRAV;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = MARIOGRAV +  abs(mario->GetVelocity().x/4) ;
        mario->SetGravity(tempGravity);
        if (mario->onGround) {
            jumpLetGo = true;
        }
    }
    
    if (mario->isDead) {
        tempGravity.y = MARIODEADMAXGRAV;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = MARIODEADGRAV;
        mario->SetGravity(tempGravity);
    }
    
    if ( mario->Turning() == true ) {
        if ( skidSound.getStatus() == 0 ) {
            PlaySound(skidSound);
        }
    }
    
    if (pressed.plus) {
        mario->SetPosition(mario->GetPosition().x, mario->GetPosition().y, mario->GetPosition().z + deltaDraw);
    }
    if (pressed.minus) {
        mario->SetPosition(mario->GetPosition().x, mario->GetPosition().y, mario->GetPosition().z - deltaDraw);
    }
    
    //if mario moves past scroll section
    if (mario->GetPosition().x > SCROLLRIGHT) {
        CAMERAOFFSET = -(mario->GetPosition().x - SCROLLRIGHT) ;
        mario->SetPosition(SCROLLRIGHT, mario->GetPosition().y, mario->GetPosition().z);
    } else if (mario->GetPosition().x < SCROLLLEFT) {
        CAMERAOFFSET = -(mario->GetPosition().x - SCROLLLEFT);
        mario->SetPosition(SCROLLLEFT, mario->GetPosition().y, mario->GetPosition().z);
    } else {
        CAMERAOFFSET = 0;
    }
    
    mario->SetAcceleration( tempVec );
    mario->SetAnimationDelay(MARIOANIMDELAY - (MARIOANIMDELAY * (abs(mario->GetVelocity().x)/MARIOMAXVELX))/2);
    
}

void PassiveMouseFunc( int x , int y ) {
    MOUSEPOS.x = x;
    MOUSEPOS.y = WINDOWHEIGHT - y;
    
    hammer->SetPosition( x  - (x % DRAWWIDTH) + fmod(CAMERAPOSX, DRAWWIDTH),
                        WINDOWHEIGHT - (y - (y % DRAWHEIGHT)), 0);
}

void MouseFunc(int button , int state , int x, int y) {
    float fixedX = x  - (x % DRAWWIDTH) + fmod(CAMERAPOSX, DRAWWIDTH);
    float fixedY = WINDOWHEIGHT - (y - (y % DRAWHEIGHT));
    switch (button){
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                PlaySound(blockSound);
                //Create block 1
                blockSprite = new Sprite();
                blockSprite->name = "New Block";
                blockSprite->AddAnimation(blankAnimation);
                blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
                blockSprite->AddFrame(selectedBlockID[blockSelection], 0);
                blockSprite->SetPosition(fixedX,fixedY , 0);
                blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
                blockSprite->SetLooping(true);
                blockSprite->isAnimated = true;
                blockSprite->IsBoundary(true);
                blockSprite->isScrolling = true;
                sprite.push_back(blockSprite);
                blockSprite = 0;
            }
            break;
            
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                hammer->SetFrame(hammerTexID, 0, 0);
                 y = WINDOWHEIGHT - y;
                for ( auto i = sprite.end() - 1 ; i != sprite.begin() ;i--) {
                    dimensions_t size = (*i)->GetSize();
                    dimensions_t pos = (*i)->GetPosition();
                    if ((*i)->IsBoundary()) {
                        //If mouse is over an object
                        if (x >= pos.x &&
                            x <= pos.x + size.x &&
                            y <= pos.y &&
                            y >= pos.y - size.y
                            ) {
                            PlaySound(destroySound);
                            cout << "Deleted " << (*i)->name << " object..." << endl;
                            sprite.erase(i);
                        } else {
                            cout << (*i)->name << " is not within cursor " << endl;
                        }
                    }
                }
            }
            break;
            
        default:
            break;
    }
}

void ControlCamera() {
    //Rotate camera using WASD and parentheses
    float angle = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    //rotate about Z axis
    if ( pressed.rbracket ) {
        angle = -turnAngle;
        z = turnAmount;
    }
    
    if ( pressed.lbracket ) {
        angle = turnAngle;
        z = turnAmount;
    }

    glRotatef(angle * deltaDraw, x * deltaDraw, y * deltaDraw, z * deltaDraw);
    
}
void CameraOffset() {
    CAMERAPOSX += CAMERAOFFSET;
for (auto i = sprite.begin() ; i != sprite.end(); i++ ) {
    Sprite* object = *i;
    if (object->isScrolling) {
        object->SetPosition(object->GetPosition().x + CAMERAOFFSET * object->scrollFactor, object->GetPosition().y, object->GetPosition().z);
        }
    }
}

void ResetCamera() {
    CAMERAOFFSET = -CAMERAPOSX;
    CameraOffset();
    CAMERAOFFSET = 0;
}

void CheckCollisions() {
    
    //check for off screen bottom
    if (mario->GetPosition().y <= -1 && !mario->isDead) {
        titleSong.stop();
        titleSong.openFromFile("dead.ogg");
        PlaySong(titleSong);
        mario->Dead();
    }
    
    if (mario->GetPosition().y <= -5000 && mario->isDead) {
        ResetCamera();
        mario->SetPosition(MARIOX, MARIOY, 0);
        CAMERAOFFSET    = lastCameraPos;
        titleSong.openFromFile("overworld.ogg");
        PlaySong(titleSong);
        mario->Reset();
    }
    int collisionAmount = 0;
    didCollide = false;
    mario->onGround = false;
    for (auto i = sprite.begin() ; i < sprite.end() ; i++ ) {
        Sprite* test = (*i);
        if (test == mario) {
            continue;
        } else if (test != mario) {
            if (mario->Colliding(*test)) {
                didCollide = true;
                collisionAmount++;
            }
        }
    }
    if (didCollide){
        tempVec.x = mario->GetVelocity().x;
        tempVec.y = mario->GetVelocity().y;

        if (mario->bottomCollision) {
            tempVec.y =0;
        }
        
        if (mario->topCollision) {
            lastCameraPos = CAMERAPOSX;
            mario->SavePosition();
        }
        
        if (mario->topCollision ) {
            if (mario->isFalling ){
                tempVec.y = 0;
                cout << "Stopped" << endl;
                 cout << "Jump " << mario->isJumping << endl << "Fall: " << mario->isFalling << " On ground " << mario->onGround << endl;
            }
        }
        
        if (mario->rightCollision) {
            tempVec.x = 0;
        }
    
        if (mario->leftCollision) {
            tempVec.x = 0;
        }
        
        
       mario->SetVelocity(tempVec);
    } else {
  
    }
    
    if (!mario->onGround) {
        mario->Fall();
    } else {
        mario->StopFall();
    }
}

void PlaySound(sf::Sound& sound) {
    if (PLAYSOUND) {
        sound.play();
    }
}

void PlaySong(sf::Music& song) {
    if (PLAYMUSIC) {
        song.play();
    }
}

void LoadObjects() {
    cout << "Loading Objects: " << "\n";
    
    //Create background
    testSprite = new Sprite();
    testSprite->name = "Background";
    testSprite->AddAnimation(blankAnimation);
    testSprite->AddFrame(bgTexID, 0);
    totalTextures++;
    testSprite->SetPosition(BACKGROUNDX, BACKGROUNDY, 0);
    testSprite->SetSize(BACKGROUNDWIDTH, BACKGROUNDHEIGHT, 0);
    testSprite->isScrolling = true ;
    testSprite->scrollFactor = BACKGROUNDSCROLL;
    sprite.push_back(testSprite);
    testSprite = 0;
    
    //Create background
    testSprite = new Sprite();
    testSprite->name = "Background 2";
    testSprite->AddAnimation(blankAnimation);
    testSprite->AddFrame(bgTexID, 0);
    totalTextures++;
    testSprite->SetPosition(BACKGROUNDX + BACKGROUNDWIDTH - .1, BACKGROUNDY, 0);
    testSprite->SetSize(BACKGROUNDWIDTH, BACKGROUNDHEIGHT, 0);
    testSprite->isScrolling = true ;
    testSprite->scrollFactor = BACKGROUNDSCROLL;
    sprite.push_back(testSprite);
    testSprite = 0;
    
    //Create background
    testSprite = new Sprite();
    testSprite->name = "Background 3";
    testSprite->AddAnimation(blankAnimation);
    testSprite->AddFrame(bgTexID, 0);
    totalTextures++;
    testSprite->SetPosition(BACKGROUNDX - BACKGROUNDWIDTH +.1, BACKGROUNDY, 0);
    testSprite->SetSize(BACKGROUNDWIDTH, BACKGROUNDHEIGHT, 0);
    testSprite->isScrolling = true ;
    testSprite->scrollFactor = BACKGROUNDSCROLL;
    sprite.push_back(testSprite);
    testSprite = 0;
    
    //Create test tiles
    for (int i = 0 ; i < 15 ; i++) {
        blockSprite = new Sprite();
        char buffer[20]  ;
        sprintf(buffer, "tile %d",i);
        blockSprite->name = buffer;
        blockSprite->AddAnimation(blankAnimation);
        blockSprite->SetAnimationDelay(TILEANIMDELAY);
        blockSprite->AddFrame("tile1.png", 0);
        totalTextures++;
        blockSprite->SetPosition(TILEX + (i *TILEWIDTH), TILEY, 0);
        blockSprite->SetSize(TILEWIDTH, TILEHEIGHT, 0.0f);
        blockSprite->IsBoundary(true);
        blockSprite->isScrolling = true;
        sprite.push_back(blockSprite);
        blockSprite = 0;
    }
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    
    for (int i = 0 ; i < 4 ; i++) {
        blockSprite->AddFrame(blockTexID[i], 0);
    }
    
    blockSprite->SetPosition(BLOCKX, BLOCKY + BLOCKHEIGHT * 1, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 2";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    for (int i = 0 ; i < 4 ; i++) {
        blockSprite->AddFrame(blockTexID[i], 0);
    }
    
    blockSprite->SetPosition(BLOCKX  + 2 * BLOCKWIDTH, BLOCKY + BLOCKHEIGHT * 1, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 2b";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    for (int i = 0 ; i < 4 ; i++) {
        blockSprite->AddFrame(blockTexID[i], 0);
    }
    blockSprite->SetPosition(BLOCKX  + 1 * BLOCKWIDTH , BLOCKY + BLOCKHEIGHT * 1, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 2c";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    for (int i = 0 ; i < 4 ; i++) {
        blockSprite->AddFrame(blockTexID[i], 0);
    }
    blockSprite->SetPosition(BLOCKX  + 4 * BLOCKWIDTH, BLOCKY + BLOCKHEIGHT*2, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 3";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    for (int i = 0 ; i < 4 ; i++) {
        blockSprite->AddFrame(blockTexID[i], 0);
    }
    blockSprite->SetPosition(BLOCKX  - 2 * BLOCKWIDTH, BLOCKY  , 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 4";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    for (int i = 0 ; i < 4 ; i++) {
        blockSprite->AddFrame(blockTexID[i], 0);
    }
    blockSprite->SetPosition(BLOCKX + 4 * BLOCKWIDTH , BLOCKY + 1* BLOCKHEIGHT , 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create mario
    marioSprite = new Sprite();
    marioSprite->SetSize(MARIOWIDTH, MARIOHEIGHT, 0);
    marioSprite->SetPosition(MARIOX, MARIOY, 0.0f);
    marioSprite->AddAnimation(blankAnimation);
    marioSprite->SetAnimationDelay(MARIOANIMDELAY);
    
    for (int i = 0 ; i < 18 ; i++ ) {
        marioSprite->AddAnimation(blankAnimation);
    }
    
    marioSprite->name = "Mario";
    //running/stopped left animation
    marioSprite->AddFrame("marioleft.png", stoppedl);
    totalTextures++;
    marioSprite->AddFrame("marioleft.png", runningl);
    totalTextures++;
   // marioSprite->AddFrame("marioleft2.png", runningl);
    //totalTextures++;
    marioSprite->AddFrame("marioleft3.png", runningl);
    totalTextures++;
    
    //running stoppped right animation
    marioSprite->AddFrame("marioright.png", stoppedr);
    totalTextures++;
    
    marioSprite->AddFrame("marioright.png", runningr);
    totalTextures++;
    //marioSprite->AddFrame("marioright2.png", runningr);
    //totalTextures++;
    marioSprite->AddFrame("marioright3.png", runningr);
    totalTextures++;
    
    //jumping left animation
    marioSprite->AddFrame("mariojumpleft.png", jumpingl);
    totalTextures++;
    
    //jumping right animation
    marioSprite->AddFrame("mariojumpright.png", jumpingr);
    totalTextures++;
    
    //falling left animation
    marioSprite->AddFrame("mariojumpleft.png", fallingl);
    totalTextures++;
    
    //falling right animation
    marioSprite->AddFrame("mariojumpright.png", fallingr);
    totalTextures++;
    
    //turning left animation
    marioSprite->AddFrame("marioturnright.png", turningl);
    totalTextures++;
    
    //turning right animation
    marioSprite->AddFrame("marioturnleft.png", turningr);
    totalTextures++;
    //dying left animation
    marioSprite->AddFrame("mariodying.png", dyingl);
    totalTextures++;
    
    //dying right animation
    marioSprite->AddFrame("mariodying.png", dyingr);
    totalTextures++;
    marioSprite->SetLooping(true);
    
    //Temporary vector for objects
    vector2d_t tVec;
    
    tVec.x = MARIOFRICTIONX;
    marioSprite->SetFriction(tVec);
    
    tVec.y = MARIOJUMP;
    marioSprite->SetJumpStrength(tVec);
    
    marioSprite->StopAnimation();
    marioSprite->direction = rightd;
    marioSprite->SetAnimation(stoppedr);
    marioSprite->IsBoundary(false);
    marioSprite->isScrolling = false;
    sprite.push_back(marioSprite);
    mario = marioSprite;
    marioSprite = 0;
    
    hammer = new Sprite();
    hammer->name = "Hammer Icon";
    hammer->AddAnimation(blankAnimation);
    hammer->AddFrame(hammerTexID, 0);
    hammer->SetSize(DRAWWIDTH, DRAWHEIGHT, 0);
    sprite.push_back(hammer);
}

//Console text output used for debugging
void Console() {
   // cout << "Jump " << mario->isJumping << endl << "Fall: " << mario->isFalling << " On ground " << mario->onGround << endl;
    //cout << "Is stopped: " << mario->isStopped << endl;
    //cout << "Vel X: " << mario->GetVelocity().x << endl << "Vel Y: " << mario->GetVelocity().y << endl;
    //cout << "JSy: " << mario->GetJumpStrength().y << endl;
    //cout << "MaxVx " << mario->GetMaxVelocity().x << endl;
    //cout << "MaxVy " << mario->GetMaxVelocity().y << endl;
    //cout << "Coll: " << mario->isColliding << endl;
    //cout << "T: " << mario->topCollision << " B: " << mario->bottomCollision << " L: " << mario->leftCollision << " R: " << mario->rightCollision << endl;
    //cout << "Did collide: " << didCollide << endl;
    //cout << "sjump " << mario->stoppedJumping << endl;
    //cout << "allow jump " << allowJump <<  endl;
    //cout << "Jump let go "  << jumpLetGo << endl;
    //cout << "X: " << mario->GetPosition().x  << "Y: " << mario->GetPosition().y << endl;
    //cout << "Camera Pos: " << CAMERAPOSX << endl;
    //cout << "Last Camera Pos: " << lastCameraPos << endl;
    //cout << "On ground " << mario->onGround << endl;
}






