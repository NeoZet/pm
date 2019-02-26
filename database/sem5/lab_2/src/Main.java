import java.util.Iterator;
import java.util.Set;

public class Main {
	static final String JDBC_DRIVER = "org.h2.Driver";
	static final String DB_URL_PERS = "jdbc:h2:file:/home/rem/direct/pm/database/lab_2/persons.db";
	static final String DB_URL_CARS = "jdbc:h2:file:/home/rem/direct/pm/database/lab_2/cars.db";

	public static void main(String[] args) {
		DaoPerson table_pers = new DaoPerson();
		// table_pers.createTable(JDBC_DRIVER, DB_URL_PERS);

		// Set<Person> persons = table_pers.selectByID(1211);

		Person p = new Person(3333, "Anny", "Benny", 22);
		table_pers.insert(p);
		Set<Person> pers = table_pers.selectAll();
		// Set<Person> persons = table_pers.selectByLastName("Smith");
		Iterator<Person> it = pers.iterator();
		while (it.hasNext()) {
			Person pers_t = it.next();
			System.out.print(pers_t.getFirstName() + " " + pers_t.getLastName() + " ");
			System.out.println(pers_t.getAge());
		}

		DaoCar table_cars = new DaoCar();
		// table_cars.createTable(JDBC_DRIVER, DB_URL_CARS);
		Set<Car> cars_f = table_cars.selectByModel("BMW-X6");
		// table_cars.selectAll();
		//
		Iterator<Car> it_cars = cars_f.iterator();
		Set<Person> persons = table_pers.selectByID(it_cars.next().getID());

		Iterator<Person> it_per = persons.iterator();
		while (it_per.hasNext()) {
			Person pers_tmp = it_per.next();
			System.out.println(pers_tmp.getFirstName());
		}
		System.out.println("\nInsert\n------------");
		table_cars.insert(new Car(1111, "Ferrari", "white"));
		cars_f = table_cars.selectAll();
		it_cars = cars_f.iterator();
		while (it_cars.hasNext()) {
			Car cars_tmp = it_cars.next();
			System.out.println(cars_tmp.getModel());
		}
		System.out.println("\nDelete\n-------------");
		table_cars.delete(new Car(1111, "Ferrari", "white"));
		cars_f = table_cars.selectAll();
		it_cars = cars_f.iterator();
		while (it_cars.hasNext()) {
			Car cars_tmp = it_cars.next();
			System.out.println(cars_tmp.getModel());
		}
	}
}
