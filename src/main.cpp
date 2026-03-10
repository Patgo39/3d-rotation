#include <ncurses.h>
#include <unistd.h> 

struct Point{
  int x;
  int y;
};

void printPoint(Point p){
  mvaddch(p.y, p.x, '&');
}

void startNcursesScreen(){
  initscr();             // Inicia el modo ncurses
  noecho();              // No muestra las teclas que presionas
  curs_set(0);           // Oculta el cursor físico
  nodelay(stdscr, TRUE); // Activa no bloqueante
}

void endNcursesScreen(){
  printw("\n\n Presiona cualquier tecla...");
  nodelay(stdscr, FALSE);    // Desactiva no bloqueante
  getch();                 // 
  endwin();                // Termina el modo ncurses
}


int main() {
  startNcursesScreen();

  // 24 80
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  printw("%d\n", max_y);
  printw("%d\n",max_x);
  
  endNcursesScreen();
  
  return 0;
}

