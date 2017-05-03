<?php
/*=====================================================================
 file: calculateGPA.php
 Description: Function to calculate the gpa for a student
 Author: Elliott Rarden
======================================================================*/

/*=====================================================================
 Function: calculateGPA
 Description: Calcualte the GPA
 Parameters: $credits (An array containing the number of credits in each cell)
             $gradeVals (Values corresponding to the grade for each credit)
			 $length (The length of the arrays to calcualte)

			 e.x.
			 $credits     = [3, 3, 4, 3, 3, 4, 3, 4]
			 $gradeValues = [4, 4, 3, 4, 3, 3, 3, 4] // A, A, B, A, B, B, B, A
			 $length      = 8
 Author: Elliott Rarden
======================================================================*/
function calculateGPA($credits, $gradeValues, $length)
{

    $total_credits = array_sum($credits);
    if( $total_credits == 0)
    {
            return 0;
    }
	$grade_points = 0;
	for ($i=0; $i < $length; $i++) {
		$grade_points += $credits[$i] * $gradeValues[$i];
	}
	return round( ($grade_points / $total_credits), 2);
}

?>
