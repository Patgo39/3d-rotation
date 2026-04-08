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
const double Z_NEAR = 4; // Distancia del origen al viewport
const double Z_FAR = 29; // Distancia del origen al final del frustrum
const double FOV = 90.0;
const double PI = std::acos(-1.0);

using Z_Buffer = std::vector<std::vector<double>>;
using Frame_Buffer = std::vector<std::vector<char>>;
using Point3D = std::array<double, 3>;
using Point2D = std::array<double, 2>;

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
Point2D matrix_multiplication(Point3D point, double &depth_value) {

  double x = point[0];
  double y = point[1];
  double z = -point[2];

  std::cout<<"xi: "<<x<<"\n";
  std::cout<<"yi: "<<y<<"\n";
  std::cout<<"zi: "<<z<<"\n";
  
  double temp_FOV = (FOV / 2) * (PI / 180);
  double tanHalfFOV = std::tan(temp_FOV);
  double x_proyectado = x / (aspect_ratio * tanHalfFOV);
  double y_proyectado = y / tanHalfFOV;

  // El valor está en el rango [0, 1]
  depth_value = ((-z * Z_FAR) / (Z_FAR - Z_NEAR)) -
    ((Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR));
  
  double w = -z; 
  
  if (w != 0) {
    x_proyectado /= w;
    y_proyectado /= w;
    depth_value /= w;
  }

  x_proyectado = ((x_proyectado + 1) / 2) * screen_width;
  y_proyectado = ((y_proyectado + 1) / 2) * screen_height;

  Point2D point2D = {x_proyectado, y_proyectado};
  
  std::cout<<"xp: "<<x_proyectado<<"\n";
  std::cout<<"yp: "<<y_proyectado<<"\n";
  std::cout<<"depth: "<<depth_value<<"\n";
  std::cout<<"\n";
  return point2D;
}

void rendererFace(Point3D p1, Point3D p2, Point3D p3, Point3D p4,
		  Frame_Buffer &frame_buffer, Z_Buffer &z_buffer){

  double depth_value1 = 0.0;
  double depth_value2 = 0.0;
  double depth_value3 = 0.0;
  double depth_value4 = 0.0;

  Point2D p1_2d = matrix_multiplication(p1, depth_value1);
  Point2D p2_2d = matrix_multiplication(p2, depth_value2);
  Point2D p3_2d = matrix_multiplication(p3, depth_value3);
  Point2D p4_2d = matrix_multiplication(p4, depth_value4);

  int min_x = static_cast<int>(
	      std::min({p1_2d[0], p2_2d[0], p3_2d[0], p4_2d[0]}));
  int max_x = static_cast<int>(
	      std::max({p1_2d[0], p2_2d[0], p3_2d[0], p4_2d[0]}));

  int min_y = static_cast<int>(
	      std::min({p1_2d[1], p2_2d[1], p3_2d[1], p4_2d[1]}));
  int max_y = static_cast<int>(
	      std::max({p1_2d[1], p2_2d[1], p3_2d[1], p4_2d[1]}));

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
    { 6,  6, 17}, // P0 
    {-6,  6, 17}, // P1 
    {-6, -6, 17}, // P2 
    { 6, -6, 17}, // P3 
    { 6,  6, 23}, // P4 
    {-6,  6, 23}, // P5 
    {-6, -6, 23}, // P6 
    { 6, -6, 23}  // P7 
    }};

  const std::vector<std::array<int, 4>> cube_faces = {
    {0, 1, 2, 3}, // Cara Frontal
    {4, 5, 6, 7}, // Cara Trasera 
    {0, 4, 7, 3}, // Cara Izquierda
    {1, 5, 6, 2}, // Cara Derecha
    {0, 1, 5, 4}, // Cara Superior
    {3, 2, 6, 7}  // Cara Inferior
  };

  rendererFace(cube_coords[0], cube_coords[1], cube_coords[5], cube_coords[4],
	       frame_buffer, z_buffer);

  printf("\033[?25h\033[0m\n");
  return 0;
}
