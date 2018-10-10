import com.j256.ormlite.field.DatabaseField;
import com.j256.ormlite.table.DatabaseTable;

@DatabaseTable(tableName = "persons", daoClass = DaoPerson.class)
public class Person {
	
	@DatabaseField(id = true, canBeNull = false)
	private int id;
	
	@DatabaseField(canBeNull = false)
	private String firstName;
	
	@DatabaseField(canBeNull = false)
	private String lastName;
	
	@DatabaseField(canBeNull = false)
	private int age;
	
	Person() {
	}
	
	Person(int _id, String _firstName, String _lastName, int _age) {
		this.id = _id;
		this.firstName = _firstName;
		this.lastName = _lastName;
		this.age = _age;
	}
	
	public void setFirstName(String _firstName) {
		firstName = _firstName;
	}
	public void setLastName(String _lastName) {
		lastName = _lastName;
	}
	public void setAge(int _age) {
		age = _age;
	}
	public void setID(int _id) {
		id = _id;
	}

	public String getFirstName() {
		return firstName;
	}
	public String getLastName() {
		return lastName;
	}
	public int getAge() {
		return age;
	}
	public int getID() {
		return id;
	} 
}









