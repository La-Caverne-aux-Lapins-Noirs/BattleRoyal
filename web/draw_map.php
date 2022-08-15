<?php
$map = shell_exec("cat /var/www/atlantis.json");
$map = json_decode($map, true);
$colors = ["red", "green", "blue", "teal", "yellow", "purple", "brown", "orange"];
$tile = [
    -1 => "hidden",
    46 => "walkable", // '.'
    88 => "wall" // 'X'
];

for ($y = 0; $y < 32; ++$y)
{
    for ($x = 0; $x < 32; ++$x)
    {
?>
    <div
	class="tile <?=$tile[$map['map'][$y][$x]]; ?>"
	id="tile_<?=$x; ?>_<?=$y; ?>"
	style="left: <?=$x * 100 / 32; ?>%; top: <?=$y * 100 / 32; ?>%"
    >
    </div>
<?php
    }
}

for ($b = 0; isset($map['bullets'][$b]); ++$b)
{
    $x = $map['bullets'][$b]['x'];
    $y = $map['bullets'][$b]['y'];
?>
    <div
	class="tile bullet"
    	style="left: <?=$x * 100 / 32; ?>%; top: <?=$y * 100 / 32; ?>%;"
    >
    </div>
<?php
}

for ($p = 0; isset($map['players'][$p]); ++$p)
{
    if ($map['players'][$p]['active'] == false)
	continue ;
    $x = $map['players'][$p]['x'];
    $y = $map['players'][$p]['y'];
?>
    <div
	class="tile player"
    	style="left: <?=$x * 100 / 32; ?>%; top: <?=$y * 100 / 32; ?>%; background-color: <?=$colors[$p % 8]; ?>"
    >
    </div>
<?php
}
?>
