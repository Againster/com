package reusable.database_connection_pool;

import java.sql.Connection;
import java.util.HashMap;
import com.mchange.v2.c3p0.ComboPooledDataSource;

public class C3p0ConnectionPool {
  private static String oracleDriverClass = "oracle.jdbc.driver.OracleDriver";
  private static String mysqlDriverClass = "com.mysql.jdbc.Driver";
  private static String sqlserverDriverClass = "com.microsoft.sqlserver.jdbc.SQLServerDriver";
  private static String db2DriverClass = "com.ibm.db2.jcc.DB2Driver";
  
  private static String oracleUrlHead = "jdbc:oracle:thin:@";
  private static String mysqlUrlHead = "jdbc:mysql://";
  private static String sqlserverUrlHead = "jdbc:sqlserver://";
  private static String db2UrlHead = "jdbc:db2://";
  
	private static C3p0ConnectionPool instance;
	private static HashMap<String, ConnectionPoolConfiguration> conf;
  private static HashMap<String, ComboPooledDataSource> pool;
	
	private C3p0ConnectionPool() {
	  conf = new HashMap<String, ConnectionPoolConfiguration>();
	  pool = new HashMap<String, ComboPooledDataSource>();
	}

	synchronized public boolean appendPool(ConnectionPoolConfiguration c) {
	  conf.put(c.poolName, c);
	  if (!pool.containsKey(c.poolName)) {
	    pool.put(c.poolName, createPool(c));
	    return true;
	  } else {
	    return false;
	  }
	}

	public synchronized static C3p0ConnectionPool getInstance() {
		if (instance == null) {
			instance = new C3p0ConnectionPool();
		}
		return instance;
	}

	public synchronized Connection getConnection(String poolName) {
		Connection conn = null;
		ComboPooledDataSource p = null;
		try {
		  p = pool.get(poolName);
			if (p != null) {
			  conn = p.getConnection();
			}
		} catch (Exception ex) {
		}
		return conn;
	}
	
	private ComboPooledDataSource createPool(ConnectionPoolConfiguration c) {
	  ComboPooledDataSource p = null;
	  String driverClass = null;
	  String url = null;
	  try {
      switch(c.type) {
      case ORACLE: //oracle
        driverClass = oracleDriverClass;
        url = oracleUrlHead + c.ip + ":" + c.port + ":" + c.database;
        break;
      case SQLSERVER: //sqlserver2005
        driverClass = sqlserverDriverClass;
        url = sqlserverUrlHead + c.ip + ":" + c.port + ";DatabaseName=" + c.database;
        break;
      case MYSQL: //Mysql
        driverClass = mysqlDriverClass;
        url = mysqlUrlHead + c.ip + ":" + c.port + "/" + c.database + "?characterEncoding=gbk&useSSL=false";//mysql
        break;
      case DB2: //DB2
        driverClass = db2DriverClass;
        url = db2UrlHead + c.ip + ":" + c.port + "/" + c.database;//DB2
        break;
      default:
        break;
      }
	  
			p = new ComboPooledDataSource();
			p.setDriverClass(driverClass);
			p.setJdbcUrl(url);
			p.setUser(c.user);
			p.setPassword(c.password);
			p.setMaxPoolSize(Integer.parseInt(c.maxPoolSize));
			p.setMinPoolSize(Integer.parseInt(c.minPoolSize));
			p.setInitialPoolSize(Integer.parseInt(c.initialPoolSize));
			p.setMaxIdleTime(Integer.parseInt(c.maxIdleTime));
			p.setAcquireIncrement(Integer.parseInt(c.acquireIncrement));
			p.setAcquireRetryAttempts(Integer.parseInt(c.acquireRetryAttempts));
			p.setAcquireRetryDelay(Integer.parseInt(c.acquireRetryDelay));
			p.setAutoCommitOnClose(Boolean.parseBoolean(c.autoCommitOnClose));
			p.setCheckoutTimeout(Integer.parseInt(c.checkoutTimeout));
			p.setIdleConnectionTestPeriod(Integer.parseInt(c.idleConnectionTestPeriod));
			p.setTestConnectionOnCheckin(Boolean.parseBoolean(c.testConnectionOnCheckin));
			p.setTestConnectionOnCheckout(Boolean.parseBoolean(c.testConnectionOnCheckout));
			p.setBreakAfterAcquireFailure(Boolean.parseBoolean(c.breakAfterAcquireFailure));
      p.setMaxStatements(Integer.parseInt(c.maxStatements));
      p.setMaxStatementsPerConnection(Integer.parseInt(c.maxStatementsPerConnection));
		} catch (Exception e) {
		}
	  return p;
	}
}
