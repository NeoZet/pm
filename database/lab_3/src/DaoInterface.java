import java.util.Set;

public interface DaoInterface<T> {	
	Set selectAll();
	void insert(T t);
	T delete(T t);
	Set selectByID(int id);
}

	
