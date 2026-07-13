#include "objects.h"
#include <chrono> // Подключаем библиотеку для работы со времене
#include <cmath>
#include <cstdio>  // для snprintf
#include <cstring> // memset
#include <iomanip>
#include <iostream>
#include <math.h>
#include <thread> // Подключаем библиотеку для работы с потоками
#include <vector>

// const long long steps_per_second = 1000;

Vector2D compute_total_momentum(const std::vector<Object> &bodies) {
  Vector2D s{};
  for (Object a : bodies) {
    s = s + a.impulse();
  }
  return s;
}

double compute_total_energy(const std::vector<Object> &b) {
  double kin = 0.0;
  double pot = 0.0;
  int N = b.size();
  for (const auto &p : b) {
    kin += p.energy_k();
  }

  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      pot += energy_p2(b[i], b[j]);
    }
  }
  return kin + pot;
}

void clear_screen() { std::cout << "\033[2J\033[H" << std::flush; }

void draw_bodies(const std::vector<Object> &bodies, int screen_width = 80,
                 int screen_height = 24, double world_x_min = -20.0,
                 double world_x_max = 20.0, double world_y_min = -15.0,
                 double world_y_max = 15.0) {
  double scale_x = (world_x_max - world_x_min) / screen_width;
  double scale_y = (world_y_max - world_y_min) / screen_height;

  std::vector<std::string> screen(screen_height,
                                  std::string(screen_width, ' '));

  for (const auto &obj : bodies) {
    int col = static_cast<int>((obj.get_x() - world_x_min) / scale_x);
    int row = static_cast<int>((obj.get_y() - world_y_min) / scale_y);
    row = screen_height - 1 - row;
    if (col >= 0 && col < screen_width && row >= 0 && row < screen_height) {
      char c = (obj.get_mass() > 100.0) ? 'O' : 'o';
      screen[row][col] = c;
    }
  }

  std::string out;
  out.reserve(screen_height * (screen_width + 1));
  for (const auto &line : screen) {
    out += line;
    out += '\n';
  }
  std::cout << out << std::flush;
}

int main() {
  Object o1{0.0, 0.0, 1000.0},
      o3{10.0, 0, 1.0, Vector2D{0, std::sqrt(G * 100)}};
  Object o2{5.0, 5.0, 10.0, Vector2D{std::sqrt(G * 40), 0}};
  Object o4{0.0, -5.0, 1.0, Vector2D{-std::sqrt(G * 200), 0.0}};
  std::vector<Object> bodies = {o1, o2, o3, o4};
  int size_of_bodies = 4;
  double t = 0;
  long long step = 0;
  const long long steps_per_second = (long long)(1.0 / T + 0.05);
  // double En0 = compute_total_energy(bodies);

  /*  while (true) {
      std::vector<Vector2D> forces_old(size_of_bodies),
          forces_new(size_of_bodies);

      for (int i = 0; i < size_of_bodies; ++i) {
        forces_old[i] = {0, 0};
        for (int j = 0; j < size_of_bodies; ++j)
          if (i != j)
            forces_old[i] = forces_old[i] + grav_force(bodies[i], bodies[j]);
      }

      for (int i = 0; i < size_of_bodies; ++i)
        verlet_step_first(bodies[i], forces_old[i]);

      for (int i = 0; i < size_of_bodies; ++i) {
        forces_new[i] = {0, 0};
        for (int j = 0; j < size_of_bodies; ++j)
          if (i != j)
            forces_new[i] = forces_new[i] + grav_force(bodies[i], bodies[j]);
      }

      for (int i = 0; i < size_of_bodies; ++i)
        verlet_step_second(bodies[i], forces_old[i], forces_new[i]);

      step++;
      t = T * step;
      if (step == 1 || (step % (steps_per_second * 100) == 0)) {
        Vector2D M = compute_total_momentum(bodies);
        double E = compute_total_energy(bodies);
        std::cout << "Time: " << t << std::endl;
        std::cout << "Full energy: " << std::scientific << std::setprecision(12)
                  << E << "| (E-E0)/E0: " << (E - En0) / En0
                  << " | Full impulse: " << M << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
      if (step == 100000 * steps_per_second)
        return 0;

    }*/

  // const int fps = 30;
  const long long steps_per_frame = steps_per_second * 1000;
  long long next_frame_step = 0;

  clear_screen();
  while (true) {

    std::vector<Vector2D> forces_old(size_of_bodies),
        forces_new(size_of_bodies);

    for (int i = 0; i < size_of_bodies; ++i) {
      forces_old[i] = {0, 0};
      for (int j = 0; j < size_of_bodies; ++j)
        if (i != j)
          forces_old[i] = forces_old[i] + grav_force(bodies[i], bodies[j]);
    }

    for (int i = 0; i < size_of_bodies; ++i)
      verlet_step_first(bodies[i], forces_old[i]);

    for (int i = 0; i < size_of_bodies; ++i) {
      forces_new[i] = {0, 0};
      for (int j = 0; j < size_of_bodies; ++j)
        if (i != j)
          forces_new[i] = forces_new[i] + grav_force(bodies[i], bodies[j]);
    }

    for (int i = 0; i < size_of_bodies; ++i)
      verlet_step_second(bodies[i], forces_old[i], forces_new[i]);
    step++;
    t = T * step;

    if (step >= next_frame_step) {
      clear_screen();
      draw_bodies(bodies);
      std::cout << "t=" << t << " E=" << compute_total_energy(bodies) << "\n";
      next_frame_step = step + steps_per_frame;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (step == 100000 * steps_per_second)
      break;
  }
}
