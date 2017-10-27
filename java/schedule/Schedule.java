package reusable.schedule;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Schedule {
  private ExecutorService pool;
  private HashMap<String, ScheduleConfiguration> map;
  private Timer timer;
  private Lock lock;
  
  public Schedule() {
    pool = Executors.newCachedThreadPool(new Factory());
    map = new HashMap<String, ScheduleConfiguration>();
    timer = new Timer();
    lock = new ReentrantLock();
  }
  
  public boolean submit(ScheduleConfiguration conf) {
    // avoid the duplication
    if (true == map.containsKey(conf.name)) {
      return false;
    }
    // avoid repeatedly submiting
    conf.cancel = false;
    conf.future.clear();
    // save
    ScheduleConfiguration c = new ScheduleConfiguration(conf);
    map.put(c.name, c);
    // submit to thread pool
    timer.scheduleAtFixedRate(new ScheduleTimerTask(c, pool, lock, timer),
        c.date, c.interval);
    return true;
  }

  public boolean kill(String name) {
    // check existence
    ScheduleConfiguration conf = map.get(name);
    if (conf == null)
      return false;
    
    try {
      lock.lock();
      for(Future<Object> f : conf.future) {
        f.cancel(true); // cancel task
      }
    } finally {
      lock.unlock();
    }
    
    conf.cancel = true; // notify timer to cancel task
    map.remove(name); // remove task
    return true;
  }

  public String update(ScheduleConfiguration conf) {
    ScheduleConfiguration c = map.get(conf.name);
    if (null == c) {
      submit(conf);
      return "start";
    }
    
    if (!conf.equals(c)) {
      lock.lock();
      try {
        if (conf.interval != c.interval ||
            !conf.runner.equals(c.runner) ||
            !conf.date.equals(c.date)) {
          // terminate original mission
          kill(c.name);
          // start new mission
          submit(conf);
          return "restart";
        } else if (conf.simultaneous != c.simultaneous ||
            conf.concurrency != c.concurrency) {
          c.simultaneous = conf.simultaneous;
          c.concurrency = conf.concurrency;
          return "reconfigure";
        } else {
          return "unchange";
        }
      } finally {
        lock.unlock();
      }
    }
    
    return "unchange";
  }
  
  public ScheduleConfiguration getConfiguration(String x) {
    ScheduleConfiguration conf = map.get(x);
    if (conf != null) {
      ScheduleConfiguration c = new ScheduleConfiguration(conf);
      return c;
    }
    return conf;
  }

}

class Factory implements ThreadFactory{
  public Thread newThread(Runnable r) {
    return new Thread(r);
  }
}

class ScheduleTimerTask extends TimerTask {
  private ScheduleConfiguration conf;
  private ExecutorService pool;
  private Lock lock;
  private Timer timer;
  
  ScheduleTimerTask (ScheduleConfiguration conf, ExecutorService pool,
      Lock lock, Timer timer) {
    this.conf = conf;
    this.pool = pool;
    this.lock = lock;
    this.timer = timer;
  }
  
  @Override
  public void run() {
    Future<Object> future = null;
    Iterator<Future<Object>> it = null;
    int counter = 0; // current task counter
    int max = 0; // task max counter
    
    // check user cancel
    if (true == conf.cancel) {
      this.cancel();
      //conf.future.clear();
      timer.purge();
      return;
    }
    
    // init max
    if (false == conf.simultaneous)
      max = 1;
    else
      max = conf.concurrency;

    // init task and remove done
    lock.lock();
    try {
      it = conf.future.iterator();
      while (it.hasNext()) {
        future = (Future<Object>) it.next();
        if (!future.isDone())
          counter++;
        else
          it.remove();
      }
    } finally {
      lock.unlock();
    }
    
    // submit task
    for (int i = counter; i < max; i++) {
      conf.future.add(pool.submit((Callable<Object>) conf.runner));
    }
  }
}
