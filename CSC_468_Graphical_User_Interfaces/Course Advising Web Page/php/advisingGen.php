<?php



function html_head_gen()
{
        return '<html lang="en">
            <head>
                <!-- include.pug - Includes libraries and stuff-->
                <!-- UTF-8 is master text encoding-->
                <meta charset="UTF-8"/>
                <!-- CSS Libraries-->
                <link href="css/normalize.css" rel="stylesheet"/>
                <link href="css/skeleton.css" rel="stylesheet"/>
                <link href="css/animate.css" rel="stylesheet"/>
                <!-- CSS Files-->
                <link href="css/custom.css" rel="stylesheet"/>
                <link href="css/loader.css" rel="stylesheet"/>
                <!-- Javascript Libraries-->
                <script src="js/jquery-2.2.4.min.js"></script>
                <script src="js/jquery-pjax.js"></script>
                <!-- Javascript files-->
                <script src="js/app.js"></script>
                <!-- Show things-->
                <title>Advisinator</title>
            </head>';
}

function html_info_gen($gpa, $mgpa, $tot_creds, $csc_creds)
{
  return '<body>
    <div class="loader" id="loadingAnimationID"></div>
    <div class="container" id="bodyContainer">
      <div class="section header">
        <h2>Advising Page</h2>
      </div>
      <hr/>
      <div class="section gpa u-full-width">
        <h5>Information</h5>
        <table class="twelve columns">
          <tbody>
            <tr>
              <td>Cumulative GPA</td>
              <td class="rightText">'.$gpa.'</td>
            </tr>
            <tr>
              <td>Major GPA</td>
              <td class="rightText">'.$mgpa.'</td>
            </tr>
            <tr>
              <td>Total Credits</td>
              <td class="rightText">'.$tot_creds.'</td>
            </tr>
            <tr>
              <td>CSC Credits</td>
              <td class="rightText">'.$csc_creds.'</td>
            </tr>
          </tbody>
        </table>
        </div>';
}

function html_prog_gen( $needed, $total)
{
        $percent = $needed/$total;
        $percent = number_format( $percent * 100, 0 );
      return'<div class="section progress u-full-width">
        <h5>Degree Progress: '.$percent.'%</h5>
        <div class="progressBar twelve columns" data-percent="'.$percent.'%">
          <div class="progressBarBar"></div>
        </div>
        <hr class="u-full-width"/>
      </div>';
}

function html_table_gen( $courses )
{
  $html = '<table class="twelve columns">
          <thead>
            <tr>
              <th>Prefix</th>
              <th class="centerText">Number</th>
              <th class="centerText">Title</th>
              <th class="rightText">Credits</th>
            </tr>
          </thead>
          <tbody>';
          foreach($courses as $c)
          {
            $html .= '<tr>
                <td>'.$c->prefix.'</td>
                <td class="centerText">'.$c->number.'</td>
                <td class="centerText">'.$c->name.'</td>
                <td class="rightText">'.$c->credits.'</td>
              </tr>';
          }
  $html.='
          </tbody>
        </table>
        </div>
        ';
          return $html;
}

function html_closing_gen()
{
        return '
                <div class="section u-full-width">
        <h5 class="u-full-width">For more help, please email or see your advisor</h5>
      </div>
      <script>setupRecommendations();</script>
    </div>
  </body>
</html>';
}

















?>
