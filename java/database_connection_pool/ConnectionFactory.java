package reusable.database_connection_pool;

import java.sql.Connection;



public class ConnectionFactory {
  
  public enum CONNECTION_POOL_TYPE {
    C3P0,
    BONECP,
    DBCP
  }
  
  static ConnectionFactory instance = null;
  private ConnectionFactory(){};
  
  public static ConnectionFactory getInstance() {
    if (instance == null) {
      instance = new ConnectionFactory();
    }
    return instance;
  }
  
  public Connection getDatabaseConnection(CONNECTION_POOL_TYPE t, String s) {
    Connection c = null;
    
    switch(t)
    {
      case C3P0: //c3p0
        c = C3p0ConnectionPool.getInstance().getConnection(s);
        break;
      default:
        break;
    }
    
    return c;
  }

  public boolean appendConnectionPool(CONNECTION_POOL_TYPE t, ConnectionPoolConfiguration c) {
    boolean b = false;
    
    switch(t)
    {
      case C3P0: //c3p0
        b = C3p0ConnectionPool.getInstance().appendPool(c);
        break;
      default:
        break;
    }
    
    return b;
  };
}
