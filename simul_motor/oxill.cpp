/*
 * oxill.cc
 */

#include "oxill.h"
#include "oxill_moc.h"

/*
 * Oscilloscope
 */
Oscilloscope::Oscilloscope(QwtText name, int _chan) : QwtPlot(name)
{
    setGeometry(600,50,640,400);
    samples_in_display = 500;
    //setAxisScale(QwtPlot::xBottom, 0.0,2.0 * M_PI);
    //setAxisScale(QwtPlot::yLeft,-1.0,1.0);
    setCanvasBackground(Qt::black);

    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    ts = 0;
    channels = _chan;

    channel_curve = new QwtPlotCurve[channels];
    channel_data = new vector<double>[channels];

    for (double t = 0; t < samples_in_display; t++)
        t_data.push_back(t);

    for (int i = 0; i < channels;i++) {
        for (double t = 0; t < samples_in_display; t++) {
            channel_data[i].push_back(0.0);
        }

        channel_curve[i].setRawSamples(&t_data[0],&channel_data[i][0],samples_in_display);
        channel_curve[i].attach(this);
    }

}

void Oscilloscope::set_attributes(int i,QwtText name, QPen p)
{
    channel_curve[i].setTitle(name);
    channel_curve[i].setPen(p);
}

void Oscilloscope::on_new_data(double * d)
{
    for (int i = 0; i < channels;i++) {
        channel_data[i].erase(channel_data[i].begin());
        channel_data[i].push_back(d[i]);
    }
    replot();
}

