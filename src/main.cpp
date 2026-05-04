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

float infinity = std::numeric_limits<float>::infinity();
float screen_width = 0.0;  // Anchura de caracteres de pantalla
float screen_height = 0.0; // Altura de caracteres de pantalla
float aspect_ratio = 0.0; // Proporción de la altura con la anchura.
const float Z_NEAR = 4; // Distancia del origen al viewport
const float Z_FAR = 29; // Distancia del origen al final del frustrum
const float FOV = 90.0;
const float PI = std::acos(-1.0);

using Z_Buffer = std::vector<std::vector<float>>;
using Frame_Buffer = std::vector<std::vector<char>>;
using Point3D = std::array<float, 3>;
using Point2D = std::array<float, 3>; // x, y, depth_value

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
/**
 * Transforma coordenadas 3d a coordenadas 2d adaptables al ancho y largo de la
 * pantalla.
 */
Point2D matrix_multiplication(Point3D point) {

  float x = point[0];
  float y = point[1];
  float z = -point[2];

  std::cout<<"xi: "<<x<<"\n";
  std::cout<<"yi: "<<y<<"\n";
  std::cout<<"zi: "<<z<<"\n";
  
  float temp_FOV = (FOV / 2) * (PI / 180);
  float tanHalfFOV = std::tan(temp_FOV);
  float x_proyectado = x / (aspect_ratio * tanHalfFOV);
  float y_proyectado = y / tanHalfFOV;

  // El valor está en el rango [0, 1]
  float depth_value = ((-z * Z_FAR) / (Z_FAR - Z_NEAR)) -
    ((Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR));
  
  float w = -z; 
  
  if (w != 0) {
    x_proyectado /= w;
    y_proyectado /= w;
    depth_value /= w;
  }
  std::cout<<"xp: "<<x_proyectado<<"\n";
  std::cout<<"yp: "<<y_proyectado<<"\n";
  std::cout<<"depth: "<<depth_value<<"\n";
  std::cout<<"\n";

  x_proyectado = ((x_proyectado + 1) / 2) * screen_width;
  y_proyectado = ((1 - y_proyectado) / 2) * screen_height;

  Point2D point2D = {x_proyectado, y_proyectado, depth_value};
  return point2D;
}

void renderCube(float cube_half_size, float z_origin, float space){


  for(float fst_coord = -cube_half_size; fst_coord <= cube_half_size; fst_coord += space){
    for(float snd_coord = -cube_half_size; snd_coord <= cube_half_size; snd_coord += space){

      Point3D cube_faces[6] = {
	{fst_coord, snd_coord, cube_half_size},
	{fst_coord, snd_coord, -cube_half_size},
	{fst_coord, cube_half_size, snd_coord}, 
	{fst_coord, -cube_half_size, snd_coord}, 
	{cube_half_size, fst_coord, snd_coord},
	{-cube_half_size, fst_coord, snd_coord}
      };

      for(Point3D point : cube_faces){
	Point3D p2 = point;
	p2[2] = p2[2] + z_origin;

	Point2D projected_point = matrix_multiplication;
      }
    }
  }
    
    

}


int main() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  screen_width = w.ws_col; // Se declara el max de X
  screen_height = w.ws_row; // Se declara el max de Y
  aspect_ratio = (screen_width / screen_height) * 0.4; // Cuantas unidades de ancho hay por cada de alto
  // Ocultar el cursor
  printf("\033[?25l");

  
  //signal(SIGINT, exitEventHandler);

  renderCube(6.0, 20.0, 0.2);
  

  printf("\033[?25h\033[0m\n");
  return 0;
}
