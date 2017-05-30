<?php
        require_once "php/parseClasses.php";
        require_once "php/parseStudent.php";
function create_advising()
{


        ini_set('display_errors','On');
        ini_set("track_errors", 1);
        ini_set("html_errors", 1);

        error_reporting(E_ALL);
        $st = new student($_COOKIE['studentID']);

        if($st->get_total_credits() >= 115 )
        {
                require_once "php/graduation.php";
        }
        else
        {
                require_once "php/classEntryGen.php"; 
                require_once "php/adviser.php";
        }
}
if(array_key_exists('stuIDInput', $_POST) &&  array_key_exists('passIDInput', $_POST)) 
{
    //if the post array is !empty set the student ID  and password with cookie
    $studentID = $_POST['stuIDInput'];
    $studentPwd = $_POST['passIDInput'];
    setcookie("studentID", $studentID, time() + 1296000);
    create_advising($studentID);
}
elseif( array_key_exists('studentID', $_COOKIE) ) //Else check if cookie exists
{
    $studentID = $_COOKIE['studentID'];
        create_advising($studentID);
}
else
{
        header(':', true, 403);
        header('X-PHP-Response-Code: 403', true, 403);

    //die("Invalid Login or Cookie");
}

?>
