/*
 * chebyshev.c
 * Fourth-order Chebyshev type II filter
 */

#include "chebyshev.h"


Chebyshev4Filter::Chebyshev4Filter(float b0, float b1, float b2, float b3, float b4,
                                   float a0, float a1, float a2, float a3, float a4,
                                   float dt)
    : Filter(dt)
{
    m_b[0] = b0;
    m_b[1] = b1;
    m_b[2] = b2;
    m_b[3] = b3;
    m_b[4] = b4;

    m_a[0] = a0;
    m_a[1] = a1;
    m_a[2] = a2;
    m_a[3] = a3;
    m_a[4] = a4;

    m_inputTm1 = 0;
    m_inputTm2 = 0;
    m_inputTm3 = 0;
    m_inputTm4 = 0;

    m_outputTm1 = 0;
    m_outputTm2 = 0;
    m_outputTm3 = 0;
    m_outputTm4 = 0;
}


float Chebyshev4Filter::apply(float currentInput)
{
    float output;

    output = m_b[0] * currentInput                +
        m_b[1] * m_inputTm1  +
        m_b[2] * m_inputTm2  +
        m_b[3] * m_inputTm3  +
        m_b[4] * m_inputTm4  -
        m_a[1] * m_outputTm1 -
        m_a[2] * m_outputTm2 -
        m_a[3] * m_outputTm3 -
        m_a[4] * m_outputTm4;

    m_inputTm4 = m_inputTm3;
    m_inputTm3 = m_inputTm2;
    m_inputTm2 = m_inputTm1;
    m_inputTm1 = currentInput;

    m_outputTm4 = m_outputTm3;
    m_outputTm3 = m_outputTm2;
    m_outputTm2 = m_outputTm1;
    m_outputTm1 = output;

    return output;
}


#if 0

t_cheby_fourth f_ax, f_ay, f_az;
t_cheby_fourth f_gx, f_gy, f_gz;

// all chebyshev computed with t_samp = 0.005 ms

// chebyshev 4th order, -60db, 40Hz bW
double cheby_B_60db_40Hz[] = {0.0042598,   0.0028104,   0.0062473,   0.0028104,   0.0042598};
double cheby_A_60db_40Hz[] = {1.00000,  -2.89472,   3.25646,  -1.66813,   0.32678};

// chebyshev 4th order, -60db, 30Hz bW
double cheby_B_60db_30Hz[] = {0.0024400,  -0.0012437,   0.0034287,  -0.0012437,   0.0024400};
double cheby_A_60db_30Hz[] = {1.00000,  -3.21787,   3.94565  -2.17727,   0.45530};

// chebyshev 4th order, -60db, 20Hz bW
double cheby_B_60db_20Hz[] = {0.0015040,  -0.0028337,   0.0037690,  -0.0028337,   0.0015040};
double cheby_A_60db_20Hz[] = {1.00000,  -3.49860,   4.61783 , -2.72306 ,  0.60494};

// chebyshev 4th order, -60db, 10Hz bW
double cheby_B_60db_10Hz[] = {0.0010666,  -0.0035206,   0.0049793,  -0.0035206,   0.0010666};
double cheby_A_60db_10Hz[] = {1.00000,  -3.75490,   5.29431,  -3.32186,   0.78252};

// chebyshev 4th order, -60db, 8Hz bW
double cheby_B_60db_8Hz[] = { 0.0010244,  -0.0036199,   0.0052204,  -0.0036199,   0.0010244};
double cheby_A_60db_8Hz[] = {1.00000,  -3.80444,   5.43224,  -3.45008,   0.82232};

// chebyshev 4th order, -60db, 5Hz bW
double cheby_B_60db_5Hz[] = {9.87786751038506e-04,
                               -3.76234890193084e-03,
                               5.55374469529079e-03,
                               -3.76234890193084e-03,
                               9.87786751038506e-04};
double cheby_A_60db_5Hz[] = {1.000000000000000,
                             -3.878129734998888,
                             5.641762572815876,
                             -3.648875955419098,
                             0.885247737995616};

double * rate_filter_a, * rate_filter_b;

double chebyshev_compute(double currentInput, t_cheby_fourth *filterParameters)
{
    double output;

    output = filterParameters->_b[0] * currentInput                +
        filterParameters->_b[1] * filterParameters->inputTm1  +
        filterParameters->_b[2] * filterParameters->inputTm2  +
        filterParameters->_b[3] * filterParameters->inputTm3  +
        filterParameters->_b[4] * filterParameters->inputTm4  -
        filterParameters->_a[1] * filterParameters->outputTm1 -
        filterParameters->_a[2] * filterParameters->outputTm2 -
        filterParameters->_a[3] * filterParameters->outputTm3 -
        filterParameters->_a[4] * filterParameters->outputTm4;

    filterParameters->inputTm4 = filterParameters->inputTm3;
    filterParameters->inputTm3 = filterParameters->inputTm2;
    filterParameters->inputTm2 = filterParameters->inputTm1;
    filterParameters->inputTm1 = currentInput;

    filterParameters->outputTm4 = filterParameters->outputTm3;
    filterParameters->outputTm3 = filterParameters->outputTm2;
    filterParameters->outputTm2 = filterParameters->outputTm1;
    filterParameters->outputTm1 = output;

    return output;
}


void acceleration_filter(float * ax, float *ay, float * az)
{
    *ax = chebyshev_compute(*ax, &f_ax);
    *ay = chebyshev_compute(*ay, &f_ay);
    *az = chebyshev_compute(*az, &f_az);
}

void gyro_filter(float * gx, float *gy, float * gz)
{
    *gx = chebyshev_compute(*gx, &f_gx);
    *gy = chebyshev_compute(*gy, &f_gy);
    *gz = chebyshev_compute(*gz, &f_gz);
}

void chebyshev_reset(t_cheby_fourth * f)
{
    f->inputTm1 = 0.0;
    f->inputTm2 = 0.0;
    f->inputTm3 = 0.0;
    f->inputTm4 = 0.0;

    f->outputTm1 = 0.0;
    f->outputTm2 = 0.0;
    f->outputTm3 = 0.0;
    f->outputTm4 = 0.0;
}

void chebyshev_setup(void)
{
    chebyshev_reset(&f_ax);
    f_ax._b = cheby_B_60db_10Hz;
    f_ax._a = cheby_A_60db_10Hz;

    chebyshev_reset(&f_ay);
    f_ay._b = cheby_B_60db_10Hz;
    f_ay._a = cheby_A_60db_10Hz;

    f_az.inputTm1 = -1.0;
    f_az.inputTm2 = -1.0;
    f_az.inputTm3 = -1.0;
    f_az.inputTm4 = -1.0;
    f_az._b = cheby_B_60db_10Hz;
    f_az._a = cheby_A_60db_10Hz;

    f_az.outputTm1 = -1.0;
    f_az.outputTm2 = -1.0;
    f_az.outputTm3 = -1.0;
    f_az.outputTm4 = -1.0;

    // -- rate filters

    rate_filter_b = cheby_B_60db_40Hz;
    rate_filter_a = cheby_A_60db_40Hz;

    chebyshev_reset(&f_gx);
    f_gx._b = rate_filter_b;
    f_gx._a = rate_filter_a;

    chebyshev_reset(&f_gy);
    f_gy._b = rate_filter_b;
    f_gy._a = rate_filter_a;

    chebyshev_reset(&f_gz);
    f_gz._b = rate_filter_b;
    f_gz._a = rate_filter_a;
}

#endif
