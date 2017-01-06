/******************************************************************************
 * File: initSystem.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains functions with hard-coded initialization values
 * for the solar system's bodies and their associated texture map images.
 *
 ******************************************************************************/
map<string,planet*> planetMap;
map<string,planet*> moonMap;
map<string,ring*> ringMap;

// Set colors for bodies
float sunColor[3] = {1.0, 1.0, 0.3};
float mercuryColor[3] = {0.8, 0.5, 0.5};
float venusColor[3] = {0.6, 0.8, 0.3};
float earthColor[3] = {0.1, 0.4, .85};
float marsColor[3] = {1.0, 0.2, 0.2};
float jupiterColor[3] = {0.54, 0.165, 0.165};
float saturnColor[3] = {0.5, 0.7, 0.0};
float uranusColor[3] = {0.2, 1.0, 0.6};
float neptuneColor[3] = {0.2, 0.4, 0.8};
float moonColor[3] = {0.4, 0.4, 0.5};

// All of the planets
void setPlanets(){
    Image_s nullImage = {0, 0, NULL};
    vector<string> satellites = {};
    vector<string> rings = {};

    planetMap.emplace("Sun", new planet ("Sun", "", 696000/10, 0, 0, 25, 0, 7.25, 0, sunColor, nullImage, satellites, rings));
    planetMap.emplace("Mercury", new planet ("Mercury", "Sun", 2439, 58000000, 88, 1416, 3.38, 2.04, 0.68, mercuryColor, nullImage, satellites, rings));
    planetMap.emplace("Venus", new planet ("Venus", "Sun", 6052, 108000000, 225, 5832, 3.86, 177.36, 0.9, venusColor, nullImage, satellites, rings));

    satellites = {"Luna"};
    planetMap.emplace("Earth", new planet ("Earth", "Sun", 6378, 150000000, 365, 24, 7.155, 23.4392811, 0.306, earthColor, nullImage, satellites, rings));

    satellites = {"Deimos", "Phobos"};
    planetMap.emplace("Mars", new planet ("Mars", "Sun", 3394, 228000000, 687, 24.6, 5.65, 25.19, 0.25, marsColor, nullImage, satellites, rings));

    satellites = {"Amalthea", "Callisto", "Europa", "Ganymede", "Io"};
    planetMap.emplace("Jupiter", new planet ("Jupiter", "Sun", 71398, 779000000, 4332, 9.8, 6.09, 3.13, 0.343, jupiterColor, nullImage, satellites, rings));

    satellites = {"Dione", "Enceladus", "Iapetus", "Mimas", "Rhea", "Tethys", "Titan"};
    rings = {"Saturn"};
    planetMap.emplace("Saturn", new planet ("Saturn", "Sun", 60270, 1424000000, 10585, 10.7, 5.51, 26.73, 0.342, saturnColor, nullImage, satellites, rings));

    satellites = {"Ariel", "Miranda", "Oberon", "Titania", "Umbriel"};
    planetMap.emplace("Uranus", new planet ("Uranus", "Sun", 25550, 2867000000, 30682, 15.5, 6.48, 97.77, 0.3, uranusColor, nullImage, satellites, rings));

    satellites = {"Triton"};
    planetMap.emplace("Neptune", new planet ("Neptune", "Sun", 24750, 4492000000, 60195, 15.8, 6.43, 28.32, 0.290, neptuneColor, nullImage, satellites, rings));
}

// All of the moons for each planet (that we could find)
void setMoons(){
    Image_s nullImage = {0, 0, NULL};
    vector<string> satellites = {};
    vector<string> rings = {};

    moonMap.emplace("Luna", new planet("Luna", "Earth",1738, 384400, 27.322, 27.322, 5.145, 6.687, 0.136, moonColor, nullImage, satellites, rings));
    
    moonMap.emplace("Deimos", new planet("Deimos", "Mars",4, 23460, 1.263, 30.312, 0.93, 0, 0.068, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Phobos", new planet("Phobos", "Mars",280, 9270, 0.319, 7.656, 1.093, 0, 0.071, moonColor, nullImage, satellites, rings));
    
    moonMap.emplace("Amalthea", new planet("Amalthea", "Jupiter", 83.5, 181300, 0.498, 11.952, 0.374, 0, 0.09, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Callisto", new planet("Callisto", "Jupiter", 2400, 1883000, 16.689, 400.536, 0.205, 0, 0.22, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Europa", new planet("Europa", "Jupiter", 1563, 670900, 3.551, 85, 0.471, 0.1, 0.67, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Ganymede", new planet("Ganymede", "Jupiter", 2638, 1070000, 7.155, 171.72, 0.204, 0.33, 0.43, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Io", new planet("Io", "Jupiter", 1814.5, 421600, 1.769, 42.456, 0.05, 0, 0.63, moonColor, nullImage, satellites, rings));
    
    moonMap.emplace("Dione", new planet("Dione", "Saturn",561.4, 377400, 2.737, 65.68596, 0.019, 0, 0.998, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Enceladus", new planet("Enceladus", "Saturn",249, 238020, 1.37, 32.885232, 0.019, 0, 0.99, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Iapetus", new planet("Iapetus", "Saturn",718, 3561300, 79.3215, 1903.716, 15.47, 0, 0.225, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Mimas", new planet("Mimas", "Saturn",199, 185520, 0.942, 22.608, 1.574, 0, 0.962,moonColor, nullImage, satellites, rings));
    moonMap.emplace("Rhea", new planet("Rhea", "Saturn",764, 527040, 4.518, 108.437088, 0.345, 0, 0.949, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Tethys", new planet("Tethys", "Saturn",530, 294660, 1.888, 45.307248, 1.12, 0, 0.8, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Titan", new planet("Titan", "Saturn",2575, 1221850, 15.945, 382.68, 0.34854, 0, 0.22, moonColor, nullImage, satellites, rings));

    moonMap.emplace("Ariel", new planet("Ariel", "Uranus",580, 191240, 2.52, 60.48, 0.260, 0, 0.23, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Miranda", new planet("Miranda", "Uranus",236, 129780, 1.414, 33.936, 4.232, 0, 0.32, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Oberon", new planet("Oberon", "Uranus",763, 582600, 13.463, 323.112, 0.058, 0, 0.14, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Titania", new planet("Titania", "Uranus",789, 435840, 8.706, 208.944, 0.340 , 0, 0.17, moonColor, nullImage, satellites, rings));
    moonMap.emplace("Umbriel", new planet("Umbriel", "Uranus",595, 265970, 4.144, 99.456, 0.128, 0, 0.10, moonColor, nullImage, satellites, rings));

    moonMap.emplace("Triton", new planet("Triton", "Neptune",1352.5, 354800, 5.877, 141.048, 156.885, 0, 0.76, moonColor, nullImage, satellites, rings));
}

// Set rings on planets that have them.  Yeah, yeah, the asteroid belt isn't
// technially a ring, but it's close enough
void setRings(){
    Image_s nullImage = {0, 0, NULL};

    float ringColor[] = {1, 1, 1};
    ringMap.emplace("Saturn", new ring ("SaturnRings", "Saturn", 67270, 140270, 0, 10.2, 0.342, ringColor, nullImage));
    float beltColor[] = {0.1, 0.1, 0.1};
    ringMap.emplace("Sun", new ring ("AstroidBelt", "Saturn", 2200000, 3200000, 0, 10.2, 0.342, beltColor, nullImage));
}

// Read out and store texture maps
void setTexImage (){
    int numR, numC;
    unsigned char *image;

    // For each planet, read out the associated file with the same name
    // e.g. "Earth" -> "earth.bmp"
    for (auto& p: planetMap){
        if(LoadBmpFile(p.second->getName(), numR, numC, image)) {
            p.second->setImage({numR, numC, image});
        } else {
            p.second->setImage({0, 0, nullptr});
        }
    }

    // For each moon, read out the associated file with the same name
    // Note, if we don't have a texmap for a moon, we just use Luna
    for (auto& m: moonMap){
        if(LoadBmpFile(m.second->getName(), numR, numC, image)) {
            m.second->setImage({numR, numC, image});
        } else {
            m.second->setImage({0, 0, nullptr});
        }
    }

    // Read out the texture maps for Saturn's rings and the asteroid belt
    if(LoadBmpFile("SaturnRings", numR, numC, image)) {
        ringMap.at("Saturn")->setImage({numR, numC, image});
    } else {
        ringMap.at("Saturn")->setImage({0, 0, nullptr});
    }
    if(LoadBmpFile("asteroidbelt", numR, numC, image)) {
        ringMap.at("Sun")->setImage({numR, numC, image});
    } else {
        ringMap.at("Sun")->setImage({0, 0, nullptr});
    }
}
