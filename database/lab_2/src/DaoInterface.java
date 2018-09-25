import java.util.List;
import java.util.Set;

public interface DaoInterface<T> {	
	Set selectAll();
	void insert(T t);
	Set selectByFirstName(String firstName);
	Set selectByLastName(String lastName);
	Set selectByAge(int age);
	Set selectByID(int id);
}

	
