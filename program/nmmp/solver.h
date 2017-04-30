#ifndef SOLVER_H
#define SOLVER_H


class Solver
{
public:
    Solver();
    double *leftdiagonal;
    double *rightdiagonal;
    double *centerdiagonal;
    double *rightside;
    int n;
    void init(int time_steps, double time, int spatial_steps, double radius, double width, double temp_border, double temp_spatial, double heat_transfer, double heat_capacity, double thermal_conductivity);
    double* solve(double *leftdiagonal, double *rightdiagonal, double *centerdiagonal, int n);
    double* fill_left(double heat_transfer, double heat_capacity, int time_steps, int spatial_steps);
    double* fill_right();
    double* fill_center();
};

#endif // SOLVER_H
