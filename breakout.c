/*************************
*
* Breackout Game using SPL
* Made by Nilesh
*
**************************/

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of paddle
#define PHEIGHT 10
#define PWIDTH 50

// number of rows, columns, brick height and padding of bricks
#define ROWS 5
#define COLS 10
#define PADD 5
#define BH 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel initLives(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
void updatelives(GWindow window, GLabel label, int lives);
GObject detectCollision(GWindow window, GOval ball);

int main(int argc, char* argv[])
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    
    // instantiate lives label
    GLabel liveslabel = initLives(window);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
    
    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    updateScoreboard(window,label,points);
    updatelives(window,liveslabel, lives);

    // keep playing until game over
    waitForClick();
    double velx = 0.2 + drand48()*0.3;
    double vely = 0.50;
    if (argc == 1)
    {
        while (lives > 0 && bricks > 0)
        {
            move(ball, velx, vely);
            // paddle follows mouse
            GMouseEvent  mouse = getNextEvent(MOUSE_EVENT);
            if (mouse != NULL)
            {
                if(getEventType(mouse) == MOUSE_MOVED)
                {
                    double x = getX(mouse) - PWIDTH / 2;
                    if (x >= 0 && x <= WIDTH - PWIDTH)
                        setLocation(paddle, x, getY(paddle));
                }
            }
            // bounce off right edge of window
            if (getX(ball) + 2*RADIUS >= WIDTH)
            {
                velx = -velx;
            }
            // bounce off top edge of window
            else if (getY(ball) <= 0)
            {
                vely = -vely;
            }
            // ball going below the window
            if (getY(ball) + 2*RADIUS > HEIGHT)
            {
                lives--;
                updatelives(window,liveslabel, lives);
                if(lives>0)
                {
                    waitForClick();
                    setLocation(ball, WIDTH/2-RADIUS, HEIGHT/2-RADIUS);
                    velx = drand48();
                }
            }
            // bounce off left edge of window
            else if (getX(ball) <= 0)
            {
                velx = -velx;
            }
            GObject object = detectCollision(window, ball);
            if (object != NULL && strcmp(getType(object), "GRect") == 0)
            {
                if (object == paddle)
                {
                    double xball = getX(ball);
                    if(getY(ball) + 2*RADIUS >= HEIGHT - 50)
                        {
                            if(xball + RADIUS < getX(paddle)+ 10)
                                velx = -fabs(velx);
                            if(xball + RADIUS > getX(paddle)+ PWIDTH - 10)
                                velx = fabs(velx);
                            vely = -vely;
                        }
                }
                else
                {
                    removeGWindow(window,object);
                    velx += 0.04;
                    if(vely < 0)
                        vely = -vely + 0.01;
                    else
                        vely = -vely - 0.01;
                    bricks--;points++;
                    updateScoreboard(window,label,points);          
                }
            }
            // pause for 1 millisec
            pause(1);
        }
    }
    if (argc == 2 && strcmp(argv[1],"GOD") == 0)
    {
        //removeGWindow(window,liveslabel);
        while (lives > 0 && bricks > 0)
        {
            move(ball, velx, vely);
            // paddle moves itself
            setLocation(paddle, getX(ball) - PWIDTH/2 + RADIUS, getY(paddle));
            // bounce off right edge of window
            if (getX(ball) + 2*RADIUS >= WIDTH)
            {
                velx = -velx;
            }
            // bounce off top edge of window
            else if (getY(ball) <= 0)
            {
                vely = -vely;
            }
            // ball going below the window
            if (getY(ball) + 2*RADIUS > HEIGHT)
            {
                lives--;
                updatelives(window,liveslabel, lives);
                if(lives>0)
                {
                    waitForClick();
                    setLocation(ball, WIDTH/2-RADIUS, HEIGHT/2-RADIUS);
                    velx = drand48();
                }
            }
            // bounce off left edge of window
            else if (getX(ball) <= 0)
            {
                velx = -velx;
            }
            GObject object = detectCollision(window, ball);
            if (object != NULL && strcmp(getType(object), "GRect") == 0)
            {
                if (object == paddle)
                {
                    double xball = getX(ball);
                    if(getY(ball) + 2*RADIUS >= HEIGHT - 48)
                        {
                            if(xball + RADIUS < getX(paddle)+ 10)
                                velx = -fabs(velx);
                            if(xball + RADIUS > getX(paddle)+ PWIDTH - 10)
                                velx = fabs(velx);
                            vely = -vely;
                        }
                }
                else
                {
                    removeGWindow(window,object);
                    velx += 0.04;
                    if(vely < 0)
                        vely = -vely + 0.010;
                    else
                        vely = -vely - 0.010;
                    bricks--;points++;
                    updateScoreboard(window,label,points);          
                }
            }
            // pause for 1 millisec
            pause(1);
        }
    
    }
    
    if(bricks == 0)
    {
        setLabel(label, "You Won!");
        double x = (getWidth(window) - getWidth(label)) / 2;
        double y = (getHeight(window) - getHeight(label)) / 2;
        setLocation(label, x, y);
        pause(500);
    }
    else if(lives == 0)
    {
        setLabel(label, "You Lost");
        double x = (getWidth(window) - getWidth(label)) / 2;
        double y = (getHeight(window) - getHeight(label)) / 2;
        setLocation(label, x, y);
        pause(500);
    }
    
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    float BW = ((float)WIDTH - (float)PADD)/(float)COLS - PADD,x;
    char colour[ROWS][10]={"RED","ORANGE","YELLOW","GREEN","CYAN"};   
    for(int i = 0,y = 40; i < ROWS; i++, y+=BH+PADD)
    {
        x = 5.0;
        for(int j=0; j < COLS; j++, x+=BW+PADD)
        {
            GRect R = newGRect(x, y, BW, BH);
            setFilled(R, 1);
            setColor(R, colour[i]);
            add(window, R); 
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval B = newGOval(WIDTH/2-RADIUS, HEIGHT/2-RADIUS, 2*RADIUS, 2*RADIUS);
    setFilled(B, 1);
    add(window, B);
    return B;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect Paddle = newGRect((WIDTH - PWIDTH) / 2, HEIGHT - 50, PWIDTH, PHEIGHT); 
    setFilled(Paddle, 1); 
    add(window,Paddle);
    sendToFront(Paddle);
    return Paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel L = newGLabel("0");
    setColor(L, "LIGHT_GRAY");
    setFont(L, "SansSerif-50");
    add(window, L);
    sendToBack(L);
    return L;
}
/**
 * Instantiates, configures, and returns label for Lives.
 */
GLabel initLives(GWindow window)
{
    GLabel L = newGLabel("3");
    setColor(L, "LIGHT_GRAY");
    setFont(L, "SansSerif-28");
    add(window, L);
    sendToBack(L);
    return L;
}


/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Updates lives's label, keeping it centered at top.
 */
void updatelives(GWindow window, GLabel label, int lives)
{
    // update label
    char s[12];
    sprintf(s, "Lives = %i", lives);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = getFontAscent(label);
    setLocation(label, x, y);
}
/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }
    // no collision
    return NULL;
}
