#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::get_system_physical_parameters() {
    radius = ui->radius_line_edit->text().toDouble();
    thickness = ui->thickness_line_edit->text().toDouble();
    atmosphere_temperature = ui->atmosphere_temperature_line_edit->text().toDouble();
    heat_transfer = ui->heat_transfer_line_edit->text().toDouble();
    borderside_temperature = ui->borderside_temperature_line_edit->text().toDouble();
    time = ui->time_line_edit->text().toDouble();
    heat_conductivity = ui->heat_conductivity_line_edit->text().toDouble();
    heat_capacity = ui->heat_capacity_line_edit->text().toDouble();
}

void MainWindow::get_sampling_steps() {
    time_step_count = ui->time_step_line_edit->text().toInt();
    spatial_step_count = ui->spatial_step_line_edit->text().toInt();
}

void MainWindow::compute_step() {
    hr = radius/spatial_step_count;
    ht = time/time_step_count;
}

void MainWindow::fill_matrix() {
    diagonal = new double[spatial_step_count + 1];
    for (int i = 1; i < spatial_step_count - 1; i++) {
        diagonal[i] = 1 - (2*ht*heat_conductivity)(hr*hr*heat_capacity) - (heat_transfer*ht)/(heat_capacity*thickness);
    }
    diagonal[0] = 1;
    diagonal[spatial_step_count] = 1;

    ddiagonal = new double[spatial_step_count + 1];
    for (int i = 1; i < spatial_step_count - 1; i++) {
        ddiagonal[i] = -(heat_conductivity*ht)/(2*i*hr*hr) + (heat_conductivity*ht)/(heat_capacity*hr*hr);
    }
    ddiagonal[0] = 0;
    ddiagonal[spatial_step_count] = 0;

    udiagonal = new double[spatial_step_count + 1];
    for (int i = 1 ; i < spatial_step_count; i++) {
        udiagonal[i] = -(heat_conductivity*ht)/(2*i*hr*hr) + (heat_conductivity*ht)/(heat_capacity*hr*hr);
    }
    udiagonal[0] = -((2*heat_conductivity*ht)/(hr))/(1+(2*heat_conductivity*ht)/(hr*hr)+(heat_transfer*ht)/(heat_capacity*thickness));
}

void MainWindow::fill_right_side() {
    rightside = new double[spatial_step_count + 1];
}

void MainWindow::on_run_computing_button_clicked()
{
    get_system_physical_parameters();
    get_sampling_steps();
    compute_step();


}
