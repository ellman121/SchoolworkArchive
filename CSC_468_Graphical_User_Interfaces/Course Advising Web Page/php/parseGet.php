<?php
require_once "parseClasses.php";
require_once "parseStudent.php";
$invalid= array();
$st = new student(null);
$studentID = $_COOKIE['studentID'];
$st->set_id($studentID);
function build_id($pre, $num, $field)
{
    return $pre.$num.$field;
}

function get_elective( $id, $electives)
{
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);

        $pre = $_GET[ $preID];
        $num = $_GET[ $numberID];
        $name = $_GET[$nameID];
        $cred = $_GET[$creditsID];
        $gra = $_GET[$id];
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

function get_GenEd($id)
{
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);
        
        $pre = $_GET[ $preID];
        $num = $_GET[ $numberID];
        $name = $_GET[$nameID];
        $cred = $_GET[$creditsID];
        $gra = $_GET[$id];
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

function val_to_letter( $g )
{
    switch($g)
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

function get_core_entries( $courses)
{
        GLOBAL $st;
        foreach( $courses as $c)
        {
                $id = build_id($c->prefix, $c->number, 'Grade');
                $course = new course($c);
                $course->grade = val_to_letter( $_GET[$id] );
                $course->set_html(add_IDs($id));
                $st->add_course($course);
        }   
}

function get_csc_entries()
{
    GLOBAL $CSC_courses;
    get_core_entries($CSC_courses);
}

function get_elec_entries()
{
        GLOBAL $st;
        GLOBAL $CSC_electives;
        $filled = true;
        for( $i = 0; $i < 4; $i++)
        {
            $id = build_id('CSElective', $i, 'Grade');
            if(!empty( $_GET[$id]))
            {
                    list($filled, $c) = get_elective( $id, $CSC_electives);
                    if($filled)
                    {
                            $c->set_html(add_IDs($id));
                            $c->grade = val_to_letter( $_GET[$id] );
                            $st->add_course($c);
                    }
                    else
                    {
                        add_errors($id);
                    }
            }
        }
}

function get_scicore_entries()
{
        GLOBAL $SCI_courses;
        get_core_entries($SCI_courses);
}
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
function add_IDs($id)
{
        $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);

        return array( $preID, $numberID, $nameID, $creditsID, $id );
}

function all_empty( $id )
{
    $creditsID = str_replace('Grade', 'Credits', $id);
        $numberID  = str_replace('Grade','Number', $id);
        $nameID    = str_replace('Grade','Name', $id );
        $preID     = str_replace('Grade','Prefix',$id);
        
        $pre = $_GET[ $preID];
        $num = $_GET[ $numberID];
        $name = $_GET[$nameID];
        $cred = $_GET[$creditsID];
        $gra = $_GET[$id];
        if( empty($pre) && empty($num) && empty($name) && empty($cred) && empty($gra) )
        {
            return true;
        }
        return false;
}

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


function get_sci_entries()
{
        get_gened_entries('SCI', 4);
}

function get_math_entries()
{
        GLOBAL $MATH_courses;
        get_core_entries($MATH_courses);
}

function get_melec_entries()
{
    $filled = true;
    GLOBAL $MATH_electives;
    GLOBAL $invalid;
    GLOBAL $st;
    $i = 0;
    $id =  build_id('MathElective',$i, 'Grade');
    if(!empty( $_GET[$id]))
    {
            list($filled, $c) = get_elective( $id, $MATH_electives );
            if($filled)
            {   
                    $c->set_html(add_IDs($id));
                    $st->add_course($c);
            }
            else
            {
                add_errors($id);
            }
    }

}

function get_eng_entries()
{
    GLOBAL $ENGL_courses;
    get_core_entries($ENGL_courses);
}


function get_ss_entries()
{
    get_gened_entries('SOC', 2 );
}

function get_hum_entries()
{
    get_gened_entries('HUM',2);
}

function get_free_entries()
{
    get_gened_entries('Free', 2);
}
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
