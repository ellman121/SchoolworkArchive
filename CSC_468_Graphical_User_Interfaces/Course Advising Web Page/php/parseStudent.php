<?php
/*=====================================================================
 file: parseStudent.php
 Description: This file contains the methods for student class
 that handles loading the student file, writing the file, checking
 pre-reqs and co-reqs, and recommendations.
 Author: Christopher Smith, Elliott Rarden
======================================================================*/






require_once "calculateGPA.php";
$ignore_reqs = array('permission of instructor' , 
                    'appropriate mathematics placement', 
                    'MATH 115 with C or better', 
                    'MATH 120 with C or better', 
                    'appropriate score on departmental Trigonometry Placement Examination');
/* this file provides the functions necessary to parse the student xml file. Also will load the student file*/


/*=====================================================================
 Class: course
 Description: course object that is used to store class information in student object 
 Author: Christopher Smith
======================================================================*/
class course{
        var $prefix;
        var $number;
        var $name;
        var $credits;
        var $grade;
        var $htmlID;
        var $prereq;
        var $coreq;
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/

        function __construct( $c )
        {
                if($c !== null)
                {
                        $this->htmlID = array();
                        $this->prefix  = $c->prefix;
                        $this->number  = $c->number;
                        $this->name    = $c->name;
                        $this->credits = $c->credits;
                        $this->grade   = $c->grade;
                        $this->prereq  = $c->prereq;
                        $this->coreq   = $c->coreq;
                }
        }
/*=====================================================================
 Function: set_html
 Description: Stores the html IDs in the course object for sending 
 invalid flags back to javascript for courses without pre-reqs and co-reqs met. 
 Parameters: $ids HTML ids to store
 Author: Christopher Smith
======================================================================*/
        function set_html($ids)
        {
            $this->htmlID = array();
            foreach($ids as $id)
            {
                array_push($this->htmlID, $id);
            }
        }
}
/*=====================================================================
 Class: Student 
 Description: Student object that contains methods for checking pre-reqs,
 co-reqs, getting recommendations, and writing and saving the the xml file
 Author: Christopher Smith
======================================================================*/
class Student{
        private $csc_credits; //CSC credits for major GPA
        private $total_credits; //Total credits for gpa
        private $needed_credits; // needed credits to graduate
        private $id; //Student ID
        private $gpa; //Students GPS
        private $mgpa; //Student major GPA
        private $completed_credits; //Completed credits so far
        private $courses; //Courses taken
        //Arrays for holding classes classified for the checklist
        private $core;//CSC courses taken
        private $math;//Math courses taken
        private $math_elec;//Math electives taken
        private $elec;//CSC electives taken
        private $sci;//Science classes taken
        private $hum;//Humanity classes taken
        private $soc;//Social classes taken
        private $engl;//English classes taken
        private $free;//Free class taken
        private $met;//Flag to set if an invalid class is taken
        private $invalids; //Invalid classes taken without pre reqs
        private $taking; //Classes currently taking
        private $taken; //Classes already taken
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/

        function getInvalids()
        {
                $tmp = array();
                foreach( $this->invalids as $inv)
                {
                    $tmp = array_merge($tmp, $inv);
                }
                $tmp = array_unique($tmp);
                return $tmp;
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        function parseReqs()
        {
                $this->invalids = array();
                $this->met = true;
                $this->parseCore();
                $this->parseMath();
                $this->parseMathElec();
                $this->parseElec();
                return $this->met;
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function g_to_i($g)
        {
                switch($g)
                {
                    case 'A':return 5;
                    case 'B':return 4;
                    case 'C':return 3;
                    case 'D':return 2;
                    case 'F':return 1;
                    default: return 0;
                };
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function check_grade($courses, $num, $gra, $type)
        {
                for( $i = 0; $i< count($courses); $i++)
                {
                        if($courses[$i]->number == $num )
                        {
                                if( $this->g_to_i($courses[$i]->grade) >= $this->g_to_i($gra) || strpos($type, $courses[$i]->grade ) !== false)
                                {
                                        return true;
                                }
                                else
                                {
                                        array_push($this->invalids, $courses[$i]->htmlID );
                                        $this->met = false;
                                        return false;
                                }
                        }
                }
                return false;
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function check_course($pre, $num, $gra, $type)
        {

                if( $pre == 'CSC' && in_array($num, csc_required()))
                {
                    return $this->check_grade($this->core, $num, $gra, $type);
                }
                elseif( $pre == 'MATH')
                {
                    return $this->check_grade($this->math, $num, $gra, $type);
                }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function push_coreq($pre, $num)
        {
                if( $pre == 'CSC' && in_array($num, csc_required()))
                {
                        for( $i=0; $i< count($this->core); $i++)
                        {
                                if($this->core[$i]->number == $num )
                                {
                                        array_push($this->invalids, $this->core[$i]->htmlID);
                                        return;
                                }
                        }
                }
                elseif( $pre == 'MATH')
                {
                        for( $i=0; $i< count($this->math); $i++)
                        {

                                if($this->math[$i]->number == $num )
                                {
                                        array_push($this->invalids, $this->math[$i]->htmlID);
                                        return;
                                }
                        } 
                }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function check_req($req, $type)
        {
                GLOBAL $ignore_reqs;
                $met = false;
                if( empty($req) )
                {
                        return true;
                }
                $classes = explode(',', $req);
                foreach( $classes as $c )
                {
                        $c = trim( $c );
                    if( !in_array( $c, $ignore_reqs ) )
                    {   
                        $prefix = strtok($c, ' ');
                        $number = strtok(' ');
                        $tmp = strtok (' ');
                        $grade = strtok(' ');
                        if($grade === false)
                        {
                                $grade = 'D';
                        }
                        if( $type == 'EX' && !$this->check_course($prefix, $number, $grade, $type) )
                        {
                                return false;
                        }
                        elseif( $type == 'EX Taking PI' && !$this->check_course($prefix, $number, $grade, $type) )
                        {
                                $this->push_coreq($prefix, $number);
                                return false;
                        }
                    }
                }
                return true;
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function checkPre($c)
        {
            for( $i = 0; $i < count($c->prereq); $i++ )
            {
                    
                    if( $this->check_req($c->prereq[$i], 'EX') )
                    {
                            return true;
                    }
            }
            if( $i != 0 )
            {
                    $this->met = false;
            }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function checkCo($c)
        {
                $i = 0;
                for( $i = 0; $i < count($c->coreq); $i++ )
                {
                        if($this->check_req($c->coreq[$i], 'EX Taking PI'))
                        {
                                return true;
                        }
                }
                if( $i != 0 )
                {
                        $this->met = false;
                }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function checkReqs($c)
        {
            $this->checkPre($c);
            $this->checkCo($c);
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function parseCore()
        {
                foreach( $this->core as $c )
                {
                        if( !in_array($c->grade, array('EX', '', 'PI')))
                        {
                                $this->checkReqs($c);
                        }
                }

        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function parseMath()
        {
                foreach( $this->math as $c )
                {
                        if( !in_array($c->grade, array('EX', '', 'PI')))
                        {
                                $this->checkReqs($c);
                        }
                }     
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function parseMathElec()
        {
            foreach( $this->math_elec as $c )
            {
                    if( !in_array($c->grade, array('EX', '', 'PI')))
                    {
                            $this->checkReqs($c);
                    }
            }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function parseElec()
        {
                foreach( $this->elec as $c)
                {
                        if( !in_array($c->grade, array('EX', '', 'PI')))
                        {
                                $this->checkReqs($c);
                        }
                }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        function __construct( $studentID )
        {
                $this->init_arrays();
                if( $studentID != null)
                {
                        $filename = 'students/'.$studentID . '.xml';
                        if( file_exists( $filename ) )
                        {
                                $this->load_student($filename);
                        }
                        else
                        {
                                $this->new_student($studentID);
                        }
                }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function init_arrays()
        {
                $this->csc_credits = 0;
                $this->needed_credits = 120;
                $this->total_credits = 0;
                $this->gpa = 0;
                $this->mgpa = 0;
                $this->completed_credits = 0;
                $this->taking = array();
                $this->taken = array();
                $this->invalids = array();
                $this->courses = array();
                $this->core = array();
                $this->math_elec = array();
                $this->math = array();
                $this->elec = array();
                $this->sci = array();
                $this->hum = array();
                $this->soc = array();
                $this->engl = array();
                $this->free = array();
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function new_student($studentID)
        {
                $this->id = $studentID;
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function load_student($studentFile)
        {
                $xmlStudent = simplexml_load_file($studentFile);
                $this->id = $xmlStudent->studentid;
                $this->gpa = $xmlStudent->gpa;
                $this->mgpa = $xmlStudent->major_gpa;
                $this->courses= $xmlStudent->courses->course;
                $this->set_classes();
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function set_classes()
        {
                $tmp = $this->courses;
                $this->courses = array();
                foreach( $tmp as $c)
                {
                        $this->add_course($c);
                } 
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function update_courselist($c)
        {       
                $csc_core  = csc_required();
                $csc_elec  = csc_elective();
                $math_core = math_required();
                $math_elecs = math_elective(); 
                $sci_pre   = get_sci_prefix();
                $hum_pre   = get_hum_prefix();
                $soc_pre   = get_soc_prefix();
                $engl_pre = eng_required();
                $hum_count = count($this->hum);
                $ele_count = count($this->elec);
                $soc_count = count($this->soc);
                if($c->prefix == 'CSC' && in_array($c->number, $csc_core) )
                {
                        array_push($this->core, $c);
                }
                else if( $c->prefix == 'MATH' && in_array($c->number, $math_core))
                {
                        array_push($this->math, $c);
                }
                else if( $c->prefix == 'MATH' && in_array($c->number, $math_elecs))
                {   
                        array_push($this->math_elec, $c);
                }
                else if( $c->prefix == 'CSC' && in_array($c->number, $csc_elec) && $ele_count < 4 )
                {
                        array_push( $this->elec, $c);
                }
                else if( in_array($c->prefix, $sci_pre))
                {
                        array_push($this->sci, $c);
                }
                else if( $c->prefix == 'ENGL' && in_array($c->number, $engl_pre))
                {
                        array_push($this->engl, $c);
                }
                else if( in_array($c->prefix, $hum_pre) && $hum_count < 2)
                {
                        array_push($this->hum, $c);
                        $hum_count++;
                }
                else if( in_array($c->prefix, $soc_pre) && $soc_count < 2)
                {
                        array_push($this->soc, $c);
                        $soc_count++;
                }
                else
                {
                        array_push($this->free, $c);
                }

        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function calc_gpa()
        {
                $gradeValues = array();
                $credits = array();
                $grades = array('A', 'B', 'C', 'D', 'F');
                foreach( $this->taken as $c )
                {
                        if( in_array($c->grade, $grades))
                        {
                                array_push($gradeValues, $this->g_to_i($c->grade)-1);
                                array_push($credits, (int) $c->credits);
                        }
                }
            $this->gpa = calculateGPA($credits, $gradeValues, count($credits));
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function calc_mgpa()
        {
                $gradeValues = array();
                $credits = array();
                $grades = array('A', 'B', 'C', 'D', 'F');
                foreach( $this->taken as $c )
                {
                        if( $c->prefix == 'CSC' && in_array($c->grade, $grades))
                        {
                                array_push($gradeValues, $this->g_to_i($c->grade)-1);
                                array_push($credits, (int)$c->credits);
                        }
                }
                $this->mgpa = calculateGPA($credits, $gradeValues, count($credits));
        }
 /*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/       
        function save_student()
        {
                $this->calc_gpa();
                $this->calc_mgpa();
            file_put_contents("students/$this->id.xml", $this->create_xml());
        }

        #Used to update a field of an already existing course in student course list
        function set_course_field($prefix, $number, $field, $value)
        {

        }
        private function course_exists($c, &$course_array)
        {
                
        }
 /*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        function update_course($c)
        {
                $csc_core  = csc_required();
                $sci_pre   = get_sci_prefix();
                $hum_pre   = get_hum_prefix();
                $soc_pre   = get_soc_prefix();
                $hum_count = count($this->hum);
                $ele_count = count($this->elec);
                $soc_count = count($this->soc);
                if($c->prefix == 'CSC' && in_array($c->number, $csc_core))
                {
                        array_push($this->core,$c);
                }
                else if( $c->prefix == 'MATH')
                {
                        if( $c->number == 321 )
                        {
                                var_dump($c);
                        }
                    array_push($this->math, $c);
                }
                else if( $c->prefix == 'CSC' && $ele_count < 4 )
                {
                        array_push($this->elec,$c);
                }
                else if( in_array($c->prefix, sci_pre))
                {
                        array_push($this->sci,$c);
                }
                else if( $c->prefix == 'ENGL' && in_array($c->number, $engl))
                {
                        array_push($this->engl,$c);
                }
                else if( in_array($c->prefix, $hum_pre) && $hum_count < 2)
                {
                        array_push($this->hum,$c);
                        $hum_count++;
                }
                else if( in_array($c->prefix, $soc_pre) && $soc_count < 2)
                {
                        array_push($this->soc,$c);
                        $soc_count++;
                }
                else
                {
                        array_push($this->free,$c);
                }

        }
        //add a course that doesn't exist in student course list
 
 /*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        function add_course($c)
        {
                array_push($this->courses, $c);
                if($c->grade != 'EX' && $c->grade != 'Taking' && $c->grade != 'PI' && $c->grade != '' )
                {
                        $this->completed_credits += $c->credits;
                }
                if(!empty($c->grade))
                {
                        if( $c->grade == 'Taking' || $c->grade == 'PI' )
                        {
                                array_push($this->taking, $c);
                        }
                        if($c->grade == 'EX')
                        {
                                $this->needed_credits -=$c->credits;
                                array_push($this->taken, $c);
                        }
                        else
                        {
                                $this->total_credits = $c->credits;
                                if($c->prefix == 'CSC')
                                {
                                        $this->csc_credits += $c->credits;
                                }
                                array_push($this->taken, $c);
                        }
                }
                $this->update_courselist($c);
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        function not_taken($c)
        {
            for( $i = 0; $i< count($this->taken); $i++)
            {
                    if($this->taken[$i] == $c->prefix && $this->taken[$i] == $c->prefix )
                    {
                            return false;
                    }
            }
            return true;
        }

/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function get_numbers($prefix)
        {
            $nums = array();
            $comp = array_merge($this->taking, $this->taken);
            foreach( $comp as $c)
            {
                if($c->prefix == $prefix)
                {
                    array_push($nums, $c->number);
                }
            }
            return $nums;
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function get_cs_course($c)
        {
            GLOBAL $CSC_courses;
            foreach( $CSC_courses as $course)
            {
                if( $course->number == $c)
                {
                    return $course;
                }
            }
        }

/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function get_math_course($c)
        {
                GLOBAL $MATH_courses;
                foreach( $MATH_courses as $course )
                {
                        if( $course-> number == $c )
                        {
                                return $course;
                        }
                }
        }
/*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/

        function get_recommendations()
        {
                GLOBAL $CSC_courses;
                GLOBAL $CSC_electives;
                $recommend = array();
                $pre_array = array();
                $num_array = array();
                $name_array = array();
                $cred_array = array();
                $comp = array_merge($this->taking, $this->taken);
                
                
                $math = math_required();
                $core_count = 0;
                $math_count = 0;
                $rec_classes = array();
                $cores = csc_core_with_prereq();
                $keys = array_keys($cores);
                $cs_nums = $this->get_numbers('CSC');
                $math_nums = $this->get_numbers('MATH');
                $flag = true;
                foreach( $cores as $cnum => $prereq )
                {
                        if( !in_array($cnum, $cs_nums) && $core_count < 2)
                        {
                            $flag = true;
                            for( $k = 0; $k < count($prereq) && $flag; $k++ )
                            {
                                if( !in_array($prereq[$k], $cs_nums) )
                                {
                                    $flag = false;
                                }
                            }
                            if($flag)
                            {
                                    $core_count++; 
                                    $course = $this->get_cs_course($cnum);
                                    array_push($rec_classes, $course);
                            }
                        }
                }
                if(!in_array(110, $cs_nums))
                {
                        $core_count++; 
                        $course = $this->get_cs_course(110);
                        array_push($rec_classes, $course);

                }
                if(!in_array(150, $cs_nums))
                {
                        $core_count++; 
                        $course = $this->get_cs_course(150);
                        array_push($rec_classes, $course);

                }
                foreach( $math as $num)
                {
                    if( !in_array($num, $math_nums) && $math_count < 1)
                    {
                        $course = $this->get_math_course($num);
                        array_push($rec_classes, $course);
                        $core_count++;
                        $math_count++;
                    }
                }
                if($core_count < 3 && empty($this->math_elec) && $math_count < 1)
                {
                        $tmp = (object) array('prefix'=>'MATH',
                                'number'=>'Number',
                                'name'=>'Math Elective',
                                'credits'=>'3-4',
                                'grade'=>'Grade');
                        array_push($rec_classes, $tmp);
                }

                $hum_count = count($this->hum);
                $ele_count = count($this->elec);
                $soc_count = count($this->soc);
                if( $soc_count < 2 )
                {
                        $tmp = (object) array( 'prefix'=>'SOC',
                                'number'=>'Number',
                                'name'=>'Social Science Requirement',
                                'credits'=>'3',
                                'grade'=>'Grade');
                        array_push($rec_classes, $tmp);
                }
                if( $hum_count < 2 )
                {
                        $tmp = (object) array( 'prefix'=>'HUM',
                                'number'=>'Number',
                                'name'=>'Humanity Requirement',
                                'credits'=>'3',
                                'grade'=>'Grade');
                        array_push($rec_classes, $tmp);
                }
                if( count ($rec_classes) < 4 && $free < 2)
                {
                            $tmp = (object) array( 'prefix'=>'GenEd',
                                    'number'=>'Number',
                                    'name'=>'Free Elective',
                                    'credits'=>'3',
                                    'grade'=>'Grade');
                            array_push($rec_classes, $tmp);
                }
                if( $this->completed_credits > 70 )
                {
                        $tmp = (object) array( 'prefix'=>'CSC',
                                'number'=>'Number',
                                'name'=>'CSC Elective',
                                'credits'=>'3-4',
                                'grade'=>'Grade');
                        array_push($rec_classes, $tmp);

                }
                elseif( count($this->sci) < 5 )
                {
                        $tmp = (object) array( 'prefix'=>'SCI',
                                'number'=>'Number',
                                'name'=>'Science Requirement',
                                'credits'=>'3',
                                'grade'=>'Grade');
                        array_push($rec_classes, $tmp);
                }
                return $rec_classes;
        }
    
    /*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
        private function create_xml()
        {
                $studentFile = 'students/'.$this->id.'.xml';
                $studentXML  = '<?xml version="1.0" encoding="UTF-8"?>';
                $studentXML .= '<!DOCTYPE student SYSTEM "student.dtd">';
                $studentXML .= '<student isActive = "yes">';
                $studentXML .= "<id>$this->id</id>
                                <gpa>$this->gpa</gpa>
                                <major_gpa>$this->mgpa</major_gpa>
                                <completed_credits>$this->completed_credits</completed_credits>
                                <csc_credits>$this->csc_credits</csc_credits>
                                <courses>";
                foreach( $this->courses as $c )
                {
                        $studentXML .= "<course>
                                            <prefix>$c->prefix</prefix>
                                            <number>$c->number</number>
                                            <name>$c->name</name>
                                            <credits>$c->credits</credits>
                                            <grade>$c->grade</grade>
                                        </course>";
                }
                $studentXML .= "</courses>
                        </student>";
                return $studentXML;
        }
    
    /*=====================================================================
 Function: __construct
 Description: Constructor for the course type to be stored in student 
 Parameters: $c course object from xml database or plaecholder for geneds
 Author: Christopher Smith
======================================================================*/
    
        function get_csc_credits()
        {
            return $this->csc_credits;
        }
        function get_needed_credits()
        {
            return $this->needed_credits;
        }
        function get_total_credits()
        {
            return $this->completed_credits;
        }
        function get_taken()
        {
            return $this->taken;
        }
        function get_taking()
        {
            return $this->taking;
        }
        
        function get_id()
        {
                return $this->id;
        }

        function get_courses()
        {
                return $this->courses;
        }

        function get_gpa()
        {
                return $this->gpa;
        }

        function get_mgpa()
        {
                return $this->mgpa;
        }

        function get_completed_credits()
        {
                return $this->completed_credits;
        }

        function get_core()
        {
                return $this->core;
        }

        function get_electives()
        {
                return $this->elec;
        }

        function get_math()
        {
                return $this->math;
        }
        function get_math_elecs()
        {
            return $this->math_elec;
        }
        function get_science()
        {
                return $this->sci;
        }

        function get_engl()
        {
            return $this->engl;
        }

        function get_hum()
        {
                return $this->hum;
        }

        function get_soc()
        {
                return $this->soc;
        }

        function get_free()
        {
                return $this->free;
        }
        function set_id( $id )
        {
            $this->id = $id;
        }
}
?>
