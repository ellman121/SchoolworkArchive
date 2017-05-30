<?php

function html_grad_head_gen()
{

        return ' <html lang="en">
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
  </head>
  <body>
    <div class="loader" id="loadingAnimationID"></div>
    <div class="container" id="bodyContainer">
      <section class="header">
        <h2>You\'re ready to graduate!</h2>
      </section>
      <hr/>
';
}

function html_grad_info_gen($gpa, $mgpa)
{
return '      <section class="u-full-width">
        <table class="twelve columns">
          <tbody>
            <tr>
              <td>Cumulative GPA</td>
              <td class="rightside">'.$gpa.'</td>
            </tr>
            <tr>
              <td>Major GPA</td>
              <td class="rightside">'.$mgpa.'</td>
            </tr>
          </tbody>
        </table>
      </section>
 ';
}

function html_grad_table_gen($courses)
{
    $html ='      <h5>A summary of your classes is below</h5>
      <table class="u-full-width">
        <thead>
          <tr>
            <th>Prefix</th>
            <th>Number</th>
            <th>Title</th>
            <th class="rightside">Grade</th>
          </tr>
        </thead>
        <tbody>' ;
    foreach($courses as $c)
    {
        $html .= "
          <tr>
            <th>$c->prefix</th>
            <th>$c->number</th>
            <th>$c->name</th>
            <th class=\"rightside\">$c->credits</th>
          </tr>
          ";
    }
    $html .= '</tbody></table>';
    return $html;
}
function html_grad_footer_gen()
{
    return '      <script>setupGrad();</script>
    </div>
  </body>
</html>
';
}

?>
