#include <stdio.h>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <sys/ioctl.h>
#include <signal.h>
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

float screen_width = 0.0;  // Anchura de caracteres de pantalla
float screen_height = 0.0; // Altura de caracteres de pantalla
float aspect_ratio = 0.0; // Proporción de la altura con la anchura.
const float Z_NEAR = 4; // Distancia del origen al viewport
const float Z_FAR = 29; // Distancia del origen al final del frustrum
const float FOV = 75.0;
const float PI = std::acos(-1.0);
const float infinity = std::numeric_limits<float>::infinity();
const float ALPHA_RATE = 7.0;

using ZBuffer = std::vector<std::vector<float>>;
using FrameBuffer = std::vector<std::vector<char>>;
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

  x_proyectado = ((x_proyectado + 1) / 2) * screen_width;
  y_proyectado = ((1 - y_proyectado) / 2) * screen_height;

  Point2D point2D = {x_proyectado, y_proyectado, depth_value};
  return point2D;
}

Point3D rotatePoint(Point3D point, float alpha){
  float alpha_rad = alpha * (PI / 180);

  float x = point[0];
  float y = point[1];
  float z = point[2];

  point[1] = (y * std::cos(alpha_rad)) + (z * std::sin(alpha_rad));
  point[2] = (y * -std::sin(alpha_rad)) + (z * std::cos(alpha_rad));

  y = point[1];
  z = point[2];

  point[0] = (x * std::cos(alpha_rad)) + (z * -std::sin(alpha_rad));
  point[2] = (x * std::sin(alpha_rad)) + (z * std::cos(alpha_rad));

  x = point[0];
  z = point[2];

  point[0] = (x * std::cos(alpha_rad)) + (y * std::sin(alpha_rad));
  point[1] = (x * -std::sin(alpha_rad)) + (y * std::cos(alpha_rad));

  return point;
  
}


void renderCube(float cube_half_size, float z_origin, float space, float A){

  char faces_symbols[6] = {'>', '=', '!', '*', '$', '@'};
  ZBuffer z_buffer(screen_height, std::vector<float>(screen_width, 999.99));
  FrameBuffer frame_buffer(screen_height, std::vector<char>(screen_width, ' '));
  

  for(float fst_coord = -cube_half_size; fst_coord <= cube_half_size; fst_coord += space){
    for(float snd_coord = -cube_half_size; snd_coord <= cube_half_size; snd_coord += space){

      Point3D cube_faces[6] = {
	{fst_coord, snd_coord, cube_half_size}, // Trasera
	{fst_coord, snd_coord, -cube_half_size}, // Frontal
	{fst_coord, cube_half_size, snd_coord}, // Superior
	{fst_coord, -cube_half_size, snd_coord}, // Inferior
	{cube_half_size, fst_coord, snd_coord}, // Derecha
	{-cube_half_size, fst_coord, snd_coord} // Izquierda
      };

      for(int i = 0; i<6; i++){
	Point3D point = cube_faces[i];

	// Rotación de puntos
	point = rotatePoint(point, A);

	// Ajuste de coordenada z
	point[2] = point[2] + z_origin;

	// Proyección 3d a 2d
	Point2D point2d = matrix_multiplication(point);
	int p_x = static_cast<int>(round(point2d[0]));
	int p_y = static_cast<int>(round(point2d[1]));
	
	if(p_x < screen_width && p_y < screen_height){
	  
	  if(z_buffer[p_y][p_x] > point2d[2]){
	    char face_symbol = faces_symbols[i];
	    z_buffer[p_y][p_x] = point2d[2];
	    frame_buffer[p_y][p_x] = face_symbol;
	  }
	}
      }
    }
  }
    
  for(int y = 0; y<screen_height; y++){
    for(int x = 0; x < screen_width; x++){
      char symbol = frame_buffer[y][x];
      printf("%c", symbol);
    }
    printf("\n");
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

  float A, B, G = 0;
  signal(SIGINT, exitEventHandler);
  while(true){
    renderCube(6.0, 20.0, 0.1, A);
    std::this_thread::sleep_for(std::chrono::milliseconds(55));
    A += ALPHA_RATE;

    if(A >= 360){
      A = 0;
    }
    
    clearScreen();
  }
  
  

  printf("\033[?25h\033[0m\n");
  return 0;
}
