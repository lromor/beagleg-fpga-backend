// g++ -o fpga-step-gen fpga-step-gen.cc && ./fpga-step-gen > /tmp/foo.data
//
// In Gnuplot:
/*
set key autotitle columnheader
set xlabel "seconds"
set ylabel "Position [steps]; speed [steps/sec]"
set y2label "Acceleration [steps/sec^2]"
set y2tics textcolor "red"

set ytics nomirror
set offsets graph 0, 1.0, 1.05, 0
set style line 1 linecolor rgb "red" linewidth 1   # accel
set style line 2 linecolor rgb "green" linewidth 4 # speed
set style line 3 linecolor rgb "blue" linewidth 4  # pos

plot "/tmp/s.dat" using 1:2 with lines ls 3, "" using 1:3 with lines ls 2, "" using 1:4 with lines ls 1 axes x1y2
*/

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>

typedef double Number;   // In FPGA some large fixed point number.

// We calculate the current position in regular intervals.
constexpr Number sample_hz = 50'000;

struct MotorRegister {
  // fractiontional motor pos; for software simulation for now just
  // our 'Number' type, but in hardware some fixed-point representation
  // of which we take a bit to genererate steps.
  Number motor_pos;
};

struct MotorRequest {
  Number jerk;
  Number target_accel;
  Number linear_speed;

  Number move_units;
};

class Plotter {
public:
  Plotter(int subsampling) : subsampling_(subsampling) {
    printf("    time      pos    speed    accel\n");
  }
  void Update(const MotorRegister &m, bool is_final = false) {
    // Don't print final as we end up with super negative acceleration.
    if (is_final) return;

    if (is_final || count_ % subsampling_ == 0) {
      const double t = 1.0 * count_ / sample_hz;
      const double dx = (m.motor_pos - last_motor_.motor_pos);
      const double dt = t - last_t;
      const double speed = dx / dt;
      const double accel = (speed - last_speed_) / dt;
      // time pos dx/dt
      printf("%8.6f %8.6f %8.6f %8.6f\n",
             t,                    // t
             m.motor_pos,          // x
             speed,                // dx/dt = speed
             accel);
      last_plot_ = count_;
      last_motor_ = m;
      last_t = t;
      last_speed_ = speed;
    }
    count_++;
  }

private:
  const int subsampling_;
  int count_ = 0;
  int last_plot_ = -1;
  MotorRegister last_motor_{0};
  double last_t = 0;
  double last_speed_ = 0;
};

int main(int, char *[]) {
  MotorRequest request;

  request.jerk = 1.0 / (sample_hz*sample_hz*sample_hz);
  request.target_accel = 10.0 / (sample_hz*sample_hz);
  request.linear_speed = 100.0 / sample_hz;

  fprintf(stderr, "Request jerk=%g target_accel=%g linear_speed=%g\n",
          request.jerk, request.target_accel, request.linear_speed);

  request.move_units = 1000.0;

  Number jerk_accel = 0;
  Number accel_speed = 0;

  const int points_per_seconds = 100;
  const int subsampling = std::max((int)sample_hz/points_per_seconds, 1);
  Plotter plotter(subsampling);
  MotorRegister motor{0};
  int last_jerk_step = 0;
  int last_accel_step = 0;
  int last_move_step = 0;
  int step = 0;
  while (motor.motor_pos < request.move_units) {
    if (jerk_accel < request.target_accel) {  // Get acceleration to target
      motor.motor_pos += accel_speed;

      accel_speed += jerk_accel;
      jerk_accel += request.jerk;

      last_jerk_step = step;
    }
    else if (accel_speed < request.linear_speed) {  // get speed to target
      motor.motor_pos += accel_speed;
      accel_speed += request.target_accel;   // increase speed

      last_accel_step = step;
    }
    else {  // linear speed
      motor.motor_pos += request.linear_speed;
      last_move_step = step;
    }
    plotter.Update(motor);
    ++step;
  }
  plotter.Update(motor, true);

  fprintf(stderr, "subsampling=%d; "
          "Steps spent in jerk:%d accel:%d fixed-speed:%d\n",
	  subsampling, last_jerk_step, last_accel_step, last_move_step);
  fprintf(stderr, "Final pos=%f\n", motor.motor_pos);
}
