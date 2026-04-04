#include <stdio.h>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>
#include <limits>

double infinity = std::numeric_limits<double>::infinity();
double screen_width = 0.0;  // Anchura de caracteres de pantalla
double screen_height = 0.0; // Altura de caracteres de pantalla
double aspect_ratio = 0.0; // Proporción de la altura con la anchura.
const double Z_NEAR = 0.1; // Distancia del origen al final del frustrum.
const double Z_FAR = 100; // Distancia del origen al viewport.
const double FOV = 90.0;


using Z_Buffer = std::vector<std::vector<double>>;
using Frame_Buffer = std::vector<std::vector<char>>;
using Point3D = std::array<double, 3>;

/**
 * Maneja el evento de CTRL + C para interrumpir
 * la simulación. Muestra otra vez el cursor.
 */
void exitEventHandler(int signal){
  printf("\033[?25h\033[0m\n");
  exit(0);
}

/**
 * Resetea el cursor. Utiliza secuencia de escape ANSI (SCI) para
 * limpiar el buffer de la terminal (2J) y devolver el cursor a la
 * posición home (H). 
 */
void clearScreen() {
    printf("\033[2J\033[H");
}


void rendererFace(Point3D p1, Point3D p2, Point3D p3, Point3D p4,
		  Frame_Buffer *frame_buffer, Z_Buffer *z_buffer){

  
}


int main() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  screen_width = w.ws_col; // Se declara el max de X
  screen_height = w.ws_row; // Se declara el max de Y
  aspect_ratio = screen_width / screen_height;
  // Ocultar el cursor
  printf("\033[?25l");

  
  //signal(SIGINT, exitEventHandler);

  Z_Buffer z_buffer(
      screen_height,
      std::vector<double>(screen_width, infinity));
  Frame_Buffer frame_buffer(
      screen_height,
      std::vector<char>(screen_width, ' ')); 

  const std::array<Point3D, 8> cube_coords = {{
    { 1,  1, 17}, // P0 
    {-1,  1, 17}, // P1 
    {-1, -1, 17}, // P2 
    { 1, -1, 17}, // P3 
    { 1,  1, 23}, // P4 
    {-1,  1, 23}, // P5 
    {-1, -1, 23}, // P6 
    { 1, -1, 23}  // P7 
    }};

  const std::vector<std::array<int, 4>> cube_faces = {
    {0, 1, 2, 3}, // Cara Frontal
    {4, 5, 6, 7}, // Cara Trasera 
    {0, 4, 7, 3}, // Cara Izquierda
    {1, 5, 6, 2}, // Cara Derecha
    {0, 1, 5, 4}, // Cara Superior
    {3, 2, 6, 7}  // Cara Inferior
};

  printf("\033[?25h\033[0m\n");
  return 0;
}
