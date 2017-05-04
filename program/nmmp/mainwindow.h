#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void get_system_physical_parameters();
    void get_sampling_steps();
    void compute_step();
    void fill_matrix();
    void fill_right_side();
    void fill_begin();
    void theoretical_graph();
    void graph();
    void solve();
    void theory();
    void write_file();

private slots:
    void on_run_computing_button_clicked();

    void on_clear_graph_button_clicked();

private:
    Ui::MainWindow *ui;

    /* System of linear equations */

    // Elements of diagonal matrix
    double *diagonal;
    double *ddiagonal;
    double *udiagonal;

    // Right side of system array
    double *rightside;

    // Solution array
    double *solution;
    double *theortical_solution;

    // System parameters
   double radius;
   double thickness;
   double atmosphere_temperature;
   double borderside_temperature;
   double heat_transfer;
   double heat_conductivity;
   double time;
   double heat_capacity;

   //Count of steps
   int time_step_count;
   int spatial_step_count;

   // Steps
   double ht;
   double hr;

};

#endif // MAINWINDOW_H
