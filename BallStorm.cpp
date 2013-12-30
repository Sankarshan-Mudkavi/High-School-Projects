#include<allegro.h>
#include<cstdlib>
#include<math.h>
#include<stdio.h>
#include<process.h>
#define pi 3.141
#include <string>
#define WHITE makecol(255, 255, 255)
using namespace std;
int acc = 0;
time_t seconds;
int stop = 0;
int flag = 1;
string gr;
BITMAP *buffer;
BITMAP *SpikeBallSprite;
BITMAP *SpikeBall2Sprite;
BITMAP *man;
BITMAP *man1;
BITMAP *set;
BITMAP *Bomb;
BITMAP *Bomb2;
BITMAP *Bomb21;
BITMAP *TitleScreen;
int whichbit;
struct 
   {
      int x;
      int y;
      int flag;
      int tempx;
      int tempy;
   }player={20,420,1};
struct Highscore
{
      string name;
      float score;
}hs[10], temp;
   class ball
   {
      float u;
      float x;
      float y;
      float tempx;
      float tempy;
      float time;
      float theta;
      float range;
      float timeofflight;
      float sx;
      float sy;
      float g;
      public:
            int useflag;
            int col;
            void displayball();
            ball()
            {
               col=128;
               useflag = 0;
               u = 80.0;
               theta = (45.0+rand()%40) * (pi/180.0);
               g = 10.0;
               timeofflight =(u*(sin(theta)))/g;
               sx = u * cos(theta) * timeofflight;
               range=0.0;
            }
            void calculate();
            void check();
            void reinit();
}pos[10];
void Sortscore(Highscore a[10])
{
     int iloop,jloop,ls;
     Highscore temp;
     for(iloop = 0;iloop < 9;iloop++)
     {
         ls = iloop;
        for(jloop = iloop + 1;jloop < 10;jloop++)
        {
               if(a[jloop].score < a[ls].score)
               {
                   ls = jloop;
               }
        }
        temp = a[ls];
        a[ls] = a[iloop];
        a[iloop] = temp;
     }
}  
                               
int ctr=-1;
int ctrmax=0;

void ball :: reinit()
{
   useflag = 0;
   u = 80.0;
   theta = (45.0+rand()%40) * (pi/180.0);
   g = 10.0;
   timeofflight =(u*(sin(theta)))/g;
   sx = u * cos(theta) * timeofflight;
   range = 0.0;
}

void ball :: displayball()
{
   //whichbit = 1 + rand()%10;
   SpikeBallSprite = load_bitmap( "Spikedball.bmp", NULL);
   //Bomb = load_bitmap( "Bomb1.bmp", NULL);
   SpikeBall2Sprite = load_bitmap( "Spikeball2.bmp", NULL);
   char buf[20]={0};
   acquire_screen();
   /*FILE*fp=fopen("data.txt","a");
   fprintf(fp,"%f %f %f %f %f\n",x,sx,range,(x-sx+range),(y));
   fclose(fp);*/
   sprintf(buf,"%f",x-sx+range);
   textout_ex( buffer, font, buf, 0, 0, makecol( 255, 0, 0), makecol( 0, 0, 0) );
   sprintf(buf,"%f",480.0-y);
   textout_ex( buffer, font, buf, 100,0,makecol(255,0,0),makecol(0,0,0));
   //circlefill(buffer,(int)(x-sx+range),(int)(480.0-y),20,makecol(0,0,0));
   draw_sprite(buffer,SpikeBall2Sprite,(int)(tempx-sx+range),(int)(480.0-tempy));
   //circlefill(buffer,(int)(x-sx+range),(int)(480.0-y),20,makecol(col,255,0));
   draw_sprite(buffer,SpikeBallSprite,(int)(x-sx+range),(int)(480.0-y));
   if(x-sx+range > 630.0)
   {
      draw_sprite(buffer,SpikeBall2Sprite,(int)(x-sx+range),(int)(480.0-y));
      ctr--;
      this->reinit();
   }
   sprintf(buf,"%d",(int)(x-sx+range));
   textout_ex( buffer, font, buf, 0, 20, makecol( 255, 0, 0), makecol( 0, 0, 0) );
   sprintf(buf,"%d",player.x);
   textout_ex( buffer, font, buf, 100,20,makecol(255,0,0),makecol(0,0,0));
   draw_sprite(screen,buffer,0,0);
   if((int)(530-y) > player.y)
   {
      if(abs((int)(x-sx+range)-(player.x+5))<=45)
      {
         player.flag=0;
      }
   }
   release_screen();
}

void ball :: calculate()
{
   if(timeofflight >= (2* u*(sin(theta))/g))
   { 
      circlefill(buffer,(int)(x-sx+range),(int)(480.0-y),20,makecol(0,0,0));
      float c = 0.0;
      float t2=2.0 * theta;
      timeofflight = 0;
      c=(u*u*sin(t2))/g;
      range+= c;
   }
   tempx = x;
   tempy = y;
   x = u * cos(theta) * timeofflight;
   y = (u* sin(theta) * timeofflight) - (0.5 * g * timeofflight * timeofflight);
   // displayball();
   timeofflight+=0.1;
}


void moveplayer()
{
   man = load_bitmap( "stickfig.bmp", NULL);
   man1 = load_bitmap( "man1.bmp", NULL);
   player.tempx=player.x;
   player.tempy=player.y; 
   if( key[KEY_RIGHT] )
   {
      player.x+=5;
   } 
   else if( key[KEY_LEFT])
   {
      player.x-=5;
   } 
   acquire_screen();
   if(player.x>1 && player.y <475 && player.x <640) // restrict player movement > 20 or 10
   {
      draw_sprite( buffer,man1, player.tempx, player.tempy);
      draw_sprite( buffer, man, player.x, player.y);
      /*rectfill( buffer, player.tempx, player.tempy, player.tempx + 10, player.tempy + 60, makecol ( 0, 0, 0));
      rectfill( buffer, player.x, player.y, player.x + 10, player.y + 60, makecol ( 0, 0, 255));*/
   }

   release_screen();
}

int main()
{
   int i,f=0;
   time_t start;
   double timediff;
   char t[20];
   allegro_init();
   install_keyboard();
   set_color_depth(16);
   set_gfx_mode( GFX_AUTODETECT, 640, 480, 0, 0);
   buffer = create_bitmap( 640, 480);
   time(&seconds);
   srand((unsigned int) seconds);
   rest(1000);
   //Filehandling code
   /*FILE *Sfile;
   Sfile = fopen("Scores.txt","w+");
   fwrite(hs,1,sizeof(hs),Sfile);
   fclose(Sfile);*/
   //File created and null high scores have been stored
   Sfile = fopen("Scores.txt","aw+");
   rewind(Sfile);
   while(!feof(Sfile))
   {
      fprintf(Sfile,"%s", hs);
      //fprintf(Sfile, "%s",hs.score);
   }
   fclose(Sfile);
   start = time(0);
   string  edittext;                         // an empty string for editting
   string::iterator iter = edittext.begin(); // string iterator
   int     caret  = 11;                       // tracks the text caret
   bool    insert = true;
   int flagscan = 1;
   while(!key[KEY_ESC])
   {                
      while(keypressed())
      {                                         
         if(flagscan == 0)
         {
           break;
         }  
         int  newkey   = readkey();
         char ASCII    = newkey & 0xff;
         char scancode = newkey >> 8;
 
         // a character key was pressed; add it to the string
         if(ASCII >= 32 && ASCII <= 126)
         {
            // add the new char, inserting or replacing as need be
            if(insert || iter == edittext.end())
               iter = edittext.insert(iter, ASCII);
            else
               edittext.replace(caret, 1, 1, ASCII);
 
            // increment both the caret and the iterator
            caret++;
            iter++;
         }
         // some other, "special" key was pressed; handle it here
         else
            switch(scancode)
            {
               case KEY_ENTER:
                    temp.name = edittext.c_str();
                    flagscan = 0;
                    stop = 1;
                    acc = 1;
                    break;
               case KEY_DEL:
                  if(iter != edittext.end()) iter = edittext.erase(iter);
               break;
 
               case KEY_BACKSPACE:
                  if(iter != edittext.begin())
                  {
                     caret--;
                     iter--;
                     iter = edittext.erase(iter);
                  }
               break;
 
               case KEY_RIGHT:
                  if(iter != edittext.end())   caret++, iter++;
               break;
 
               case KEY_LEFT:
                  if(iter != edittext.begin()) caret--, iter--;
               break;
 
               case KEY_INSERT:
                  if(insert) insert = 0; else insert = 1;
               break;
 
               default:
               break;
            }
      }
 
      if(flagscan != 0)
      {
      clear(buffer);
      TitleScreen = load_bitmap("TitleScreen.bmp",NULL);
      draw_sprite(buffer,TitleScreen,0,0);
      textout_ex( buffer, font, "WELCOME TO FALLING BALLS!! Please Enter Your Name To Begin!", 100,240,makecol(255,255,0),makecol(0,0,0));
      // output the string to the screen
      textout(buffer, font, edittext.c_str(), 100, 250, WHITE);
      
      /*if(insert)
         textout(buffer, font, "Inserting", 0, 50, WHITE);
      else
         textout(buffer, font, "Replacing", 0, 50, WHITE);
 */
      // draw the caret
      vline(buffer, caret * 9, 247, 257, WHITE);
 
      // blit to screen
      blit(buffer, screen, 0, 0, 0, 0, 640, 480);
      }
      if(acc == 1)
      {
       clear(buffer);
      }
      //fputs(temp.name,15,Sfile);
      //fgets( 
   //stop = 1;
      if(stop == 1)
      {
          acc = 2;         
         if(!player.flag)
         {
            clear(buffer);             
            //temp.name = edittext;
            temp.score = timediff;             
            textout_ex( buffer, font, "You Lose !", 300,220,makecol(255,255,0),makecol(0,0,0));
            textout_ex( buffer, font, "Your score is :", 300,240,makecol(255,255,0),makecol(0,0,0));
            //textout_ex( buffer, font, "Your name is :", 320,280,makecol(255,255,0),makecol(0,0,0));
            sprintf(t,"%f",temp.score);
            textout_ex( buffer, font,t,300,260,makecol(255,255,0),makecol(0,0,0));
            /*sprintf(t,"*f",temp.name);
            textout_ex( buffer, font,t,320,300,makecol(255,255,0),makecol(0,0,0));*/
            draw_sprite(screen,buffer,0,0);
            rest(3000);
            break; 
         }
         //if(key[KEY_S])
         if(ctr<ctrmax)
            for(i=0;i<=ctrmax;i++)
         if(pos[i].useflag==0)
         {
            pos[i].useflag=1;
            ctr++;
            break;
         }
         for(i=0;i<=ctrmax;i++)
         {
            if(pos[i].useflag == 1)
            {
            pos[i].col = 100 + rand() % 156 ;
            pos[i].calculate();
            pos[i].displayball();
            }
         }
         timediff=difftime( time(0), start);
         if((int)timediff % 5 == 0 && f)
         {
            ctrmax++;
            f=0;
         }
         if((int)timediff > (ctrmax)*5+1)
         f=1;
         sprintf(t,"%f",timediff);
         textout_ex( buffer, font, t, 600,0,makecol(255,0,0),makecol(0,0,0));
         sprintf(t,"%d",ctrmax);
         textout_ex( buffer, font, t, 500,0,makecol(255,0,0),makecol(0,0,0));
         rest(10);
         moveplayer();
         }
   }
      
   return 0;
}
END_OF_MAIN();
