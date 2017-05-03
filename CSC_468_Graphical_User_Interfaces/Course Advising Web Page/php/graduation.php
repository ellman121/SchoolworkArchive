<?php

require_once "graduationGen.php";
require_once "parseStudent.php";

$gpa = $st->get_gpa();
$mgpa = $st->get_mgpa();
$courses = $st->get_courses();



$page = html_grad_head_gen();
$page .= html_grad_info_gen($gpa, $mgpa);
$page .= html_grad_table_gen($courses);
$page .= html_grad_footer_gen();
echo $page;


?>
