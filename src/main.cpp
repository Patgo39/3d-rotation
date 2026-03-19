#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>

// Serán declarados más adelante
int max_width = 0; 
int max_height = 0;

/**
 * Maneja el evento de CTRL + C para interrumpir
 * la simulación.
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
 * Dibuja un carácter en coordenadas específicas de la terminal.
 * Se usa la secuencia de escape ANSI 'CUP' para mover el cursor
 * a y, x antes de imprimir. 
 */
void putChar(int x, int y, char c){
  printf("\033[%d;%dH#", y, x);
  fflush(stdout); // Se imprime inmediatamente el carácter.
}

/**
 * Obtiene los máximos de 
 */
void buildEscentials(){
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  max_width = w.ws_col; // Se declara el max de X
  max_height = w.ws_row; // Se declara el max de Y
  // Ocultar el cursor
  printf("\033[?25l");
}

/**
 * Inicia la simulación.
 * La combianción CTRL + C termina la ejecución. 
 */
void startSimulation(){
  signal(SIGINT, exitEventHandler);
  
  int x = max_width / 2; 
  int y = max_height / 2; 
  int dx = 1;         
  int dy = 1;          
  
  while (1) {
    clearScreen();
    
    putChar(x, y, '#');
    
    x += dx;
    y += dy;
    
    if (x >= max_width || x <= 1) {
      dx *= -1;
    }
    
    if (y >= max_height || y <= 1) {
      dy *= -1;
    }
    
    usleep(50000);
  }
}

int main() {
  
  buildEscentials();
  startSimulation();
 
  return 0;
}
