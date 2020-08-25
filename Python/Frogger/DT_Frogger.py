#Daniel Tse
#6/4/14
#3.2
#Fro gger

import pygame, sys, time, random
from pygame.locals import *

start = time.time()
# set up pygame
pygame.init()
mainClock = pygame.time.Clock()

# set up the window
WINDOWWIDTH = 600
WINDOWHEIGHT = 700
windowSurface = pygame.display.set_mode((WINDOWWIDTH, WINDOWHEIGHT), 0, 32)
pygame.display.set_caption('Frogger')

#Set up colors
GREEN = (0,255,0)
BLACK = (0,0,0)
PURPLE = (0,0,255)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
TEXTCOLOR = (255, 255, 0)

# set up text
def drawText(text, font, surface, x, y):
    textobj = font.render(text, 1, TEXTCOLOR)
    textrect = textobj.get_rect()
    textrect.topleft = (x, y)
    surface.blit(textobj, textrect)
font = pygame.font.SysFont(None, 48)

#Speed
MOVESPEED = 50
CARSPEED = 3
LOGSPEED = 4
CAR = 4
LOG = 4

# set up movement variables
moveLeft = False
moveRight = False
moveUp = False
moveDown = False

#other variables
z = 0
mes = ' '
end = 0
Lives = 0
font = pygame.font.SysFont(None, 48)

Frog = (pygame.Rect(300,650,50,50))
Frogs = [Frog]
Death = pygame.Rect(-50,-50,50,50)
End1 = pygame.Rect(75,-50,50,50)
End2 = pygame.Rect(175,-50,50,50)
End3 = pygame.Rect(275,-50,50,50)
End4 = pygame.Rect(375,-50,50,50)
End5 = pygame.Rect(475,-50,50,50)
finished = [End1,End2,End3,End4,End5]

#first set of cars (move left)
Car1 = (pygame.Rect(600,600,100,50))
Car2 = (pygame.Rect(850,600,100,50))
Car3 = (pygame.Rect(1100,600,100,50))
#second set of cars (move right)
Car4 = (pygame.Rect(-50,550,100,50))
Car5 = (pygame.Rect(-350,550,100,50))
Car6 = (pygame.Rect(-650,550,100,50))
#third set of cars (move left)
Car7 = (pygame.Rect(700,500,100,50))
Car8 = (pygame.Rect(1000,500,100,50))
Car9 = (pygame.Rect(1300,500,100,50))
#fourth set of cars (more right)
Car10 = (pygame.Rect(-100,450,100,50))
Car11 = (pygame.Rect(-350,450,100,50))
Car12 = (pygame.Rect(-600,450,100,50))
#fifth set of cars (move left)
Car13 = (pygame.Rect(800,400,100,50))
Car14 = (pygame.Rect(1050,400,100,50))
Car15 = (pygame.Rect(1300,400,100,50))

set1 = [Car1, Car2, Car3]
set2 = [Car4, Car5, Car6]
set3 = [Car7, Car8, Car9]
set4 = [Car10, Car11, Car12]
set5 = [Car13, Car14, Car15]
vehicle = [set1,set2,set3,set4,set5]

#first set of logs
Log1 = (pygame.Rect(650,300,150,50))
Log2 = (pygame.Rect(900,300,150,50))
Log3 = (pygame.Rect(1150,300,150,50))
#second set of logs
Log4 = (pygame.Rect(-50,250,100,50))
Log5 = (pygame.Rect(-300,250,100,50))
Log6 = (pygame.Rect(-550,250,100,50))
#third set of logs
Log7 = (pygame.Rect(600,200,200,50))
Log8 = (pygame.Rect(1000,200,200,50))
#fourth set of logs
Log9  = (pygame.Rect(-100,150,150,50))
Log10 = (pygame.Rect(-400,150,150,50))
Log11 = (pygame.Rect(-600,150,150,50))
#fifth set of logs
Log12 = (pygame.Rect(700,100,100,50))
Log13 = (pygame.Rect(900,100,100,50))
Log14 = (pygame.Rect(1150,100,100,50))

Logs1 = [Log1,Log2,Log3]
Logs2 = [Log4,Log5,Log6]
Logs3 = [Log7,Log8]
Logs4 = [Log9,Log10,Log11]
Logs5 = [Log12,Log13,Log14]
block = [Logs1,Logs2,Logs3,Logs4,Logs5]

#lily pads
Lily1 = (pygame.Rect(75,50,50,50))
Lily2 = (pygame.Rect(175,50,50,50))
Lily3 = (pygame.Rect(275,50,50,50))
Lily4 = (pygame.Rect(375,50,50,50))
Lily5 = (pygame.Rect(475,50,50,50))

LilyPads = [Lily1,Lily2,Lily3,Lily4,Lily5]

images = ['Car1.png','Car2.png','Car3.png','Car4.png','Car5.png','Car.png','Frog.png','Log.png','DeadFrog.png','LilyPad.png']

# run the game loop
while True:
    # check for the QUIT event
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
    MOVESPEED = 0
    if time.time()-start > 7:
        MOVESPEED = 50
    
    if event.type == KEYDOWN:
        if event.key == K_UP and Frog.top > 0:
            Frog.top -= MOVESPEED
            time.sleep(.05)
        if event.key == K_DOWN and Frog.bottom < WINDOWHEIGHT:
            Frog.top += MOVESPEED
            time.sleep(.05)
        if event.key == K_RIGHT and Frog.right < WINDOWWIDTH:
            Frog.left += MOVESPEED
            time.sleep(.05)
        if event.key == K_LEFT and Frog.left > 0:
            Frog.left -= MOVESPEED
            time.sleep(.05)
        #CHEAT
        if event.key == K_SPACE:
            CARSPEED = 0
            LOGSPEED = 0  
    if event.type == KEYUP:
        if event.key == K_SPACE:
            CARSPEED = CAR
            LOGSPEED = LOG  
        
    windowSurface.fill(BLACK)
    pygame.draw.rect(windowSurface, RED, (0, 650, 600, 50))
    pygame.draw.rect(windowSurface, RED, (0, 350, 600, 50))
    pygame.draw.rect(windowSurface, PURPLE, (0, 0, 600, 350))
    pygame.draw.rect(windowSurface, GREEN , (0, 0, 600, 50))
    
    pygame.draw.rect(windowSurface, GREEN , (0, 50, 75, 50))
    pygame.draw.rect(windowSurface, GREEN , (125, 50, 50, 50))
    pygame.draw.rect(windowSurface, GREEN , (225, 50, 50, 50))
    pygame.draw.rect(windowSurface, GREEN , (325, 50, 50, 50))
    pygame.draw.rect(windowSurface, GREEN , (425, 50, 50, 50))
    pygame.draw.rect(windowSurface, GREEN , (525, 50, 75, 50))

    #Cars moving left
    f = 0
    for i in range (len(set1)):        
        set1[f].left -= CARSPEED
        f += 1
        if set1[0].right < 0:
            set1[0] = Car1 = (pygame.Rect(WINDOWWIDTH+50,600,100,50))
        if set1[1].right < 0:
            set1[1] = Car1 = (pygame.Rect(WINDOWWIDTH+50,600,100,50))
        if set1[2].right < 0:
            set1[2] = Car1 = (pygame.Rect(WINDOWWIDTH+50,600,100,50))
    g = 0
    for i in range (len(set3)):
        set3[g].left -= CARSPEED
        g += 1
        if set3[0].right < 0:
            set3[0] = Car1 = (pygame.Rect(WINDOWWIDTH+100,500,100,50))
        if set3[1].right < 0:
            set3[1] = Car1 = (pygame.Rect(WINDOWWIDTH+100,500,100,50))
        if set3[2].right < 0:
            set3[2] = Car1 = (pygame.Rect(WINDOWWIDTH+100,500,100,50))
    h = 0
    for i in range (len(set5)):  
        set5[h].left -= CARSPEED
        h += 1
        if set5[0].right < 0:
            set5[0] = Car1 = (pygame.Rect(WINDOWWIDTH+150,400,100,50))
        if set5[1].right < 0:
            set5[1] = Car1 = (pygame.Rect(WINDOWWIDTH+150,400,100,50))
        if set5[2].right < 0:
            set5[2] = Car1 = (pygame.Rect(WINDOWWIDTH+150,400,100,50))
        
    #Cars moving right
    j = 0
    for i in range (len(set2)):
        set2[j].right += CARSPEED
        j += 1
        if set2[0].left > WINDOWWIDTH:
            set2[0] = Car1 = (pygame.Rect(-200,550,100,50))
        if set2[1].left > WINDOWWIDTH:
            set2[1] = Car1 = (pygame.Rect(-200,550,100,50))
        if set2[2].left > WINDOWWIDTH:
            set2[2] = Car1 = (pygame.Rect(-200,550,100,50))
    k = 0
    for i in range (len(set4)):
        set4[k].right += CARSPEED
        k += 1
        if set4[0].left > WINDOWWIDTH:
            set4[0] = Car1 = (pygame.Rect(-100,450,100,50))
        if set4[1].left > WINDOWWIDTH:
            set4[1] = Car1 = (pygame.Rect(-100,450,100,50))
        if set4[2].left > WINDOWWIDTH:
            set4[2] = Car1 = (pygame.Rect(-100,450,100,50))
    #logs moving left
    l = 0
    for i in range (len(Logs1)):        
        Logs1[l].left -= LOGSPEED
        l += 1
        if Logs1[0].right < 0:
            Logs1[0] = Log1 = (pygame.Rect(600,300,150,50))
        if Logs1[1].right < 0:
            Logs1[1] = Log1 = (pygame.Rect(600,300,150,50))
        if Logs1[2].right < 0:
            Logs1[2] = Log1 = (pygame.Rect(600,300,150,50))
    m = 0
    for i in range (len(Logs3)):        
        Logs3[m].left -= LOGSPEED
        m += 1
        if Logs3[0].right < 0:
            Logs3[0] = Log1 = (pygame.Rect(600,200,150,50))
        if Logs3[1].right < 0:
            Logs3[1] = Log1 = (pygame.Rect(600,200,150,50))
    n = 0
    for i in range (len(Logs5)):        
        Logs5[n].left -= LOGSPEED
        n += 1
        if Logs5[0].right < 0:
            Logs5[0] = Log1 = (pygame.Rect(600,100,100,50))
        if Logs5[1].right < 0:
            Logs5[1] = Log1 = (pygame.Rect(600,100,100,50))
        if Logs5[2].right < 0:
            Logs5[2] = Log1 = (pygame.Rect(600,100,100,50))

    #logs moving right
    o = 0
    for i in range (len(Logs2)):
        Logs2[o].right += LOGSPEED
        o += 1
        if Logs2[0].left > WINDOWWIDTH:
            Logs2[0] = Log1 = (pygame.Rect(-100,250,100,50))
        if Logs2[1].left > WINDOWWIDTH:
            Logs2[1] = Log1 = (pygame.Rect(-100,250,100,50))
        if Logs2[2].left > WINDOWWIDTH:
            Logs2[2] = Log1 = (pygame.Rect(-100,250,100,50))
    p = 0
    for i in range (len(Logs4)):
        Logs4[p].right += LOGSPEED
        p += 1
        if Logs4[0].left > WINDOWWIDTH:
            Logs4[0] = Log1 = (pygame.Rect(-100,150,100,50))
        if Logs4[1].left > WINDOWWIDTH:
            Logs4[1] = Log1 = (pygame.Rect(-100,150,100,50))
        if Logs4[2].left > WINDOWWIDTH:
            Logs4[2] = Log1 = (pygame.Rect(-100,150,100,50))
    
    #check if car hit frog
    if (Frog.colliderect(vehicle[0][0])) or Frog.colliderect(vehicle[0][1]) or Frog.colliderect(vehicle[0][2]) or Frog.colliderect(vehicle[1][0]) or Frog.colliderect(vehicle[1][1]) or Frog.colliderect(vehicle[1][2]) or Frog.colliderect(vehicle[2][0]) or Frog.colliderect(vehicle[2][1]) or Frog.colliderect(vehicle[2][2]) or Frog.colliderect(vehicle[3][0]) or Frog.colliderect(vehicle[3][1]) or Frog.colliderect(vehicle[3][2]) or Frog.colliderect(vehicle[4][0]) or Frog.colliderect(vehicle[4][1]) or Frog.colliderect(vehicle[4][2]):
       Death = pygame.Rect(Frog.left,Frog.top,50,50)
       Lives += 1
       Frog = (pygame.Rect(300,650,50,50))
    #check if frog jumped on log
    if Frog.colliderect(block[0][0]) or (Frog.colliderect(block[0][1])) or (Frog.colliderect(block[0][2])) or (Frog.colliderect(block[1][0])) or (Frog.colliderect(block[1][1])) or (Frog.colliderect(block[1][2])) or (Frog.colliderect(block[2][0])) or (Frog.colliderect(block[2][1]) or Frog.colliderect(block[3][0]) or Frog.colliderect(block[3][1]) or Frog.colliderect(block[3][2]) or Frog.colliderect(block[4][0]) or Frog.colliderect(block[4][1]) or Frog.colliderect(block[4][2])):
        if Frog.colliderect(block[0][0]) or (Frog.colliderect(block[0][1])) or (Frog.colliderect(block[0][2])) or Frog.colliderect(block[2][0]) or Frog.colliderect(block[2][1]) or Frog.colliderect(block[4][0]) or Frog.colliderect(block[4][1]) or Frog.colliderect(block[4][2]):
            Frog.left -= LOGSPEED
        else:
            Frog.right += LOGSPEED
    if  Frog.left < 0 or Frog.right > WINDOWWIDTH or Frog.top < 350 and not Frog.colliderect(LilyPads[0]) and not Frog.colliderect(LilyPads[1])and not Frog.colliderect(LilyPads[2]) and not Frog.colliderect(LilyPads[3]) and not Frog.colliderect(LilyPads[4]) and not Frog.colliderect(block[0][0]) and not Frog.colliderect(block[0][1]) and not Frog.colliderect(block[0][2]) and not Frog.colliderect(block[1][0]) and not Frog.colliderect(block[1][1]) and not Frog.colliderect(block[1][2]) and not Frog.colliderect(block[2][0]) and not Frog.colliderect(block[2][1]) and not Frog.colliderect(block[3][0]) and not Frog.colliderect(block[3][1]) and not Frog.colliderect(block[3][2]) and not Frog.colliderect(block[4][0]) and not Frog.colliderect(block[4][1]) and not Frog.colliderect(block[4][2]):
        time.sleep(0.05)
        Death = pygame.Rect(Frog.left,Frog.top,50,50)
        Lives += 1
        Frog = (pygame.Rect(300,650,50,50))
    #check if frog jumped on lilypad
    if Frog.colliderect(LilyPads[0]) or Frog.colliderect(LilyPads[1]) or Frog.colliderect(LilyPads[2]) or Frog.colliderect(LilyPads[3]) or Frog.colliderect(LilyPads[4]):
        end += 1
        if Frog.colliderect(LilyPads[0]):
            End1.bottom += 100
        if Frog.colliderect(LilyPads[1]):
            End2.bottom += 100
        if Frog.colliderect(LilyPads[2]):
            End3.bottom += 100
        if Frog.colliderect(LilyPads[3]):
            End4.bottom += 100
        if Frog.colliderect(LilyPads[4]):
            End5.bottom += 100
        Frog = (pygame.Rect(300,650,50,50))
        
    #draw death animation
    windowSurface.blit((pygame.transform.scale(pygame.image.load(images[8]),(50,50))), Death)
    
    #draw the cars
    a = 0
    for i in range (len(set1)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[0]),(100,50))), set1[a])
        a += 1
    b = 0
    for i in range (len(set2)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[1]),(100,50))), set2[b])
        b += 1
    c = 0
    for i in range (len(set3)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[2]),(100,50))), set3[c])
        c += 1
    d = 0
    for i in range (len(set4)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[3]),(100,50))), set4[d])
        d += 1
    e = 0
    for i in range (len(set5)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[4]),(100,50))), set5[e])
        e += 1

    #draw the logs
    q = 0
    for i in range (len(Logs1)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[7]),(150,50))), Logs1[q])
        q += 1
    r = 0
    for i in range (len(Logs2)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[7]),(100,50))), Logs2[r])
        r += 1
    s = 0
    for i in range (len(Logs3)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[7]),(200,50))), Logs3[s])
        s += 1
    t = 0
    for i in range (len(Logs4)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[7]),(150,50))), Logs4[t])
        t += 1
    u = 0
    for i in range (len(Logs5)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[7]),(100,50))), Logs5[u])
        u += 1

    #draw the lilypads
    v = 0
    for i in range (len(LilyPads)):
        windowSurface.blit((pygame.transform.scale(pygame.image.load(images[9]),(50,50))), LilyPads[v])
        v += 1

    #draw the finshed frogs
    w = 0
    for i in range (len(finished)):
        windowSurface.blit((pygame.transform.scale(pygame.transform.rotate(pygame.image.load(images[6]),270),(50,50))), finished[w])
        w += 1
    # draw the player onto the surface
    windowSurface.blit((pygame.transform.scale(pygame.transform.rotate(pygame.image.load(images[6]),270),(50,50))), Frog)

    #draw text
    if time.time()-start > 2:
        z = 220
        mes = 'GET READY'
    if time.time()-start > 5:
        z = 240
        mes = 'GET SET'
    if time.time()-start > 8:
        z = 290
        mes = 'GO'
    if time.time()-start > 9:
        mes = ' '
    drawText(mes, font, windowSurface, (z), (360))
    pygame.display.update()
     
    #game over
    if end == 5:
        MOVESPEED = 0
        drawText('YOU FINISHED THE GAME IN '+str(Lives)+' LIVES', font, windowSurface, (0), (360))
        pygame.display.update()
    
    # draw the window onto the screen
    pygame.display.update()
    mainClock.tick(40)
