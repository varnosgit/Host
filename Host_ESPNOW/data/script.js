  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  
  window.addEventListener('load', onLoad);

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
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
	document.getElementById('button2').addEventListener('click', toggle2);
  }
  function toggle(){
    websocket.send('toggle');
  }
    function toggle2(){
    websocket.send('newp');
  }