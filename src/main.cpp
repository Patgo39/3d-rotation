#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>

void exitHandler(int signal){
  printf("\033[?25h\033[0m\n");
  exit(0);
}

void clearScreen() {
    printf("\033[2J\033[H");
}

void printCharToXY(double x, double y, char c){
  printf("\033[%d;%dH#", (int)y, (int)x);
  fflush(stdout);
}

int main() {
  signal(SIGINT, exitHandler);
  
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  double x = w.ws_col / 2.0; 
  double y = w.ws_row / 2.0; 
  double dx = 1.0;         
  double dy = 1.0;          
  
    // Ocultar el cursor
  printf("\033[?25l");
  
  while (1) {
    clearScreen();
    
    printCharToXY(x, y, '#');
    
    x += dx;
    y += dy;
    
    if (x >= w.ws_col || x <= 1) {
      dx *= -1;
    }
    
    if (y >= w.ws_row || y <= 1) {
      dy *= -1;
    }
    
    usleep(50000);
  }
  
  return 0;
}
