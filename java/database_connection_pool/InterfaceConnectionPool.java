package reusable.database_connection_pool;

import java.sql.Connection;

public interface InterfaceConnectionPool {
	public abstract Connection getDatabaseConnection(String pool, String database);
  public abstract boolean appendPool(ConnectionPoolConfiguration conf);
}
