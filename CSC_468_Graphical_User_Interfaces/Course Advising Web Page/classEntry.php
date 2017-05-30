<?php
    /* 
     * This file will get the login id number and check the number and verify if it exists in database
     * if the file exists it will load that one for the advising tool 
     * otherwise it will start with an empty advising tool
     */ 

        require_once 'php/parseClasses.php'; //Pre parses the courses.xml file and sorts classes
        require_once 'php/parseStudent.php'; //Pre parsees the student.xml file for adding courses to the the entry form.
        require_once 'php/classEntryGen.php'; // contains the functions for generating html code for classentry.html
ini_set('display_errors',1);
error_reporting(-1);

$studentID = 0; 
function create_page($studentID)
{
        GLOBAL $CSC_courses;
        GLOBAL $CSC_electives;
        GLOBAL $SCI_courses;
        GLOBAL $MATH_courses;
        GLOBAL $MATH_electives;
        GLOBAL $ENGL_courses;
        //Creates a student object based on .xml file if exists or new one other wise
        $student = new Student( $studentID );
        //Generates the classEntry page based on .xml file and what student has taken if any.
        $classEntry  = get_header();
        $classEntry .= csc_core(      $CSC_courses,  $student->get_core() );
        $classEntry .= csc_electives( $CSC_electives,  $student->get_electives() );
        $classEntry .= sci_classes(   $SCI_courses,  $student->get_science() );
        $classEntry .= math_classes(  $MATH_courses, $student->get_math() );
        $classEntry .= math_electives($MATH_electives, $student->get_math_elecs());
        $classEntry .= eng_classes(   $ENGL_courses, $student->get_engl() );
        $classEntry .= soc_classes(   $student->get_soc()  );
        $classEntry .= hum_classes(   $student->get_hum()  );
        $classEntry .= free_classes(  $student->get_free() );
        $classEntry .= get_button();
        echo $classEntry;
}

if(array_key_exists('stuIDInput', $_POST) &&  array_key_exists('passIDInput', $_POST)) 
{
    //if the post array is !empty set the student ID  and password with cookie
    $studentID = $_POST['stuIDInput'];
    $studentPwd = $_POST['passIDInput'];
    setcookie("studentID", $studentID, time() + 1296000);
    create_page($studentID);
}
elseif( array_key_exists('studentID', $_COOKIE) ) //Else check if cookie exists
{
    $studentID = $_COOKIE['studentID'];
        create_page($studentID);
}
else
{
        header(':', true, 403);
        header('X-PHP-Response-Code: 403', true, 403);

    //die("Invalid Login or Cookie");
}

//PUT IN OTHER PHP FILES
//echo $_COOKIE['studentID'];
?>
