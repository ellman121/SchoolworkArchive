<?php
require_once "advisingGen.php";
$gpa  = $st->get_gpa();
$mgpa =  $st->get_mgpa();
$tot_creds = $st->get_total_credits();
$csc_creds = $st->get_csc_credits();
$needed = $st->get_needed_credits();
$taken = $st->get_taken();
$taking = $st->get_taking();
$completed = array_merge($taking, $taken);
$recommendations = $st->get_recommendations();


$completed = array_merge( $taking, $taken);
$page = html_head_gen();
$page .= html_info_gen($gpa, $mgpa, $tot_creds,$csc_creds);
$page .= html_prog_gen($tot_creds, $needed);
$page .= hide_section('Recommend', 'Classes we Recommend');
$page .= html_table_gen($recommendations);
$page .= hide_section('Completed', 'Classes you\'ve Completed');
$page .= html_table_gen($completed);
$page .= html_closing_gen();

echo $page;
?>
