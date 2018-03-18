/*
 * periodic_task.h
 */

#ifndef __PERIODIC_TASK_H
#define __PERIODIC_TASK_H

class PeriodicTask {

    friend void run_all_tasks();

 public:
    PeriodicTask(const char * name, int period, float time_unit, int jitter, bool insert_as_last = false);
    virtual void run() = 0;
    virtual void on() { m_on = true; };
    virtual void off() { m_on = false; };
    virtual bool on_status() { return m_on; };
    virtual void set_period(int v) { m_period = v; };

 private:
    void execute();
    const char * m_name;
    int m_ticks;
    int m_period;
    int m_jitter;
    bool m_on;
    PeriodicTask * m_task_next;
 protected:
    float m_real_time_period;
    float m_time_unit;
};

void run_all_tasks();

#endif
