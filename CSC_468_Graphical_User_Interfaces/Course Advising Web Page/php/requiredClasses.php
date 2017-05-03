<?php
/*=====================================================================
 file: requiredClasses.php
 Description: This file contains helper functions to genearte arrays of
 	course numbers based on prefix, requred vs elective, etc.
 Author: Elliott Rarden
======================================================================*/

function csc_required()
{
    return array('110', '150', '250', '251', '300', '314', '317', '372',
                             '456', '461', '464', '465', '468', '470', '484');
}

function csc_core_with_prereq()
{
    // These are general rules to point recommendations in the right direction
    // Not hard and fast according to the XML
    return array(
    // Freshman Year
    '150' => array(''),              // 150
    '250' => array('150'),           // 250
    '251' => array('150'),           // Finite

    // Sophomore Year
    '300' => array('250', '251'),    // Data Struct
    '314' => array('250'),           // Assembly
    '317' => array('314'),           // Org/Arch

    // Junior Year
    '372' => array('300'),           // AoA
    '461' => array('300'),           // PL
    '484' => array('300'),           // Database
    '470' => array('372'),           // Soft Eng

    // Senior Year
    '456' => array('300', '317'),    // OS
    '468' => array('461'),           // GUI
    '464' => array('470', '484'),    // Design 1
    '465' => array('464'));          // Design 2
}

function csc_elective()
{
    return array('410', '412', '414', '415', '416', '426', '433', '441',
            '442', '445', '447', '449', '454', '476', '476L');
}

function math_required()
{
    return array('101', '102', '115', '120', '123', '125', '225', '315', '381');
}

function math_elective()
{
        return array('321', '382', '353', '413', '421', '423', '443');
}

function sci_required()
{
        return array( '211' );
}

function eng_required()
{
        return array( '101', '279', '289');
}

function get_soc_prefix()
{
        return array('ABS', 'AIS/HIST','ANTH','CJUS','ECON','GEOG','GLST',
                'HDFS','HDFS/EPSY','HIST','INED','INFO','NATV','POLS','PSYC',
                'REL','SOC','UHON','WmSt');
}

function get_hum_prefix()
{
        return array('ARAB','ARCH','ART','ARTH','ARTH/AIS','CHIN','ENGL','FREN',
                'GER','GFA','GREE','HIST','HUM','LAKL/AIS','LATI','MCOM','MFL',
                'MUS/ANTH', 'MUS','PHIL','REL','RUSS','SPAN','THEA');
}

function get_sci_prefix()
{
    return array( 'PHYS', 'BIOL', 'CHEM', 'GEOL');
}

?>
