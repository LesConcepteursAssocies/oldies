<html>
<head>
<title>Unité expérimentale de Lexicosynthèse</title>
</head>

<body BGCOLOR="#FFFFFF">
<center><h1>Le LOGOTRON, Unité Expérimentale de Lexicosynthèse ;-)</h1></center>
<hr>
<p>
D'après les travaux de <a href="http://www.jp-petit.com">Jean-Pierre PETIT</a>.
</p>
<p>
Jean-Pierre Petit a signé chez Belin une série de bandes dessinées, "Les aventures d'Anselme Lanturlu", qui mettent à la portée de tous des questions scientifiques complexes: les géométries non-euclidiennes ("LE GEOMETRICON"), la cybernétique "A QUOI REVENT LES ROBOTS"), etc. 
</p>
<p>
Le présent programme est écrit en PHP et les sources sont <a href="/cgi-bin/viewcvs.cgi/logotron">ici</a>.
</p>
<hr>
<?php
	// Renvoie TRUE si le premier caractère de la chaine donnée est une voyelle.
	function IsVoyelle($chaine)
	{
		$car = $chaine[0];
		$car = strtoupper($car);
		$pos = strpos("AEIOUY", $car);
		if (is_string($pos) && !$pos)
		{
			// non trouvé
			return FALSE;
		}
		return TRUE;
	}

	// Génère un mot à partir d'un préfixe et d'un suffixe.
	// Enlève la dernière voyelle du préfixe si le premier caractère du suffixe est aussi une voyelle.
	function GenWord($pr,$sf)
	{
		$lastpref = substr($pr,-1);
		if(IsVoyelle($lastpref))
		{
			$firstsuff = substr($sf,0,1);
			if(IsVoyelle($firstsuff))
			{
				$prf = substr($pr,0,strlen($pr)-1);
				return ($prf . $sf);
			}
		}
		return ($pr . $sf);
	}

	// Renvoie TRUE si la signification du préfixe commence par "LE"
	function Pref_LE($spr)
	{
		if(strcasecmp(substr($spr,0,3),"LE ") == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	// Renvoie TRUE si la signification du préfixe commence par "LA"
	function Pref_LA($spr)
	{
		if(strcasecmp(substr($spr,0,3),"LA ") == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	// Renvoie TRUE si la signification du préfixe commence par "L'"
	function Pref_Lq($spr)
	{
		if(strcasecmp(substr($spr,0,2),"L'") == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	// Renvoie TRUE si la signification du préfixe commence par "LES"
	function Pref_LES($spr)
	{
		if(strcasecmp(substr($spr,0,4),"LES ") == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	// Renvoie TRUE si la signification du suffixe se termine par "DE"
	function Suff_DE($ssf)
	{
		if(strcasecmp(substr($ssf,-3)," DE") == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	// Renvoie TRUE si la signification du suffixe se termine par "A"
	function Suff_A($ssf)
	{
		if(strcasecmp(substr($ssf,-2)," A") == 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	// Génère une signification "lisible" du mot
	function Signification($spr,$ssf)
	{
		if(Suff_DE($ssf))
		{
			if(Pref_LES($spr))
			{
				// Remplace "DE LES" par "DES"
				return (substr($ssf,0,strlen($ssf)-2) . "DES" . substr($spr,3));
			}
			if(Pref_LE($spr))
			{
				// Remplace "DE LE" par "DU"
				return (substr($ssf,0,strlen($ssf)-2) . "DU" . substr($spr,2));
			}
		}
		if(Suff_A($ssf))
		{
			if(Pref_LES($spr))
			{
				// Remplace "A LES" par "AUX"
				return (substr($ssf,0,strlen($ssf)-1) . "AUX" . substr($spr,3));
			}
			if(Pref_LE($spr))
			{
				// Remplace "A LE" par "AU"
				return (substr($ssf,0,strlen($ssf)-1) . "AU" . substr($spr,2));
			}
		}
		return ($ssf . " " . $spr);
	}

	// Initialise le générateur aléatoire
	srand((double)microtime()*1000000);

	// connection à la base ...
	$logotron = pg_Connect("dbname=logotron port=5432");
	if($logotron == 0)
	{
		echo htmlentities("<b>Impossible de se connecter à la base du logotron!<b>");
	}
	else
	{
		// Nombre de préfixes
		$query_pr = pg_exec($logotron,"SELECT * from prefixes;");
		$nbpref = pg_numrows($query_pr);
		// Nombre de suffixes
		$query_sf = pg_exec($logotron,"SELECT * from suffixes;");
		$nbsuff = pg_numrows($query_sf);
		echo htmlentities("La base contient $nbpref prefixes et $nbsuff suffixes");

		// C'est parti...
		$pref = rand(0,$nbpref-1);
		$suff = rand(0,$nbsuff-1);

		$prefixe = pg_fetch_row($query_pr,$pref);
		$suffixe = pg_fetch_row($query_sf,$suff);

		$mot = GenWord($prefixe[0],$suffixe[0]);
		$sig = Signification($prefixe[1],$suffixe[1]);
	}
?>
<br><br>
<CENTER>
<TABLE WIDTH="80%" CELLPADDING="6" BORDER="1">
<TR>
  <TD COLSPAN="2" ALIGN="CENTER" BGCOLOR="#008080">
  <FONT SIZE="8" COLOR="#FFFFFF">
  <B>
  <?php echo htmlentities("$mot"); ?>
  </B>
  </FONT>
  </TD>
</TR>
<TR>
  <TD WIDTH="50%" BGCOLOR="#008080">
  <FONT SIZE="4" COLOR="#FFFFFF">
  De :<BR>
  <?php echo htmlentities("$suffixe[0]"); ?>,
  <I><?php echo htmlentities("$suffixe[1]"); ?></I>
  </FONT>
  </TD>
  <TD WIDTH="50%" BGCOLOR="#008080">
  <FONT SIZE="4" COLOR="#FFFFFF">
  et :<BR>
  <?php echo htmlentities("$prefixe[0]"); ?>,
  <I><?php echo htmlentities("$prefixe[1]"); ?></I>
  </FONT>
  </TD>
</TR>
<TR>
  <TD COLSPAN="2" ALIGN="CENTER" BGCOLOR="#008080">
  <FONT SIZE="6" COLOR="#FFFFFF">
  <I>Signification calculée:</I><br><br>
  <?php echo htmlentities("$sig"); ?>
  </FONT>
  </TD>
</TR>
</TABLE>

<br><br>
<p>
Appuyez sur reload de votre navigateur pour générer un nouveau mot.
</p>
<hr noshade size="1">
<center>
<a href="http://www.postgresql.org/"><img src="/images/pgsql-powered.gif" alt="[Powered by PostgreSQL]" border=0></a>
</center>
</body>
</html>

