/*
 * oxill.h
 */

#ifndef __OXILL_H
#define __OXILL_H

#include <QObject>

#include <vector>
#include <iostream>
#include <cmath>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>

using namespace std;

class Oscilloscope : public QwtPlot  {

    Q_OBJECT ;

    int samples_in_display;
    int channels;
    QwtPlotCurve * channel_curve;
    vector<double> * channel_data;
    vector<double> t_data;
    double ts;
public:
    Oscilloscope(QwtText name, int _chan);
    void set_attributes(int i,QwtText name, QPen p);
public slots:
    void on_new_data(double * p);
};

class OmegaPlot : public Oscilloscope {
public:
    OmegaPlot() : Oscilloscope(QwtText("Omega"), 2) {
        set_attributes(0, QwtText("Omega Current"), QPen(Qt::yellow));
        set_attributes(1, QwtText("Omega Target"), QPen(Qt::green));
        setAxisScale(QwtPlot::yLeft,0, 30);
    };
};


class TorquePlot : public Oscilloscope {
public:
    TorquePlot() : Oscilloscope(QwtText("Torque"), 1) {
        set_attributes(0, QwtText("Torque"), QPen(Qt::yellow));
        setAxisScale(QwtPlot::yLeft,0, 50);
    };
};


#endif


