import java.sql.*;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

public class Main {
	static final String JDBC_DRIVER = "org.h2.Driver";
	static final String DB_URL = "jdbc:h2:file:/home/rem/direct/pm/database/lab_2/persons.db";

	public static void main(String[] args) {
    	DaoPerson table = new DaoPerson();
//    	table.createTable(JDBC_DRIVER, DB_URL);
//    	Set persons = table.selectAll();
    	Person p = new Person();
    	table.insert(p);
    	Set persons = table.selectByLastName("Smith");
    	int i = 0;
    	Iterator<Person> it = persons.iterator();
    	while(it.hasNext()) {
    		Person pers = it.next();
    		System.out.println(pers.getFirstName());
    		i += 1;
    	}
    }
}
