//Imports

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import javax.swing.*;
import java.util.*;

//End Imports

/**
 * <p>
 * The JPanel that the polylines from bonexml are drawn to. This class inherits
 * from JPanel, and handles all of the Drawing, Resizing, and Movements
 * performed on it.
 *
 * @author Brady Shimp, Elliott Rarden
 * @version 1
 */
public class GraphicBoneYard extends JPanel
{
// Private Class Members

	private Dimension _OriginalDimension = new Dimension(0,0);
	private Dimension _CurrentDimension = new Dimension(0,0);
	private ArrayList<Bone> _Bones = new ArrayList<>();
	private Point2D.Double _WalkwayMin = null;
	private Point2D.Double _WalkwayMax = null;
	private Point _MousePosition = null;
	private boolean _FirstLoad = true;
	private int elevSliderVal = 5;
	private int detailSliderVal = 15;
	private double _DeltaX = 0;
	private double _DeltaY = 0;
	private double _XScale = 0;
	private double _YScale = 0;
	private int _Scale = 1;
	private double _minElevation = 0.0;
	private double _maxElevation = 0.0;

	// Wether or not to draw things
	private boolean _drawMale = true;
	private boolean _drawFemale = true;
	private boolean _drawColumbi = true;
	private boolean _drawPrimigenius = true;
	private boolean _drawUnidentifiedMammoth = true;
	private boolean _drawUnidentified = true;
	private boolean _drawUndesignated = true;

// End Private Class Members

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Public Class Methods

    /**
    * <p>
    * The constructor sets default values, adds Mouse Event Listeners, and
    * makes the call to the BonePit class to parse the bonexml folder for
    * all bonexml data
    */
	public GraphicBoneYard() {
		super();

		// get all bonexml data
		_Bones = BonePit.readBones();
		_minElevation = BonePit.getMinElevation();
		_maxElevation = BonePit.getMaxElevation();
		this.setDoubleBuffered(true);

		this.addMouseListener(new MouseAdapter()
		{                                                    //track position of mouse press for Panning
			@Override public void mousePressed(MouseEvent e) { _MousePosition = new Point(e.getX(), e.getY()); }
                                                             //untrack mouse position, done Panning
			@Override public void mouseReleased(MouseEvent e) { _MousePosition = null; }

			@Override public void mouseClicked(MouseEvent e)
			{
			    if(SwingUtilities.isLeftMouseButton(e)){
			        java.util.Timer singleClick = new java.util.Timer();

			        singleClick.schedule(new TimerTask(){
			            @Override public void run(){        //Single click if only clicked once withing 375 milliseconds
			                if(e.getClickCount() == 1){         //on single click try to open pop-up for nearest bone
			                    ((GraphicBoneYard)e.getSource()).FindClosestBone( new Point2D.Double(e.getX(), e.getY()) );
			                }
			            }
			        }, 375);

			        if(e.getClickCount() == 2){             //otherwise double click, zoom to mouse click point
			            ZoomToPoint( new Point(e.getX(), e.getY()) );
			        }
			    }
			}
		});
                                    //Dragging while mouse pressed to scroll screen
		this.addMouseMotionListener(new MouseMotionAdapter()
		{
			@Override public void mouseDragged(MouseEvent e)
			{
				double x = e.getX() - _MousePosition.getX();
				double y = e.getY() - _MousePosition.getY();

				JViewport port = (JViewport)((JPanel)e.getSource()).getParent();
				Rectangle visible = port.getViewRect();
				visible.x -= x / (_Scale);
				visible.y -= y / (_Scale);
				scrollRectToVisible(visible);
			}
		});
	}

    /**
    * <p>
    * Overriden version of the JPanel getPreferredSize.
    *
    * @return _CurrentDimension, a private variable of type Dimension that
    *         tracks the frame size with a scaling factor applied
    */
	@Override public Dimension getPreferredSize() {
		return _CurrentDimension;
	}

    /**
    * <p>
    * Overriden paintComponent method, performs the draw for all the
    * polylines that represent the bones at Mammoth Site.
    *
    */
	@Override public void paintComponent(Graphics g) {
		super.paintComponent( g );

		if(_FirstLoad)
        {
            _OriginalDimension = this.getSize();
            _CurrentDimension = this.getSize();
            _FirstLoad = false;
        }

        Graphics2D graph = (Graphics2D)g;
        Draw(graph);
        graph.dispose();
        revalidate();
    }

	/**
	* <p>
	* A gateway method for resizing this JPanel. The method guarantees the
	* validity of the new scaling value before calling the method that will
	* actually alter the Panels dimensions.
	*
	* @param scale      The value to alter the current scaling factor by.
	*/
	public void setScale(int scale) {
	    int newValue = scale + _Scale;
	    if(newValue > 0){
	        if(newValue < _Scale) UpdatePreferredSize(-1);
	        else if (newValue > _Scale) UpdatePreferredSize(1);
	    }
	}

    /**
    * <p>
    * A method that allows external adjustment of the JSlider in the toolbar.
    *
    * @param val        The value to be the new JSlider value
    */
	public void setElevSliderValue(int val) {
		this.elevSliderVal = val;
		repaint();
	}

	/**
    * <p>
    * A method that allows external adjustment of the JSlider in the toolbar.
    *
    * @param val        The value to be the new JSlider value
    */
	public void setDetailSliderValue(int val) {
		this.detailSliderVal = val;
		repaint();
	}

	/**
	* <p>
	* A method to restore the view to it's original state.
	*/
	public void Reset()
	{
	    _CurrentDimension.setSize(_OriginalDimension.getWidth(), _OriginalDimension.getHeight());
	    this.setPreferredSize(_OriginalDimension);
	    getParent().doLayout();
	    setElevSliderValue(5);
	    setDetailSliderValue(15);
		revalidate();
	    _Scale = 1;
		repaint();
	}

	/**
	* <p>
	* A method to take integer values and shift the current view
	* in the specified direction by that value.
	*
	* @param x      The value to move the view in the x-direction
	* @param y      the value to move the view in the y-direction
	*/
	public void AdjustScrollWithArrows(int x, int y)
	{
	    JViewport port = (JViewport)this.getParent();
		Rectangle visible = port.getViewRect();

        visible.x += x * (_Scale); //also adjust by current scale factor
        visible.y += y * (_Scale);
		scrollRectToVisible(visible);
	}

	// Setters for filters

	/**
	* <p>
	* A setter property for the Draw Male filter option.
	*
	* @param _drawMale      boolean to determine whether or not to draw
	*                       Male bones
	*/
	public void setDrawMale(boolean _drawMale) {
	    this._drawMale = _drawMale;
	    repaint();
	}

	/**
	* <p>
	* A setter property for the Draw Femal filter option.
	*
	* @param _drawFemale      boolean to determine whether or not to draw
	*                         Female bones
	*/
	public void setDrawFemale(boolean _drawFemale) {
	    this._drawFemale = _drawFemale;
	    repaint();
	}

	/**
	* <p>
	* A setter property for the Draw Undesignated filter option.
	*
	* @param _drawUndesignated      boolean to determine whether or not to
	*                               draw Undesignated bones
	*/
	public void setDrawUndesignated(boolean _drawUndesignated) {
	    this._drawUndesignated = _drawUndesignated;
	    repaint();
	}

	/**
	* <p>
	* A setter property for the Draw Columbi filter option.
	*
	* @param _drawColumbi      boolean to determine whether or not to draw
	*                          Columbi bones
	*/
	public void setDrawColumbi(boolean _drawColumbi) {
	    this._drawColumbi = _drawColumbi;
	    repaint();
	}

	/**
	* <p>
	* A setter property for the Draw Primigenius filter option.
	*
	* @param _drawPrimigenius      boolean to determine whether or not
	*                              to draw Primigenius bones
	*/
	public void setDrawPrimigenius(boolean _drawPrimigenius) {
	    this._drawPrimigenius = _drawPrimigenius;
	    repaint();
	}

	/**
	* <p>
	* A setter property for the Draw Unidentified Mammoth filter option.
	*
	* @param _drawUnidentifiedMammoth      boolean to determine whether or
	*                                      not to draw Unidentified Mammoth bones
	*/
	public void setDrawUnidentifiedMammoth(boolean _drawUnidentifiedMammoth) {
	    this._drawUnidentifiedMammoth = _drawUnidentifiedMammoth;
	    repaint();
	}

	/**
	* <p>
	* A setter property for the Draw Unidentified filter option.
	*
	* @param _drawUnidentified      boolean to determine whether or not to
	*                               draw Unidentified bones
	*/
	public void setDrawUnidentified(boolean _drawUnidentified) {
	    this._drawUnidentified = _drawUnidentified;
	    repaint();
	}
// End Public Class Methods

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Priavte Class Methods

    /**
    * <p>
    * A helper method for paintComponent. Given the Graphics2D Object from
    * paintComponent, this method draws the walkway and all bones that meet
    * the current filtering criteria with the current scaling factor.
    *
    * @param graph      The Graphics2D Object from paintComponent
    */
	private void Draw(Graphics2D graph) {
		double topMargin = 5;
		double botMargin = 5;
		double leftMargin = 5;
		double rightMargin = 5;
		Walkway ww = Walkway.getWalkwayInstance();
		_WalkwayMin = new Point2D.Double(ww.xMin, ww.yMin);
		_WalkwayMax = new Point2D.Double(ww.xMax, ww.yMax);
        _DeltaX = (ww.xMax + rightMargin) - (ww.xMin - leftMargin);
        _DeltaY = (ww.yMax + botMargin)- (ww.yMin - topMargin);
		_XScale = _CurrentDimension.getWidth() / (_DeltaX);
		_YScale = _CurrentDimension.getHeight() / (_DeltaY);

		for(ArrayList<Point2D.Double> line : ww.polylines)
		{
			for(int i = 0; i < line.size() - 1; i++)
			{
				Point2D.Double p1 = line.get(i);
				Point2D.Double p2 = line.get(i+1);
				graph.drawLine((int) ((p1.getX() - ww.xMin) * _XScale), (int)((ww.yMax - p1.getY())* _YScale), (int)((p2.getX() - ww.xMin)* _XScale), (int)((ww.yMax - p2.getY())* _YScale));
			}
		}

		for(Bone bone : _Bones)
		{
			// The slider value enables/disables bones based on an elevation threshhold
			double elev = bone.elevation;
			double threshhold = (((_maxElevation - _minElevation) / 5) * (elevSliderVal) + 1) + _minElevation;

			// Decide if we need to draw this bone based on filters
			boolean drawGender = false;
			if (bone.gender.equals("MALE") && _drawMale) {
				drawGender = true;
			} else if (bone.gender.equals("FEMALE") && _drawFemale) {
				drawGender = true;
			} else if (bone.gender.equals("UNDESIGNATED") && _drawUndesignated) {
				drawGender = true;
			}

			boolean drawTaxon = false;
			if (bone.taxon.equals("MAMMUTHUS COLUMBI") && _drawColumbi) {
				drawTaxon = true;
			} else if (bone.taxon.equals("UNIDENTIFIED MAMMOTH") && _drawUnidentifiedMammoth) {
				drawTaxon = true;
			} else if (bone.taxon.equals("UNIDENTIFIED") && _drawUnidentified) {
				drawTaxon = true;
			} else if (bone.taxon.equals("MAMMUTHUS PRIMIGENIUS") && _drawPrimigenius) {
				drawTaxon = true;
			}

			// A bone isn't visible until it's drawn
			bone.isVisible = false;
			if (bone.elevation <= threshhold && drawTaxon && drawGender && bone.detail <= (detailSliderVal * 3))
			{
				bone.isVisible = true;

				// Select our colour based on completeness
				if (bone.completeness.equals("CO")) { // Complete bone?
					graph.setColor(Color.green);
				} else if (bone.completeness.equals("PC")) { // Partially complete?
					graph.setColor(Color.orange);
				} else if (bone.completeness.equals("PE")) { // Partially Excavated?
					graph.setColor(Color.red);
				} else { // SHAFT?
					graph.setColor(Color.black);
				}

				for (ArrayList<Point2D.Double> line : bone.polylines)
				{
					for(int i = 0; i < line.size() - 1; i++)
					{
						Point2D.Double p1 = line.get(i);
						Point2D.Double p2 = line.get(i+1);
						graph.drawLine((int) ((p1.getX() - ww.xMin) * _XScale), (int)((ww.yMax - p1.getY())* _YScale), (int)((p2.getX() - ww.xMin)* _XScale), (int)((ww.yMax - p2.getY())* _YScale));
					}
				}
			}
		}
	}

    /**
    * <p>
    * Adjusts the current scaling factor applied to the JPanel, and maintains
    * the visible center when applying the new scaling factor.
    *
    * @parm resize      The value to adjust the current scaling factor by
    */
	private void UpdatePreferredSize(int resize) {
		_CurrentDimension.setSize((int)(_OriginalDimension.getWidth() * (_Scale + resize)), (int)(_OriginalDimension.getHeight() * (_Scale + resize)));
		JViewport port = (JViewport)this.getParent();
		Rectangle visible = port.getViewRect();

	    visible.x = (int)((visible.x / _Scale) * (_Scale + resize));
		visible.y = (int)((visible.y / _Scale) * (_Scale + resize));

        port.setViewPosition(new Point(visible.x, visible.y));
        this.setPreferredSize(_CurrentDimension);
		_Scale = _Scale + resize;
		getParent().doLayout();
		revalidate();
		repaint();
	}

    /**
    * <p>
    * Given an input point, the point will become the new view center for
    * applying an increased scaling factor.
    *
    * @param p      The point as the new view center
    */
	private void ZoomToPoint( Point p )
	{
	    JViewport port = (JViewport)this.getParent();
		Rectangle visible = port.getViewRect();

        port.setViewPosition(new Point((int)(p.getX() - visible.getWidth()/2.0), (int)(p.getY() - visible.getHeight()/2.0) ));
        UpdatePreferredSize(1);
	}

    /**
    * <p>
    * Given a point on the screen, the point is compared to every bone drawing
    * to determine if the point is within that drawings bounding perimeter.
    * Of these bones in range, the one whose center point is closest to the
    * given point is displayed in a JDialog.
    *
    * @param p      The point from which to compare proximity
    */
    private void FindClosestBone(Point2D.Double p)
    {
    	boolean closer = false;
    	boolean anyBone = false;
    	Bone closestBone = null;
    	Point2D.Double boneCenter = new Point2D.Double(0,0);
	    Point2D.Double closestCenter = new Point2D.Double(0,0);

    	for(Bone bone : _Bones)
    	{
            if( bone.isVisible && bone.xMin != null && bone.xMax != null && bone.yMin != null && bone.yMax != null)
            {
    	        if(p.getX() <= (bone.xMax - _WalkwayMin.getX()) * _XScale
    	         && p.getX() >= (bone.xMin - _WalkwayMin.getX()) * _XScale
    	         && p.getY() >= (_WalkwayMax.getY() - bone.yMax) * _YScale
    	         && p.getY() <= (_WalkwayMax.getY() - bone.yMin) * _YScale)
    	        {
    	            boneCenter = new Point2D.Double(bone.xMax/2.0, bone.yMax/2.0);

    	    	    if(closestBone == null) {
    	    	    	closer = true;
    	    	    } else if( DistanceFormula(p, boneCenter) < DistanceFormula(p, closestCenter) ) {
    	    	    	closer = true;
    	    	    }

    		        if(closer)
    		        {
    		            anyBone = true;
    		    	    closer = false;
    		    	    closestBone = bone;
    		    	    closestCenter = new Point2D.Double(closestBone.xMax/2.0, closestBone.yMax/2.0);
    		        }
    		    }
            }
    	}

        if(anyBone)
            ImageDisplay.DisplayBone(closestBone);
    }

    /**
    * <p>
    * The standard formula to compute distance between two 2-dimensional points
    *
    * @param a      The point measuring from
    * @param b      The point measured to
    *
    * @return       The computed distance between a and b.
    */
    private double DistanceFormula(Point2D.Double a, Point2D.Double b)
    {
    	double distance = (a.getX() - b.getX()) * (a.getX() - b.getX());
	    return distance += (a.getY() - b.getY()) * (a.getY() - b.getY());
    }
// End Private Class Methods
}
