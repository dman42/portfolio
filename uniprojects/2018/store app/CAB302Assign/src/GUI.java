import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.math.BigDecimal;
import java.util.HashMap;
import java.util.List;

/**
 * Class that outputs the programs "Graphical User Interface".
 * 
 * @author John Tang
 */
public class GUI {
	
	private Manifest manifest;
	private Stock stock;
	private Store store; 
	private JTable jt;
	private JTable assetJt;
	private JScrollPane sp;
	private String filePath; // Used to store the path of the user selected file
    public static String directoryPath; // Directory for loaded manifest
    private BigDecimal capital; // Running total of store capital
    private List<String[]> itemData; // Contains the store's inventory quantities
    private List<String[]> assetItemData;
    
    public GUI() {}

    public void drawGUI() {
        // Main frame
        JFrame frame = new JFrame("Colesworth Manager"); // Frame that will house the GUI
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // Set default action when closed

        JTabbedPane tabFrame = new JTabbedPane(); // Tabs pane
        JPanel panelAssets = new JPanel(); // Assets tab
        JPanel panelSales = new JPanel(); // Sales tab
        JPanel panelOrders = new JPanel(); // Orders tab
        panelAssets.setLayout(null); // Uses absolute referencing coordinates
        panelSales.setLayout(null);
        panelOrders.setLayout(null);

        // Adding all the tabs to the tab panel
        tabFrame.add("Assets", panelAssets);
        tabFrame.add("Sales", panelSales);
        tabFrame.add("Orders", panelOrders);
        frame.getContentPane().add(tabFrame); // Adding the tabbed pane to the main frame

        // ASSETS TAB ------
        JLabel assetsLabel1 = new JLabel("Asset Operations");
        assetsLabel1.setBounds(20,20,300,20);
        panelAssets.add(assetsLabel1);

        JLabel assetsLabel2 = new JLabel("Capital: $0");
        assetsLabel2.setBounds(350,20,200,20);
        panelAssets.add(assetsLabel2);

		JButton assetsButton2 = new JButton("Load Item Properties");
		assetsButton2.setBounds(20,40,200,20);
		panelAssets.add(assetsButton2);

		JButton assetsButton3 = new JButton("Add To Stock");
		assetsButton3.setBounds(20,360,200,20);
		assetsButton3.setEnabled(false);
		panelAssets.add(assetsButton3);

     	// SALES TAB ------
        JLabel salesLabel1 = new JLabel("Sales Operations");
        salesLabel1.setBounds(20,20,200,20);
        panelSales.add(salesLabel1);

        JButton salesButton1 = new JButton("Load Sales Log");
        salesButton1.setBounds(20,40,200,20);
        salesButton1.setEnabled(false);
        panelSales.add(salesButton1);
        
        JButton salesButton2 = new JButton("Apply");
        salesButton2.setBounds(20,360,200,20);
        salesButton2.setEnabled(false);
        panelSales.add(salesButton2);
        
        // ORDERS TAB ------
        JLabel ordersLabel1 = new JLabel("Manifest Operations");
        ordersLabel1.setBounds(20,20,200,20);
        panelOrders.add(ordersLabel1);

        JButton ordersButton1 = new JButton("Load Manifest");
        ordersButton1.setBounds(20,40,200,20);
        ordersButton1.setEnabled(false);
        panelOrders.add(ordersButton1);

        JButton ordersButton2 = new JButton("Export Manifest");
        ordersButton2.setBounds(225,40,200,20);
        ordersButton2.setEnabled(false);
        panelOrders.add(ordersButton2);
        
        JButton ordersButton3 = new JButton("Apply");
        ordersButton3.setBounds(20,360,200,20);
        ordersButton3.setEnabled(false);
        panelOrders.add(ordersButton3);

        // Item properties added
        assetsButton3.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                CSVTranslator csvt = new CSVTranslator();
                try {
					assetItemData = csvt.processCSVFile(filePath); // Select and process a csv file, returning the data
					int itemArraySize = assetItemData.size();
	                Item[] itemArray = new Item[itemArraySize];
	                Double temp = null;

	                for (int i = 0; i < itemArraySize; i++) {
	                    try { // Some items do not have temperatures, so we wrap inside try/catch
	                        temp = Double.parseDouble(assetItemData.get(i)[5]);

	                    } catch (IndexOutOfBoundsException eI) {}
	                    // Getting the data from each row
	                    itemArray[i] = new Item(assetItemData.get(i)[0], Double.parseDouble(assetItemData.get(i)[1]),
	                            Double.parseDouble(assetItemData.get(i)[2]), Integer.parseInt(assetItemData.get(i)[4]),
	                            Integer.parseInt(assetItemData.get(i)[3]), temp);
	                }
	                stock = new Stock(itemArray);
	                store = new Store("store name");
	                manifest = new Manifest(store, stock);
	                assetsButton3.setEnabled(false); // This will alter the state of buttons based on the user events
	                assetsButton2.setEnabled(false);
	                salesButton1.setEnabled(true);
	                ordersButton1.setEnabled(true);
	                ordersButton2.setEnabled(true);
	                capital = new BigDecimal(store.getCapital());
	                capital = capital.setScale(2, BigDecimal.ROUND_DOWN); //Updates capital and sets it to two decimal places.
	                assetsLabel2.setText("Capital: $" + capital);
	                JOptionPane.showMessageDialog(panelAssets, "All items added."); // Confirmation dialog
				} catch (CSVFormatException e1) {}
            }
        });

        //Creates item properties.
        assetsButton2.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                final JFileChooser fc = new JFileChooser(); // Open file chooser
                int returnVal = fc.showOpenDialog(panelOrders); // What the user selects (cancel/selects a file)
                if (returnVal == JFileChooser.APPROVE_OPTION) { // If the user selects a file
                    File file = fc.getSelectedFile();
                    filePath = file.getAbsolutePath(); // File path
                    directoryPath = file.getParent(); // This obtains the directory path rather than the file path
                    CSVTranslator csvt = new CSVTranslator();
                    try {
                    	assetItemData = csvt.processCSVFile(filePath);
                    	assetJt = new JTable(); // Initialise a new table object
	                    String[] columnNames = new String[] { // Set the columns for the new table
	                            "NAME", "QUANTITY", "C/PRICE", "S/PRICE", "R/O PNT", "R/O AMT", "TEMP"};
	                    Object[][] content = new Object[assetItemData.size()][7]; // Initialise the data structure for the table

						// Populate the 'content' 2d array with data
	                    for(int i = 0; i < assetItemData.size(); i++) {
	                        content[i][0] = assetItemData.get(i)[0];
	                        content[i][1] = 0; // Quantities start at 0
	                        content[i][2] = assetItemData.get(i)[1];
	                        content[i][3] = assetItemData.get(i)[2];
	                        content[i][4] = assetItemData.get(i)[3];
	                        content[i][5] = assetItemData.get(i)[4];
	                        try { // Some items do not have temperatures, so we wrap inside try/catch
	                            content[i][6] = assetItemData.get(i)[5];
	                        } catch (IndexOutOfBoundsException eI) {}
	                    }

	                    assetJt.setModel(new DefaultTableModel(content,columnNames));
	                    assetJt.setEnabled(false); // This disables user editing
	                    JScrollPane sp = new JScrollPane(assetJt); // Make the table scrollable
	                    sp.setBounds(20, 60, 450, 300);
	                    panelAssets.add(sp); // Add scroll panel (which contains the table) to the tabbed panel
	                    assetsButton3.setEnabled(true); // Alter the state of buttons based on user actions
					} catch (CSVFormatException e1) {
						JOptionPane.showMessageDialog(panelAssets, "Invalid or missing file."); // User selected invalid file.
					}
                }
            }
        });

        // Add to sales log
        salesButton2.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                CSVTranslator csvt = new CSVTranslator();
                try {
					itemData = csvt.processCSVFile(filePath); // Get data from a selected file
					HashMap<Item, Integer> sales = new HashMap<Item, Integer>(); // Data structure to hold the item and quantity sold

					// Stores the items and quantities in a hashmap
	                for (int i = 0; i < itemData.size(); i++) {
	                    for (HashMap.Entry<Item, Integer> stockedItem : stock.getInventory().entrySet()) {
	                        if (stockedItem.getKey().getName().equals(itemData.get(i)[0])) {
	                            sales.put(stockedItem.getKey(), Integer.parseInt(itemData.get(i)[1]));
	                        }
	                    }
	                }

	                try {
	                    stock.salesLog(store, sales); // This will process the sales from the sales log file
						for (int i = 0; i < assetJt.getRowCount(); i++) {
							for(HashMap.Entry<Item,Integer> item : stock.getInventory().entrySet()) {
								if (item.getKey().getName().equals(assetJt.getValueAt(i, 0))) {
									assetJt.setValueAt(item.getValue(), i, 1);
								}
							}
						}
	                    
						panelOrders.remove(jt);
						panelOrders.remove(sp);
						panelOrders.validate();
						panelOrders.repaint();
						
	                    salesButton2.setEnabled(false); // Alter the state of buttons based on user action
	                    salesButton1.setEnabled(true);
	                    capital = new BigDecimal(store.getCapital());
	                    capital = capital.setScale(2, BigDecimal.ROUND_DOWN); //Updates capital and sets it to two decimal places.
	                    assetsLabel2.setText("Capital: $" + capital);
	                    JOptionPane.showMessageDialog(panelAssets, "Inventory and capital updated.");
	                } catch (StockException e1) {
	                	JOptionPane.showMessageDialog(panelAssets, "ERROR: Insufficent items to have sold.");
	                }
				} catch (CSVFormatException e2) {}
            }
        });

        // Load sales log button
        salesButton1.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                final JFileChooser fc = new JFileChooser(); // Open file chooser
                int returnVal = fc.showOpenDialog(panelOrders); // What the user selects (cancel/selects a file)
                if (returnVal == JFileChooser.APPROVE_OPTION) { // If the user selects a file
                    File file = fc.getSelectedFile();
                    filePath = file.getAbsolutePath(); // File path
                    CSVTranslator csvt = new CSVTranslator();
                    try {
						itemData = csvt.processCSVFile(filePath); // Gets the data from a user selected file
						jt = new JTable(); // Creates a new table
	                    String[] columnNames = new String[] { // Creates columns for the table
	                            "NAME", "NO. SOLD"};
	                    Object[][] content = new Object[itemData.size()][3]; // Data structure to hold table data

						// Put the table data from the csv file into the 'content' 2d array
	                    for(int i = 0; i < itemData.size(); i++) {
	                        content[i][0] = itemData.get(i)[0];
	                        content[i][1] = itemData.get(i)[1];
	                    }

	                    jt.setModel(new DefaultTableModel(content,columnNames));
	                    jt.setEnabled(false); // This disables user editing
	                    sp = new JScrollPane(jt); // Make the table scrollable
	                    sp.setBounds(20, 60, 450, 300);
	                    panelSales.add(sp); // Add scroll panel (which contains the table) to the tabbed panel
	                    salesButton2.setEnabled(true); // Alter the buttons based on user action
					} catch (CSVFormatException e1) {
						JOptionPane.showMessageDialog(panelAssets, "Invalid or missing file."); // User selected an invalid file
					}
                }
            }
        });

        // Loads Manifest file.        
        ordersButton1.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                final JFileChooser fc = new JFileChooser(); // Open file chooser
                int returnVal = fc.showOpenDialog(panelOrders); // What the user selects (cancel/selects a file)
                if (returnVal == JFileChooser.APPROVE_OPTION) { // If the user selects a file
                    File file = fc.getSelectedFile();
                    filePath = file.getAbsolutePath(); // File path
                    CSVTranslator csvt = new CSVTranslator();
                    try {
						itemData = csvt.processCSVFile(filePath); // Get the data from the user selected csv file
						jt = new JTable(); // Construct a new table to house the data
	                    String[] columnNames = new String[] {
	                        "NAME", "NO. BOUGHT"}; // Columns for the table
	                    Object[][] content = new Object[itemData.size()][3]; // Data structure for the table

	                    try {
	                    	for(int i = 0; i < itemData.size(); i++) { // Put the data into the 'content' 2d array
	                    		if (!(itemData.get(i)[0].contains("<"))){
		                            content[i][0] = itemData.get(i)[0];
		                            content[i][1] = itemData.get(i)[1];
	                    		}
	                        }
	                    	jt.setModel(new DefaultTableModel(content,columnNames));
	                        jt.setEnabled(false); // This disables user editing
	                        sp = new JScrollPane(jt); // Make the table scrollable
	                        sp.setBounds(20, 60, 450, 300);
	                        panelOrders.add(sp); // Add scroll panel (which contains the table) to the tabbed panel
	                        ordersButton3.setEnabled(true); // Alter the button states based on user action
	                    } catch (IndexOutOfBoundsException eI) {
	                    	JOptionPane.showMessageDialog(panelAssets, "Invalid file."); // User selected an invalid file
	                    }
					} catch (CSVFormatException e1) {
						JOptionPane.showMessageDialog(panelAssets, "Invalid or missing file.");
					}
                }
            }
        });
        
        //Commits changes done by manifest.
        ordersButton3.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e) {
            	CSVTranslator csvt = new CSVTranslator();
            	try {
					itemData = csvt.processCSVFile(filePath); // Get data from user selected csv file
					HashMap<Item, Integer> purchased = new HashMap<Item, Integer>(); // Data structure to house the item data

					// Putting the data into the 'purchased' hashmap
	            	for (int i = 0; i < itemData.size(); i++) {
	            		for (HashMap.Entry<Item, Integer> stockedItem : stock.getInventory().entrySet()) {
	              			if (stockedItem.getKey().getName().equals(itemData.get(i)[0])) {
	            				purchased.put(stockedItem.getKey(), Integer.parseInt(itemData.get(i)[1]));
	            			}
	            		}
	            	}
	            	
	            	try {
						manifest.loadManifest(purchased);
						for (int i = 0; i < assetJt.getRowCount(); i++) {
							for(HashMap.Entry<Item,Integer> item : stock.getInventory().entrySet()) {
								if (item.getKey().getName().equals(assetJt.getValueAt(i, 0))) {
									assetJt.setValueAt(item.getValue(), i, 1);
								}
							}
						}
						
						panelOrders.remove(jt);
						panelOrders.remove(sp);
						panelOrders.validate();
						panelOrders.repaint();
						
						ordersButton3.setEnabled(false); // Alter the button state based on user action
						ordersButton1.setEnabled(true);
						ordersButton2.setEnabled(true);
						capital = new BigDecimal(store.getCapital());
						capital = capital.setScale(2, BigDecimal.ROUND_DOWN);
						assetsLabel2.setText("Capital: $" + capital);
			    		JOptionPane.showMessageDialog(panelAssets, "Inventory and capital updated.");
					} catch (DeliveryException e1) {
						JOptionPane.showMessageDialog(panelAssets, "ERROR: Insufficent funds to afford items.");
					}
				} catch (CSVFormatException e2) {} // Get the data from the CSV file
            }
        });

        //Exports manifest file.
        ordersButton2.addActionListener(new ActionListener(){
        	public void actionPerformed(ActionEvent e) {
        		try {
					manifest.exportManifest();
                    JOptionPane.showMessageDialog(panelAssets, "File exported successfully to " + GUI.directoryPath);
				} catch (IOException e1) {
					JOptionPane.showMessageDialog(panelAssets, "Failed to export .csv file.");
				}
            } 
        });

        // Setting the window properties and displaying it
        frame.setMinimumSize(new Dimension(635, 570)); // User can't minimise lower than this
        frame.setPreferredSize(new Dimension(635, 570)); // Set default size when opened
        frame.setVisible(true); // Show the GUI
    }
}

