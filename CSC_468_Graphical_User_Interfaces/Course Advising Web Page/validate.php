<?php

$studentID = 0; 
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
    //die("Invalid Login or Cookie");
}
ini_set('display_errors','On');
ini_set("track_errors", 1);
ini_set("html_errors", 1);

error_reporting(E_ALL);

//$_POST['CSC300Grade'] = 1;
//$_POST['CSC250Grade'] = 5;
require_once 'php/parsePost.php';
//$invalid = array();
//list($canSubmit, $invalid) = check_classes();
$canSubmit = true;
if(!$st->parseReqs())
{
        $tmp = $st->getInvalids();
        $invalid = array_merge($tmp, $invalid);
}
if(!empty($invalid))
{
    $canSubmit = false;
}
$json = json_encode(array('canSubmit'=>$canSubmit, 'invalid'=>$invalid)); 
if($canSubmit)
{
        $st->save_student();
}
echo $json;



//echo $_COOKIE['studentID'];

?>
