function currentTime() {
  let date = new Date(); 
  let hh = date.getHours();
  let mm = date.getMinutes();
  let ss = date.getSeconds();
  let session = "AM";
  
  if(hh > 12){
      session = "PM";
   }
   hh = (hh < 10) ? "0" + hh : hh;
   mm = (mm < 10) ? "0" + mm : mm;
   ss = (ss < 10) ? "0" + ss : ss;
   let time = hh + ":" + mm + ":" + ss + " " + session;
  document.getElementById("clock").innerText = time; 
  let t = setTimeout(function(){ currentTime() }, 1000); 
}

currentTime();
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);

function showHome() {
 /* var x = document.getElementById("homee");
  if (x.style.display === "none") {
    x.style.display = "block";
  } else {
    x.style.display = "none";
  }*/
  var x = document.getElementById("zones");
  x.style.display = "none";
  x = document.getElementById("homee");
  x.style.display = "block";
}

function showZone() {
  var x = document.getElementById("zones");
  x.style.display = "block";
  x = document.getElementById("homee");
  x.style.display = "none";
}

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
	  /*
		var state;
		if (event.data == "1"){
		  state = "ON";
		}
		else if (event.data == "0"){
		  state = "OFF";
		}
		else if (event.data == "2"){
			let btn = document.createElement("button");
			btn.innerHTML = "Click Me";
			document.body.appendChild(btn);
		}
		else
			*/
		{
			document.getElementById(event.data).checked="checked";
		}
   // document.getElementById('state').innerHTML = state;

  }
  function onLoad(event) {
	var x = document.getElementById("zones");
	x.style.display = "none";
    initWebSocket();
    initButton();
  }
  function initButton() {
   // document.getElementById('button').addEventListener('click', toggle);
	//document.getElementById('button2').addEventListener('click', toggle2);
	document.getElementById('off').addEventListener('change', toggle2);
	document.getElementById('heat').addEventListener('change', toggle2);
	document.getElementById('cool').addEventListener('change', toggle2);
	document.getElementById('auto').addEventListener('change', toggle2);
	document.getElementById('on').addEventListener('change', toggle2);
  }
  function toggle(){
    websocket.send('toggle');
  }
    function toggle2(){
    websocket.send(this.id);
	//console.log('sss');
	//alert('Horray! Someone wrote "' + this.id + '"!');
  }
  