#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "qcustomplot.h"
#include "QTextStream"

QTextStream cout(stdout);

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
    for (int i = 1; i < spatial_step_count; i++) {
        diagonal[i] = -(1 + (2*heat_conductivity*ht)/(heat_capacity*hr*hr) + (2*heat_transfer*ht)/(heat_capacity*thickness));
    }
    diagonal[0] = 1 + (4*heat_conductivity*ht)/(heat_capacity*hr*hr) + (2*heat_transfer*ht)/(heat_capacity*thickness);
    diagonal[spatial_step_count] = 1;

    ddiagonal = new double[spatial_step_count + 1];
    for (int i = 1; i < spatial_step_count; i++) {
        ddiagonal[i] = -(heat_conductivity*ht)/(2*i*hr*hr*heat_capacity) + (heat_conductivity*ht)/(heat_capacity*hr*hr);
    }
    ddiagonal[0] = 0;
    ddiagonal[spatial_step_count] = 0;

    udiagonal = new double[spatial_step_count + 1];
    for (int i = 1 ; i < spatial_step_count; i++) {
        udiagonal[i] = (heat_conductivity*ht)/(2*i*hr*hr*heat_capacity) + (heat_conductivity*ht)/(heat_capacity*hr*hr);
    }
    udiagonal[0] = -(4*heat_conductivity*ht)/(heat_capacity*hr*hr);
    udiagonal[spatial_step_count] = 0;

}

void MainWindow::fill_right_side() {
    rightside = new double[spatial_step_count + 1];
    rightside[0] = (2 * heat_transfer * ht * borderside_temperature)/(heat_capacity*thickness) + solution[0];
    for(int i = 1; i < spatial_step_count; i++) {
        rightside[i] = - solution[i] - (2 * heat_transfer*borderside_temperature*ht)/(heat_capacity*thickness);
    }
    rightside[spatial_step_count] = borderside_temperature;
}

void MainWindow::fill_begin() {
    solution = new double[spatial_step_count + 1];
    for (int i = 0; i < spatial_step_count; i++) {
        solution[i] = borderside_temperature + j0(2.404825557*i*hr/radius);
    }
    solution[spatial_step_count] = borderside_temperature;

}

void MainWindow::graph() {
    QVector<double> x(spatial_step_count + 1), y0(spatial_step_count + 1), y1(spatial_step_count + 1); // initialize with entries 0..100
    for (int i=0; i < spatial_step_count + 1; ++i)
    {
      x[i] = i*hr;
      y0[i] = solution[i];
      y1[i] = theortical_solution[i]
;    }
    // create graph and assign data to it:
    ui->qcustomplot->clearGraphs();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setData(x, y0);
    ui->qcustomplot->graph(1)->setData(x, y1);
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::red)); // line color blue for first graph
    // give the axes some labels:
    ui->qcustomplot->xAxis->setLabel("R, cm");
    ui->qcustomplot->yAxis->setLabel("T, K");
    // set axes ranges, so we see all data:
    ui->qcustomplot->graph(0)->setName("Experiement");
    ui->qcustomplot->graph(1)->setName("Theory");
    ui->qcustomplot->xAxis->setRange(0, radius);
    double max1 = 0.0;
    double max2 = 0.0;
    for (int i = 0; i < spatial_step_count + 1; i++) {
        if (solution[i]> max1) max1 = solution[i];
    }

    for (int i = 0; i < spatial_step_count + 1; i++) {
        if (theortical_solution[i]> max1) max2 = theortical_solution[i];
    }

    if (max1 > max2) {
        ui->qcustomplot->yAxis->setRange(borderside_temperature, max1);
    } else {
        ui->qcustomplot->yAxis->setRange(borderside_temperature, max2);
    }
    ui->qcustomplot->legend->setVisible(true);
    ui->qcustomplot->replot();
}

void MainWindow::solve() {

    cout << "Solution at time " << 0*ht << endl;
    for (int i = 0; i < spatial_step_count + 1; i++) {
        cout << solution[i] << " ";
    }
    cout << endl;

            /* Прогонка */
    for (int k = 1; k < time_step_count + 1; k++) {

            double *a = new double[spatial_step_count + 1];
            double *b = new double[spatial_step_count + 1];

            a[0] = 0;
            b[0] = 0;

            a[1] = -udiagonal[0]/diagonal[0];
            b[1] = rightside[0]/diagonal[0];

            //cout << "Help coefficients " << a[1] << " " << b[1] << endl;

            for (int i = 1; i < spatial_step_count; i++) {
                a[i+1] = -udiagonal[i]/(diagonal[i] + ddiagonal[i]*a[i]);
                b[i+1] = (-ddiagonal[i]*b[i] + rightside[i])/(diagonal[i] + ddiagonal[i]*a[i]);
            }

            solution[spatial_step_count] = (-ddiagonal[spatial_step_count]*b[spatial_step_count] + rightside[spatial_step_count])/(diagonal[spatial_step_count] + ddiagonal[spatial_step_count]*a[spatial_step_count]);

            for (int i = spatial_step_count - 1; i > -1; i--) {
                solution[i] = a[i+1]*solution[i+1] + b[i+1];
            }
            cout << "Solution at time " << k*ht << endl;
            for (int i = 0; i < spatial_step_count + 1; i++) {
                cout << solution[i] << " ";
            }
            cout << endl;
            fill_right_side();

        }
}

void MainWindow::on_run_computing_button_clicked()
{
    get_system_physical_parameters();
    get_sampling_steps();
    compute_step();
    fill_matrix();

    fill_begin();

    fill_right_side();

    theory();

    solve();

    //theoretical_graph();
    graph();
}

void MainWindow::theory() {
    theortical_solution = new double[spatial_step_count + 1];

    for (int i = 0; i < spatial_step_count + 1; i++) {
        theortical_solution[i] = exp(-((2*heat_transfer)/(heat_capacity*thickness) + (heat_conductivity*2.404825557*2.404825557)/(heat_capacity*radius*radius))*time)*j0(2.404825557*i*hr/radius) + borderside_temperature;
    }
}
