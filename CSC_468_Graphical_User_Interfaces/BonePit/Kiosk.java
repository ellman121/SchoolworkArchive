//Imports

import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;

//End Imports

/**
 * <p>
 * The beginning point of the project. This class inherits from JFrame, and is
 * home to all of the visual components, except for Bone Detail Pop-ups.
 *
 * @author Brady Shimp, Elliott Rarden
 * @version 1
 */
public class Kiosk extends JFrame{
// Private Class Members

	private GraphicBoneYard _BoneYard = null;
	private JScrollPane _ScrollYard = null;
	private JSlider _ElevationSlider = null;
	private JSlider _DetailSlider = null;
	private int _ElevationSliderValue = 0;
	private int _DetailSliderValue = 0;
	
// End Private Class Members

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Public Class Members

	public static String path = "bonexml/";

// End Public Class Members

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Public Class Methods

    /**
    * <p>
    * Constructor for the main window of the project. Defines all initial 
    * layouts and has Key and Mouse Listeners for various events on the window
    */
	public Kiosk(){
		super("Welcome to Mammoth Site");
		JPanel top = new JPanel();
		JPanel right = new JPanel();
		JPanel bottom = new JPanel();
		Container content = getContentPane();
		BorderLayout layout = new BorderLayout();

        //Set these to a flat size so the left and center get all the room
        top.setMaximumSize(new Dimension(0, 0));
		right.setMaximumSize(new Dimension(0,0));
		bottom.setMaximumSize(new Dimension(0,0));

        //key listeners for escape to close, arrows to adjust view on the
        //Bone Yard, and +/- keys to zoom in and out of the Bone Yard
		this.addKeyListener( new KeyAdapter(){
			@Override public void keyPressed(KeyEvent e) { 
			    //kill the program on <esc> at main window level
			    if(e.getKeyCode() == KeyEvent.VK_ESCAPE) System.exit(0); 
			    else{
			        int incValue = 10;
			        switch( e.getKeyCode() ) { 
                        case (KeyEvent.VK_UP): // scroll up
                             _BoneYard.AdjustScrollWithArrows(0, -incValue);
                             break;
                        case (KeyEvent.VK_DOWN): // scroll down
                             _BoneYard.AdjustScrollWithArrows(0, incValue);
                             break;
                        case (KeyEvent.VK_LEFT): // scroll left
                             _BoneYard.AdjustScrollWithArrows(-incValue, 0);
                             break;
                        case (KeyEvent.VK_RIGHT) : //scroll right
                             _BoneYard.AdjustScrollWithArrows(incValue, 0);
                             break;
                        case (KeyEvent.VK_ADD) :
                        case (KeyEvent.VK_EQUALS) : //essentially equivalent to '+'
                             _BoneYard.setScale(1); //zoom in
                             break;
                        case (KeyEvent.VK_SUBTRACT) : 
                        case (KeyEvent.VK_MINUS) :
                             _BoneYard.setScale(-1); //zoom out
                             break;
                        default: break;
                    }
			    }
			}
		});

        //if a click is detected on the window, apply focus to the window
		this.addMouseListener(new MouseAdapter(){
			@Override public void mouseClicked(MouseEvent e) { if(!hasFocus()) requestFocus(); }
		});

        //if component comes out of hiding or is dragged around, ensure focus
		this.addComponentListener(new ComponentAdapter(){
			@Override public void componentShown(ComponentEvent e) { if(!hasFocus()) requestFocus(); }
			@Override public void componentMoved(ComponentEvent e) { if(!hasFocus()) requestFocus(); }
		});
        
        
		content.setLayout( layout ); // set the layout
		content.add(top, BorderLayout.NORTH); //empty panel top
		content.add(right, BorderLayout.EAST); // empty panel right
		content.add(bottom, BorderLayout.SOUTH); // empty panel bottom
		content.add(CreateFilterMenu(), BorderLayout.WEST); //filters left
		content.add(CreateMainKioskBoneYardPanel(), BorderLayout.CENTER); //BoneYard Center

		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setContentPane(content);
		this.setResizable(false);     //don't resize
		this.setFocusable(true);
		this.setSize(1280, 720);      //set Width, Height
		this.setVisible(true);
	}

	/**
	* <p>
	* Starting point of the program. First ensures the availability of the 
	* bonexml folder. If not available, a JFileChooser is presented to the
	* user so that they can browse to where the folder exists.
	* <ul> <li> Credit to Elliot Rarden </ul><br>
	* The GUI is then created and ran on a new thread
	*
	* @param args      The command line arguments
	*/
	public static void main(String[] args){
		File f = new File("bonexml/bones.xml");
		if(!f.exists()) {
			while(true){
		    	JFileChooser jfc = new JFileChooser();
    		    jfc.setCurrentDirectory(new File(System.getProperty("user.home")));
    		    jfc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
    		    jfc.setDialogTitle("Select your bonexml folder");
    		    int retval = jfc.showOpenDialog(null);
    		    
    		    // Leave if they don't give us XML
    		    if (retval != JFileChooser.APPROVE_OPTION) {
    		    	System.exit(0);
    		    }
    
    		    // Get the selected file
    		    f = jfc.getSelectedFile();
    		    if (f.getName().equals("bonexml") && f.isDirectory()) {
    		    	break;
    		    }
		    }
		    Kiosk.path = f.getAbsolutePath() + "/";
		}

		SwingUtilities.invokeLater(new Runnable() { public void run() { CreateAndShowGUI(); } });
	}

// End Public Class Methods

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Private Class Methods

    /**
    * <p> 
    * Method that, on a background thread, creates the Kiosk window
    */
	private static void CreateAndShowGUI(){
	   Kiosk kiosk = new Kiosk();
	}

    /**
    * <p>
    * Create the left panel that has the bone filtering options
    * <ul> <li> Credit to Elliot Rarden </ul>
    */
	private JPanel CreateFilterMenu(){
		JPanel filterMenu = new JPanel();

		// Add a border to our thing so it looks "nice"
		filterMenu.setBorder(BorderFactory.createTitledBorder("Filter Bones"));

		// Set left orientation
		filterMenu.setAlignmentX(JScrollPane.LEFT_ALIGNMENT);

		// Add check boxes
		JCheckBox cb = new JCheckBox("Show male bones", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawMale(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		cb = new JCheckBox("Show female bones", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawFemale(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		cb = new JCheckBox("Show undesignated bones", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawUndesignated(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		cb = new JCheckBox("Show mammuthus columbi", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawColumbi(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		cb = new JCheckBox("Show mammuthus primigenius", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawPrimigenius(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		cb = new JCheckBox("Show unidentified mammoth", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawUnidentifiedMammoth(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		cb = new JCheckBox("Show unidentified bones", true);
		cb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox b = (JCheckBox) e.getSource();
				_BoneYard.setDrawUnidentified(b.getModel().isSelected());
				((JCheckBox)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		});
		filterMenu.add(cb);

		filterMenu.setPreferredSize(new Dimension(250, 600));

		return filterMenu;
	}

    /**
    * <p>
    * Set the layout of the portion of the Kiosk window that isn't the 
    * filter panel. This will contain the JScrollPane that _BoneYard exists in
    * and a JToolBar along the bottom.
    *
    * @returns  A formatted JPanel containing the _BoneYard and JToolBar
    */
	private JPanel CreateMainKioskBoneYardPanel(){
		JPanel top = new JPanel();
		JPanel left = new JPanel();
		JPanel right = new JPanel();
		_BoneYard = new GraphicBoneYard();
		JPanel graphicPanel = new JPanel();
		_ScrollYard = new JScrollPane(_BoneYard);
		BorderLayout boneyardLayout = new BorderLayout();

		_ScrollYard.setViewportView(_BoneYard);
		_ScrollYard.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		_ScrollYard.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		top.setMaximumSize(new Dimension(0,0)); //flat
		left.setMaximumSize(new Dimension(0,0)); //flat
		right.setMaximumSize(new Dimension(0,0)); //flat

		graphicPanel.setLayout(boneyardLayout);
		graphicPanel.add(top, BorderLayout.NORTH); //flat
		graphicPanel.add(left, BorderLayout.WEST); //flat
		graphicPanel.add(right, BorderLayout.EAST); //flat
		//ScrollPane with bones
		graphicPanel.add(_ScrollYard, BorderLayout.CENTER);
		//toolbar
		graphicPanel.add(CreateBottomOfScreenControls(), BorderLayout.SOUTH);

		return graphicPanel;
	}

    /**
    * <p>
    * Delegates filling the components of the JToolBar to other methods
    * 
    * @returns A Formatted JPanel containing the filled JToolBar
    */
	private JPanel CreateBottomOfScreenControls(){
		JToolBar toolBar = new JToolBar();
		JPanel kioskControls = new JPanel();
		kioskControls.setLayout(new FlowLayout());

        //fill toolbar with components
        SetDetailSlidercontrol(toolBar);
		SetElevationSliderControl(toolBar);
		SetZoomControls(toolBar);
		SetResetControl(toolBar);
		SetHelpControl(toolBar);

        toolBar.setFloatable(false); //lock the toolbar in place
		kioskControls.add(toolBar);
	    kioskControls.add(makeColorLegend());

		return kioskControls;
	}

    /**
    * <p>
    * Creates the JSlider control to add to the JToolBar
    *
    * @param toolBar        The JToolBar to place this control on
    */
	private void SetElevationSliderControl(JToolBar toolBar){
		JPanel sliderPanel = new JPanel();
		_ElevationSlider = new JSlider();

        //event to track changes to the sliders value
		_ElevationSlider.addChangeListener((ChangeEvent e) ->{
			//From: http://docs.oracle.com/javase/tutorial/uiswing/events/changelistener.html
			JSlider source = (JSlider)e.getSource();
			if(!source.getValueIsAdjusting()){
				_ElevationSliderValue = source.getValue();
				_BoneYard.setElevSliderValue(_ElevationSliderValue);
				source.transferFocusBackward(); //set focus back to JFrame
			}
		});

		_ElevationSlider.setMinimum(1);
		_ElevationSlider.setMaximum(5);

		_ElevationSlider.setMajorTickSpacing(1);
		_ElevationSlider.setMinorTickSpacing(1);

		_ElevationSlider.setPaintTicks(true);
		_ElevationSlider.setPaintLabels(true);
		_ElevationSlider.setSnapToTicks(true);

		_ElevationSlider.setPreferredSize(new Dimension(300, 50));

        JLabel sliderLabel = new JLabel("Elevation:");
        // sliderPanel.setLayout(new BoxLayout(sliderPanel, BoxLayout.Y_AXIS));
		sliderPanel.setPreferredSize(new Dimension(350, 75));
		sliderLabel.setAlignmentX(JLabel.CENTER);
		sliderPanel.add(sliderLabel);
		sliderPanel.add(_ElevationSlider);
		
		toolBar.add(sliderPanel);
	}

	/**
    * <p>
    * Creates the JSlider control to add to the JToolBar
    *
    * @param toolBar        The JToolBar to place this control on
    */
	private void SetDetailSlidercontrol(JToolBar toolBar){
		JPanel sliderPanel = new JPanel();
		_DetailSlider = new JSlider();

        //event to track changes to the sliders value
		_DetailSlider.addChangeListener((ChangeEvent e) -> {
			//From: http://docs.oracle.com/javase/tutorial/uiswing/events/changelistener.html
			JSlider source = (JSlider)e.getSource();
			if(!source.getValueIsAdjusting()){
				_DetailSliderValue = source.getValue();
				_BoneYard.setDetailSliderValue(_DetailSliderValue);
				source.transferFocusBackward(); //set focus back to JFrame
			}
		});

		_DetailSlider.setMinimum(1);
		_DetailSlider.setMaximum(5);

		_DetailSlider.setMajorTickSpacing(1);
		_DetailSlider.setMinorTickSpacing(1);

		_DetailSlider.setPaintTicks(true);
		_DetailSlider.setPaintLabels(true);
		_DetailSlider.setSnapToTicks(true);

		_DetailSlider.setPreferredSize(new Dimension(300, 50));

        JLabel sliderLabel = new JLabel("Level of Detail:");
        // sliderPanel.setLayout(new BoxLayout(sliderPanel, BoxLayout.Y_AXIS));
		sliderPanel.setPreferredSize(new Dimension(350, 75));
		sliderLabel.setAlignmentX(JLabel.CENTER);
		sliderPanel.add(sliderLabel);
		sliderPanel.add(_DetailSlider);
		
		toolBar.add(sliderPanel);
	}

    /**
    * <p>
    * Adds the +, - buttons to the toolBar for zooming in and out
    *
    * @param toolBar        The JToolBar to place this control on
    */
	private void SetZoomControls(JToolBar toolBar){
		JButton zoomOut = new JButton ("-");
		JButton zoomIn = new JButton("+");
		JPanel zoomPanel = new JPanel();

        //a listener to determine who clicked it and how to zoom
		ActionListener zoomListener = new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e){
				if(e.getSource() == zoomIn) setZoomFactor(1); //zoom in
				else setZoomFactor(-1); //zoom out
				//set focus back to main window
				((JButton)e.getSource()).getTopLevelAncestor().requestFocus();
			}
		};
		
		//set action Listener on both buttons
		zoomIn.addActionListener(zoomListener);
		zoomOut.addActionListener(zoomListener);

        //try to set sizes
        zoomPanel.setPreferredSize(new Dimension(100, 90));
		zoomOut.setPreferredSize(new Dimension(70, 30));
		zoomIn.setPreferredSize(new Dimension(70, 30));
		
		//add buttons along with descriptive label
		JLabel zoomLabel = new JLabel("Zoom: ");
		zoomLabel.setAlignmentX(JLabel.CENTER);
		zoomPanel.add(zoomLabel);
		zoomPanel.add(zoomIn);
		zoomPanel.add(zoomOut);

		toolBar.add(zoomPanel);
	}
	






    //needs documentation
	private JPanel makeColorLegend()
	{
	    JPanel emptyPanel = new JPanel();
	    JPanel legendPanel = new JPanel();
	    JLabel red = new JLabel("red");
	    JLabel green = new JLabel("green");
	    JLabel orange = new JLabel("orange");
	    JPanel legendContents = new JPanel();
	    JLabel legendTitle = new JLabel("Colors By Completeness: ");
	    
	    legendTitle.setAlignmentX(JLabel.CENTER);
	    legendPanel.setLayout(new BorderLayout());
	    emptyPanel.setMaximumSize(new Dimension(0,0));
	    legendContents.setLayout(new GridLayout(0, 2));
	    
	    red.setForeground(Color.RED);
	    green.setForeground(Color.GREEN);
	    orange.setForeground(Color.ORANGE);

	    legendContents.add(new JLabel("CO : "));
	    legendContents.add(green);	    
	    legendContents.add(new JLabel("PC : "));
	    legendContents.add(orange);	    
	    legendContents.add(new JLabel("PE : "));
	    legendContents.add(red);
	    legendContents.add(new JLabel("other : "));
	    legendContents.add(new JLabel("black"));
	    
	    legendPanel.add(emptyPanel, BorderLayout.WEST);
	    legendPanel.add(emptyPanel, BorderLayout.EAST);
	    legendPanel.add(emptyPanel, BorderLayout.SOUTH);
	    legendPanel.add(legendTitle, BorderLayout.NORTH);
	    legendPanel.add(legendContents, BorderLayout.CENTER);	    
	    
	    return legendPanel;
	}
	
	/**
	* <p>
	* Adds the reset button to the toolBar
	*
	* @param toolBar        The JToolBar to place this control on
	*/
	private void SetResetControl(JToolBar toolBar){
	    JPanel resetPanel = new JPanel();
	    JButton reset = new JButton("Reset");
	    
	    //add action listener to button to call a reset method
	    reset.addActionListener((ActionEvent e) ->{
	            _BoneYard.Reset(); //tell _BoneYard to reset
	            _DetailSlider.setValue(5); //reset to slider to starting value
	            _ElevationSlider.setValue(5);
	            
	            //set focus back to main window
	            ((JButton)e.getSource()).getTopLevelAncestor().requestFocus();
	    });
	    
	    resetPanel.add(reset);
	    toolBar.add(resetPanel);
	}
	
	/**
	* <p>
	* Adds the about button to the toolBar
	*
	* @param toolBar        The JToolBar to place this control on
	*/
	private void SetHelpControl(JToolBar toolBar){
	    JButton about = new JButton("About");
	    JPanel aboutPanel = new JPanel();
	    
	    //add action listener to button to call a method to display About
	    about.addActionListener((ActionEvent e) -> {
	        DisplayAbout();
	        //set focus back to main window
	        ((JButton)e.getSource()).getTopLevelAncestor().requestFocus();
	    });
	    
	    aboutPanel.add(about);
	    toolBar.add(aboutPanel);
	}
	
	/**
	* <p>
	* Displays a pop-up window with information about this Kiosk program
	*/
	private void DisplayAbout(){
	    JOptionPane.showMessageDialog(null, "Welcome to the Mammoth Site Kiosk.\nCreated by Brady Shimp, Elliott Rarden, and Aaron Alphonsus\n\nThe boxes on the left hand side allow you to enable and disable bones\nThe slider at the bottom allows you to show bones above an elevation\nThe zoom controlls allow you to zoom in and out of the bone yard\nThe reset button takes you back to the default screen");
	}

    /**
    * <p>
    * Passes the request to zoom in/out from zoom controls to _BoneYard
    *
    * @param alterValue     The value to change the scaling rate by
    */
	private void setZoomFactor(int alterValue){
	    _BoneYard.setScale(alterValue);
	}

// End Private Class Methods
}
