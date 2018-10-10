import java.io.IOException;
import java.sql.SQLException;

import com.j256.ormlite.jdbc.JdbcPooledConnectionSource;


public class ConnectionFactory {
	public static final String DB_URL = "jdbc:h2:file:/home/rem/direct/pm/database/lab_3/persons.db";
	private static JdbcPooledConnectionSource connectionSource;
	ConnectionFactory() {
		
	}
	
	//not so good
	static {
		try {			 
			connectionSource = new JdbcPooledConnectionSource(DB_URL);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database");
		}
	}
		
	public static JdbcPooledConnectionSource getConnection() {
		return connectionSource;
	}
	
	public void closeConnection() {
		try {
			connectionSource.close();
		} catch (IOException e) {
			throw new RuntimeException("Error connecting to the database");
		}
	}
}
