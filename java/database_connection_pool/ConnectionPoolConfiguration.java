package reusable.database_connection_pool;



public class ConnectionPoolConfiguration {
  public enum DATABASE_TYPE {
    MYSQL,
    SQLSERVER,
    ORACLE,
    DB2
  }
  
  public DATABASE_TYPE type;
  public String poolName;
  public String ip;
  public String port;
  public String database;
  public String user;
  public String password;
  
  public String maxPoolSize;
  public String minPoolSize;
  public String initialPoolSize;
  
  public String maxIdleTime;
  public String acquireIncrement;
  public String acquireRetryAttempts;
  public String acquireRetryDelay;
  
  public String autoCommitOnClose;
  public String checkoutTimeout;
  public String idleConnectionTestPeriod;
  public String testConnectionOnCheckin;
  public String testConnectionOnCheckout;
  public String breakAfterAcquireFailure;
  public String maxStatements;
  public String maxStatementsPerConnection;
  
  public ConnectionPoolConfiguration () {
    type = DATABASE_TYPE.MYSQL;
    poolName = "default";
    ip = "127.0.0.1";
    port = "3306";
    database = "default";
    user = "root";
    password = "123456";
    
    maxPoolSize = "200";
    minPoolSize = "10";
    initialPoolSize = "10";
    maxIdleTime = "60";
    acquireIncrement = "5";
    acquireRetryAttempts = "100";
    acquireRetryDelay = "1000";
    autoCommitOnClose = "true";
    acquireRetryDelay = "1000";
    autoCommitOnClose  = "true";
    checkoutTimeout  = "10000";
    idleConnectionTestPeriod = "10";
    testConnectionOnCheckin = "true";
    testConnectionOnCheckout = "true";
    breakAfterAcquireFailure = "false";
    maxStatements = "0";
    maxStatementsPerConnection = "100";
  }
}
