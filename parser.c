#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    
    if(strcmp(line,"line") == 0){
      fgets(line,255,f);
      line[strlen(line)-1] = '\0';
      int arr[6];
      int c=0;
      char *holder = line;
      while (c < 6){
	arr[c] = atoi( strsep(&holder," "));
	c++;
      }
      add_edge(edges,arr[0],arr[1],arr[2],arr[3],arr[4],arr[5]);
    }
    if(strcmp(line,"ident") == 0)
      ident(transform);
    if(strcmp(line,"scale") == 0){
      fgets(line,255,f);
      line[strlen(line)-1] = '\0';
      int arr[3];
      int c=0;
      char *holder = line;
      while (c < 3){
	arr[c] = atoi( strsep(&holder," "));
	c++;
      }
      transform = make_scale( arr[0],arr[1],arr[2]);
    }
    if(strcmp(line,"move") == 0){
      fgets(line,255,f);
      line[strlen(line)-1] = '\0';
      int arr[3];
      int c=0;
      char *holder = line;
      while (c < 3){
	arr[c] = atoi( strsep(&holder," "));
	c++;
      }
      transform = make_translate(arr[0],arr[1],arr[2]);
    }
    if(strcmp(line,"rotate") == 0){
      fgets(line,255,f);
      line[strlen(line)-1] = '\0';
      char **arr;
      int c=0;
      char *holder = line;
      while (c < 2){
	arr[c] = strsep(&holder," ");
	c++;	
      }
      if(strcmp(arr[0],"x") == 0)
	transform = make_rotX(atoi(arr[1]));
      if(strcmp(arr[0],"y") == 0)
	transform = make_rotY(atoi(arr[1]));
      if(strcmp(arr[0],"z") == 0)
	transform = make_rotZ(atoi(arr[1]));
    }
    if(strcmp(line,"apply") == 0)
      matrix_mult(transform,edges);
    if(strcmp(line,"display") == 0){
      color c;
      c.red = 34;
      c.green = 245;
      c.blue = 167;
      clear_screen(s);
      draw_lines(edges,s,c);
      display(s);
    }
    if(strcmp(line,"save") == 0){
      color c;
      c.red = 34;
      c.green = 245;
      c.blue = 167;
      clear_screen(s);
      draw_lines(edges,s,c);
      fgets(line,255,f);
      char* holder = line;
      save_extension(s,holder);
    }
  }
}
