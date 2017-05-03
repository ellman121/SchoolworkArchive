<?php
/*=====================================================================
 file: parsePost.php
 Description: This file will parse the $_POST array sent from the client
 Author: Elliott Rarden
======================================================================*/

// Requires
require_once "parseClasses.php";
require_once "parseStudent.php";

// Initialize the data we need
$invalid= array();
$st = new student(null);
$studentID = $_COOKIE['studentID'];
$st->set_id($studentID);

/*=====================================================================
 Function: build_id
 Description: return the courses id string (e.g. MATH123Number // Number is the field)
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function build_id($pre, $num, $field)
{
    return $pre.$num.$field;
}
/*=====================================================================
 Function: get_elective
 Description: If all fields are entered correclty, create and return an electives
 		course, otherwise return array(false, false)
 Parameters: $id (the ID of the course)
 			 $electives (The set of electives)
 Author: Christopher Smith
======================================================================*/
function get_elective( $id, $electives)
{
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);

        $pre = $_POST[ $preID];
        $num = $_POST[ $numberID];
        $name = $_POST[$nameID];
        $cred = $_POST[$creditsID];
        $gra = $_POST[$id];
        if( empty($num) || empty($name) || empty($cred) || empty($gra) )
        {
                return array( false, false);
        }
        for( $i = 0; $i < count($electives); $i++)
        {
                if($num == $i+1)
                {
                        return array(true, new course($electives[$i]) );
                }
        }
        return array(false, false);
}
/*=====================================================================
 Function: get_GenEd
 Description: If all fields are entered correclty, create and return a gen ed
 		course, otherwise return array(false, false)
 Parameters: $id (the ID of the course)
 Author: Christopher Smith
======================================================================*/
function get_GenEd($id)
{
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);

        $pre = $_POST[ $preID];
        $num = $_POST[ $numberID];
        $name = $_POST[$nameID];
        $cred = $_POST[$creditsID];
        $gra = $_POST[$id];
        if( empty($pre) || empty($num) || empty($name) || empty($cred) || empty($gra) )
        {
            return array( false, false);
        }

        $course = new course(null);
        $course->prefix = $pre;
        $course->number = $num;
        $course->name = $name;
        $course->credits = $cred;
        $course->grade = $gra;
        return array( true, $course);
}
/*=====================================================================
 Function: val_to_letter
 Description: Convert the given value to a letter grade
 Parameters: $v (The value to convert)
 Author: Christopher Smith
======================================================================*/
function val_to_letter( $v )
{
    switch($v)
    {
        case 1: return 'A';
        case 2: return 'B';
        case 3: return 'C';
        case 4: return 'D';
        case 5: return 'F';
        case 6: return 'EX';
        case 7: return 'PI';
        case 8: return 'Taking';
        default: '';
    }
}
/*=====================================================================
 Function: get_core_entries
 Description: Walk through the core csc classes, creating courses along the way
 Parameters: $courses (The core courses)
 Author: Christopher Smith
======================================================================*/
function get_core_entries( $courses)
{
        GLOBAL $st;
        foreach( $courses as $c)
        {
                $id = build_id($c->prefix, $c->number, 'Grade');
                $course = new course($c);
                $course->grade = val_to_letter( $_POST[$id] );
                $course->set_html(add_IDs($id));
                $st->add_course($course);
        }
}
/*=====================================================================
 Function: get_csc_entries
 Description: Trigger the function call to get_core_entries
 Parameters: $courses (The core courses)
 Author: Christopher Smith
======================================================================*/
function get_csc_entries()
{
    GLOBAL $CSC_courses;
    get_core_entries($CSC_courses);
}
/*=====================================================================
 Function: get_elec_entries
 Description: Walk through the elective classes, creating courses along the way
 Parameters: $courses (The core courses)
 Author: Christopher Smith
======================================================================*/
function get_elec_entries()
{
        GLOBAL $st;
        GLOBAL $CSC_electives;
        $filled = true;
        for( $i = 0; $i < 4; $i++)
        {
            $id = build_id('CSElective', $i, 'Grade');
            if(!empty( $_POST[$id]))
            {
                    list($filled, $c) = get_elective( $id, $CSC_electives);
                    if($filled)
                    {
                            $c->set_html(add_IDs($id));
                            $c->grade = val_to_letter( $_POST[$id] );
                            $st->add_course($c);
                    }
                    else
                    {
                        add_errors($id);
                    }
            }
        }
}
/*=====================================================================
 Function: get_scicore_entries
 Description: Walk through the core science classes, creating courses along the way
 Parameters: $courses (The core courses)
 Author: Christopher Smith
======================================================================*/
function get_scicore_entries()
{
        GLOBAL $SCI_courses;
        get_core_entries($SCI_courses);
}
/*=====================================================================
 Function: add_errors
 Description: Add an erroronous course to our returned object
 Parameters: $id (The id's of the erronours course)
 Author: Christopher Smith
======================================================================*/
function add_errors($id)
{
        GLOBAL $invalid;
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);
        array_push( $invalid, $creditsID );
        array_push( $invalid, $numberID );
        array_push( $invalid, $nameID);
        array_push( $invalid, $preID );
        array_push( $invalid, $id );
}
/*=====================================================================
 Function: add_IDs
 Description: Create the id's for a course and return them as an array
 Parameters: $courses (The core courses)
 Author: Christopher Smith
======================================================================*/
function add_IDs($id)
{
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);

        return array( $preID, $numberID, $nameID, $creditsID, $id );
}
/*=====================================================================
 Function: all_empty
 Description: Return wether or not an ID has all empty fields in the _POST array
 Parameters: $id (The course in question)
 Author: Christopher Smith
======================================================================*/
function all_empty( $id )
{
    $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);

        $pre = $_POST[ $preID];
        $num = $_POST[ $numberID];
        $name = $_POST[$nameID];
        $cred = $_POST[$creditsID];
        $gra = $_POST[$id];
        if( empty($pre) && empty($num) && empty($name) && empty($cred) && empty($gra) )
        {
            return true;
        }
        return false;
}
/*=====================================================================
 Function: get_gened_entries
 Description: Create coureses for our gened entries
 Parameters: $pre (The prefix of the gened)
 			 $num (The number of iterations)
 Author: Christopher Smith
======================================================================*/
function get_gened_entries($pre, $num)
{
        GLOBAL $st;
        for( $i = 0; $i < $num; $i++)
        {
                $id =  build_id($pre,$i, 'Grade');       
                if(!all_empty($id))
                {
                        list($filled, $c) = get_GenEd( $id );
                        if($filled)
                        {        
                                $c->grade = val_to_letter($c->grade);
                                $c->set_html(add_IDs($id));
                                $st->add_course($c);
                        }
                        else
                        {
                                add_errors($id);
                        }
                }
        }

}
/*=====================================================================
 Function: get_sci_entries
 Description: Get the entries for the science courses
 Author: Christopher Smith
======================================================================*/
function get_sci_entries()
{
        get_gened_entries('SCI', 4);
}
/*=====================================================================
 Function: get_math_entries
 Description: Get the entries for the mathematics section
 Author: Christopher Smith
======================================================================*/
function get_math_entries()
{
        GLOBAL $MATH_courses;
        get_core_entries($MATH_courses);
}
/*=====================================================================
 Function: get_melec_entries
 Description: Get the math electives
 Author: Christopher Smith
======================================================================*/
function get_melec_entries()
{
    $filled = true;
    GLOBAL $MATH_electives;
    GLOBAL $invalid;
    GLOBAL $st;
    $i = 0;
    $id =  build_id('MathElective',$i, 'Grade');

    $creditsID = str_replace('Grade', 'Credits', $id);
    $numberID  = str_replace('Grade','Number', $id);
    $nameID    = str_replace('Grade','Name', $id );
    $preID     = str_replace('Grade','Prefix',$id);
    $pre = $_POST[ $preID];
    $num = $_POST[ $numberID];
    $name = $_POST[$nameID];
    $cred = $_POST[$creditsID];
    $gra = $_POST[$id];

    if(empty($num) && empty($name) && empty($cred) && empty($gra) )
    {
            return;
    }

    if(!all_empty($id))
    {
            list($filled, $c) = get_elective( $id, $MATH_electives );
            if($filled)
            {
                    $c->grade = val_to_letter($_POST[$id]);
                    $c->set_html(add_IDs($id));
                    $st->add_course($c);
            }
            else
            {
                    add_errors($id);
            }
    }

}
/*=====================================================================
 Function: get_eng_entries
 Description: Get the english courses
 Author: Christopher Smith
======================================================================*/
function get_eng_entries()
{
    GLOBAL $ENGL_courses;
    get_core_entries($ENGL_courses);
}
/*=====================================================================
 Function: get_ss_entries
 Description: Get the social studies entries
 Author: Christopher Smith
======================================================================*/
function get_ss_entries()
{
    get_gened_entries('SOC', 2 );
}
/*=====================================================================
 Function: get_hum_entries
 Description: Get the humanities entries
 Author: Christopher Smith
======================================================================*/
function get_hum_entries()
{
    get_gened_entries('HUM',2);
}
/*=====================================================================
 Function: get_free_entries
 Description: Get the free elective entries
 Author: Christopher Smith
======================================================================*/
function get_free_entries()
{
    get_gened_entries('Free', 2);
}

// Build our document
get_csc_entries();
get_elec_entries();
get_scicore_entries();
get_sci_entries();
get_math_entries();
get_melec_entries();
get_eng_entries();
get_ss_entries();
get_hum_entries();
get_free_entries();

?>
