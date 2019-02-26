import java.sql.*;

public class Main { 
   // JDBC driver name and database URL 
   static final String JDBC_DRIVER = "org.h2.Driver";   
   static final String DB_URL = "jdbc:h2:file:/home/rem/direct/pm/database/db/persons.db";  
   
  
   public static void main(String[] args) { 
      Connection conn = null; 
      Statement stmt = null; 
      try { 
         // STEP 1: Register JDBC driver 
         Class.forName(JDBC_DRIVER).newInstance(); 
             
         //STEP 2: Open a connection 
         
         conn = DriverManager.getConnection(DB_URL);  
        
         stmt = conn.createStatement(); 
//         stmt.execute("CREATE TABLE PERSONS (FIRST_NAME VARCHAR(255), LAST_NAME VARCHAR(255), AGE INTEGER)");     
//	 	  
//         stmt.execute("INSERT INTO PERSONS VALUES ('Erofei', 'Pavlov', 23)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Erofei', 'Ivanov', 19)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Ivan', 'Emin', 20)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Jhon', 'Smirnov', 21)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Semyon', 'Pavlov', 20)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Andrey', 'Novov', 19)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Anton', 'Emin', 18)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Genry', 'Smirnov', 21)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Genry', 'Pavlov', 20)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Anton', 'No', 19)");
//         stmt.execute("INSERT INTO PERSONS VALUES ('Anton', 'Emin', 18)");
         ResultSet rs = stmt.executeQuery("SELECT * FROM PERSONS WHERE FIRST_NAME='Anton' or FIRST_NAME='Jhon'");
         System.out.println("NAME LAST_NAME AGE\n");
         while(rs.next()) {
        	 String name = rs.getString("FIRST_NAME");
        	 String l_name = rs.getString("LAST_NAME");
        	 int age = rs.getInt("AGE");
        	 System.out.println(name + " " + l_name + " " + age);
         }
         System.out.println(); 
         
         stmt.close(); 
         conn.close(); 
      } catch(SQLException se) { 
         se.printStackTrace(); 
      } catch(Exception e) { 
         e.printStackTrace(); 
      } finally { 
         try{ 
            if(stmt!=null) stmt.close(); 
         } catch(SQLException se2) { 
         }
         try { 
            if(conn!=null) conn.close(); 
         } catch(SQLException se){ 
            se.printStackTrace(); 
         } 
         }
      System.out.println("Goodbye!");
   } 
}