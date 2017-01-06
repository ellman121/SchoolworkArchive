extern int height, width;

/* drawLightSource()
 *
 * Draw the light source (i.e. sun's lighting) in all it's glory
 *
 * Parameters:
 *
 * Returns:
 *
 */
void drawLighSource (){
	    GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	    GLfloat ambient[] = { 0.05, 0.05, 0.05, 1.0 };       // ambient light
	    GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1.0 };       // diffuse light
	    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };      // highlights
	    GLfloat	attenuation[] = {0.0, 1.0, 1.0};
	    // Set up solar system light source
	    glEnable( GL_LIGHT0 );
	    glLightModelfv(  GL_LIGHT_MODEL_AMBIENT, ambient );
	    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	    glLightfv( GL_LIGHT0, GL_POSITION, position );
	    glLightfv( GL_LIGHT0,  GL_QUADRATIC_ATTENUATION, attenuation );
	    // Eliminate hidden surfaces
	    glEnable(GL_BLEND);
	    glEnable( GL_DEPTH_TEST );
	    glEnable( GL_NORMALIZE );
	    glEnable( GL_CULL_FACE );
	    glCullFace( GL_BACK );
}

/* drawRings()
 *
 * Draw some rings on the screen
 *
 * Parameters:
 *		ring* body - The rings to be drawn
 * Returns:
 *
 */
void drawRings (ring* body){

	float color[3];
	body->getColor(color);

	// get distance between body surface and start of rings
	float iRadius = body->getInnerRadius();
	float oRadius = body->getOuterRadius();

	GLUquadricObj* ring = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(ring, color, 1, false);
	}

	Image_s texImg = body->getImage();
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texImg.nCols, texImg.nRows, GL_RGB, GL_UNSIGNED_BYTE, texImg.img);
    glDisable(GL_CULL_FACE);

	// Draw a texture mapped top ring
	if (smoothFlag){
		gluQuadricNormals(ring, GLU_SMOOTH);
	} else {
		gluQuadricNormals(ring, GLU_FLAT);
	}
	if (texFlag){
		gluQuadricTexture(ring, GL_TRUE);
	} else {
		gluQuadricTexture(ring, GL_FALSE);
	}

	gluCylinder( ring, iRadius, oRadius, 0, 100, 1);

	gluDeleteQuadric(ring);
	glEnable(GL_CULL_FACE);
}

/* drawSun()
 *
 * Draw the sun at the centre of the solar system (heliocentrism provided by
 * these sponsors: Aristarchus, Copernicus, Kepler, and Galilei)
 *
 * Parameters:
 *		planet* sun - The sun's planet object in memroy (probably a bad idea
 *						to name our ADT planet, but too late)
 * Returns:
 *
 */
void drawSun(planet* sun){
	// Get sun color
	float color[3] = {};
	sun->getColor(color);
	float radius = sun->getRadius();
	GLUquadricObj* sphere = gluNewQuadric();

	// Set material properties
	if (lightFlag){
	    setMaterials(sphere, color, sun->getAlbedo(), true);
	}
	if (texFlag){
		setTexture(sun);
	}
	glPushMatrix();

	// Rotate about y axis for body rotation
	glRotatef( sun->getRotation(), 0.0, 1.0, 0.0 );

	// rotate about x axis to adjust latitude and longinal lines in wireframe
	glRotatef( 90, 1.0, 0.0, 0.0 );

	// Draw body
    glColor3fv(color);
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius*10), (int) (radius*10) ) : glutWireSphere( radius, 10, 10 );
	drawRings(ringMap.at("Sun"));		

	// reverse axial tilt before drawing label
	glRotatef( -90, 1.0, 0.0, 0.0 );
 	if (bodyLabelFlag){
	    drawBodyName("Sun", radius);
 	}

 	// Reverse body rotation
 	glRotatef( -1 * sun->getOrbit(), 0.0, 1.0, 0.0 );

	glPopMatrix();
}

/* drawBody()
 *
 * Draw a heavenly body
 *
 * Parameters:
 *		planet* body - The body to be drawn
 *		bool sat 	 - Wether or not the body has rings to be drawn
 * Returns:
 *
 */
void drawBody(planet* body, bool sat){
	// Get body color and parent name
	float color[3] = {};
	body->getColor(color);
	string name = body->getName();
	string parent = body->getParent();
	float radius = body->getRadius();

	GLUquadricObj* sphere = gluNewQuadric();
	// get distance between body surface and parent surface
	float distance = planetMap.at(parent)->getRadius()+ radius +body->getDistance();
	// Set material properties
	if (lightFlag){
	    setMaterials(sphere, color, body->getAlbedo(), false);
	}

	// make sure system is not being viewed relative to current body
	// roate about the z axis for orbital incline
	if (name != relative){
		glRotatef( body->getIncline(), 0.0, 0.0, 1.0 );
	}

	// rotate around y axis to get orbital position
	glRotatef( body->getOrbit(), 0.0, 1.0, 0.0 );
	// translate distance in x plane  to place body
	glTranslatef( distance, 0.0, 0.0 );

	glPushMatrix();

	// Rotate about y axis for body rotation
	glRotatef( body->getRotation(), 0.0, 1.0, 0.0 );

	if (texFlag){
		setTexture(body);
	}

	glColor3fv(color);
	// rotate about x axis for body axial tilt
	glRotatef( body->getTilt()+270, 1.0, 0.0, 0.0 );

	// Draw body
	(solidFlag) ? gluSphere( sphere, radius, (int) (radius*10), (int) (radius*10) ) : glutWireSphere( radius, 10, 10 );

	// Draw any rings around the body
	vector<string> rings = body->getRings();
	for (int r = 0; r < rings.size(); r++){
		drawRings(ringMap.at(rings.at(r)));
	}

	glDisable(GL_TEXTURE_2D);

	// reverse axial tilt before drawing label
	glRotatef( -1 * (body->getTilt()+270), 1.0, 0.0, 0.0 );
    if ((!sat && bodyLabelFlag) || (sat && moonLabelFlag)){
		drawBodyName(name, radius);
    }

    glPopMatrix();

    // Draw satellite for body
	vector<string> satellites = body->getSatellites();
	for (int s = 0; s < satellites.size(); s++){
		drawBody(moonMap.at(satellites.at(s)), true);
	}

	// Move back to starting position
	glTranslatef( -1 * distance, 0.0, 0.0 );
	glRotatef( -1 * body->getOrbit(), 0.0, 1.0, 0.0 );
	glRotatef( -1 * body->getIncline(), 0.0, 0.0, 1.0 );
}
