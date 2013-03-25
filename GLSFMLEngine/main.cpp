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

//Debug header
vector<Sprite*> sprite;
Sprite* marioSprite;
Sprite* testSprite;
Sprite* blockSprite;
Sprite* mario;

vector2d_t tempVec;
vector2d_t secondVec;

GLuint totalTextures;

const int MAX_FPS = 60;
bool PLAYMUSIC = false;
bool DEPTH_ENABLED = false;
bool FULLSCREEN = false;
string windowTitle = "Super Mario Open - By Evan Chapman";
string title = "Super Mario Open - By Evan Chapman";
bool running;
bool falling;
bool jumping;
bool stopped;
bool didCollide;

dimensions_t    position;
dimensions_t    deltaPosition;
dimensions_t    size;

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
float   delta             = .0025f;
float   lastTime          =  0.0f;
float   curTime           =  0.0f;
float   deltaTime         =  0.0f;
float   timeOfLastDraw    =  0.0f;
float   deltaDraw         =  0.0f;

//game constants
const float BACKGROUNDWIDTH     =   2.0f;
const float BACKGROUNDHEIGHT    =   2.0f;
const float BACKGROUNDX         =  -1.0f;
const float BACKGROUNDY         =   1.0f;

const float MARIOX          = -.9;
const float MARIOY          = -.62;
const float MARIOWIDTH      = .13;
const float MARIOHEIGHT     = .15;
const float MARIOANIMDELAY  = .04;
const float MARIOFRICTIONX  = .004f;
const float MARIOMAXY = 10.0f;
const float MARIOMAXX = 1.3f;
const float MARIOMINX = -1.5f;
const float MARIOMINY = -10.0f;
const float MARIOJUMP = 2.5f;

const float BLOCKX          = -.4;
const float BLOCKY          = -.2;
const float BLOCKWIDTH      = .15;
const float BLOCKHEIGHT     = .19;
const float BLOCKANIMDELAY  = .10;

const float TILEX          = -.8;
const float TILEY          = -.85;
const float TILEWIDTH      = 0.1;
const float TILEHEIGHT     = 0.1;
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

sf::SoundBuffer hurtBuffer;
sf::SoundBuffer startBuffer;
sf::SoundBuffer jumpBuffer;
sf::SoundBuffer skidBuffer;

void Init();
void Display();
void Reshape( int w,int h );
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x ,int y );
void SpecialKeyUp( int key,int x ,int y );
void SpriteState();
void SetScene();
void ControlCamera();
void Animate();
void CheckInput();
void Update();
void CheckCollisions();
void CalculateFPS();
void Timer();
void Idle();
void LoadObjects();
void CalculateDrawTime();
void Quit();
void Console();

int main ( int argc,char ** argv ) {
    Init();
    startGameSound.play();
    //Create opengl context
    cout << "Initializing Opengl...\n";
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition( 200, 200 );
    glutInitWindowSize( 1024, 768 );
    glutGameModeString("1024x768:32");
    if ( !FULLSCREEN ) {
        
        glutCreateWindow( windowTitle.c_str() );
    } else {
        glutCreateWindow( windowTitle.c_str() );
        glutFullScreen();
    }
    glClearColor( 0.0f, 0.37f, 0.71f, 0.0f );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutIdleFunc( Idle );
    glutKeyboardFunc( Keyboard );
    glutKeyboardUpFunc( KeyboardUp );
    glutSpecialFunc( SpecialKey );
    glutSpecialUpFunc( SpecialKeyUp );
    glutJoystickFunc(Joystick,1);
    //Created opengl Context
    
    LoadObjects();
    displayList = glGenLists( 1 );
    glutMainLoop();
    Quit();

	return EXIT_SUCCESS;
}

void Idle() {
    CheckCollisions();
    CheckInput();
    ControlCamera();
    Update();
    Animate();
    SetScene();
    CalculateFPS();
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
    
    //Load jump sound
    jumpSound.setBuffer(jumpBuffer);
    startGameSound.setBuffer(startBuffer);
    skidSound.setBuffer(skidBuffer);
    
    //Play music
    if (PLAYMUSIC){
        titleSong.setVolume(50.0f);
        titleSong.play();
    }
}

void Display() {
    timeOfLastDraw = glutGet( GLUT_ELAPSED_TIME ) * .001;
    glClear( GL_COLOR_BUFFER_BIT );
    glCallList( displayList );
    glutSwapBuffers();
    totalFramesRendered++;
    CalculateDrawTime();
}

void Reshape(int w, int h) {
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glOrtho( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 );
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
    for ( vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ) {
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
        sprintf(buffer, "Super Mario Open - By Evan Chapman - FPS: %d", FPS);
        windowTitle = buffer;
        glutSetWindowTitle(windowTitle.c_str());
    }
}

void CalculateDrawTime() {
    deltaDraw = glutGet( GLUT_ELAPSED_TIME ) *.001 - timeOfLastDraw;
}

void Update() {
    for (vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ){
        (*i)->Update(deltaDraw);
    }
}

void Animate(){
    for (vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; i++ ){
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
        hurtSound.play();
        titleSong.stop();
        Quit();
        cout << "Exit \n";
        exit(0);
    }
    
    if ( pressed.f && held.f != true ) {
        if (FULLSCREEN){
            glutReshapeWindow(1024, 768 );
            glutPositionWindow(600, 300 );
            FULLSCREEN = false;
        } else {
            FULLSCREEN = true;
            glutFullScreen();
        }
        
    }
    //press left or right
    if ( pressed.kleft || pressed.jleft ) {
        tempVec.x = -0.009f;
        mario->direction = leftd;
        mario->isAnimated = true;
        mario->isStopping = false;
        
    } else if( pressed.kright || pressed.jright) {
        tempVec.x = 0.009f;
        mario->direction = rightd;
        mario->isAnimated = true;
        mario->isStopping = false;
    } else {
        tempVec.x = 0.0f;
        mario->isStopping = true;
    }
    
    //Press run button
    if (pressed.shift || held.jbutton1 || pressed.z) {
        secondVec.x = 1.8f;
        secondVec.y = mario->GetMaxVelocity().y;
        mario->SetMaxVelocity(secondVec);
        secondVec.x = -1.8f;
        secondVec.y = mario->GetMinVelocity().y;
        mario->SetMinVelocity(secondVec);
    } else {
        secondVec.x = 1.4f;
        secondVec.y = mario->GetMaxVelocity().y;
        mario->SetMaxVelocity(secondVec);
        secondVec.x = -1.4f;
        secondVec.y = mario->GetMinVelocity().y;
        mario->SetMinVelocity(secondVec);
    }
    
    //Press Jump button make mario jump
    if ( (pressed.x || pressed.jbutton2)
        && !mario->isFalling
        && !mario->isJumping
        ) {
        jumpSound.play();
        mario->Jump();
    } else {
        tempVec.y = 0.0f;
    }
    
    //change jump height based on length of jump button hold
    vector2d_t  tempGravity;
    if ( pressed.x|| held.jbutton2 ) {
        tempGravity.y = -2.4f;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = -0.002f;
        mario->SetGravity(tempGravity);
    } else {
        tempGravity.y = -2.4f;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = -0.009f;
        mario->SetGravity(tempGravity);
    }
    
    if ( mario->Turning() == true ) {
        if ( skidSound.getStatus() == 0 ) {
            skidSound.play();
        }
    }
    mario->SetAcceleration( tempVec );
    mario->SetAnimationDelay(1.5 - abs(mario->GetVelocity().x));
    
}

void ControlCamera() {
    //Rotate camera using WASD and parentheses
    if ( pressed.rbracket ) {
        glRotatef(1, 0.0f,0.0f, delta/1000);
    }
    
    if ( pressed.lbracket ) {
        glRotatef(1, 0.0f,0.0f, -delta/1000);
    }
    
    if ( pressed.w ) {
        glRotatef(1, -delta/1000,0.0f, 0.0f);
    }
    
    if ( pressed.a ) {
        glRotatef(1, 0.0f,-delta/1000, 0.0f);
    }
    
    if ( pressed.s ) {
        glRotatef(1, delta/1000,0.0f, 0.0f);
    }
    
    if ( pressed.d ) {
        glRotatef(1, 0.0f,delta/1000, 0.0f);
    }
}

void CheckCollisions() {
    for (vector<Sprite*>::iterator i = sprite.begin() ; i < sprite.end() ; i++ ) {
        Sprite* test = (*i);
        if (test == mario) {
            continue;
        } else if (test != mario) {
            if (mario->Colliding(*test)) {
                didCollide = true;
            }
            else didCollide = false;
        }
    }
    
    if (didCollide){
        if (mario->bottomCollision) {
            mario->isJumping = false;
            mario->isFalling = true;
            tempVec.x = mario->GetVelocity().x;
            tempVec.y = mario->GetMaxGravity().y/2;
            mario->SetVelocity(tempVec);
            mario->SetPosition(mario->GetPosition().x,
                               mario->GetOverlapPos().y - mario->GetOverlapSize().y,
                               mario->GetPosition().z);
        }
        
        if (mario->topCollision) {
            mario->isJumping = false;
            mario->isFalling = false;
            tempVec.x = mario->GetVelocity().x;
            tempVec.y = 0;
            mario->SetVelocity(tempVec);
            mario->SetPosition(mario->GetPosition().x,
                               mario->GetOverlapPos().y + mario->GetSize().y,
                               mario->GetPosition().z);
        }
        
        if (mario->rightCollision) {
            mario->SetPosition(mario->GetOverlapPos().x + mario->GetOverlapSize().x,
                               mario->GetPosition().y,
                               mario->GetPosition().z);
            tempVec.x = 0;
            tempVec.y = mario->GetVelocity().y;
            mario->SetVelocity(tempVec);
        }
        
        
        if (mario->leftCollision) {
            mario->SetPosition(mario->GetOverlapPos().x - mario->GetSize().x,
                               mario->GetPosition().y,
                               mario->GetPosition().z);
            tempVec.x = 0;
            tempVec.y = mario->GetVelocity().y;
            mario->SetVelocity(tempVec);
        }
    } else {
        
    }
}

void LoadObjects() {
    cout << "Loading Objects: " << "\n";
    
    //Use blank canvas for animation
    vector<int> blankAnimation;
    
    //Create background
    testSprite = new Sprite();
    testSprite->name = "Background";
    testSprite->AddAnimation(blankAnimation);
    testSprite->AddFrame("background1.png", 0);
    totalTextures++;
    testSprite->SetPosition(BACKGROUNDX, BACKGROUNDY, 0);
    testSprite->SetSize(BACKGROUNDWIDTH, BACKGROUNDHEIGHT, 0);
    sprite.push_back(testSprite);
    
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
    marioSprite->AddFrame("marioleft2.png", runningl);
    totalTextures++;
    marioSprite->AddFrame("marioleft3.png", runningl);
    totalTextures++;
    
    //running stoppped right animation
    marioSprite->AddFrame("marioright.png", stoppedr);
    totalTextures++;
    
    marioSprite->AddFrame("marioright.png", runningr);
    totalTextures++;
    marioSprite->AddFrame("marioright2.png", runningr);
    totalTextures++;
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
    
    tVec.x = MARIOMAXX;
    tVec.y = MARIOMAXY;
    marioSprite->SetMaxVelocity(tVec);
    
    tVec.x = MARIOMINX;
    tVec.y = MARIOMINY;
    marioSprite->SetMinVelocity(tVec);
    
    tVec.y = MARIOJUMP;
    marioSprite->SetJumpStrength(tVec);
    
    marioSprite->StopAnimation();
    marioSprite->direction = rightd;
    marioSprite->SetAnimation(stoppedr);
    marioSprite->IsBoundary(true);
    sprite.push_back(marioSprite);
    mario = marioSprite;
    marioSprite = 0;
    
    //Create block
    blockSprite = new Sprite();
    blockSprite->name = "Block";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    blockSprite->AddFrame("block1.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block2.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block3.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block4.png", 0);
    totalTextures++;
    blockSprite->SetPosition(BLOCKX, BLOCKY, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    sprite.push_back(blockSprite);
    
    //Create test tiles
    for (int i = 0 ; i < 10 ; i++) {
        blockSprite = new Sprite();
        char buffer[20]  ;
        sprintf(buffer, "tile %d",i);
        blockSprite->name = buffer;
        blockSprite->AddAnimation(blankAnimation);
        blockSprite->SetAnimationDelay(TILEANIMDELAY);
        blockSprite->AddFrame("tile1.png", 0);
        totalTextures++;
        blockSprite->SetPosition(TILEX + (i *.1), TILEY, 0);
        blockSprite->SetSize(TILEWIDTH, TILEHEIGHT, 0.0f);
        blockSprite->IsBoundary(true);
        sprite.push_back(blockSprite);
    }
}

//Console text output used for debugging
void Console() {
    //cout << "Jump " << mario->isJumping << endl << "Fall: " << mario->isFalling << endl;
    //cout << "Vel X: " << mario->GetVelocity().x << endl << "Vel Y: " << mario->GetVelocity().y << endl;
    //cout << "JSy: " << mario->GetJumpStrength().y << endl;
    //cout << "MaxVx " << mario->GetMaxVelocity().x << endl;
    //cout << "MaxVy " << mario->GetMaxVelocity().y << endl;
    //cout << "Coll: " << mario->isColliding << endl;
    //cout << "T: " << mario->topCollision << " B: " << mario->bottomCollision << " L: " << mario->leftCollision << " R: " << mario->rightCollision << endl;
    cout << "Did collide: " << didCollide << endl;
}






