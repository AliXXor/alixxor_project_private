<?php
$html_attributes = "lang=\"{$language->language}\" dir=\"{$language->dir}\" {$rdf->version}{$rdf->namespaces}";
?>
<?php print $doctype; ?>
<!--[if IE 8 ]><html <?php print $html_attributes; ?> class="no-js ie8"><![endif]-->
<!--[if IE 9 ]><html <?php print $html_attributes; ?> class="no-js ie9"><![endif]-->
<!--[if (gt IE 9)|!(IE)]><!--><html <?php print $html_attributes; ?> class="no-js"><!--<![endif]-->

<head <?php print $rdf->profile; ?>>
  <!-- Global site tag (gtag.js) - Google Analytics -->
  <script async src="https://www.googletagmanager.com/gtag/js?id=UA-151575813-1"></script>
  <script>
  window.dataLayer = window.dataLayer || [];
  function gtag(){dataLayer.push(arguments);}
  gtag('js', new Date());

  gtag('config', 'UA-151575813-1');
  </script>
<?php print $head; ?>

<title><?php print $head_title; ?></title>
<meta name="viewport" content="initial-scale = 1.0,maximum-scale = 1.0">

<?php print $styles; ?>
<?php print $scripts; ?>

<script type="text/javascript">
  WebFontConfig = {

    google: { families: [ 'Lato:700,400,300:latin' ] }

  };

  (function() {

    var wf = document.createElement('script');

    wf.src = ('https:' == document.location.protocol ? 'https' : 'http') +

      '://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';

    wf.type = 'text/javascript';

    wf.async = 'true';

    var s = document.getElementsByTagName('script')[0];

    s.parentNode.insertBefore(wf, s);

  })();
 </script>
<script type="text/javascript">
  jQuery(document).ready(function($) {

    if(navigator.platform == 'iPad' || navigator.platform == 'iPhone' || navigator.platform == 'iPod'){

      $("div#colorbox").css("display", "none !important");

      $("#cboxOverlay").css("visibility", "hidden");

      $("#back-top").css("visibility", "hidden");

    };

  });
</script>
</head>
<body id="body" class="<?php print $classes; ?>" <?php print $attributes;?>>
 <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
