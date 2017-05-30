<?php
/*=====================================================================
 file: classEntryGen.php
 Description: This file will generate the html for the class entry page 
 Author: Elliott Rarden
======================================================================*/

/*php file for generating class entry form html codel*/
require_once "requiredClasses.php";

/*=====================================================================
 Function: create_placeholder
 Description: Creates a placeholder course that will be text that is filled for forms
 Parameters: $prefix, $number, $name are all course information
 Author: Christopher Smith
======================================================================*/
function create_placeholder( $prefix = "GENED", $number = "Number", $name = "Course Title" )
{
        $placeholder = (object) array('isactive' =>'no', 'prefix' => $prefix, 'number' => $number, 'name' => $name, 'credits' => 'credits', 'grade' =>'Grade');
        return $placeholder;
}
/*=====================================================================
 Function: student_taken
 Description: Checks if a course is taken by the student and if so returns grade
 Parameters: $course, $st_courses
 Author: Christopher Smith
======================================================================*/
function student_taken($course, $st_courses)
{
        $grade = "Grade";
        $found = false;
        $i = 0;
        $n = count($st_courses);
        while( !$found && $i < $n )
        {

                $st_course = $st_courses[$i];
                if(     (string)$st_course->prefix == (string)$course->prefix &&
                        (string)$st_course->number == (string)$course->number)
                {
                        $grade = $st_course->grade;
                        $found = true;
                }
                $i++;
        }
        return array($found, $grade);
}

/*=====================================================================
 Function: get_header
 Description: Generates the html header for classEntry.php
 Parameters:
 Author: Christopher Smith
======================================================================*/
function get_header()
{
        return  '<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- include.pug - Includes libraries and stuff-->
    <!-- UTF-8 is master text encoding-->
    <meta charset="UTF-8">
    <!-- CSS Libraries-->
    <link href="css/normalize.css" rel="stylesheet">
    <link href="css/skeleton.css" rel="stylesheet">
    <link href="css/animate.css" rel="stylesheet">
    <!-- CSS Files-->
    <link href="css/custom.css" rel="stylesheet">
    <link href="css/loader.css" rel="stylesheet">
    <!-- Javascript Libraries-->
    <script src="js/jquery-2.2.4.min.js"></script>
    <script src="js/jquery-pjax.js"></script>
    <!-- Javascript files-->
    <script src="js/app.js"></script>
    <!-- Show things-->
    <title>Advisinator</title>
  </head>
  <body>
    <div class="container" id="bodyContainer">
    <h2>Enter the classes you\'ve taken below</h2>
    <hr>
    <div class="reference">
        <p class="four columns centerText"><a href="http://www.mcs.sdsmt.edu/csc468/Assignments/PA2/CS%20checklist%202016-17.pdf">Course Checklist</a></p>
        <p class="four columns centerText"><a href="http://ecatalog.sdsmt.edu/mime/media/15/744/New-System+General+Education+Courses+08.21.15.pdf">Gen Ed Reference</a></p>
        <p class="four columns centerText"><a href="http://www.mcs.sdsmt.edu/csc468/Assignments/PA2/CS%20flowchart%202016-17.pdf">Flowchart</a></p><br>
    </div>
    <hr>
    <form id="classEntryForm" >';


}
/*=====================================================================
 Function: hide_section
 Description: Generates the html code for sections and hiding the section
 Parameters: $section, $title is the display text on the webpage
                $type is either 'Recommend' or 'Class' depending on page
 Author: Christopher Smith
======================================================================*/
function hide_section($section, $title)
{

 return '<div class="section">
          <h5>'.$title.'
            <label class="top leftside" id="'.$section.'Plus" for="hide'.$section.'" position="fixed" width="0px;">'."&#x25b6;".'</label>
          </h5>
        <input id="hide'.$section.'" style="display:none;" data-toggle="fold" type="checkbox" checked>
        </div>
        <div class="section" id="'.$section.'">';


}

/*=====================================================================
 Function: html_gen
 Description: Generates the html for a single row for forms without combo boxes in the name
 Also fills the grade if the student has a grade for the class
 Parameters: $course, $st_courses, $readonly
 Author: Christopher Smith
======================================================================*/
function html_gen($course, $st_courses,$readonly)
{
    list($found, $grade) = student_taken($course, $st_courses);
    $form  = '<div class="row">
                <div class="two columns">
                <input class="u-full-width"
				id="'.$course->prefix.$course->number.'Prefix"'.
                ' name="'.$course->prefix.$course->number."Prefix".
                '" type="text" placeholder="'.$course->prefix.'" '.$readonly[0].'>
            </div>
            <div class="two columns">
                <input class="u-full-width"
				id="'.$course->prefix.$course->number."Number".
                '" name="'.$course->prefix.$course->number.'Number" '.
                'type="text" placeholder="'.$course->number.'" '.$readonly[1].'>
            </div>
            <div class="four columns">
                <input class="u-full-width"
				id="'.$course->prefix.$course->number."Name".
                '" name="'.$course->prefix.$course->number."Name".
                '" type="text" placeholder="'.$course->name.'" '.$readonly[2].'>
            </div>
            <div class="two columns">
                <input class="u-full-width"
                id="'.$course->prefix.$course->number."Credits".
                '" name="'.$course->prefix.$course->number."Credits".
                '" type="text" placeholder="'.$course->credits.'" '.$readonly[3].'>
            </div>
                    ';
    $form .= html_gen_grades($course->prefix, $course->number,$course, $grade) . '</div>
            ';
    return $form;
}
/*=====================================================================
 Function: html_gen_combo
 Description: Generates the html combo box with options
 Parameters: $items (items to be put in combo box)
             $selected ( the selected option)
             $placeholder (text to be displayed when no option is selected)
 Author: Christopher Smith
======================================================================*/
function html_gen_combo( $items, $selected, $placeholder)
{
        $k = 0;
        $opt = '<option value="" '.$selected[$k].'>'.$placeholder.'</option>';
        foreach( $items as $i )
        {
            $k++;
            $opt .= '<option value="'.$k.'" '.$selected[$k].">$i</option>";
        }
        return $opt;
}
/*=====================================================================
 Function: html_gen_grades
 Description: Generates the html combo boxes for grades
 Parameters: $c_type (class type eg: MATH CSC)
             $sect_num (row number in section)
             $course (Course the grades are generated for)
             $grade (if the student has a grade its the grade)
 Author: Christopher Smith
======================================================================*/
function html_gen_grades( $c_type, $sect_num, $course, $grade )
{
        $placeholder =  "Grade";
        $selected = array();
        $items = array('A','B','C','D','F','EX', 'PI', 'Taking' );
        $selected[0] = 'selected="selected"';
        for( $i = 0; $i < count($items); $i++)
        {
                array_push($selected,'');
                if($items[$i] == $grade )
                {
                        $selected[$i+1] = 'selected="selected"';
                        $selected[0] = '';
                }
        }
        return '<div class="two columns">
                <select class="u-full-width"'.
				' id="'.$c_type.$sect_num.'Grade"'.
				' name="'.$c_type.$sect_num.'Grade" type="text">'.
                    html_gen_combo($items, $selected, $placeholder).'
                </select>
            </div>';
}
/*=====================================================================
 Function: html_gen_mathElectives
 Description: Generates the html for the MATH elective
 Parameters: $courses (Math Elective courses that can be taken)
             $sect_num (student course if it exists)
 Author: Christopher Smith
======================================================================*/

function html_gen_mathElectives( $courses, $c)
{
    $form = '';
    $c_type = 'MathElective';
    $i = 0;
    if(empty($c))
    {
            $c = create_placeholder('MATH', 'Number', 'MATH Elective', 'Grade');
    }
    $form .= '<div class="row">';
    $form .= html_gen_pref($c_type, $i, $courses, $c->prefix);
    $form .= html_gen_numb($c_type, $i, $courses, $c->number);
    $form .= html_gen_name($c_type, $i, $courses, $c->name);
    $form .= html_gen_credits( $c_type, $i, $courses, $c->credits);
    $form .= html_gen_grades($c_type, $i, $c, $c->grade );
    $form .= '</div>';
    return $form;
}
/*=====================================================================
 Function: html_gen_mathElectives
 Description: Generates the html for the CSC electives
 Parameters: $courses (CSC Elective courses that can be taken)
             $sect_num (student courses if they exists)
 Author: Christopher Smith
======================================================================*/
function html_gen_csElectives($courses, $st_courses)
{
        $form = '';
        $c_type = 'CSElective';
        $i = 0;
        foreach($st_courses as $c)
        {
                $form .= '<div class="row">';
                $form .= html_gen_pref($c_type, $i, $courses, $c->prefix);
                $form .= html_gen_numb($c_type, $i, $courses, $c->number);
                $form .= html_gen_name($c_type, $i, $courses, $c->name);
                $form .= html_gen_credits( $c_type, $i, $courses, $c->credits);
                $form .= html_gen_grades($c_type, $i, $c, $c->grade );
                $form .= '</div>';
                $i++;
        }
        $tmp = create_placeholder('CSC', 'Number', 'CSC Elective', 'Grade');
        while($i < 4)
        {
                $form .= '<div class="row">';
                $form .= html_gen_pref($c_type, $i, $courses, $tmp);
                $form .= html_gen_numb($c_type, $i, $courses, $tmp);
                $form .= html_gen_name($c_type, $i, $courses, $tmp);
                $form .= html_gen_credits( $c_type, $i, $courses, $tmp);
                $form .= html_gen_grades($c_type, $i, $tmp, $tmp->grade);
                $form .= '</div>';
                $i++;
        }
        return $form;
}
/*=====================================================================
 Function: html_gen_pref
 Description: Generates the html for the prefix combo box
 Parameters: $c_type (class type eg: MATH CSC)
             $sect_num (row number in section)
             $course (Course the grades are generated for)
             $st_course (if the student has a course for this prefix)
 Author: Christopher Smith
======================================================================*/
function html_gen_pref($c_type, $sect_num, $courses, $st_course)
{

        $items = array();
        $selected = array();
        $placeholder = "PREF";
        foreach( $courses as $c )
        {
                if( $c['isactive'] =="yes" );
                {
                        array_push( $items, $c->prefix );
                }
        }
        $items = array_unique($items);
        $selected[0] = 'selected="selected"';
        for( $i = 0; $i < count($items); $i++)
        {
                array_push($selected, '');
                if($items[$i].'' ==$st_course)
                {
                        $selected[$i+1] = 'selected="selected"';
                        $selected[0] = '';
                }
        }
    return '<div class="two columns">
                <input class="u-full-width"
				id="'.$c_type.$sect_num.'Prefix" '.
                'name="'.$c_type.$sect_num.'Prefix" type="text" value="'.$courses[0]->prefix.'" placeholder ="'.$courses[0]->prefix.'" readonly \>'.
                '</div>';
}
/*=====================================================================
 Function: html_gen_numb
 Description: Generates the html for the number combo box
 Parameters: $c_type (class type eg: MATH CSC)
             $sect_num (row number in section)
             $course (Course the grades are generated for)
             $st_course (if the student has a course for this number)
 Author: Christopher Smith
======================================================================*/
function html_gen_numb($c_type, $sect_num, $courses, $st_course)
{
        $items = array();
        $selected = array();
        $placeholder = "Number";
        foreach( $courses as $c )
        {
                if($c['isactive']=="yes")
                {
                        array_push( $items, $c->number);
                }
        }
        $selected[0] = 'selected="selected"';
        for( $i = 0; $i < count($items); $i++)
        {
                array_push($selected, '');
                if($items[$i].'' == $st_course)
                {
                        $selected[$i+1] = 'selected="selected"';
                        $selected[0] = '';
                }
        }
    return '<div class="two columns">
                <select class="u-full-width" id="'.$c_type.$sect_num.'Number" name="'.$c_type.$sect_num.'Number" type="text">'.
                    html_gen_combo($items, $selected, $placeholder). '</select>'.
            '</div>';
}
/*=====================================================================
 Function: html_gen_name
 Description: Generates the html for the name combo box
 Parameters: $c_type (class type eg: MATH CSC)
             $sect_num (row number in section)
             $course (Course the grades are generated for)
             $st_course (if the student has a course for this name)
 Author: Christopher Smith
======================================================================*/
function html_gen_name( $c_type, $sect_num, $courses, $st_course)
{
        $items = array();
        $selected = array();
        $placeholder = "Course Name";
        foreach( $courses as $c )
        {
                if($c['isactive'] == "yes")
                {
                    array_push( $items, $c->name);
                }
        }
        $selected[0] = 'selected="selected"';
        for( $i = 0; $i < count($items); $i++)
        {
                array_push($selected, '');
                if($items[$i].'' == $st_course)
                {
                        $selected[$i+1] = 'selected="selected"';
                        $selected[0] = '';
                }
        }
        return '<div class="four columns">
                    <select class="u-full-width"
					id="'.$c_type.$sect_num.'Name"'.
					' name="'.$c_type.$sect_num."Name".
                    '" type="text">'.html_gen_combo($items, $selected, $placeholder).'</select>'.'
            </div>';

}
/*=====================================================================
 Function: html_gen_numb
 Description: Generates the html for the credits text
 Parameters: $c_type (class type eg: MATH CSC)
             $sect_num (row number in section)
             $course (Course the grades are generated for)
             $st_course (If the student has a course for this course)
 Author: Christopher Smith
======================================================================*/
function html_gen_credits($c_type, $sect_num, $courses, $st_course)
{
        $items = array();
        $selected = array();
        $placeholder = "Credits";
        foreach( $courses as $c )
        {
                if($c['isactive']=="yes")
                {
                    array_push( $items, $c->credits);
                }
        }
        $selected[0] = 'selected="selected"';
        $placeholder = "Credits";
        $place = '';
        for( $i = 0; $i < count($items); $i++)
        {
                array_push($selected, '');
                if($items[$i].'' == $st_course)
                {
                        $placeholder = $items[$i];
                        $place = $items[$i];
                        $selected[$i+1] = 'selected="selected"';
                        $selected[0] = '';
                }
        }

           return '<div class="two columns">
                   <input class="u-full-width"
				   id="'.$c_type.$sect_num."Credits\" ".
				   'name="'.$c_type.$sect_num."Credits".
                   '" type="text" value="'.$place.'"placeholder="'.$placeholder.'" \>
            </div>';
}
/*=====================================================================
 Function: csc_core
 Description: Generates the html for the core CSC course entry
 Parameters: $courses (courses to be added to the HTML document)
             $st_courses (the student's courses)
 Author: Christopher Smith
======================================================================*/
function csc_core($courses, $st_courses)
{
        //computer science required classes
        $readonly = array('readonly', 'readonly', 'readonly', 'readonly');
        $core  = hide_section('CSCore',  'Computer Science Core');
        $required = csc_required();
        foreach($courses as $course)
        {
                if( in_array($course->number, $required))
                {
                        $core .= html_gen($course, $st_courses, $readonly);
                }
        }
        $core .= '</div>';
        return $core;

}
/*=====================================================================
 Function: csc_electives
 Description: Generates the html for the csc electives block
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function csc_electives($courses, $st_courses)
{
        //computer science electives
        $electives  = hide_section('CSElective', 'Computer Science Electives');
        $electives .= html_gen_csElectives($courses, $st_courses);
        $electives .= '</div>';
        return $electives;
}
/*=====================================================================
 Function: sci_classes
 Description: Generates the html for the science courses block
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function sci_classes($courses, $st_courses)
{
        //science classes
        $prefix = array('PHYS', 'CHEM', 'BIOL', 'GEOL');
        $taken = array();
        $required = sci_required();
        $tmp = create_placeholder("PHYS", 211, "University Physics 1");
        $tmp->credits = '3';
        $sci = hide_section('SciReq',  'Science Requirements');
        //readonly array for physics 211
        $readonly = array('readonly', 'readonly', 'readonly','readonly');
        $sci .= html_gen($tmp, $st_courses, $readonly);
        //generic readonly array for remaining science classes
        $readonly = array('','','','');
        foreach($st_courses as $course)
        {
                if( $course->prefix != 'PHYS' && $course->number != 211)
                {
                        array_push($taken, $course);
                }
        }
        do{
                array_push($taken, create_placeholder( 'SCI'.count($taken),'Number','Science Requirement'));
                array_push($taken, create_placeholder( 'SCI'.count($taken),'NumberL','Science Requirement Lab'));
        }while(count($taken) < 4);
        for( $i = 0; $i < count($taken); $i++)
        {
            $sci .= gen_GenEd_html($taken[$i], $st_courses, 'SCI', $i);
        }
        $sci .='</div>';
        return $sci;
}
/*=====================================================================
 Function: math_classes
 Description: Generates the html for the mathematics block
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function math_classes($courses, $st_courses)
{
        //math classes
        $math = hide_section('MathReq', 'Mathematics Requirements');
        $readonly = array('readonly', 'readonly', 'readonly','readonly');
        foreach($courses as $course)
        {
                $math .= html_gen($course, $st_courses, $readonly).PHP_EOL;
        }
        return $math;
}

function math_electives($courses, $st_courses)
{
        $math = '';
        $i = 0;
        $tmp = create_placeholder( "MATH");
        foreach($st_courses as $c )
        {
                $math .= html_gen_mathElectives( $courses, $c);
                $i++;
        }
        if( $i == 0 )
        {
            $math .= html_gen_mathElectives($courses,$tmp);
        }
        $math .= '</div>';
        return $math;
}
/*=====================================================================
 Function: eng_classes
 Description: Generates the html for the engligh courses
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function eng_classes($courses, $st_courses)
{
        //english classes
        $readonly = array('readonly', 'readonly', 'readonly','readonly');
        $eng = hide_section('GenEdReq', 'English, Humanities, and Social Sciences');
        $required = eng_required();
        foreach($courses as $course)
        {
                if( in_array($course->number, $required))
                {
                        $eng .= html_gen($course, $st_courses, $readonly).PHP_EOL;
                }
        }
        return $eng;



}
/*=====================================================================
 Function: gen_GenEd_html
 Description: Generates the html for the general educatino courses
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function gen_GenEd_html($course, $st_courses, $pre, $num)
{
        $preval = $course->prefix;
        $numval = $course->number;
        $nameval = $course->name;
        $credval = $course->credits;
        list($found, $grade) = student_taken($course, $st_courses);
        if(!$found)
        {
            $preval = '';
            $numval = '';
            $nameval = '';
            $credval = '';
        }
        $form  = '<div class="row">
                <div class="two columns">
                <input class="u-full-width"
				id="'.$pre.$num.'Prefix" '.
                'name="'.$pre.$num."Prefix".
                '" type="text" value="'.$preval.'" placeholder="'.$course->prefix.'">
            </div>
            <div class="two columns">
                <input class="u-full-width"
				id="'.$pre.$num."Number".
                '" name="'.$pre.$num.'Number" '.
                ' type="text" value="'.$numval.'" placeholder="'.$course->number.'">
            </div>
            <div class="four columns">
                <input class="u-full-width"
				id="'.$pre.$num."Name".
                '" name="'.$pre.$num."Name".
                '" type="text" value="'.$nameval.'" placeholder="'.$course->name.'">
            </div>
            <div class="two columns">
            <input class="u-full-width"
                id="'.$pre.$num."Credits".
                '" name="'.$pre.$num."Credits".
                '" type="text" value="'.$credval.'" placeholder="'.$course->credits.'">
            </div>
                    ';
        $form .= html_gen_grades($pre, $num,$course, $grade) . '</div>
            ';
        return $form;

}
/*=====================================================================
 Function: soc_classes
 Description: Generates the html for the social studies courses
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function soc_classes($st_courses)
{
        $taken = array();
        $gen_ed = '';
        //humanity and ss classes
        foreach($st_courses as $c)
        {
                array_push($taken, $c);
        }
        while(count($taken) < 2 )
        {
                array_push( $taken, create_placeholder('SOC'.count($taken)) );
        }
        for( $i = 0; $i < count($taken); $i++)
        {
                $gen_ed .= gen_GenEd_html($taken[$i], $st_courses, 'SOC', $i);
        }
        return $gen_ed;



}
/*=====================================================================
 Function: hum_classes
 Description: Generates the html for the humanaties courses
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function hum_classes($st_courses )
{
        $taken = array();
        $hum = '';
        $readonly = array('','','','');

        foreach($st_courses as $c)
        {
                array_push($taken, $c);
        }
        while(count($taken) < 2 )
        {
                array_push($taken, create_placeholder('HUM'.count($taken)) );
        }
        for($i = 0; $i < count($taken); $i++)
        {
                $hum .= gen_GenEd_html($taken[$i], $st_courses, 'HUM', $i);
        }
        return $hum;
}
/*=====================================================================
 Function: free_classes
 Description: Generates the html for the free elective classes block
 Parameters: $courses (The courses to add to the block)
             $st_courses (The student's courses)
 Author: Christopher Smith
======================================================================*/
function free_classes($st_courses)
{
        $free = '';
        $taken = array();
        $readonly = array('','','','');
        //free electives the student can take
        //check if not a required class and that it is either the third or fourth humanities/ss class taken
        foreach($st_courses as $c)
        {
                array_push($taken, $c);
        }
        while( count( $taken ) < 2 )
        {
                array_push($taken, create_placeholder('Free'.count($taken)));
        }
        for($i = 0; $i < count($taken); $i++)
        {
                $free .= gen_GenEd_html($taken[$i], $st_courses, 'Free', $i);
        }
        $free .='</div>';
        return $free;
}
/*=====================================================================
 Function: get_button
 Description: Generates the html for the submit button and closes the document
 Author: Christopher Smith
======================================================================*/
function get_button()
{
        return '<div class="section">
               <input class="button-primary two columns rightside" id="submitButton" value="submit" style="bottom:-27px;" type="submit">
          </div>
        </form>
        <script>setupClassEntry();</script>
        <div id="toaster">You entered invalid data</div>

    </div>
    <div class="loader" id="loadingAnimationID"></div>
  </body>
</html>';
}

?>
