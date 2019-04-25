import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

/**
 * Class that both generates and loads manifests.
 * 
 * @author David McClarty
 */
public class Manifest{
    private ArrayList<FridgeTruck> fridgeTruckList; // List of refrigerated trucks.
    private ArrayList<OrdinaryTruck> ordinaryTruckList; // List of non-refrigerated trucks.
    private Object[][] refrigerated; // List of refrigerated items.
    private Object[][] nonrefrigerated; // List of non-refrigerated items.
    private Store store; // Store class.
    private Stock stock; // Class that stores the inventory.
    
    /**
     * Constructor for the Manifest object. Initializes all hashmaps with new empty ones, and
     * saves the store and stock classes in use as variables.
     * 
     * @param store The store.
     * @param stock The stock that keeps track of the store's inventory.
     */
    public Manifest(Store store, Stock stock) {
    	fridgeTruckList = new ArrayList<FridgeTruck>();
 		ordinaryTruckList = new ArrayList<OrdinaryTruck>();
 	 	this.store = store;
    	this.stock = stock;
    }
    
    /**
     * Sorts a double array of items (and their respective values) from 
     * lowest maximum temperature to highest. This method was designed 
     * specifically for the list of refrigerated items.
     * 
     * @param unsorted Unsorted double array. The first instance of the 
     * array instance must be an Item with a set maximum temperature, the
     * second instance must be an integer.
     * 
     * @param arrayLength Stores the amount of items in "unsorted" 
     * (not the length of unsorted).
     */
    public void sortFridgeArray(Object[][] unsorted, int arrayLength){
    	int lengthSort = 1;
        while (lengthSort < arrayLength){
            Object[] newItem = unsorted[lengthSort];

            int currentPos = lengthSort;
            while (currentPos > 0 && ((Item) unsorted[currentPos - 1][0]).getMaxTemp() > ((Item) newItem[0]).getMaxTemp()) {
                unsorted[currentPos] = unsorted[currentPos - 1];
                currentPos--;
            }

            unsorted[currentPos] = newItem;
            lengthSort++;
        }
    }
    
    /**
     * Produces a manifest, by finding how many items are contained in each truck, 
     * then applying those changes by adding to the store's personal inventory with the items 
     * and decreasing the capital with the cost of the items and the trucks.
     * 
     * Throws a Delivery Exception if the store has insufficent funds to pay for the items/trucks.
     */
    public void applyManifest() throws DeliveryException {
    	double totalCost = 0;
		for (OrdinaryTruck truck : ordinaryTruckList) {
			// For every item purchased, the item quantity is increased, and the store capital is decreased
			for (HashMap.Entry<Item, Integer> cargo : truck.getCargo().entrySet()) {
				stock.increaseQuantity(cargo.getKey(), cargo.getValue());
				totalCost += (cargo.getKey().getManufacturerCost()) * cargo.getValue();
			}
			totalCost += truck.getCost(); // Add the truck's cost to the running total
		}
		for (FridgeTruck truck : fridgeTruckList) {
			// For every item purchased, the item quantity is increased, and the store capital is decreased
			for (HashMap.Entry<Item, Integer> cargo : truck.getCargo().entrySet()) {
				stock.increaseQuantity(cargo.getKey(), cargo.getValue());
				totalCost += (cargo.getKey().getManufacturerCost()) * cargo.getValue();
			}
			totalCost += truck.getCost(); // Add the fridge truck's cost to the running total
		}
		if (store.getCapital() < totalCost) { // If there isn't enough capital to purchase the trucks, throw exception
			throw new DeliveryException();
		}
		
		store.setCapital(-(totalCost)); // Update the capital
		
		fridgeTruckList = new ArrayList<FridgeTruck>(); // List of fridge trucks we need
 		ordinaryTruckList = new ArrayList<OrdinaryTruck>(); // List of ordinary trucks we need
    }
            
    /**
     * Determines how many trucks are neccesary to carry the necessary items, then inserts the items
     * into the trucks, based on the items and quantities specified by the parameter. If the item's type
     * is not included in the stock inventory, the item's type is also added to stock.
     * 
     * @param importedManifest The purchased items and their respective quantities.
     */
    public void loadManifest(HashMap <Item, Integer> importedManifest) throws DeliveryException {
    	
    	refrigerated = new Object[importedManifest.size()][2]; 
		nonrefrigerated = new Object[importedManifest.size()][2];
		
		int ordAmount = 0; // Keeps track of the amount of ordinary items.
		int coldAmount = 0; // Keeps track of the amount of refrigerated items.
		
		// Each item is placed in one of two double arrays based on their temperature status,
		// and the amount of each items are tallied.
		
    	for (HashMap.Entry<Item, Integer> item : importedManifest.entrySet()) {
     		if (item.getKey().getMaxTemp() == null) {
    			nonrefrigerated[ordAmount][0] = item.getKey();
    			nonrefrigerated[ordAmount][1] = item.getValue();
    			ordAmount++;
    		}
    		else {
    			refrigerated[coldAmount][0] = item.getKey();
    			refrigerated[coldAmount][1] = item.getValue();
    			coldAmount++;
    		}
    	}
    	    
    	
    	sortFridgeArray(refrigerated, coldAmount); // Array of refridgerated items is sorted by temperature.
    	
    	if (refrigerated.length > 0) { // If no refrigerated items, the program doesn't create any refrigerated trucks.
    		fridgeTruckList.add(new FridgeTruck());
    	}
    			
    	int truckNum = 0;
    	for (int i = 0; i < coldAmount; i++) { // For each refrigerated item...
    		int value = (int) refrigerated[i][1]; // Amount of the item to be put into the trucks.
    		
    		// If the current truck cannot fit the item's reorder amount, two or more trucks are
    		// created and the item amount is split across them.
    		if (fridgeTruckList.get(truckNum).getCargoCount() + value > Truck.coldTruckCap) {
    			do {
    				int remainingSpace = Truck.coldTruckCap - fridgeTruckList.get(truckNum).getCargoCount();
	    			if (remainingSpace != 0) {
	    				try {
							fridgeTruckList.get(truckNum).addCargo((Item) refrigerated[i][0], remainingSpace);
						} catch (DeliveryException e) {
							e.printStackTrace();
						}
	    			}
	    			fridgeTruckList.add(new FridgeTruck());
	    			truckNum++;
	    			value -= remainingSpace;
    			} while (value >= Truck.coldTruckCap);
    		}
    		
    		// Adds the neccesary item amount to the current truck's cargo.
    		try {
				fridgeTruckList.get(truckNum).addCargo((Item) refrigerated[i][0], value);
			} catch (DeliveryException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	
    	// Becomes true once it is determined whether or not the final refrigerated truck is full.
    	boolean truckFull = false; 
    	
    	// Keeps track of whether a specific ordinary item has to be carried over both ordinary and refrigerated trucks.
    	boolean contItem = false; 
    	
    	// Keeps track of how much of the aformentioned item has to be carried over.
    	int contItemValue = 0;
    	   	
       	for (int i = 0; i < ordAmount; i++) { // For each ordinary item...
       		
       		int value; // Amount of the item to be put into the trucks.
       		
       		if (contItem) { // See description of "contItem" and "contItemValue" for explanation.
       			value = contItemValue; // Value is whatever is leftover.
       			i--;
       			contItem = false; 
       		}
       		else {
       			value = (int) nonrefrigerated[i][1]; // Value is the item's reorder amount.
       		}
    		
    		  		
    		if (!truckFull) { // If the last fridge truck wasn't confirmed to be full...
    			
    			//...and it isn't full...
    			if ((fridgeTruckList.get(truckNum).getCargoCount() < Truck.coldTruckCap) && (!(fridgeTruckList.isEmpty()))) {
    				
    				// ...if the current truck cannot fit the item's reorder amount, the truck is filled with the item
    				// and what is leftover is saved to put into the next truck.
    				if (fridgeTruckList.get(truckNum).getCargoCount() + value >= Truck.coldTruckCap) {
						int remainingSpace = Truck.coldTruckCap - fridgeTruckList.get(truckNum).getCargoCount();
		    			if (remainingSpace != 0) {
		    				try {
								fridgeTruckList.get(truckNum).addCargo((Item) nonrefrigerated[i][0], remainingSpace);
							} catch (DeliveryException e) {
								e.printStackTrace();
							}
		    			}
		    			
		    			contItem = true;
		    			contItemValue = value - remainingSpace;
    				}
    				
    				//...otherwise, the ordinary items are placed in the refrigerated truck
    				else {
    					try {
    						fridgeTruckList.get(truckNum).addCargo((Item) nonrefrigerated[i][0], value);
    					} catch (DeliveryException e) {
    						e.printStackTrace();
    					}
    				}
    				
            	}
        		
    			//...but it is full, then an ordinary truck is created.
        		else {
        			truckFull = true;
        			truckNum = 0;
        			ordinaryTruckList.add(new OrdinaryTruck());
        		}
    		}
    		
    		
    		if (truckFull) { // If the last fridge truck was confirmed to be full, fills the neccesary ordinary trucks.
				if (ordinaryTruckList.get(truckNum).getCargoCount() + value >= Truck.ordTruckCap) {
					
					// If the current truck cannot fit the item's reorder amount, two or more trucks are
		    		// created and the item amount is split across them.
					do {
						int remainingSpace = Truck.ordTruckCap - ordinaryTruckList.get(truckNum).getCargoCount();
		    			if (remainingSpace != 0) {
		    				try {
								ordinaryTruckList.get(truckNum).addCargo((Item) nonrefrigerated[i][0], remainingSpace);
							} catch (DeliveryException e) {
								e.printStackTrace();
							}
		    			}
		    			ordinaryTruckList.add(new OrdinaryTruck());
		    			truckNum++;
		    			value -= remainingSpace;
					} while (value >= Truck.ordTruckCap);
					
				}
				
				// Adds the neccesary item amount to the current truck's cargo.
				try {
					ordinaryTruckList.get(truckNum).addCargo((Item) nonrefrigerated[i][0], value);
				} catch (DeliveryException e) {
					e.printStackTrace();
				}
    		}  
       	}
       	
       	// All relevant item quantities are increased and the capital is decreased.
    	applyManifest(); 
    }
    
    /**
     * Creates a manifest .csv file, made up of whatever items are neccesary to reorder
     * based on whether their quantities are below their reorder points, and their reorder amounts.
     * The number of files exported increments at the end.
     * 
     * @throws IOException 
     */
    public void exportManifest() throws IOException {
    	
    	String fileName = "manifestExpt.csv";
    	FileWriter fw = new FileWriter(new File(GUI.directoryPath, fileName));
        StringBuilder sb = new StringBuilder();
        sb.append(">Refrigerated\n");
        for (HashMap.Entry<Item, Integer> item : stock.getInventory().entrySet()) {
        	if ((item.getValue() < item.getKey().getReorderPoint()) && (item.getKey().getMaxTemp() != null)) {
		        sb.append(item.getKey().getName());
		        sb.append(',');
		        sb.append(item.getKey().getReorderAmount());
		        sb.append('\n');
        	}
        }
        
        sb.append(">Ordinary\n");

        for (HashMap.Entry<Item, Integer> item : stock.getInventory().entrySet()) {
        	if (item.getValue() < item.getKey().getReorderPoint() && (item.getKey().getMaxTemp() == null)) {
		        sb.append(item.getKey().getName());
		        sb.append(',');
		        sb.append(item.getKey().getReorderAmount());
		        sb.append('\n');
        	}
        }
        fw.write(sb.toString());
        fw.close();
    }			
}