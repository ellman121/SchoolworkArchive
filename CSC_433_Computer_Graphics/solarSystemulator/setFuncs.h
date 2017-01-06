extern bool solidFlag, lightFlag, smoothFlag, texFlag, bodyLabelFlag, moonLabelFlag, pauseFlag, velocityFlag;
extern float hourSpeed, xVelocity, yVelocity, zVelocity, xTranslate, yTranslate, zTranslate, xRotate, yRotate;
extern string relative;

/* setNextFrame()
 *
 * Simulate the next frame
 *
 * Parameters:
 *
 * Returns:
 *
 */
void setNextFrame(){
    // Move bodies if not paused
    if (!pauseFlag){
        // Move planets ahead by hour speed
        for (auto& p: planetMap)
            p.second->step(hourSpeed);
        // Move satellites ahead by hour speed
        for (auto& m: moonMap)
            m.second->step(hourSpeed);
    }
    // increment x,y,z by corresponding velocity values
    if (velocityFlag){
        xTranslate += xVelocity;
        yTranslate += yVelocity;
        zTranslate += zVelocity;
    }
}

/* setDrawMode()
 *
 * Change the draw mode to the new mode
 *
 * Parameters:
 *      Mode mode - The new mode we are drawing in
 * Returns:
 *
 */
void setDrawMode(Mode mode){
    switch (mode) {
        case wire:
            solidFlag = false;
            smoothFlag = false;
            texFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glDisable(GL_TEXTURE_2D);
        break;

        case flat:
            solidFlag = true;
            smoothFlag = false;
            texFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_FLAT );
            glDisable(GL_TEXTURE_2D);
        break;

        case smooth:
            solidFlag = true;
            smoothFlag = true;
            texFlag = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_SMOOTH );
            glDisable(GL_TEXTURE_2D);
        break;

        case image:
            solidFlag = true;
            smoothFlag = true;
            texFlag = true;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glShadeModel( GL_SMOOTH );
            glEnable(GL_TEXTURE_2D);
        break;
    }
}

/* resetView()
 *
 * Reset our view to the original position
 *
 * Parameters:
 *
 * Returns:
 *
 */
void resetView(){
    xRotate = 0.0;
    yRotate = 0.0;
    xTranslate = 0.0;
    yTranslate = 0.0;
    zTranslate = -200.0;
}

/* drawLightSource()
 *
 * Sets our velocity.  Not currently implemented or used
 *
 * Parameters:
 *
 * Returns:
 *
 */
void setVelocity(int x, int y, int z){
}

/* setMaterials()
 *
 * Set the material properties for a given quadric
 *
 * Parameters:
 *      GLUquadric* quad - The quadric to modify
 *      float color[]    - Array of colors to set the quadric to
 *      float albedo     - Diffuse light value
 *      float emiss      - The emissivity of the quadric
 * Returns:
 *
 */
void setMaterials(GLUquadric* quad, float color[], float albedo, bool emiss){
    gluQuadricDrawStyle( quad, GLU_FILL );
    // set normals for object based on draw mode
    if (solidFlag) {
        if (smoothFlag){
            gluQuadricNormals(quad, GLU_SMOOTH);
        } else {
            gluQuadricNormals(quad, GLU_FLAT);
        }
    } else {
        gluQuadricNormals(quad, GLU_NONE);
    }

    GLfloat ambient[4] = {0, 0, 0, 1.0};
    GLfloat diffuse[4] = {0, 0, 0, 1.0};
    GLfloat specular[4] = {0, 0, 0, 1.0};

    // set texture coordinates based  texture flag
    if (texFlag){
        gluQuadricTexture(quad, GLU_TRUE);
        // Set material property values
        for (int i = 0; i < 3; i++){
            ambient[i] = 0.4;
            diffuse[i] = albedo;
            specular[i] = 0.8;
        }

        } else {
            gluQuadricTexture(quad, GLU_FALSE);
            for (int i = 0; i < 3; i++){
                ambient[i] = color[i] * 0.6;
                diffuse[i] = color[i] * albedo;
                specular[i] = color[i];
            }
        }

	    GLfloat shininess = 3.5;
	    GLfloat emission[4];

	    // Set material properties
	    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
	    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
	    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
        // Set emissivity, if emiss flag is true, emissivity to body colors, else set to zeros
        if (emiss) {
            GLfloat emission[] = { (float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0 }; 
            glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
        } else {
            GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0};
            glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
        }
}

/* setTexture()
 *
 * Give a planet the texture it desrerves
 *
 * Parameters:
 *      planet* body - The planet to change.
 * Returns:
 *
 */
void setTexture(planet* body){
    Image_s texImg = body->getImage();
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texImg.nRows, texImg.nCols, GL_RGB, GL_UNSIGNED_BYTE, texImg.img);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );     // or GL_CLAMP
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );     // or GL_CLAMP
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glEnable(GL_TEXTURE_2D);
}


