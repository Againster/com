package reusable.schedule;

import java.util.Date;
import java.util.LinkedList;
import java.util.concurrent.Callable;
import java.util.concurrent.Future;

public class ScheduleConfiguration {
  public String name;
  public Date date;
  public long interval; // measurement unit is millisecond
  public Callable<Object> runner;
  public boolean simultaneous;
  public int concurrency;
  public LinkedList<Future<Object>> future;
  
  protected boolean cancel;
  
  public ScheduleConfiguration() {
    name = "";
    date = new Date();
    interval = 1000;
    runner = null;
    simultaneous = false;
    concurrency = 1;
    cancel = false;
    future = new LinkedList<Future<Object>>();
  }
  
  public ScheduleConfiguration(ScheduleConfiguration conf) {
    name = new String(conf.name);
    date = new Date(conf.date.getTime());
    interval = conf.interval;
    runner = conf.runner;
    simultaneous = conf.simultaneous;
    concurrency = conf.concurrency;
    cancel = conf.cancel;
    future = new LinkedList<Future<Object>>(conf.future);
  }
  
  public boolean equals(Object o) {
    ScheduleConfiguration x = (ScheduleConfiguration) o;
    if (!this.name.equals(x.name) ||
        !this.date.equals(x.date) ||
        !this.runner.equals(x.runner) ||
        this.interval != x.interval ||
        this.simultaneous != x.simultaneous ||
        this.concurrency != x.concurrency)
      return false;
    else
      return true;
  }
}
