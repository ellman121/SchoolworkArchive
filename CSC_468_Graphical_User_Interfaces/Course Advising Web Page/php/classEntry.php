<?php
/*=====================================================================
 file: classEntry.php
 Description: This file will get the login id number and check the number and
 	verify if it exists in database if the file exists it will load that one for
 	the advising tool otherwise it will start with an empty advising tool
 Author: Elliott Rarden
======================================================================*/

require 'parseClasses.php';
require 'parseStudent.php';
require 'classEntryGen.php';
if( !empty($_POST) && array_key_exists('stuIDInput', $_POST) &&  array_key_exists('passIDInput', $_POST)) 
{
    $studentID = $_POST['stuIDInput'];
    $studentPwd = $_POST['passIDInput'];
    setcookie("studentID", $studentID, time() + 86400);
}
elseif( array_key_exists('studentID', $_COOKIE) ) //Else check if cookie exists
{
    $studentID = $_COOKIE['studentID'];
}
else
{
        echo 'INS ELSO';
var_dump(http_response_code(403));
}
echo var_dump($_COOKIE);
echo 'HELLO';
// Get the necessary data out of our POST array
$student = new Student( $studentID );
$classEntry = 'HELLO WORLD INSIDE OUTSIDE';
// Build the body of the document
$classEntry  .= get_header();
$classEntry .= csc_core(      $CSC_courses,  $student->get_core() );
$classEntry .= csc_electives( $CSC_courses,  $student->get_electives() );
$classEntry .= sci_classes(   $SCI_courses,  $student->get_science() );
$classEntry .= math_classes(  $MATH_courses, $student->get_math() );
$classEntry .= eng_classes(   $ENGL_courses, $student->get_engl() );
$classEntry .= soc_classes(   $student->get_soc()  );
$classEntry .= hum_classes(   $student->get_hum()  );
$classEntry .= free_classes(  $student->get_free() );
$classEntry .= get_button();

// Return the body
//echo $classEntry.var_dump($classEntry);
?>
