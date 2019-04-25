import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Class that converts a .csv file to a computer readable list.
 * 
 * @author John Tang
 */
public class CSVTranslator {
	
	/**
	 * Constructor for the CSVTranslator object. Does nothing.
	 */
    public CSVTranslator() {}

    /**
	 * After retrieving the .csv file and reading it, the file content is converted into a string,
	 * each line then broken down into an array broken apart by the strings, then added to a list
	 * which is then returned.
	 * 
	 * @param filePath The location path of the .csv file.
	 * @return A list of string arrays (essentially a double array) that contain the contents of the .csv file.
	 */
    public List<String[]> processCSVFile(String filePath) throws CSVFormatException {
        BufferedReader br = null;
        String line = "";
        int numOfColumns; // This is used to count the data columns, ensuring the file is valid
        List<String[]> data = new ArrayList<>(); // This will hold the data from the csv file

        boolean invalid = false;
        
        try { // User selects a file.
			br = new BufferedReader(new FileReader(new File(filePath)));
		} catch (FileNotFoundException e) {
			invalid = true;
		} 
        
        try {
			while((line = br.readLine())!=null) { // If the line isn't null (contains something)
			    if (line.contains(">")) { // If the line contains a '>' char, indicating it is a truck type, not an item
			        continue; // Move onto next iteration
			    }
			    String[] splitted = line.split(","); // Separates the line into elements using the comma to delimit
			    numOfColumns = splitted.length; // Find how many columns there are
			    // If it doesn't match any of the accepted file formats, throw exception
			    if ((numOfColumns != 2) && (numOfColumns != 5) && (numOfColumns != 6)) {
			    	invalid = true;
			    }
			    data.add(splitted); // Add the splitted line array
			}
			
			br.close(); // Close the file and let others use it
		} catch (IOException e) {
			invalid = true;
		}
       
        if (invalid) {
        	throw new CSVFormatException();
        }
        
        return data;
    }
}
