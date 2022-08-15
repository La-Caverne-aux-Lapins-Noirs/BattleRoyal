<!DOCTYPE html>
<html>
    <head>
	<title>Atlantis</title>
	<style>
	 .map
	 {
	     position: absolute;
	     left: 10%;
	     top: 10%;
	     height: 80%;
	     width: 80%;

	 }
	 .tile
	 {
	     position: absolute;
	     width: calc(100% / 32);
	     height: calc(100% / 32);
	 }
	 .bullet, .player
	 {
	     border-radius: 50%;
	 }
	 .bullet
	 {
	     background-color: white;
	     box-shadow: 0px 0px 10px red;
	 }
	 .walkable
	 {
	     background-color: gray;
	     box-shadow: 0px 0px 2px black inset;
	 }
	 .wall
	 {
	     backgroud-color: white;
	     box-shadow: 0px 0px 2px black inset;
	 }
	 .hidden
	 {
	     background-color: black;
	 }
	</style>
    </head>
    <body>
	<div class="map" id="map">
	</div>
	<script>
	 function loop()
	 {
	     var x = new XMLHttpRequest();
	     x.onreadystatechange = function()
	     {
		 if (this.readyState == 4 && this.status == 200)
		 {
		     console.log("Refresh");
		     document.getElementById("map").innerHTML = this.responseText;
		 }
	     };
	     setTimeout(loop, 250);
	     x.open("GET", "draw_map.php", true);
	     x.send();
	 }
	 var map = document.getElementById("map");

	 map.style.width = map.offsetHeight + "px";
	 map.style.left = (window.innerWidth - map.offsetWidth) / 2 + "px";
	 loop();
	</script>
    </body>
</html>
