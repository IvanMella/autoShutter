constexpr char html[] = R"""(
HTTP/1.1 200 OK
Content-type:text/html
Connection: close

<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<link rel="icon" href="data:,">

		<style> body {
			text-align: center;
			font-family: "Trebuchet MS", Arial;
			margin-left:auto; margin-right:auto;}
		.slider { width: 300px; }</style>
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"> cd cd </script>
	</head>
	<body>
		<h1>Tapparella Sesame</h1>
		<p>Position: <span id="servoPos"> </span> </p>

		<!-- bar -->
		<input type="range" min="0" max="180" class="slider" id="servoSlider" onchange="servo(this.value)" value="+valueString+"/>


		<p>Fan: <span id="servoPower"> </span> </p>
		<input type="button" id="btn" onclick="switchBtn()" value="Off" ></input>

		<!-- switchbutton -->
	<!--		<label class="switch">
			<input type="checkbox">
			<span class="slider"> </span>
		</label>-->

		<!-- Javascript -->
		<script>
			var btnVal = 0
			function switchBtn()
			{
				if (btnVal) document.getElementById("btn").value = "Off", btnVal = 0;
				else document.getElementById("btn").value = "On", btnVal = 1;

				console.log("!");
				$.ajaxSetup({timeout:1000});
				$.get("/?value=switch&"); {Connection: close};
			}
			var slider = document.getElementById("servoSlider");
			var servoP = document.getElementById("servoPos");
			servoP.innerHTML = slider.value;
			console.log(slider.value);
			slider.oninput = function() {slider.value = this.value; servoP.innerHTML = this.value; }
			$.ajaxSetup({timeout:1000}); function servo(pos) {
			$.get("/?value=" + pos + "&"); {Connection: close};}
		</script>
	</body>
</html>
)""";
