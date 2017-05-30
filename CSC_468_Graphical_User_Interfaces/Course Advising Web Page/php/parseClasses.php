<?php
/*=====================================================================
 file: classEntry.php
 Description: This file loads in the course database and provides the functions
 	necessary to parse it.
 Author: Elliott Rarden
======================================================================*/

require_once "requiredClasses.php";
$CSC_courses = array();
$CSC_electives = array();
$MATH_courses = array();
$MATH_electives = array();
$ENGL_courses = array();
$SCI_courses = array();
/*=====================================================================
 Function: sort_courses
 Description: Goes through all of the courses and place them into the right buckets
 Author: Christopher Smith
======================================================================*/
function sort_courses()
{
        global $CSC_courses;
        global $CSC_electives;
        global $MATH_courses;
        global $ENGL_courses;
        global $SCI_courses;
        global $MATH_electives;
        $mcore = math_required();
        $melec = math_elective();
        $core = csc_required();
        $elec = csc_elective();
        $courses = simplexml_load_file("course_data/courses.xml");
        foreach( $courses as $c)
        {
                if( $c->prefix == 'CSC' && in_array($c->number, $core))
                {
                        array_push($CSC_courses,$c);
                }
                elseif( $c->prefix == 'CSC' && in_array($c->number, $elec) )
                {
                    array_push($CSC_electives, $c );
                }
                elseif( $c->prefix=='MATH'  && in_array($c->number, $mcore))
                {
                        array_push($MATH_courses, $c);
                }
                elseif( $c->prefix=='MATH' && in_array($c->number, $melec))
                {
                    array_push($MATH_electives,$c);
                }
                elseif($c->prefix=='ENGL')
                {
                        array_push($ENGL_courses, $c);
                }
                elseif($c->prefix=='PHYS')
                {
                        array_push($SCI_courses, $c);
                }
        }
}
/*=====================================================================
 Function: get_prefix
 Description: Return the prefix for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_prefix($course)
{
    return $course->prefix;
}
/*=====================================================================
 Function: get_number
 Description: Return the number for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_number($course)
{
    return $course->number;
}
/*=====================================================================
 Function: get_name
 Description: Return the name for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_name($course)
{
    return $course->name;
}
/*=====================================================================
 Function: get_course
 Description: Return the course's full text body (e.g. "MATH123 Calculus I")
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_course($course)
{
    $course_id = get_prefix($course)." ".get_number($course)." ".get_name($course);
    return $course_id;
}
/*=====================================================================
 Function: get_credits
 Description: Return the credits for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_credits($course)
{
    return $course->credits;
}
/*=====================================================================
 Function: get_description
 Description: Return the description for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_description($course)
{
    return $course->description;
}
/*=====================================================================
 Function: get_prereqs
 Description: Return the prerequisits for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_prereqs($course)
{
    $prereqs = "";
    foreach( $course->prereq as $prereq)
    {
            $prereqs .= $prereq;
            $prereqs .= "; ";
    }
    $prereqs = rtrim($prereqs,"; ");
    return $prereqs;
}
/*=====================================================================
 Function: get_coreqs
 Description: Return the corequisits for a course
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_coReqs($course)
{
    $coreqs = "";
    foreach( $course->coreq as $coreq)
    {
        $coreqs .= $coreq;
        $coreqs .= "; ";
    }
    $coreqs = rtrim($coreqs,"; ");
    return $coreqs;
}
/*=====================================================================
 Function: get_offered
 Description: Return when the course is offered
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_offered($course)
{
    return $course->offered;
}
/*=====================================================================
 Function: get_notes
 Description: Return the courses notes
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_notes($course)
{
    return $course->notes;
}
/*=====================================================================
 Function: get_courses
 Description: Return a full course body (e..g "MATH123 Calculus I\nAn introduction to calculus")
 Parameters: $course (The courses in question)
 Author: Christopher Smith
======================================================================*/
function get_courses($courses)
{
    foreach( $courses as $course )
    {
            echo get_course($course), PHP_EOL;
            echo get_description($course), PHP_EOL, PHP_EOL;
    }
}

// Sort our courses once we have declared our functions
sort_courses();
?>
